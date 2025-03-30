int LED_R = 10;  // LED Vermelho - Indica sistema armado
int LED_G = 11;  // LED Verde - Acesso permitido
int LASER = 7;   // Laser para ativação
int BUZZER = 9;  // Buzzer para alerta
int LDR = A4;    // Sensor de luz (fototransistor)
int ESP_TX = 6;  // Pino que envia sinal para a ESP transmissora
String senhaCorreta = "teste";
String senhaRecebida = "";
bool sistemaArmado = false;
bool alertaAtivo = false;
unsigned long tempoInicio = 0;

void setup() {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(LDR, INPUT);
    pinMode(ESP_TX, OUTPUT);
    digitalWrite(ESP_TX, LOW); // Começa desativado

    Serial.begin(9600);
    Serial.println("Iniciar projeto? (s/n)");
}

void loop() {
    int leituraLDR = analogRead(LDR);
    Serial.print("Luminosidade: ");
    Serial.println(leituraLDR);

    if (!sistemaArmado) {
        if (Serial.available()) {
            char resposta = Serial.read();
            if (resposta == 's' || resposta == 'S') {
                iniciarSistema();
            } else if (resposta == 'n' || resposta == 'N') {
                Serial.println("Sistema nao iniciado. Aguardando...");
                Serial.println("Iniciar projeto? (s/n)");
            }
        }
    } else {
        verificarLDR(leituraLDR);
        verificarSenha();
    }
    delay(500);
}

void iniciarSistema() {
    sistemaArmado = true;
    alertaAtivo = false;  
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW); 
    digitalWrite(LASER, HIGH);
    digitalWrite(ESP_TX, HIGH); // Envia 1 para ESP transmissora
    Serial.println("Sistema armado. Insira a senha:");

    if (tempoInicio == 0) {
        tempoInicio = millis();
    }
}

void verificarLDR(int leituraLDR) {
    if (leituraLDR < 800) {
        Serial.println("Alerta! Intrusao detectada!");
        alertaAtivo = true;
    } 

    if (alertaAtivo) {
        tone(BUZZER, 1000);
    } 

    if (leituraLDR >= 800) {  
        alertaAtivo = false;
        noTone(BUZZER);
    }
}

void verificarSenha() {
    if (Serial.available()) {
        senhaRecebida = Serial.readStringUntil('\n');
        senhaRecebida.trim();

        if (senhaRecebida == senhaCorreta) {
            unsigned long tempoAtual = millis() - tempoInicio;
            Serial.print("Acesso permitido! Tempo decorrido: ");
            Serial.print(tempoAtual / 1000);
            Serial.print("s ");
            Serial.print(tempoAtual % 1000);
            Serial.println("ms");

            digitalWrite(LED_R, LOW);
            digitalWrite(LED_G, HIGH);
            digitalWrite(LASER, LOW);
            digitalWrite(ESP_TX, LOW); // Envia 0 para ESP transmissora
            noTone(BUZZER);

            delay(2000);
            Serial.println("Rearmar sistema? (s/n)");

            while (true) {
                if (Serial.available()) {
                    char resposta = Serial.read();
                    if (resposta == 's' || resposta == 'S') {
                        iniciarSistema();
                        break;
                    } else if (resposta == 'n' || resposta == 'N') {
                        Serial.println("Sistema desativado.");
                        sistemaArmado = false;
                        digitalWrite(LED_G, HIGH);
                        digitalWrite(LASER, LOW);
                        digitalWrite(ESP_TX, LOW);
                        break;
                    }
                }
            }
        } else {
            Serial.println("Acesso negado!");
            Serial.println("Sistema armado. Insira a senha:");
            alertaErro();
        }
    }
}

void alertaErro() {
    for (int i = 0; i < 3; i++) {
        tone(BUZZER, 1000);
        delay(300);
        noTone(BUZZER);
        delay(300);
    }
}
