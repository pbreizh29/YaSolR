# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2023-2025 Mathieu Carbou
#
#!/usr/bin/env bash

if [ "$1" == "" ]; then
  echo "Usage: $0 <host>"
  exit 1
fi
HOST="$1"
DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
source "${DIR}/utils/api.sh"

assert "$(keys /api/config)" "admin_pwd,ap_mode_enable,debug_enable,pin_i2c_scl,pin_i2c_sda,disp_enable,disp_angle,disp_type,grid_freq,grid_pow_mqtt,ha_disco_enable,ha_disco_topic,hostname,jsy_enable,pin_jsy_rx,pin_jsy_tx,lights_enable,pin_lights_g,pin_lights_r,pin_lights_y,mqtt_enable,mqtt_port,mqtt_pub_itvl,mqtt_pwd,mqtt_secure,mqtt_server,mqtt_topic,mqtt_user,ntp_server,ntp_timezone,o1_ab_enable,o1_relay_enable,pin_o1_relay,o1_relay_type,o1_ad_enable,o1_dim_enable,o1_dim_limit,pin_o1_dim,o1_time_stop,o1_time_start,o1_ds18_enable,o1_temp_stop,o1_temp_start,pin_o1_ds18,o1_days,o2_ab_enable,o2_relay_enable,pin_o2_relay,o2_relay_type,o2_ad_enable,o2_dim_enable,o2_dim_limit,pin_o2_dim,o2_time_stop,o2_time_start,o2_ds18_enable,o2_temp_stop,o2_temp_start,pin_o2_ds18,o2_days,portal_timeout,relay1_enable,pin_relay1,relay1_load,relay1_type,relay2_enable,pin_relay2,relay2_load,relay2_type,ds18_sys_enable,pin_ds18,wifi_pwd,wifi_ssid,wifi_timeout,zcd_enable,pin_zcd"
assert "$(value /api/config 'admin_pwd')" ""
assert "$(value /api/config 'ap_mode_enable')" "false"
assert "$(value /api/config 'debug_enable')" "true"
assert "$(value /api/config 'pin_i2c_scl')" "22"
assert "$(value /api/config 'pin_i2c_sda')" "21"
assert "$(value /api/config 'disp_enable')" "true"
assert "$(value /api/config 'disp_angle')" "0"
assert "$(value /api/config 'disp_type')" "SH1106"
assert "$(value /api/config 'grid_pow_mqtt')" "homeassistant/states/sensor/grid_power/state"
assert "$(value /api/config 'ha_disco_enable')" "true"
assert "$(value /api/config 'ha_disco_topic')" "homeassistant/discovery"
assert "$(value /api/config 'hostname')" "yasolr-a1c48"
assert "$(value /api/config 'jsy_enable')" "true"
assert "$(value /api/config 'pin_jsy_rx')" "17"
assert "$(value /api/config 'pin_jsy_tx')" "16"
assert "$(value /api/config 'lights_enable')" "true"
assert "$(value /api/config 'pin_lights_g')" "0"
assert "$(value /api/config 'pin_lights_r')" "15"
assert "$(value /api/config 'pin_lights_y')" "2"
assert "$(value /api/config 'mqtt_enable')" "true"
assert "$(value /api/config 'mqtt_port')" "1883"
assert "$(value /api/config 'mqtt_pub_itvl')" "5"
assert "$(value /api/config 'mqtt_pwd')" "********"
assert "$(value /api/config 'mqtt_secure')" "false"
assert "$(value /api/config 'mqtt_user')" "homeassistant"
assert "$(value /api/config 'ntp_server')" "pool.ntp.org"
assert "$(value /api/config 'ntp_timezone')" "Europe/Paris"
assert "$(value /api/config 'o1_ab_enable')" "true", "false"
assert "$(value /api/config 'o1_relay_enable')" "true"
assert "$(value /api/config 'pin_o1_relay')" "32"
assert "$(value /api/config 'o1_relay_type')" "NO"
assert "$(value /api/config 'o1_dim_enable')" "true"
assert "$(value /api/config 'o1_dim_limit')" "100"
assert "$(value /api/config 'pin_o1_dim')" "25"
assert "$(value /api/config 'o1_time_stop')" "06:00"
assert "$(value /api/config 'o1_time_start')" "22:00"
assert "$(value /api/config 'o1_ds18_enable')" "true"
assert "$(value /api/config 'o1_temp_stop')" "60"
assert "$(value /api/config 'o1_temp_start')" "50"
assert "$(value /api/config 'pin_o1_ds18')" "18"
assert "$(value /api/config 'o1_days')" "sun,mon,tue,wed,thu,fri,sat"
assert "$(value /api/config 'o2_ab_enable')" "true"
assert "$(value /api/config 'o2_relay_enable')" "true"
assert "$(value /api/config 'pin_o2_relay')" "33"
assert "$(value /api/config 'o2_relay_type')" "NO"
assert "$(value /api/config 'o2_dim_enable')" "true"
assert "$(value /api/config 'o2_dim_limit')" "100"
assert "$(value /api/config 'pin_o2_dim')" "26"
assert "$(value /api/config 'o2_time_stop')" "06:00"
assert "$(value /api/config 'o2_time_start')" "22:00"
assert "$(value /api/config 'o2_ds18_enable')" "true"
assert "$(value /api/config 'o2_temp_stop')" "60"
assert "$(value /api/config 'o2_temp_start')" "50"
assert "$(value /api/config 'pin_o2_ds18')" "5"
assert "$(value /api/config 'o2_days')" "sun,mon,tue,wed,thu,fri,sat"
assert "$(value /api/config 'portal_timeout')" "180"
assert "$(value /api/config 'relay1_enable')" "true"
assert "$(value /api/config 'pin_relay1')" "13"
assert "$(value /api/config 'relay1_type')" "NO"
assert "$(value /api/config 'relay2_enable')" "true"
assert "$(value /api/config 'pin_relay2')" "12"
assert "$(value /api/config 'relay2_type')" "NO"
assert "$(value /api/config 'ds18_sys_enable')" "true"
assert "$(value /api/config 'pin_ds18')" "4"
assert "$(value /api/config 'wifi_pwd')" ""
assert "$(value /api/config 'wifi_ssid')" "IoT"
assert "$(value /api/config 'wifi_timeout')" "15"
assert "$(value /api/config 'zcd_enable')" "true"
assert "$(value /api/config 'pin_zcd')" "35"
check "$(value /api/config 'grid_freq')" "!=60"

assert "$(body /api/config/backup | wc -l | xargs)" "84"

echo "===================================================================="
echo "SUCCESS: $0"
echo "===================================================================="
