#pragma once
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

// Config Wi-Fi
#define WIFI_SSID "AT_SoftwareSolutions_2G"
#define WIFI_PASS "123at456at"

// Controle de eventos
extern EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
extern const char *TAG;

void wifi_init_sta(void);