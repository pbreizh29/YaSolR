// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#include <YaSolRWebsite.h>

#define TAG        "WEBSITE"
#define HIDDEN_PWD "********"

void YaSolR::WebsiteClass::initLayout() {
  logger.debug(TAG, "Initializing layout...");

#ifdef APP_MODEL_PRO
  // output 1 (status)
  _output1State.setTab(&_output1Tab);
  _output1DS18State.setTab(&_output1Tab);
  _output1DimmerSlider.setTab(&_output1Tab);
  _output1DimmerSliderRO.setTab(&_output1Tab);
  _output1Bypass.setTab(&_output1Tab);
  _output1BypassRO.setTab(&_output1Tab);

  _output1Power.setTab(&_output1Tab);
  _output1PowerFactor.setTab(&_output1Tab);
  _output1THDi.setTab(&_output1Tab);
  _output1Energy.setTab(&_output1Tab);

  _output1ApparentPower.setTab(&_output1Tab);
  _output1Voltage.setTab(&_output1Tab);
  _output1Current.setTab(&_output1Tab);
  _output1Resistance.setTab(&_output1Tab);

  // output 2 (status)
  _output2State.setTab(&_output2Tab);
  _output2DS18State.setTab(&_output2Tab);
  _output2DimmerSlider.setTab(&_output2Tab);
  _output2DimmerSliderRO.setTab(&_output2Tab);
  _output2Bypass.setTab(&_output2Tab);
  _output2BypassRO.setTab(&_output2Tab);

  _output2Power.setTab(&_output2Tab);
  _output2PowerFactor.setTab(&_output2Tab);
  _output2THDi.setTab(&_output2Tab);
  _output2Energy.setTab(&_output2Tab);

  _output2ApparentPower.setTab(&_output2Tab);
  _output2Voltage.setTab(&_output2Tab);
  _output2Current.setTab(&_output2Tab);
  _output2Resistance.setTab(&_output2Tab);

  // output 1 (control)
  _output1AutoBypass.setTab(&_output1Tab);
  _output1DimmerAuto.setTab(&_output1Tab);
  _output1AutoStartTemp.setTab(&_output1Tab);
  _output1AutoStartTime.setTab(&_output1Tab);
  _output1AutoStartWDays.setTab(&_output1Tab);
  _output1AutoStoptTemp.setTab(&_output1Tab);
  _output1AutoStoptTime.setTab(&_output1Tab);
  _output1DimmerLimiter.setTab(&_output1Tab);

  _boolConfig(_output1AutoBypass, KEY_ENABLE_OUTPUT1_AUTO_BYPASS);
  _boolConfig(_output1DimmerAuto, KEY_ENABLE_OUTPUT1_AUTO_DIMMER);
  _daysConfig(_output1AutoStartWDays, KEY_OUTPUT1_DAYS);
  _numConfig(_output1AutoStartTemp, KEY_OUTPUT1_TEMPERATURE_START);
  _numConfig(_output1AutoStoptTemp, KEY_OUTPUT1_TEMPERATURE_STOP);
  _sliderConfig(_output1DimmerLimiter, KEY_OUTPUT1_DIMMER_LIMITER);
  _textConfig(_output1AutoStartTime, KEY_OUTPUT1_TIME_START);
  _textConfig(_output1AutoStoptTime, KEY_OUTPUT1_TIME_STOP);
  _outputBypassSwitch(_output1Bypass, output1);
  _outputDimmerSlider(_output1DimmerSlider, output1);

  // output 2 (control)
  _output2AutoBypass.setTab(&_output2Tab);
  _output2DimmerAuto.setTab(&_output2Tab);
  _output2AutoStartTemp.setTab(&_output2Tab);
  _output2AutoStartTime.setTab(&_output2Tab);
  _output2AutoStartWDays.setTab(&_output2Tab);
  _output2AutoStoptTemp.setTab(&_output2Tab);
  _output2AutoStoptTime.setTab(&_output2Tab);
  _output2DimmerLimiter.setTab(&_output2Tab);

  _boolConfig(_output2AutoBypass, KEY_ENABLE_OUTPUT2_AUTO_BYPASS);
  _boolConfig(_output2DimmerAuto, KEY_ENABLE_OUTPUT2_AUTO_DIMMER);
  _daysConfig(_output2AutoStartWDays, KEY_OUTPUT2_DAYS);
  _numConfig(_output2AutoStartTemp, KEY_OUTPUT2_TEMPERATURE_START);
  _numConfig(_output2AutoStoptTemp, KEY_OUTPUT2_TEMPERATURE_STOP);
  _sliderConfig(_output2DimmerLimiter, KEY_OUTPUT2_DIMMER_LIMITER);
  _textConfig(_output2AutoStartTime, KEY_OUTPUT2_TIME_START);
  _textConfig(_output2AutoStoptTime, KEY_OUTPUT2_TIME_STOP);
  _outputBypassSwitch(_output2Bypass, output2);
  _outputDimmerSlider(_output2DimmerSlider, output2);

  // relays (control)
  _relay1Load.setTab(&_relaysTab);
  _relay1Switch.setTab(&_relaysTab);
  _relay1SwitchRO.setTab(&_relaysTab);
  _relay2Load.setTab(&_relaysTab);
  _relay2Switch.setTab(&_relaysTab);
  _relay1SwitchRO.setTab(&_relaysTab);

  _relaySwitch(_relay1Switch, "relay1");
  _relaySwitch(_relay2Switch, "relay2");

  // management
  _configBackup.setTab(&_managementTab);
  _configRestore.setTab(&_managementTab);
  _consoleLink.setTab(&_managementTab);
  _debugMode.setTab(&_managementTab);
  _otaLink.setTab(&_managementTab);
  _reset.setTab(&_managementTab);
  _restart.setTab(&_managementTab);
  _energyReset.setTab(&_managementTab);

  _boolConfig(_debugMode, KEY_ENABLE_DEBUG);

  _energyReset.attachCallback([] PUSH_BUTTON_CARD_CB {
    jsy.resetEnergy();
    pzemO1.resetEnergy();
    pzemO2.resetEnergy();
  });
  _reset.attachCallback([] PUSH_BUTTON_CARD_CB { resetTask.resume(); });
  _restart.attachCallback([] PUSH_BUTTON_CARD_CB { restartTask.resume(); });

  // GPIO (configuration)
  _pinDimmerO1.setTab(&_pinConfigTab);
  _pinDimmerO2.setTab(&_pinConfigTab);
  _pinDisplayClock.setTab(&_pinConfigTab);
  _pinDisplayData.setTab(&_pinConfigTab);
  _pinDS18O1.setTab(&_pinConfigTab);
  _pinDS18O2.setTab(&_pinConfigTab);
  _pinDS18Router.setTab(&_pinConfigTab);
  _pinJsyRX.setTab(&_pinConfigTab);
  _pinJsyTX.setTab(&_pinConfigTab);
  _pinLEDGreen.setTab(&_pinConfigTab);
  _pinLEDRed.setTab(&_pinConfigTab);
  _pinLEDYellow.setTab(&_pinConfigTab);
  _pinPZEMRX.setTab(&_pinConfigTab);
  _pinPZEMTX.setTab(&_pinConfigTab);
  _pinRelay1.setTab(&_pinConfigTab);
  _pinRelay2.setTab(&_pinConfigTab);
  _pinRelayO1.setTab(&_pinConfigTab);
  _pinRelayO2.setTab(&_pinConfigTab);
  _pinZCD.setTab(&_pinConfigTab);

  _pinConfig(_pinDimmerO1, KEY_PIN_OUTPUT1_DIMMER);
  _pinConfig(_pinDimmerO2, KEY_PIN_OUTPUT2_DIMMER);
  _pinConfig(_pinDisplayClock, KEY_PIN_DISPLAY_SCL);
  _pinConfig(_pinDisplayData, KEY_PIN_DISPLAY_SDA);
  _pinConfig(_pinDS18O1, KEY_PIN_OUTPUT1_DS18);
  _pinConfig(_pinDS18O2, KEY_PIN_OUTPUT2_DS18);
  _pinConfig(_pinDS18Router, KEY_PIN_ROUTER_DS18);
  _pinConfig(_pinJsyRX, KEY_PIN_JSY_RX);
  _pinConfig(_pinJsyTX, KEY_PIN_JSY_RT);
  _pinConfig(_pinLEDGreen, KEY_PIN_LIGHTS_GREEN);
  _pinConfig(_pinLEDRed, KEY_PIN_LIGHTS_RED);
  _pinConfig(_pinLEDYellow, KEY_PIN_LIGHTS_YELLOW);
  _pinConfig(_pinPZEMRX, KEY_PIN_PZEM_RX);
  _pinConfig(_pinPZEMTX, KEY_PIN_PZEM_TX);
  _pinConfig(_pinRelay1, KEY_PIN_RELAY1);
  _pinConfig(_pinRelay2, KEY_PIN_RELAY2);
  _pinConfig(_pinRelayO1, KEY_PIN_OUTPUT1_RELAY);
  _pinConfig(_pinRelayO2, KEY_PIN_OUTPUT2_RELAY);
  _pinConfig(_pinZCD, KEY_PIN_ZCD);

  // Hardware
  _display.setTab(&_hardwareEnableTab);
  _jsy.setTab(&_hardwareEnableTab);
  _led.setTab(&_hardwareEnableTab);
  _mqtt.setTab(&_hardwareEnableTab);
  _output1Dimmer.setTab(&_hardwareEnableTab);
  _output1PZEM.setTab(&_hardwareEnableTab);
  _output1Relay.setTab(&_hardwareEnableTab);
  _output1DS18.setTab(&_hardwareEnableTab);
  _output2Dimmer.setTab(&_hardwareEnableTab);
  _output2PZEM.setTab(&_hardwareEnableTab);
  _output2Relay.setTab(&_hardwareEnableTab);
  _output2DS18.setTab(&_hardwareEnableTab);
  _relay1.setTab(&_hardwareEnableTab);
  _relay2.setTab(&_hardwareEnableTab);
  _routerDS18.setTab(&_hardwareEnableTab);
  _zcd.setTab(&_hardwareEnableTab);

  _boolConfig(_display, KEY_ENABLE_DISPLAY);
  _boolConfig(_jsy, KEY_ENABLE_JSY);
  _boolConfig(_led, KEY_ENABLE_LIGHTS);
  _boolConfig(_mqtt, KEY_ENABLE_MQTT);
  _boolConfig(_output1Dimmer, KEY_ENABLE_OUTPUT1_DIMMER);
  _boolConfig(_output1DS18, KEY_ENABLE_OUTPUT1_DS18);
  _boolConfig(_output1PZEM, KEY_ENABLE_OUTPUT1_PZEM);
  _boolConfig(_output1Relay, KEY_ENABLE_OUTPUT1_RELAY);
  _boolConfig(_output2Dimmer, KEY_ENABLE_OUTPUT2_DIMMER);
  _boolConfig(_output2DS18, KEY_ENABLE_OUTPUT2_DS18);
  _boolConfig(_output2PZEM, KEY_ENABLE_OUTPUT2_PZEM);
  _boolConfig(_output2Relay, KEY_ENABLE_OUTPUT2_RELAY);
  _boolConfig(_relay1, KEY_ENABLE_RELAY1);
  _boolConfig(_relay2, KEY_ENABLE_RELAY2);
  _boolConfig(_routerDS18, KEY_ENABLE_DS18_SYSTEM);
  _boolConfig(_zcd, KEY_ENABLE_ZCD);

  // Hardware (config)
  _displayRotation.setTab(&_hardwareConfigTab);
  _displayType.setTab(&_hardwareConfigTab);
  _displaySpeed.setTab(&_hardwareConfigTab);
  _gridFreq.setTab(&_hardwareConfigTab);
  _gridVolt.setTab(&_hardwareConfigTab);
  _output1PZEMSync.setTab(&_hardwareConfigTab);
  _output1RelayType.setTab(&_hardwareConfigTab);
  _output2PZEMSync.setTab(&_hardwareConfigTab);
  _output2RelayType.setTab(&_hardwareConfigTab);
  _relay1Type.setTab(&_hardwareConfigTab);
  _relay2Type.setTab(&_hardwareConfigTab);

  _numConfig(_displayRotation, KEY_DISPLAY_ROTATION);
  _numConfig(_gridFreq, KEY_GRID_FREQUENCY);
  _numConfig(_gridVolt, KEY_GRID_VOLTAGE);
  _numConfig(_relay1Load, KEY_RELAY1_LOAD);
  _numConfig(_relay2Load, KEY_RELAY2_LOAD);
  _textConfig(_displayType, KEY_DISPLAY_TYPE);
  _textConfig(_output1RelayType, KEY_OUTPUT1_RELAY_TYPE);
  _textConfig(_output2RelayType, KEY_OUTPUT2_RELAY_TYPE);
  _textConfig(_relay1Type, KEY_RELAY1_TYPE);
  _textConfig(_relay2Type, KEY_RELAY2_TYPE);
  _sliderConfig(_displaySpeed, KEY_DISPLAY_SPEED);

  _output1PZEMSync.attachCallback([] PUSH_BUTTON_CARD_CB { pzemO1PairingTask.resume(); });
  _output2PZEMSync.attachCallback([] PUSH_BUTTON_CARD_CB { pzemO2PairingTask.resume(); });

  // mqtt (config)
  _haDiscovery.setTab(&_mqttConfigTab);
  _haDiscoveryTopic.setTab(&_mqttConfigTab);
  _mqttGridPower.setTab(&_mqttConfigTab);
  _mqttGridVoltage.setTab(&_mqttConfigTab);
  _mqttPort.setTab(&_mqttConfigTab);
  _mqttPublishInterval.setTab(&_mqttConfigTab);
  _mqttPwd.setTab(&_mqttConfigTab);
  _mqttSecured.setTab(&_mqttConfigTab);
  _mqttServer.setTab(&_mqttConfigTab);
  _mqttServerCert.setTab(&_mqttConfigTab);
  _mqttTopic.setTab(&_mqttConfigTab);
  _mqttUser.setTab(&_mqttConfigTab);

  _boolConfig(_haDiscovery, KEY_ENABLE_HA_DISCOVERY);
  _boolConfig(_mqttSecured, KEY_MQTT_SECURED);
  _numConfig(_mqttPort, KEY_MQTT_PORT);
  _passwordConfig(_mqttPwd, KEY_MQTT_PASSWORD);
  _sliderConfig(_mqttPublishInterval, KEY_MQTT_PUBLISH_INTERVAL);
  _textConfig(_haDiscoveryTopic, KEY_HA_DISCOVERY_TOPIC);
  _textConfig(_mqttGridPower, KEY_GRID_POWER_MQTT_TOPIC);
  _textConfig(_mqttGridVoltage, KEY_GRID_VOLTAGE_MQTT_TOPIC);
  _textConfig(_mqttServer, KEY_MQTT_SERVER);
  _textConfig(_mqttTopic, KEY_MQTT_TOPIC);
  _textConfig(_mqttUser, KEY_MQTT_USERNAME);

  // network (config)
  _adminPwd.setTab(&_networkConfigTab);
  _apMode.setTab(&_networkConfigTab);
  _ntpServer.setTab(&_networkConfigTab);
  _ntpSync.setTab(&_networkConfigTab);
  _ntpTimezone.setTab(&_networkConfigTab);
  _wifiPwd.setTab(&_networkConfigTab);
  _wifiSSID.setTab(&_networkConfigTab);

  _boolConfig(_apMode, KEY_ENABLE_AP_MODE);
  _passwordConfig(_adminPwd, KEY_ADMIN_PASSWORD);
  _passwordConfig(_wifiPwd, KEY_WIFI_PASSWORD);
  _textConfig(_ntpServer, KEY_NTP_SERVER);
  _textConfig(_ntpTimezone, KEY_NTP_TIMEZONE);
  _textConfig(_wifiSSID, KEY_WIFI_SSID);

  _ntpSync.attachCallback([](const char* value) {
    const String str = String(value);
    const size_t len = str.length();
    const timeval tv = {str.substring(0, len - 3).toInt(), str.substring(len - 3).toInt()};
    Mycila::NTP.sync(tv);
  });
#endif
}

void YaSolR::WebsiteClass::initCards() {
  logger.debug(TAG, "Initializing cards...");

  // Statistics
  _appManufacturer.set(Mycila::AppInfo.manufacturer.c_str());
  _appModel.set((Mycila::AppInfo.model.c_str()));
  _appName.set((Mycila::AppInfo.name.c_str()));
  _appVersion.set(Mycila::AppInfo.version.c_str());
  _deviceBootCount.set(String(Mycila::System.getBootCount()).c_str());
  _deviceCores.set(String(ESP.getChipCores()).c_str());
  _deviceModel.set(ESP.getChipModel());
  _deviceRev.set(String(ESP.getChipRevision()).c_str());
  _deviceHeapTotal.set((String(ESP.getHeapSize()) + " bytes").c_str());
  _deviceID.set(Mycila::AppInfo.id.c_str());
  _firmwareBuildHash.set(Mycila::AppInfo.buildHash.c_str());
  _firmwareBuildTimestamp.set(Mycila::AppInfo.buildDate.c_str());
  _firmwareFilename.set(Mycila::AppInfo.firmware.c_str());
  _networkAPMAC.set(ESPConnect.getMACAddress(ESPConnectMode::AP).c_str());
  _networkEthMAC.set(ESPConnect.getMACAddress(ESPConnectMode::ETH).isEmpty() ? "N/A" : ESPConnect.getMACAddress(ESPConnectMode::ETH).c_str());
  _networkHostname.set(Mycila::AppInfo.defaultHostname.c_str());
  _networkWiFiMAC.set(ESPConnect.getMACAddress(ESPConnectMode::STA).c_str());

#ifdef APP_MODEL_PRO
  // output 1 (control)
  bool autoDimmerO1Activated = config.getBool(KEY_ENABLE_OUTPUT1_AUTO_DIMMER);
  bool autoBypassActivated = config.getBool(KEY_ENABLE_OUTPUT1_AUTO_BYPASS);
  _output1DimmerAuto.update(autoDimmerO1Activated);
  _output1DimmerLimiter.update(static_cast<int>(config.get(KEY_OUTPUT1_DIMMER_LIMITER).toInt()));
  _output1AutoBypass.update(autoBypassActivated);
  _output1AutoStartWDays.update(config.get(KEY_OUTPUT1_DAYS));
  _output1AutoStartTemp.update(config.get(KEY_OUTPUT1_TEMPERATURE_START));
  _output1AutoStartTime.update(config.get(KEY_OUTPUT1_TIME_START));
  _output1AutoStoptTemp.update(config.get(KEY_OUTPUT1_TEMPERATURE_STOP));
  _output1AutoStoptTime.update(config.get(KEY_OUTPUT1_TIME_STOP));
  _output1Tab.setDisplay(config.getBool(KEY_ENABLE_OUTPUT1_DIMMER) || config.getBool(KEY_ENABLE_OUTPUT1_RELAY) || config.getBool(KEY_ENABLE_OUTPUT1_DS18));
  _output1DimmerSlider.setDisplay(!autoDimmerO1Activated);
  _output1DimmerSliderRO.setDisplay(autoDimmerO1Activated);
  _output1Bypass.setDisplay(!autoBypassActivated);
  _output1BypassRO.setDisplay(autoBypassActivated);

  // output 2 (control)
  bool autoDimmerO2Activated = config.getBool(KEY_ENABLE_OUTPUT2_AUTO_DIMMER);
  bool autoBypassO2Activated = config.getBool(KEY_ENABLE_OUTPUT2_AUTO_BYPASS);
  _output2DimmerAuto.update(autoDimmerO2Activated);
  _output2DimmerLimiter.update(static_cast<int>(config.get(KEY_OUTPUT2_DIMMER_LIMITER).toInt()));
  _output2AutoBypass.update(autoBypassO2Activated);
  _output2AutoStartWDays.update(config.get(KEY_OUTPUT2_DAYS));
  _output2AutoStartTemp.update(config.get(KEY_OUTPUT2_TEMPERATURE_START));
  _output2AutoStartTime.update(config.get(KEY_OUTPUT2_TIME_START));
  _output2AutoStoptTemp.update(config.get(KEY_OUTPUT2_TEMPERATURE_STOP));
  _output2AutoStoptTime.update(config.get(KEY_OUTPUT2_TIME_STOP));
  _output2Tab.setDisplay(config.getBool(KEY_ENABLE_OUTPUT2_DIMMER) || config.getBool(KEY_ENABLE_OUTPUT2_RELAY) || config.getBool(KEY_ENABLE_OUTPUT2_DS18));
  _output2DimmerSlider.setDisplay(!autoDimmerO2Activated);
  _output2DimmerSliderRO.setDisplay(autoDimmerO2Activated);
  _output2Bypass.setDisplay(!autoBypassO2Activated);
  _output2BypassRO.setDisplay(autoBypassO2Activated);

  // relays (control)
  int32_t load1 = config.get(KEY_RELAY1_LOAD).toInt();
  int32_t load2 = config.get(KEY_RELAY2_LOAD).toInt();
  _relay1Load.update(static_cast<int>(load1));
  _relay2Load.update(static_cast<int>(load2));
  _relaysTab.setDisplay(config.getBool(KEY_ENABLE_RELAY1) || config.getBool(KEY_ENABLE_RELAY2));
  _relay1Load.setDisplay(config.getBool(KEY_ENABLE_RELAY1));
  _relay1Switch.setDisplay(config.getBool(KEY_ENABLE_RELAY1) && load1 <= 0);
  _relay1SwitchRO.setDisplay(config.getBool(KEY_ENABLE_RELAY1) && load1 > 0);
  _relay2Load.setDisplay(config.getBool(KEY_ENABLE_RELAY2));
  _relay2Switch.setDisplay(config.getBool(KEY_ENABLE_RELAY2) && load2 <= 0);
  _relay2SwitchRO.setDisplay(config.getBool(KEY_ENABLE_RELAY2) && load2 > 0);

  // management
  _configBackup.update("/api/config/backup");
  _configRestore.update("/api/config/restore");
  _consoleLink.update("/console");
  _debugMode.update(config.getBool(KEY_ENABLE_DEBUG));
  _otaLink.update("/update");
  _energyReset.setDisplay(config.getBool(KEY_ENABLE_JSY) || config.getBool(KEY_ENABLE_OUTPUT1_PZEM) || config.getBool(KEY_ENABLE_OUTPUT2_PZEM));

  // GPIO

  std::map<int32_t, Card*> pinout = {};
  _pinout(_pinDimmerO1, config.get(KEY_PIN_OUTPUT1_DIMMER).toInt(), pinout);
  _pinout(_pinDimmerO2, config.get(KEY_PIN_OUTPUT2_DIMMER).toInt(), pinout);
  _pinout(_pinDisplayClock, config.get(KEY_PIN_DISPLAY_SCL).toInt(), pinout);
  _pinout(_pinDisplayData, config.get(KEY_PIN_DISPLAY_SDA).toInt(), pinout);
  _pinout(_pinDS18O1, config.get(KEY_PIN_OUTPUT1_DS18).toInt(), pinout);
  _pinout(_pinDS18O2, config.get(KEY_PIN_OUTPUT2_DS18).toInt(), pinout);
  _pinout(_pinDS18Router, config.get(KEY_PIN_ROUTER_DS18).toInt(), pinout);
  _pinout(_pinJsyRX, config.get(KEY_PIN_JSY_RX).toInt(), pinout);
  _pinout(_pinJsyTX, config.get(KEY_PIN_JSY_RT).toInt(), pinout);
  _pinout(_pinLEDGreen, config.get(KEY_PIN_LIGHTS_GREEN).toInt(), pinout);
  _pinout(_pinLEDRed, config.get(KEY_PIN_LIGHTS_RED).toInt(), pinout);
  _pinout(_pinLEDYellow, config.get(KEY_PIN_LIGHTS_YELLOW).toInt(), pinout);
  _pinout(_pinPZEMRX, config.get(KEY_PIN_PZEM_RX).toInt(), pinout);
  _pinout(_pinPZEMTX, config.get(KEY_PIN_PZEM_TX).toInt(), pinout);
  _pinout(_pinRelay1, config.get(KEY_PIN_RELAY1).toInt(), pinout);
  _pinout(_pinRelay2, config.get(KEY_PIN_RELAY2).toInt(), pinout);
  _pinout(_pinRelayO1, config.get(KEY_PIN_OUTPUT1_RELAY).toInt(), pinout);
  _pinout(_pinRelayO2, config.get(KEY_PIN_OUTPUT2_RELAY).toInt(), pinout);
  _pinout(_pinZCD, config.get(KEY_PIN_ZCD).toInt(), pinout);
  pinout.clear();

  // Hardware
  _status(_display, KEY_ENABLE_DISPLAY, display.isEnabled());
  _status(_led, KEY_ENABLE_LIGHTS, lights.isEnabled());
  _status(_output1Relay, KEY_ENABLE_OUTPUT1_RELAY, bypassRelayO1.isEnabled());
  _status(_output2Relay, KEY_ENABLE_OUTPUT2_RELAY, bypassRelayO2.isEnabled());
  _status(_relay1, KEY_ENABLE_RELAY1, relay1.isEnabled());
  _status(_relay2, KEY_ENABLE_RELAY2, relay2.isEnabled());

  // Hardware (config)
  _gridFreq.update(config.get(KEY_GRID_FREQUENCY).toInt() == 60 ? "60 Hz" : "50 Hz", "50 Hz,60 Hz");
  _gridVolt.update(config.get(KEY_GRID_VOLTAGE).toInt() == 110 ? "110 V" : "220 V", "110 V,220 V");
  _output1RelayType.update(config.get(KEY_OUTPUT1_RELAY_TYPE), "NO,NC");
  _output2RelayType.update(config.get(KEY_OUTPUT2_RELAY_TYPE), "NO,NC");
  _relay1Type.update(config.get(KEY_RELAY1_TYPE), "NO,NC");
  _relay2Type.update(config.get(KEY_RELAY2_TYPE), "NO,NC");
  _displayType.update(config.get(KEY_DISPLAY_TYPE), "SH1106,SH1107,SSD1306");
  _displaySpeed.update(static_cast<int>(config.get(KEY_DISPLAY_SPEED).toInt()));
  _displayRotation.update(config.get(KEY_DISPLAY_ROTATION) + "°", "0°,90°,180°,270°");

  _output1RelayType.setDisplay(config.getBool(KEY_ENABLE_OUTPUT1_RELAY));
  _output2RelayType.setDisplay(config.getBool(KEY_ENABLE_OUTPUT2_RELAY));
  _relay1Type.setDisplay(config.getBool(KEY_ENABLE_RELAY1));
  _relay2Type.setDisplay(config.getBool(KEY_ENABLE_RELAY2));
  _displayType.setDisplay(config.getBool(KEY_ENABLE_DISPLAY));
  _displayRotation.setDisplay(config.getBool(KEY_ENABLE_DISPLAY));
  _output1PZEMSync.setDisplay(config.getBool(KEY_ENABLE_OUTPUT1_PZEM));
  _output2PZEMSync.setDisplay(config.getBool(KEY_ENABLE_OUTPUT2_PZEM));

  // mqtt (config)
  _haDiscovery.update(config.getBool(KEY_ENABLE_HA_DISCOVERY));
  _haDiscoveryTopic.update(config.get(KEY_HA_DISCOVERY_TOPIC));
  _mqttGridPower.update(config.get(KEY_GRID_POWER_MQTT_TOPIC));
  _mqttGridVoltage.update(config.get(KEY_GRID_VOLTAGE_MQTT_TOPIC));
  _mqttPort.update(config.get(KEY_MQTT_PORT));
  _mqttPublishInterval.update(config.get(KEY_MQTT_PUBLISH_INTERVAL));
  _mqttPwd.update(config.get(KEY_MQTT_PASSWORD).isEmpty() ? "" : HIDDEN_PWD);
  _mqttSecured.update(config.getBool(KEY_MQTT_SECURED));
  _mqttServer.update(config.get(KEY_MQTT_SERVER));
  _mqttServerCert.update("/api/config/mqttServerCertificate");
  _mqttTopic.update(config.get(KEY_MQTT_TOPIC));
  _mqttUser.update(config.get(KEY_MQTT_USERNAME));
  _mqttConfigTab.setDisplay(config.getBool(KEY_ENABLE_MQTT));

  // network (config)
  _adminPwd.update(config.get(KEY_ADMIN_PASSWORD).isEmpty() ? "" : HIDDEN_PWD);
  _apMode.update(config.getBool(KEY_ENABLE_AP_MODE));
  _ntpServer.update(config.get(KEY_NTP_SERVER));
  _ntpTimezone.update(config.get(KEY_NTP_TIMEZONE), "/timezones");
  _wifiPwd.update(config.get(KEY_WIFI_PASSWORD).isEmpty() ? "" : HIDDEN_PWD);
  _wifiSSID.update(config.get(KEY_WIFI_SSID));
#endif
}

void YaSolR::WebsiteClass::updateCards() {
  // stats
  Mycila::SystemMemory memory = Mycila::System.getMemory();
  ESPConnectMode mode = ESPConnect.getMode();
  _output1RelaySwitchCount.set(String(bypassRelayO1.getSwitchCount()).c_str());
  _output2RelaySwitchCount.set(String(bypassRelayO2.getSwitchCount()).c_str());
  _deviceHeapUsage.set((String(memory.usage) + " %").c_str());
  _deviceHeapUsed.set((String(memory.used) + " bytes").c_str());
  _gridEnergy.set((String(Mycila::Grid.getActiveEnergy(), 3) + " kWh").c_str());
  _gridEnergyReturned.set((String(Mycila::Grid.getActiveEnergyReturned(), 3) + " kWh").c_str());
  _gridFrequency.set((String(Mycila::Grid.getFrequency()) + " Hz").c_str());
  _gridVoltage.set((String(Mycila::Grid.getVoltage()) + " V").c_str());
  _networkAPIP.set(ESPConnect.getIPAddress(ESPConnectMode::AP).toString().c_str());
  _networkEthIP.set(ESPConnect.getIPAddress(ESPConnectMode::ETH).toString().c_str());
  _networkInterface.set(mode == ESPConnectMode::AP ? "AP" : (mode == ESPConnectMode::STA ? "WiFi" : (mode == ESPConnectMode::ETH ? "Ethernet" : "")));
  _networkWiFiIP.set(ESPConnect.getIPAddress(ESPConnectMode::STA).toString().c_str());
  _networkWiFiRSSI.set((String(ESPConnect.getWiFiRSSI()) + " dBm").c_str());
  _networkWiFiSignal.set((String(ESPConnect.getWiFiSignalQuality()) + " %").c_str());
  _networkWiFiSSID.set(ESPConnect.getWiFiSSID().c_str());
  _relay1SwitchCount.set(String(relay1.getSwitchCount()).c_str());
  _relay2SwitchCount.set(String(relay2.getSwitchCount()).c_str());
  _time.set(Mycila::Time::getLocalStr().c_str());
  _uptime.set(Mycila::Time::toDHHMMSS(Mycila::System.getUptime()).c_str());
#ifdef APP_MODEL_TRIAL
  _trialRemainingTime.set(Mycila::Time::toDHHMMSS(Mycila::Trial.getRemaining()).c_str());
#endif

  // home
  _routerPower.update(Mycila::Router.getTotalRoutedPower());
  _routerPowerFactor.update(Mycila::Router.getTotalPowerFactor());
  _routerTHDi.update(Mycila::Router.getTotalTHDi() * 100);
  _routerEnergy.update(Mycila::Router.getTotalRoutedEnergy());
  _gridPower.update(Mycila::Grid.getActivePower());
  _temperature(_routerDS18State, ds18Sys);

#ifdef APP_MODEL_PRO
  // Output 1 (status)
  switch (output1.getState()) {
    case Mycila::RouterOutputState::OUTPUT_DISABLED:
    case Mycila::RouterOutputState::OUTPUT_IDLE:
      _output1State.update(output1.getStateString(), DASH_STATUS_IDLE);
      break;
    case Mycila::RouterOutputState::OUTPUT_BYPASS_AUTO:
    case Mycila::RouterOutputState::OUTPUT_BYPASS_MANUAL:
      _output1State.update(output1.getStateString(), DASH_STATUS_WARNING);
      break;
    case Mycila::RouterOutputState::OUTPUT_ROUTING:
      _output1State.update(output1.getStateString(), DASH_STATUS_SUCCESS);
      break;
    default:
      _output1State.update("Unknown", DASH_STATUS_DANGER);
      break;
  }
  _temperature(_output1DS18State, ds18O1);
  _output1DimmerSlider.update(static_cast<int>(dimmerO1.getLevel()));
  _output1DimmerSliderRO.update(static_cast<int>(dimmerO1.getLevel()));
  _output1Bypass.update(output1.isBypassOn());
  _output1BypassRO.update(YASOLR_STATE(output1.isBypassOn()), output1.isBypassOn() ? DASH_STATUS_SUCCESS : DASH_STATUS_IDLE);
  _output1Power.update(output1.getActivePower());
  _output1ApparentPower.update(output1.getApparentPower());
  _output1PowerFactor.update(output1.getPowerFactor());
  _output1THDi.update(output1.getTHDi() * 100);
  _output1Voltage.update(output1.getOutputVoltage());
  _output1Current.update(output1.getCurrent());
  _output1Resistance.update(output1.getResistance());
  _output1Energy.update(output1.getEnergy());

  // output 2
  switch (output2.getState()) {
    case Mycila::RouterOutputState::OUTPUT_DISABLED:
    case Mycila::RouterOutputState::OUTPUT_IDLE:
      _output2State.update(output2.getStateString(), DASH_STATUS_IDLE);
      break;
    case Mycila::RouterOutputState::OUTPUT_BYPASS_AUTO:
    case Mycila::RouterOutputState::OUTPUT_BYPASS_MANUAL:
      _output2State.update(output2.getStateString(), DASH_STATUS_WARNING);
      break;
    case Mycila::RouterOutputState::OUTPUT_ROUTING:
      _output2State.update(output2.getStateString(), DASH_STATUS_SUCCESS);
      break;
    default:
      _output2State.update("Unknown", DASH_STATUS_DANGER);
      break;
  }
  _temperature(_output2DS18State, ds18O2);
  _output2DimmerSlider.update(static_cast<int>(dimmerO2.getLevel()));
  _output2DimmerSliderRO.update(static_cast<int>(dimmerO2.getLevel()));
  _output2Bypass.update(output2.isBypassOn());
  _output2BypassRO.update(YASOLR_STATE(output1.isBypassOn()), output2.isBypassOn() ? DASH_STATUS_SUCCESS : DASH_STATUS_IDLE);
  _output2Power.update(output2.getActivePower());
  _output2ApparentPower.update(output2.getApparentPower());
  _output2PowerFactor.update(output2.getPowerFactor());
  _output2THDi.update(output2.getTHDi() * 100);
  _output2Voltage.update(output2.getOutputVoltage());
  _output2Current.update(output2.getCurrent());
  _output2Resistance.update(output2.getResistance());
  _output2Energy.update(output2.getEnergy());

  // relays
  _relay1Switch.update(relay1.isOn());
  _relay1SwitchRO.update(YASOLR_STATE(relay1.isOn()), relay1.isOn() ? DASH_STATUS_SUCCESS : DASH_STATUS_IDLE);
  _relay2Switch.update(relay2.isOn());
  _relay2SwitchRO.update(YASOLR_STATE(relay2.isOn()), relay2.isOn() ? DASH_STATUS_SUCCESS : DASH_STATUS_IDLE);

  // Hardware (status)
  const bool gridOnline = Mycila::Grid.isConnected();
  _status(_jsy, KEY_ENABLE_JSY, jsy.isEnabled(), jsy.isConnected(), "No electricity");
  _status(_mqtt, KEY_ENABLE_MQTT, mqtt.isEnabled(), mqtt.isConnected(), mqtt.getLastError() ? mqtt.getLastError() : "Disconnected");
  _status(_output1Dimmer, KEY_ENABLE_OUTPUT1_DIMMER, dimmerO1.isEnabled(), gridOnline, "No electricity");
  _status(_output1DS18, KEY_ENABLE_OUTPUT1_DS18, ds18O1.isEnabled(), ds18O1.getLastTime() > 0, "Read error");
  _status(_output1PZEM, KEY_ENABLE_OUTPUT1_PZEM, pzemO1.isEnabled(), pzemO1.isConnected(), "No electricity");
  _status(_output2Dimmer, KEY_ENABLE_OUTPUT2_DIMMER, dimmerO2.isEnabled(), gridOnline, "No electricity");
  _status(_output2DS18, KEY_ENABLE_OUTPUT2_DS18, ds18O2.isEnabled(), ds18O2.getLastTime() > 0, "Read error");
  _status(_output2PZEM, KEY_ENABLE_OUTPUT2_PZEM, pzemO2.isEnabled(), pzemO2.isConnected(), "No electricity");
  _status(_routerDS18, KEY_ENABLE_DS18_SYSTEM, ds18Sys.isEnabled(), ds18Sys.getLastTime() > 0, "Read error");
  _status(_zcd, KEY_ENABLE_ZCD, Mycila::ZCD.isEnabled(), Mycila::ZCD.isConnected(), "No electricity");
#endif
}

void YaSolR::WebsiteClass::_sliderConfig(Card& card, const char* key) {
  card.attachCallback([key, &card](int value) {
    config.set(key, String(MAX(0, value)));
    card.update(static_cast<int>(config.get(key).toInt()));
    dashboard.refreshCard(&card);
  });
}

void YaSolR::WebsiteClass::_numConfig(Card& card, const char* key) {
#ifdef APP_MODEL_PRO
  card.attachCallback([key, &card](const char* value) {
    if (strlen(value) == 0) {
      config.unset(key);
    } else {
      config.set(key, String(strtol(value, nullptr, 10)));
    }
    card.update(static_cast<int>(config.get(key).toInt()));
    dashboard.refreshCard(&card);
  });
#endif
}

void YaSolR::WebsiteClass::_pinConfig(Card& card, const char* key) {
#ifdef APP_MODEL_PRO
  card.attachCallback([key, &card, this](const char* value) {
    if (strlen(value) == 0) {
      config.unset(key);
    } else {
      config.set(key, String(strtol(value, nullptr, 10)));
    }
    initCards();
    dashboard.refreshCard(&card);
  });
#endif
}

void YaSolR::WebsiteClass::_boolConfig(Card& card, const char* key) {
  card.attachCallback([key, &card, this](int value) {
    config.setBool(key, value);
    card.update(static_cast<int>(config.getBool(key)));
    dashboard.refreshCard(&card);
  });
}

void YaSolR::WebsiteClass::_textConfig(Card& card, const char* key) {
#ifdef APP_MODEL_PRO
  card.attachCallback([key, &card](const char* value) {
    config.set(key, value);
    card.update(config.get(key));
    dashboard.refreshCard(&card);
  });
#endif
}

void YaSolR::WebsiteClass::_daysConfig(Card& card, const char* key) {
#ifdef APP_MODEL_PRO
  card.attachCallback([key, &card, this](const char* value) {
    config.set(key, strlen(value) == 0 ? "none" : value);
    card.update(config.get(key));
    dashboard.refreshCard(&card);
  });
#endif
}

void YaSolR::WebsiteClass::_passwordConfig(Card& card, const char* key) {
#ifdef APP_MODEL_PRO
  card.attachCallback([key, &card, this](const char* value) {
    if (strlen(value) == 0) {
      config.unset(key);
    } else if (strlen(value) >= 8) {
      config.set(key, value);
    }
    card.update(config.get(key).isEmpty() ? "" : HIDDEN_PWD);
    dashboard.refreshCard(&card);
  });
#endif
}

void YaSolR::WebsiteClass::_relaySwitch(Card& card, const char* relayName) {
  card.attachCallback([&card, relayName, this](int value) {
    const Mycila::Relay& relay = Mycila::RelayManager.relay(relayName);
    if (relay.isEnabled()) {
      Mycila::RelayManager.tryRelayState(relayName, value);
    }
    card.update(relay.isOn());
    dashboard.refreshCard(&card);
  });
}

void YaSolR::WebsiteClass::_outputBypassSwitch(Card& card, Mycila::RouterOutput& output) {
  card.attachCallback([&card, &output, this](int value) {
    if (output.isBypassRelayEnabled()) {
      output.tryBypassRelayState(value);
    }
    card.update(output.isBypassOn());
    dashboard.refreshCard(&card);
    dashboardTask.requestEarlyRun();
  });
}

void YaSolR::WebsiteClass::_outputDimmerSlider(Card& card, Mycila::RouterOutput& output) {
  card.attachCallback([&card, &output, this](int value) { // 0-100
    if (output.dimmer().isEnabled()) {
      output.tryDimmerLevel(value);
    }
    card.update(static_cast<int>(output.dimmer().getLevel()));
    dashboard.refreshCard(&card);
    dashboardTask.requestEarlyRun();
  });
}

void YaSolR::WebsiteClass::_temperature(Card& card, Mycila::DS18& sensor) {
  if (!sensor.isEnabled()) {
    card.update("Disabled", "");
  } else if (sensor.getLastTime() == 0) {
    card.update("Pending...", "");
  } else {
    card.update(sensor.getLastTemperature(), "°C");
  }
}

void YaSolR::WebsiteClass::_status(Card& card, const char* key, bool enabled, bool active, const char* err) {
  const bool configEnabled = config.getBool(key);
  if (!configEnabled)
    card.update(config.getBool(key), DASH_STATUS_IDLE ",Disabled");
  else if (!enabled)
    card.update(config.getBool(key), DASH_STATUS_DANGER ",Failed to start");
  else if (!active)
    card.update(config.getBool(key), (String(DASH_STATUS_WARNING) + "," + err).c_str());
  else
    card.update(config.getBool(key), DASH_STATUS_SUCCESS ",Enabled");
}

void YaSolR::WebsiteClass::_pinout(Card& card, int32_t pin, std::map<int32_t, Card*>& pinout) {
  if (pin == GPIO_NUM_NC) {
    card.update("Disabled", DASH_STATUS_IDLE);
  } else if (pinout.find(pin) != pinout.end()) {
    String v = String(pin) + " (Duplicate)";
    pinout[pin]->update(v, DASH_STATUS_DANGER);
    card.update(v, DASH_STATUS_DANGER);
  } else if (!GPIO_IS_VALID_GPIO(pin)) {
    pinout[pin] = &card;
    card.update(String(pin) + " (Invalid)", DASH_STATUS_DANGER);
  } else if (!GPIO_IS_VALID_OUTPUT_GPIO(pin)) {
    pinout[pin] = &card;
    card.update(String(pin) + " (Input Only)", DASH_STATUS_WARNING);
  } else {
    pinout[pin] = &card;
    card.update(String(pin) + " (I/O)", DASH_STATUS_SUCCESS);
  }
}

namespace YaSolR {
  WebsiteClass Website;
} // namespace YaSolR
