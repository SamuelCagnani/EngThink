#include <ESP8266WiFi.h>
#include <espnow.h>

#define LASER D1

void quandoReceber(uint8_t *mac, uint8_t *dados, uint8_t tamanho) {
    int estadoRecebido;
    memcpy(&estadoRecebido, dados, sizeof(estadoRecebido));
    digitalWrite(LASER, estadoRecebido);
}

void setup() {
    Serial.begin(115200);
    pinMode(LASER, OUTPUT);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0) {
        Serial.println("Erro ao iniciar ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(quandoReceber);
}

void loop() {
    delay(100);
}
