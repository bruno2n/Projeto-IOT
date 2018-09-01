#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <Firebase.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN D3
#define RST_PIN D4
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

#define FIREBASE_HOST "esp82666-e0f1d.firebaseio.com"
#define FIREBASE_AUTH "YR5c1cd7s2lSFn1TKKJ6nrUF77pRVWptNB8FuNO9"
#define WIFI_SSID "Casa"
#define WIFI_PASSWORD "C454.N0V0.1.2.3"

LiquidCrystal_I2C lcd(0x27, 16, 2);

int n=0;

void setup() {
 // Serial.begin(9600); // Inicia a serial
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
 
   // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(2000);
  //Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);
  //Serial.println();
  //Serial.print("connected: ");
  //Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("RFID/Contador", n++);

  lcd.begin();
  lcd.backlight();
  lcd.print("Aproxime cartao!");
  
  //Serial.println("Aproxime o seu cartao do leitor...");
  //Serial.println();
}

void loop() {
  
  Firebase.setBool("RFID/Leitura", false);
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  //Mostra UID na serial
  //Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  //Serial.println();
  delay(200);
 //Serial.print("Mensagem : ");
  conteudo.toUpperCase();

/*if (conteudo.substring(1) == "E7 67 B9 62") //UID 2 - Cartao
  {
    Serial.println("Ola Cartao !");
    Serial.println("BORA BAHEA!");
    Serial.println();
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();*/

  Firebase.setInt("RFID/Contador", n++);
  Firebase.setBool("RFID/Leitura", true);
  Firebase.setString("RFID/Tag", conteudo);  
  
  // handle error
  if (Firebase.failed()) {
    //  Serial.print("setting /truth failed:");
      //Serial.println(Firebase.error());  
      return;
  }
 // }
  
  delay(1000);
  
}
