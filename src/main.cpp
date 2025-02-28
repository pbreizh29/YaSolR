// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#include <yasolr.h>

void setup() {
  yasolr_boot();         // boot sequence
  yasolr_init_config();  // load configuration from NVS
  yasolr_init_logging(); // init logging
  yasolr_init_system();  // init system (safeboot, restart, reset, etc)

  // hardware
  yasolr_init_display();
  yasolr_init_ds18();
  yasolr_init_grid();
  yasolr_init_jsy();
  yasolr_init_jsy_remote();
  yasolr_init_lights();
  yasolr_init_mqtt();
  yasolr_init_network();
  yasolr_init_pzem();
  yasolr_init_relays();
  yasolr_init_router();
  yasolr_init_trial();
  yasolr_init_web_server();

  // core task manager
  assert(coreTaskManager.asyncStart(512 * 7, 5, 1, 100, true));

  // task manager for long running tasks like mqtt / pzem
  if (unsafeTaskManager.tasks())
    assert(unsafeTaskManager.asyncStart(512 * 8, 1, 1, 100, false));

  // STARTUP READY!
  logger.info(TAG, "Started %s", Mycila::AppInfo.nameModelVersion.c_str());
}

// Destroy default Arduino async task
void loop() { vTaskDelete(NULL); }

// #include "esp_heap_caps.h"
// void esp_heap_trace_alloc_hook(void* ptr, size_t size, uint32_t caps) {
//   if (size >= 1024 && caps == 6144) {
//     ets_printf("alloc: %p %d\n", ptr, size);
//     if (size > 2308) {
//       assert(false);
//     }
//   }
// }
// void esp_heap_trace_free_hook(void* ptr) {
//   // ets_printf("free: %p\n", ptr);
// }
