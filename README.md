# Projeto de Segurança com Arduino e ESP8266

Este repositório contém o firmware para um sistema de segurança baseado em Arduino e ESP8266, onde um laser é ativado/desativado conforme a autenticação de uma senha. A comunicação entre os ESP8266 é feita via ESP-NOW.

## 📁 Estrutura do Repositório

```
ENGTHINK/
│── firmwares/
│   │── arduino.c                  # Código do Arduino para controle do sistema
│   │── esp8266_transmissora.c      # Código da ESP8266 transmissora
│   │── esp8266_receptora.c         # Código da ESP8266 receptora
│   │── mac_address.c               # Arquivo auxiliar para os endereços MAC
│── README.md                       # Documentação do projeto
```

## 🛠️ Hardware Utilizado
- **Arduino Uno** (ou similar)
- **ESP8266** (dois módulos)
- **Laser**
- **LDR** (Sensor de Luz)
- **Buzzer**
- **LEDs** (Vermelho e Verde)
- **Resistores de 10kΩ e 330Ω**
- **Protoboard e jumpers**

## 🔄 Funcionamento
1. O **Arduino** controla o sistema e envia um sinal digital (HIGH ou LOW) para a **ESP8266 transmissora**.
2. A **ESP8266 transmissora** recebe esse estado pelo pino digital e repassa via **ESP-NOW** para a **ESP8266 receptora**.
3. A **ESP8266 receptora** recebe o estado e ativa/desativa o laser.
4. Caso a senha correta seja digitada no Arduino, ele envia `LOW`, desligando o laser. Se o sistema for rearmado, o laser volta a ser ativado.

## 📜 Códigos

### 1️⃣ Arduino (`arduino.c`)
Este código gerencia o sistema de segurança, detectando a interrupção do laser e verificando a senha para ativar/desativar o alarme.

### 2️⃣ ESP8266 Transmissora (`esp8266_transmissora.c`)
Este código lê o estado do pino digital (recebido do Arduino) e transmite via ESP-NOW para a ESP8266 receptora.

### 3️⃣ ESP8266 Receptora (`esp8266_receptora.c`)
Este código recebe o estado via ESP-NOW e controla o laser, ativando ou desativando conforme o comando enviado pela transmissora.

### 4️⃣ Endereços MAC (`mac_address.c`)
Este arquivo contém os **endereços MAC das ESP8266**, que devem ser configurados corretamente para a comunicação ESP-NOW funcionar.

## 📌 Configuração

### 🔌 Ligações Físicas
- **Pino 6 do Arduino (ESP_TX)** → **D1 da ESP8266 Transmissora**
- **GND do Arduino** → **GND da ESP8266 Transmissora**
- **Resistor de 10kΩ entre D1 e GND** *(evita leituras flutuantes)*

### 📡 Configuração do ESP-NOW
1. Descobrir os endereços MAC das ESP8266:
   - Faça o upload de um código que imprime o MAC no `Serial Monitor`.
2. Atualizar o arquivo `mac_address.c` com os endereços corretos.
3. Compilar e carregar os códigos nas respectivas placas.

## 🚀 Como Usar
1. Conecte os dispositivos conforme o esquema.
2. Abra o **Monitor Serial** do Arduino e inicie o sistema (`s` para iniciar).
3. Se a senha for digitada corretamente, o laser será desativado.
4. Para rearmar, digite `s` no monitor serial.

## ⚠️ Possíveis Melhorias
- Implementar autenticação via RFID ou teclado numérico.
- Substituir o Arduino por um ESP32 para um sistema mais compacto.
- Integrar um banco de dados para armazenar logs de acesso.

## 📩 Contato
Caso tenha dúvidas ou sugestões, sinta-se à vontade para abrir uma **Issue** ou um **Pull Request**!

---
✍️ Desenvolvido por Samuel Cagnani

