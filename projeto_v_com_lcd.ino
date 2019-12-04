/*                  CONEXÕES
  PINOS DO ARDUINO
  VCC------------------------------ Pino K  (16) DISPLAY
  GND------------------------------ Pino A  (15) DISPLAY
        D13------------------------ Pino D7 (14) DISPLAY
        D12------------------------ Pino D6 (13) DISPLAY
        D11------------------------ Pino D5 (12) DISPLAY
        D10------------------------ Pino D4 (11) DISPLAY
        D9------------------------- Pino E  (6)  DISPLAY
  GND------------------------------ Pino RW (5)  DISPLAY
        D8------------------------- Pino RS (4)  DISPLAY                              GND--|
                                    Pino VO (3)  DISPLAY----Pino central de Potenciômetro--| Controle de contraste
  VCC------------------------------ Pino VDD(2)  DISPLAY                              VCC--|
  GND------------------------------ Pino VSS(1)  DISPLAY

  A0-  D5-------------------------- Led 2
  |    D4-------------------------- Buzzer
  |    D3-------------------------- LED 1 
  |    D2-------------------------- Pino data DH11
  |
  |______________________________MQ2
*/
#include <DHT.h> //https://github.com/adafruit/DHT-sensor-library
#include <LiquidCrystal.h>
LiquidCrystal lcd(8 , 9, 10, 11, 12, 13);
DHT sensor_dht(2, DHT11);
int mq2_pin = A0;
int tempo = 5000;
String dados;

void setup() {

  lcd.begin(16, 2);   //(16 colunas, 2 linhas)
  Serial.begin(9600);
  sensor_dht.begin();
  pinMode(3, OUTPUT); // led 1 (blink
  pinMode(5, OUTPUT); // led 2 (Indicador de detecção)
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  lcd.setCursor(0, 0); lcd.print("Aguarde...");
  delay(2000);
  lcd.clear();
}
void loop() {

  int leitura_mq2 = analogRead(mq2_pin);
  float umidade = sensor_dht.readHumidity();
  int temperatura = sensor_dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    lcd.setCursor(0, 0);
    lcd.print("Falha de leitura");
    lcd.setCursor(0, 1);
    lcd.print("no sensor DHT11");
  }
  else {

    // Informações enviadas pela porta serial
    dados = String(umidade) + "|" + String(temperatura) + "|" + String(leitura_mq2);
    Serial.println(dados);
    digitalWrite(3, LOW);
    delay(tempo);
    //Exibindo informações no display
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Umi:"); lcd.setCursor(4,  0);  lcd.print(int(umidade));  lcd.setCursor(6, 0);  lcd.print("%");
    lcd.setCursor(8, 0); lcd.print("Tem:"); lcd.setCursor(12, 0);  lcd.print(temperatura);   lcd.setCursor(14, 0); lcd.print((char)223); lcd.setCursor(15, 0); lcd.print("C");
    lcd.setCursor(0, 1); lcd.print("MQ2:"); lcd.setCursor(4,  1);  lcd.print(leitura_mq2);
    digitalWrite(3, HIGH);
  }
  if (leitura_mq2 >= 350) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Gas detectado"); lcd.setCursor(0, 1); lcd.print("MQ2:"); lcd.setCursor(4, 1); lcd.print(leitura_mq2);
    digitalWrite(5 , HIGH);
    for (int freq = 40; freq <= 440; freq++) { // frequência crescente 40hz-440hz
      tone(4, freq);
      delay(1);
    }
  }
  else {
    digitalWrite(5, LOW);
    noTone(4);
  }
}
