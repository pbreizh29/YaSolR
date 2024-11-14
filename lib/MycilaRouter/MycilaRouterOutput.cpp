// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#include <MycilaRouterOutput.h>

#include <MycilaNTP.h>
#include <MycilaTime.h>

#include <string>

#ifdef MYCILA_LOGGER_SUPPORT
  #include <MycilaLogger.h>
extern Mycila::Logger logger;
  #define LOGD(tag, format, ...) logger.debug(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) logger.info(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) logger.warn(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) logger.error(tag, format, ##__VA_ARGS__)
#else
  #define LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#endif

#define TAG "OUTPUT"

static const char* StateNames[] = {
  "DISABLED",
  "IDLE",
  "ROUTING",
  "BYPASS_MANUAL",
  "BYPASS_AUTO",
};

static const char* DaysOfWeek[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

const char* Mycila::RouterOutput::getStateName() const { return StateNames[static_cast<int>(getState())]; }

// output

Mycila::RouterOutput::State Mycila::RouterOutput::getState() const {
  if (!_dimmer->isEnabled() && !_relay->isEnabled())
    return State::OUTPUT_DISABLED;
  if (_autoBypassEnabled)
    return State::OUTPUT_BYPASS_AUTO;
  if (_bypassEnabled)
    return State::OUTPUT_BYPASS_MANUAL;
  if (_dimmer->isOn())
    return State::OUTPUT_ROUTING;
  return State::OUTPUT_IDLE;
}

#ifdef MYCILA_JSON_SUPPORT
void Mycila::RouterOutput::toJson(const JsonObject& root, float gridVoltage) const {
  root["bypass"] = isBypassOn() ? "on" : "off";
  root["enabled"] = isDimmerEnabled();
  root["state"] = getStateName();
  root["temperature"] = _temperature.orElse(0);

  Metrics outputMeasurements;
  getMeasurements(outputMeasurements);
  toJson(root["measurements"].to<JsonObject>(), outputMeasurements);

  Metrics dimmerMetrics;
  getDimmerMetrics(dimmerMetrics, gridVoltage);
  toJson(root["metrics"].to<JsonObject>(), dimmerMetrics);
}

void Mycila::RouterOutput::toJson(const JsonObject& dest, const Metrics& metrics) {
  dest["apparent_power"] = metrics.apparentPower;
  dest["current"] = metrics.current;
  dest["energy"] = metrics.energy;
  dest["power"] = metrics.power;
  dest["power_factor"] = metrics.powerFactor;
  dest["resistance"] = metrics.resistance;
  dest["thdi"] = metrics.thdi;
  dest["voltage"] = metrics.voltage;
  dest["voltage_dimmed"] = metrics.dimmedVoltage;
}
#endif

// dimmer

bool Mycila::RouterOutput::setDimmerDutyCycle(float dutyCycle) {
  if (!_dimmer->isEnabled()) {
    LOGW(TAG, "Dimmer '%s' is disabled", _name);
    return false;
  }

  if (_autoBypassEnabled) {
    LOGW(TAG, "Auto Bypass '%s' is activated: unable to change dimmer level", _name);
    return false;
  }

  if (config.autoDimmer) {
    LOGW(TAG, "Auto Dimmer '%s' is activated: unable to change dimmer level", _name);
    return false;
  }

  if (dutyCycle > 0 && isDimmerTemperatureLimitReached()) {
    LOGW(TAG, "Dimmer '%s' reached its temperature limit of %.02f °C", _name, config.dimmerTempLimit);
    return false;
  }

  _setBypass(false);
  _dimmer->setDutyCycle(dutyCycle);

  LOGD(TAG, "Set Dimmer '%s' duty to %f", _name, _dimmer->getDutyCycle());

  return true;
}

void Mycila::RouterOutput::applyTemperatureLimit() {
  if (_autoBypassEnabled)
    return;

  if (_bypassEnabled)
    return;

  if (_dimmer->isOff())
    return;

  if (isDimmerTemperatureLimitReached()) {
    LOGW(TAG, "Dimmer '%s' reached its temperature limit of %.02f °C", _name, config.dimmerTempLimit);
    _dimmer->off();
    return;
  }
}

float Mycila::RouterOutput::autoDivert(float gridVoltage, float availablePowerToDivert) {
  if (!_dimmer->isEnabled() || _autoBypassEnabled || !config.autoDimmer || config.calibratedResistance <= 0 || isDimmerTemperatureLimitReached()) {
    _dimmer->off();
    return 0;
  }

  // maximum power of the load based on the calibrated resistance value
  const float maxPower = gridVoltage * gridVoltage / config.calibratedResistance;

  // power allowed to be diverted to the load after applying the reserved excess power ratio
  const float reservedPowerToDivert = constrain(availablePowerToDivert * config.reservedExcessPowerRatio, 0, maxPower);

  // convert to a duty
  const float dutyCycle = maxPower == 0 ? 0 : reservedPowerToDivert / maxPower;

  // try to apply duty
  _dimmer->setDutyCycle(dutyCycle);

  // returns the used power as per the dimmer state
  return maxPower * _dimmer->getDutyCycle();
}

// bypass

bool Mycila::RouterOutput::setBypass(bool switchOn) {
  if (_autoBypassEnabled && !switchOn) {
    LOGW(TAG, "Auto Bypass '%s' is activated: unable to turn of bypass relay", _name);
    return false;
  }
  _setBypass(switchOn);
  return _bypassEnabled;
}

void Mycila::RouterOutput::applyAutoBypass() {
  if (!config.autoBypass) {
    if (_autoBypassEnabled) {
      LOGW(TAG, "Auto Bypass disabled: stopping Auto Bypass '%s'", _name);
      _autoBypassEnabled = false;
      _setBypass(false);
    }
    return;
  }

  // dimmer & relay checks

  if (!_relay->isEnabled() && !_dimmer->isEnabled()) {
    if (_autoBypassEnabled) {
      LOGW(TAG, "Relay and dimmer disabled: stopping Auto Bypass '%s'", _name);
      _autoBypassEnabled = false;
      _setBypass(false);
    }
    return;
  }

  // time checks

  if (!NTP.isSynced()) {
    if (_autoBypassEnabled) {
      LOGW(TAG, "NTP not available: stopping Auto Bypass '%s'", _name);
      _autoBypassEnabled = false;
      _setBypass(false);
    }
    return;
  }

  struct tm timeInfo;
  if (!getLocalTime(&timeInfo, 5)) {
    if (_autoBypassEnabled) {
      LOGW(TAG, "Unable to get time: stopping Auto Bypass '%s'", _name);
      _autoBypassEnabled = false;
      _setBypass(false);
    }
    return;
  }

  // temperature checks

  if (!_temperature.neverUpdated()) {
    if (!_temperature.isPresent()) {
      if (_autoBypassEnabled) {
        LOGW(TAG, "Invalid temperature sensor value: stopping Auto Bypass '%s'", _name);
        _autoBypassEnabled = false;
        _setBypass(false);
      }
      return;
    }

    const float temp = _temperature.get();

    if (temp >= config.autoStopTemperature) {
      if (_autoBypassEnabled) {
        LOGI(TAG, "Temperature reached %.02f °C: stopping Auto Bypass '%s'", temp, _name);
        _autoBypassEnabled = false;
        _setBypass(false);
      }
      return;
    }

    if (temp >= config.autoStartTemperature) {
      // temperature OK, no need to start
      return;
    }
  }

  const int inRange = Time::timeInRange(timeInfo, config.autoStartTime.c_str(), config.autoStopTime.c_str());
  if (inRange == -1) {
    if (_autoBypassEnabled) {
      LOGW(TAG, "Time range %s to %s is invalid: stopping Auto Bypass '%s'", config.autoStartTime.c_str(), config.autoStopTime.c_str(), _name);
      _autoBypassEnabled = false;
      _setBypass(false);
    }
    return;
  }

  if (!inRange) {
    if (_autoBypassEnabled) {
      LOGI(TAG, "Time reached %s: stopping Auto Bypass '%s'", config.autoStopTime.c_str(), _name);
      _autoBypassEnabled = false;
      _setBypass(false);
    }
    return;
  }

  // time and temp OK, let's start
  if (!_autoBypassEnabled) {
    // auto bypass is not enabled, let's start it
    if (!_relay->isEnabled() && !_dimmer->isEnabled()) {
      return;
    }
    const char* wday = DaysOfWeek[timeInfo.tm_wday];
    if (config.weekDays.find(wday) != std::string::npos) {
      LOGI(TAG, "Time within %s-%s on %s: starting Auto Bypass '%s' at %.02f °C", config.autoStartTime.c_str(), config.autoStopTime.c_str(), wday, _name, _temperature.orElse(0));
      _setBypass(true);
      _autoBypassEnabled = _bypassEnabled;
    }
    return;
  }

  // auto bypass is enabled

  // relay is on ?
  if (_relay->isOn())
    return;

  // or relay is disabled and dimmer at full power tu replace it ?
  if (!_relay->isEnabled() && _dimmer->isOnAtFullPower())
    return;

  // start bypass
  LOGI(TAG, "Auto Bypass '%s' is activated: restarting Relay", _name);
  _setBypass(true);
}

// metrics

void Mycila::RouterOutput::getDimmerMetrics(Metrics& metrics, float gridVoltage) const {
  metrics.resistance = config.calibratedResistance;
  metrics.voltage = gridVoltage;
  metrics.energy = _pzem->data.activeEnergy;
  const float dutyCycle = _dimmer->getDutyCycle();
  const float maxPower = metrics.resistance == 0 ? 0 : metrics.voltage * metrics.voltage / metrics.resistance;
  metrics.power = dutyCycle * maxPower;
  metrics.powerFactor = sqrt(dutyCycle);
  metrics.dimmedVoltage = metrics.powerFactor * metrics.voltage;
  metrics.current = metrics.resistance == 0 ? 0 : metrics.dimmedVoltage / metrics.resistance;
  metrics.apparentPower = metrics.current * metrics.voltage;
  metrics.thdi = dutyCycle == 0 ? 0 : sqrt(1 / dutyCycle - 1);
}

bool Mycila::RouterOutput::getMeasurements(Metrics& metrics) const {
  if (!_pzem->isConnected())
    return false;
  metrics.voltage = _pzem->data.voltage;
  metrics.energy = _pzem->data.activeEnergy;
  if (getState() == State::OUTPUT_ROUTING) {
    metrics.apparentPower = abs(_pzem->data.apparentPower);
    metrics.current = abs(_pzem->data.current);
    metrics.dimmedVoltage = abs(_pzem->data.dimmedVoltage());
    metrics.power = abs(_pzem->data.activePower);
    metrics.powerFactor = abs(_pzem->data.powerFactor);
    metrics.resistance = abs(_pzem->data.resistance());
    metrics.thdi = abs(_pzem->data.thdi(0));
  }
  return true;
}

// private

void Mycila::RouterOutput::_setBypass(bool state, bool log) {
  if (state) {
    // we want to activate bypass
    if (_relay->isEnabled()) {
      // we have a relay in-place: use it
      _dimmer->off();
      if (_relay->isOff()) {
        if (log)
          LOGD(TAG, "Turning Bypass Relay '%s' ON", _name);
        _relay->setState(true);
      }
      _bypassEnabled = true;

    } else {
      // we don't have a relay: use the dimmer
      if (_dimmer->isEnabled()) {
        if (log)
          LOGD(TAG, "Turning Dimmer '%s' ON", _name);
        _dimmer->on();
        _bypassEnabled = true;
      } else {
        if (log)
          LOGW(TAG, "Dimmer '%s' is not connected to the grid: unable to activate bypass", _name);
        _bypassEnabled = false;
      }
    }

  } else {
    // we want to deactivate bypass
    if (_relay->isEnabled()) {
      if (_relay->isOn()) {
        if (log)
          LOGD(TAG, "Turning Bypass Relay '%s' OFF", _name);
        _relay->setState(false);
      }
    } else {
      if (log)
        LOGD(TAG, "Turning Dimmer '%s' OFF", _name);
      _dimmer->off();
    }
    _bypassEnabled = false;
  }
}
