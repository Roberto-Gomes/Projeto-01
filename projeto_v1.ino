
#include <DHT.h> //https://github.com/adafruit/DHT-sensor-library
#include <LiquidCrystal.h>

//pinos do display( RS  E  D4  D5  D6  D7)
LiquidCrystal lcd(8 , 9, 10, 11, 12, 13); // pinos digitais do arduino

DHT sensor_dht(2, DHT11); //(pino ,tipo de sensor dht)
int mq2 = A0;
int tempo = 5000;
String dados;


void setup() {
  
  lcd.begin(16, 2); //16 colunas * 2 linhas
  Serial.begin(9600);
  sensor_dht.begin();
  pinMode(3, OUTPUT);// led de envio
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  lcd.setCursor(0, 0); lcd.print("Aguarde...");
  delay(2000);
  lcd.clear();
}

void loop() {

  int leitura_mq2 = analogRead(mq2);
  float h = sensor_dht.readHumidity();//Umidade
  int t = sensor_dht.readTemperature();//temperatura

  
  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.print("Falha de leitura");
    lcd.setCursor(0, 1);
    lcd.print("no sensor DHT11");
    return;
  }
  else {
    
    // Informações enviadas pela porta serial
    dados = String(h) + "|" + String(t) + "|" + String(leitura_mq2); 
    Serial.println(dados);
    digitalWrite(3, LOW);
    delay(tempo);
    //Imprimimdo informações no display
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Umi:"); lcd.setCursor(4,  0);  lcd.print(int(h));  lcd.setCursor(6, 0);  lcd.print("%");
    lcd.setCursor(8, 0); lcd.print("Tem:"); lcd.setCursor(12, 0);  lcd.print(t);       lcd.setCursor(14, 0); lcd.print((char)223); lcd.setCursor(15, 0); lcd.print("C");
    lcd.setCursor(0, 1); lcd.print("MQ2:"); lcd.setCursor(4,  1);  lcd.print(leitura_mq2);
    digitalWrite(3, HIGH);
  }

  if (leitura_mq2 >= 350) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Gas detectado"); lcd.setCursor(0, 1); lcd.print("MQ2:"); lcd.setCursor(4, 1); lcd.print(leitura_mq2);
    digitalWrite(5 , HIGH);
    for (int freq = 40; freq <= 440; freq++) {
      tone(4, freq);
      delay(1);
    }
  }
  else {
    digitalWrite(5, LOW);
    noTone(4);
  }
}
