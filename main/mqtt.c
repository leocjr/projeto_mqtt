#include "mqtt.h"

// Variaveis
bool mqtt_connected = false; // flag de conexão
const char *TAG = "mqtt_example";
esp_mqtt_client_handle_t client = NULL;


void log_error_if_nonzero(const char *message, int error_code) {
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}


bool mqtt_client_connected(void) {
    return mqtt_connected;
}


// publish_payload publicar mensagens
bool publish_payload(const char* payload_content, const char* topic) {
    if (client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return false;
    }

    int msg_id = esp_mqtt_client_publish(
        client,                    // cliente MQTT
        topic,                     // tópico
        payload_content,     // conteúdo
        0,                    // tamanho (0 = calcula automaticamente para string)
        1,                    // QoS 1
        0                  // retain = false
    );

    if (msg_id < 0) {
        ESP_LOGI(TAG, "Error while trying to publish message, msg_id=%d", msg_id);
        return false;
    }

    ESP_LOGI(TAG, "Message published successfully, msg_id=%d", msg_id);
    return true;
}

Publish mensagem = {
    .topic = "topic/test",
    .payload = "{Testando...}"
};

/*void publish_task(void *pvParameter) {
    while(1) {
        // publicando a mensagem global 'mensagem'
        publish_payload(mensagem.payload, mensagem.topic);

        // espera 5 segundos antes de publicar novamente
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}*/

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            mqtt_connected = true; // atualiza flag
            ESP_LOGI(TAG, "MQTT conectado");
            ESP_LOGI(TAG, "Flag de conexão: %s", mqtt_connected ? "true" : "false");

            msg_id = esp_mqtt_client_subscribe(client, "topic/test", 0);
            ESP_LOGI(TAG, "Subscribed, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            mqtt_connected = false; // atualiza flag
            ESP_LOGI(TAG, "MQTT desconectado");
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Mensagem recebida");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "Inscrito no tópico, msg_id=%d", event->msg_id);
            break;

        default:
            ESP_LOGI(TAG, "Outro evento MQTT: %d", event_id);
            break;
        }
}

void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_3_1_1,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}