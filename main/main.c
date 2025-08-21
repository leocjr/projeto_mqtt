#include "wifi.h"
#include "mqtt.h"

extern Publish mensagem;

void publish_task_main(void *pvParameter) {
    // Espera até que o MQTT esteja conectado
    while(!mqtt_connected) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    ESP_LOGI("PUBLISH_TASK", "MQTT conectado, iniciando publicação");

    while(1) {
        publish_payload(mensagem.payload, mensagem.topic);
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 segundos
    }
}

void app_main(void){

    // Inicializa NVS
    ESP_ERROR_CHECK(nvs_flash_init());

    // Inicializa Wi-Fi
    wifi_init_sta();

    // Inicializa MQTT
    mqtt_app_start();

    // Cria a task de publicação
    xTaskCreate(publish_task_main, "publish_task_main", 4096, NULL, 5, NULL);
    
}