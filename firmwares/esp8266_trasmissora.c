#include <ESP8266WiFi.h>
#include <espnow.h>

#define PINO_SENSOR D1
uint8_t enderecoDestino[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // Utilize o código mac_address.c para obter o mac e substitua nos conjuntos 00
int estadoAnterior = -1;

void quandoEnviar(uint8_t *mac, uint8_t status) {
    Serial.println(status == 0 ? "Sucesso" : "Falha");
}

void setup() {
    Serial.begin(115200);
    pinMode(PINO_SENSOR, INPUT);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0) {
        Serial.println("Erro ao iniciar ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(enderecoDestino, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_register_send_cb(quandoEnviar);
}

void loop() {
    int estadoAtual = digitalRead(PINO_SENSOR);
    if (estadoAtual != estadoAnterior) {
        esp_now_send(enderecoDestino, (uint8_t *)&estadoAtual, sizeof(estadoAtual));
        estadoAnterior = estadoAtual;
    }
    delay(100);
}
