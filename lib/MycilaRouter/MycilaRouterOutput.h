// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#pragma once

#include <MycilaDS18.h>
#include <MycilaDimmer.h>
#include <MycilaPZEM004Tv3.h>
#include <MycilaRelay.h>

#ifdef MYCILA_JSON_SUPPORT
  #include <ArduinoJson.h>
#endif

namespace Mycila {
  enum class RouterOutputState {
    // output disabled
    OUTPUT_DISABLED = 0,
    // idle
    OUTPUT_IDLE,
    // excess power sent to load
    OUTPUT_ROUTING,
    // full power sent to load through relay (manual trigger)
    OUTPUT_BYPASS_MANUAL,
    // full power sent to load through relay (auto trigger)
    OUTPUT_BYPASS_AUTO
  };

  typedef struct {
      float apparentPower = 0;
      float current = 0;
      float dimmedVoltage = 0;
      float energy = 0;
      float power = 0;
      float powerFactor = 0;
      float resistance = 0;
      float thdi = 0;
      float voltage = 0;
  } RouterOutputMetrics;

  typedef std::function<void()> RouterOutputStateCallback;

  class RouterOutput {
    public:
      typedef struct {
          float calibratedResistance = 0;
          bool autoDimmer = false;
          uint16_t dimmerDutyLimit = MYCILA_DIMMER_MAX_DUTY;
          uint8_t dimmerTempLimit = 0;
          bool autoBypass = false;
          uint8_t autoStartTemperature = 0;
          uint8_t autoStopTemperature = 0;
          String autoStartTime;
          String autoStopTime;
          String weekDays;
          float reservedExcessPowerRatio = 1;
      } Config;

      RouterOutput(const char* name,
                   Dimmer& dimmer,
                   Relay& relay,
                   DS18& temperatureSensor,
                   PZEM& pzem) : _name(name),
                                 _dimmer(&dimmer),
                                 _relay(&relay),
                                 _temperatureSensor(&temperatureSensor),
                                 _pzem(&pzem) {}
      // output

      RouterOutputState getState() const {
        if (!_dimmer->isEnabled() && !_relay->isEnabled())
          return RouterOutputState::OUTPUT_DISABLED;
        if (_autoBypassEnabled)
          return RouterOutputState::OUTPUT_BYPASS_AUTO;
        if (_bypassEnabled)
          return RouterOutputState::OUTPUT_BYPASS_MANUAL;
        if (_dimmer->getDuty() > 0)
          return RouterOutputState::OUTPUT_ROUTING;
        return RouterOutputState::OUTPUT_IDLE;
      }
      const char* getStateName() const;
      const char* getName() const { return _name; }

      void listen(RouterOutputStateCallback callback) { _callback = callback; }

#ifdef MYCILA_JSON_SUPPORT
      void toJson(const JsonObject& root, float gridVoltage) const {
        root["enabled"] = isDimmerEnabled();
        root["state"] = getStateName();
        root["bypass"] = isBypassOn() ? "on" : "off";

        _dimmer->toJson(root["dimmer"].to<JsonObject>());
        _temperatureSensor->toJson(root["ds18"].to<JsonObject>());

        RouterOutputMetrics outputMeasurements;
        getMeasurements(outputMeasurements);

        JsonObject measurements = root["measurements"].to<JsonObject>();
        measurements["apparent_power"] = outputMeasurements.apparentPower;
        measurements["current"] = outputMeasurements.current;
        measurements["power"] = outputMeasurements.power;
        measurements["power_factor"] = outputMeasurements.powerFactor;
        measurements["resistance"] = outputMeasurements.resistance;
        measurements["thdi"] = outputMeasurements.thdi;
        measurements["voltage"] = outputMeasurements.voltage;
        measurements["voltage_dimmed"] = outputMeasurements.dimmedVoltage;

        RouterOutputMetrics dimmerMetrics;
        getDimmerMetrics(dimmerMetrics, gridVoltage);

        JsonObject metrics = root["metrics"].to<JsonObject>();
        metrics["apparent_power"] = dimmerMetrics.apparentPower;
        metrics["current"] = dimmerMetrics.current;
        metrics["power"] = dimmerMetrics.power;
        metrics["power_factor"] = dimmerMetrics.powerFactor;
        metrics["resistance"] = dimmerMetrics.resistance;
        metrics["thdi"] = dimmerMetrics.thdi;
        metrics["voltage"] = dimmerMetrics.voltage;
        metrics["voltage_dimmed"] = dimmerMetrics.dimmedVoltage;

        _pzem->toJson(root["pzem"].to<JsonObject>());
        _relay->toJson(root["relay"].to<JsonObject>());
      }
#endif

      // dimmer

      bool isDimmerEnabled() const { return _dimmer->isEnabled(); }
      bool isAutoDimmerEnabled() const { return _dimmer->isEnabled() && config.autoDimmer; }
      bool isDimmerTemperatureLimitReached() const { return config.dimmerTempLimit > 0 && _temperatureSensor->getValidTemperature() >= config.dimmerTempLimit; }
      uint16_t getDimmerDuty() const { return _dimmer->getDuty(); }
      float getDimmerDutyCycle() const { return _dimmer->getDutyCycle(); }
      // Power Duty Cycle [0, MYCILA_DIMMER_MAX_DUTY]
      bool tryDimmerDuty(uint16_t duty);
      bool autoSetDimmerDuty(uint16_t duty);
      // Power Duty Cycle [0, 1]
      // At 0% power, duty == 0
      // At 100% power, duty == 1
      bool tryDimmerDutyCycle(float dutyCycle) { return tryDimmerDuty(dutyCycle * MYCILA_DIMMER_MAX_DUTY); }
      bool autoSetDimmerDutyCycle(float dutyCycle) { return autoSetDimmerDuty(dutyCycle * MYCILA_DIMMER_MAX_DUTY); }
      void applyDimmerLimits();
      uint16_t getDimmerDutyLimit() const { return config.dimmerDutyLimit; }
      float getDimmerDutyCycleLimit() const { return static_cast<float>(config.dimmerDutyLimit) / MYCILA_DIMMER_MAX_DUTY; }

      // bypass

      bool isBypassEnabled() const { return _relay->isEnabled() || _dimmer->isEnabled(); }
      bool isAutoBypassEnabled() const { return isBypassEnabled() && config.autoBypass; }
      bool isBypassOn() const { return _bypassEnabled; }
      bool tryBypassState(bool state);
      void applyAutoBypass();

      // metrics

      void getDimmerMetrics(RouterOutputMetrics& metrics, float gridVoltage) const {
        metrics.resistance = config.calibratedResistance;
        metrics.voltage = gridVoltage;
        metrics.energy = _pzem->getEnergy();
        const float dutyCycle = _dimmer->getDutyCycle();
        const float maxPower = metrics.resistance == 0 ? 0 : metrics.voltage * metrics.voltage / metrics.resistance;
        metrics.power = dutyCycle * maxPower;
        metrics.powerFactor = sqrt(dutyCycle);
        metrics.dimmedVoltage = metrics.powerFactor * metrics.voltage;
        metrics.current = metrics.resistance == 0 ? 0 : metrics.dimmedVoltage / metrics.resistance;
        metrics.apparentPower = metrics.current * metrics.voltage;
        metrics.thdi = dutyCycle == 0 ? 0 : sqrt(1 / dutyCycle - 1);
      }

      void getMeasurements(RouterOutputMetrics& metrics) const {
        metrics.voltage = _pzem->getVoltage();
        metrics.energy = _pzem->getEnergy();
        if (getState() == RouterOutputState::OUTPUT_ROUTING) {
          metrics.apparentPower = _pzem->getApparentPower();
          metrics.current = _pzem->getCurrent();
          metrics.dimmedVoltage = _pzem->getDimmedVoltage();
          metrics.power = _pzem->getPower();
          metrics.powerFactor = _pzem->getPowerFactor();
          metrics.resistance = _pzem->getResistance();
          metrics.thdi = _pzem->getTHDi(0);
        }
      }

    public:
      Config config;

    private:
      const char* _name;
      Dimmer* _dimmer;
      Relay* _relay;
      Mycila::DS18* _temperatureSensor;
      PZEM* _pzem;
      bool _autoBypassEnabled = false;
      bool _bypassEnabled = false;
      RouterOutputStateCallback _callback = nullptr;

    private:
      void _setBypass(bool state, bool log = true);
  };
} // namespace Mycila
