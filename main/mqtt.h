#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "mqtt_client.h"  // header correto do ESP-IDF MQTT

// Configuração do broker
#define CONFIG_BROKER_URL "mqtt://192.168.18.201:1883"

// Struct para publicar mensagens
typedef struct {
    char topic[64];
    char payload[246];
} Publish;

// Variáveis globais acessíveis em outros arquivos
//extern esp_mqtt_client_handle_t client;
extern const char *TAG;
extern Publish mensagem;
extern bool mqtt_connected; // flag de conexão

// Funções MQTT
void mqtt_app_start(void);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

// Funções de publicação
bool publish_payload(const char* payload_content, const char* topic);
void publish_task(void *pvParameter);
