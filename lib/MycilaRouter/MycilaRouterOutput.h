// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#pragma once

#include <MycilaDimmer.h>
#include <MycilaExpiringValue.h>
#include <MycilaPZEM004Tv3.h>
#include <MycilaRelay.h>

#ifdef MYCILA_JSON_SUPPORT
  #include <ArduinoJson.h>
#endif

#include <string>

namespace Mycila {
  class RouterOutput {
    public:
      enum class State {
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
          float apparentPower = NAN;
          float current = NAN;
          float dimmedVoltage = NAN;
          float energy = NAN;
          float power = NAN;
          float powerFactor = NAN;
          float resistance = NAN;
          float thdi = NAN;
          float voltage = NAN;
      } Metrics;

      typedef struct {
          float calibratedResistance = 0;
          bool autoDimmer = false;
          uint8_t dimmerTempLimit = 0;
          bool autoBypass = false;
          uint8_t autoStartTemperature = 0;
          uint8_t autoStopTemperature = 0;
          std::string autoStartTime;
          std::string autoStopTime;
          std::string weekDays;
          float reservedExcessPowerRatio = 1;
      } Config;

      RouterOutput(const char* name,
                   Dimmer& dimmer,
                   Relay& relay,
                   PZEM& pzem) : _name(name),
                                 _dimmer(&dimmer),
                                 _relay(&relay),
                                 _pzem(&pzem) {}
      // output

      State getState() const;
      const char* getStateName() const;
      const char* getName() const { return _name; }

#ifdef MYCILA_JSON_SUPPORT
      void toJson(const JsonObject& root, float gridVoltage) const;
      static void toJson(const JsonObject& dest, const Metrics& metrics);
#endif

      // dimmer

      bool isAutoDimmerEnabled() const { return config.autoDimmer && config.calibratedResistance > 0 && !_autoBypassEnabled && !_bypassEnabled; }
      bool isDimmerTemperatureLimitReached() const { return config.dimmerTempLimit > 0 && _temperature.orElse(0) >= config.dimmerTempLimit; }
      bool isDimmerOn() const { return _dimmer->isOn(); }
      float getDimmerDutyCycle() const { return _dimmer->getDutyCycle(); }
      float getDimmerDutyCycleLive() const { return _dimmer->getDutyCycleLive(); }
      float getDimmerDutyCycleLimit() const { return _dimmer->getDutyCycleLimit(); }
      // Power Duty Cycle [0, 1]
      // At 0% power, duty == 0
      // At 100% power, duty == 1
      bool setDimmerDutyCycle(float dutyCycle);
      bool setDimmerOff() { return setDimmerDutyCycle(0); }
      void applyTemperatureLimit();

      float autoDivert(float gridVoltage, float availablePowerToDivert);

      // bypass

      bool isAutoBypassEnabled() const { return config.autoBypass; }
      bool isBypassOn() const { return _bypassEnabled; }
      bool setBypass(bool state);
      bool setBypassOn() { return setBypass(true); }
      bool setBypassOff() { return setBypass(false); }
      void applyAutoBypass();

      // metrics

      // get output theoretical metrics based on the dimmer state and the grid voltage
      void getOutputMetrics(Metrics& metrics, float gridVoltage) const;
      // get PZEM measurements, and returns false if the PZEM is not connected, true if measurements are available
      bool getOutputMeasurements(Metrics& metrics) const;

      float getOutputPower() const { return _pzem->isConnected() ? _pzem->data.activePower : 0; }

      // temperature

      ExpiringValue<float>& temperature() { return _temperature; }
      const ExpiringValue<float>& temperature() const { return _temperature; }

    public:
      Config config;

    private:
      const char* _name;
      Dimmer* _dimmer;
      Relay* _relay;
      PZEM* _pzem;
      bool _autoBypassEnabled = false;
      bool _bypassEnabled = false;
      ExpiringValue<float> _temperature;

    private:
      void _setBypass(bool state, bool log = true);
  };
} // namespace Mycila
