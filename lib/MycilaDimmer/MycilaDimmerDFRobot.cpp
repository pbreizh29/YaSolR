// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2025 Mathieu Carbou
 */
#include <MycilaDimmerDFRobot.h>

// logging
#include <esp32-hal-log.h>

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

#define TAG "DFR_DIMMER"

void Mycila::DFRobotDimmer::begin() {
  if (_enabled)
    return;

  uint8_t resolution = getResolution();
  if (!resolution) {
    LOGE(TAG, "Disable DFRobot Dimmer: SKU not set!");
    return;
  }

  // sanity checks
  if (_sku == SKU::DFR1071_GP8211S) {
    if (_channel > 0) {
      LOGW(TAG, "DFRobot DFR1071 (GP8211S) has only one channel: switching to channel 0");
      _channel = 0;
    }
  }

  // discovery
  bool success = false;
  if (_deviceAddress) {
    LOGI(TAG, "Searching for DFRobot Dimmer @ 0x%02x...", _deviceAddress);
    for (int i = 0; i < 5; i++) {
      _wire->beginTransmission(_deviceAddress);
      int err = _wire->endTransmission();
      if (err) {
        LOGW(TAG, "DFRobot Dimmer @ 0x%02x: TwoWire communication error: %d", _deviceAddress, err);
        delay(10);
      } else {
        success = true;
        break;
      }
    }

  } else {
    LOGI(TAG, "Searching for DFRobot Dimmer @ 0x58-0x5F (discovery)...");
    for (uint8_t addr = 0x58; !success && addr <= 0x5F; addr++) {
      for (int i = 0; i < 5; i++) {
        _wire->beginTransmission(addr);
        int err = _wire->endTransmission();
        if (err) {
          LOGW(TAG, "DFRobot Dimmer @ 0x%02x: TwoWire communication error: %d", addr, err);
          delay(10);
        } else {
          _deviceAddress = addr;
          success = true;
          break;
        }
      }
    }
  }

  if (success) {
    LOGI(TAG, "Enable DFRobot Dimmer @ 0x%02x and channel %d", _deviceAddress, _channel);
  } else {
    LOGE(TAG, "DFRobot Dimmer: TwoWire communication error: cannot communicate with device");
    return;
  }

  // set output
  switch (_output) {
    case Output::RANGE_0_5V: {
      LOGI(TAG, "Set output range to 0-5V");
      uint8_t data = 0x00;
      uint8_t err = _send(0x01, &data, 1);
      if (err) {
        LOGE(TAG, "Disable DFRobot Dimmer: TwoWire communication error: %d", err);
        return;
      }
      break;
    }
    case Output::RANGE_0_10V: {
      LOGI(TAG, "Set output range to 0-10V");
      uint8_t data = 0x11;
      uint8_t err = _send(0x01, &data, 1);
      if (err) {
        LOGE(TAG, "Disable DFRobot Dimmer: TwoWire communication error: %d", err);
        return;
      }
    }
    default:
      break;
  }

  _enabled = true;

  // restart with last saved value
  setDutyCycle(_dutyCycle);
}

void Mycila::DFRobotDimmer::end() {
  if (!_enabled)
    return;
  _enabled = false;
  LOGI(TAG, "Disable DFRobot Dimmer");
  // Note: do not set _dutyCycle to 0 in order to keep last set user value
  _delay = UINT16_MAX;
}

bool Mycila::DFRobotDimmer::apply() {
  uint8_t resolution = getResolution();
  uint16_t duty = getFiringRatio() * ((1 << resolution) - 1);
  duty = duty << (16 - resolution);
  switch (_channel) {
    case 0: {
      uint8_t buffer[2] = {uint8_t(duty & 0xff), uint8_t(duty >> 8)};
      return _send(0x02, buffer, 2) == 0;
    }
    case 1: {
      uint8_t buffer[2] = {uint8_t(duty & 0xff), uint8_t(duty >> 8)};
      return _send(0x04, buffer, 2) == 0;
    }
    case 2: {
      uint8_t buffer[4] = {uint8_t(duty & 0xff), uint8_t(duty >> 8), uint8_t(duty & 0xff), uint8_t(duty >> 8)};
      return _send(0x02, buffer, 4) == 0;
    }
    default:
      return false;
  }
}

uint8_t Mycila::DFRobotDimmer::_send(uint8_t reg, uint8_t* buffer, size_t size) {
  _wire->beginTransmission(_deviceAddress);
  _wire->write(reg);
  for (uint16_t i = 0; i < size; i++) {
    _wire->write(buffer[i]);
  }
  return _wire->endTransmission();
}
