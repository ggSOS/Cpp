#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#define PINO_SS 10
#define PINO_RST 9

MFRC522 leitor(PINO_SS, PINO_RST);

String tagAutorizada = "43 B6 49 5"; // troque pelo UID do seu cartão

void setup() {
  Serial.begin(9600);
  SPI.begin();
  leitor.PCD_Init();

  Serial.println("Aproxime o cartao...");
}

void loop() {

  if (!leitor.PICC_IsNewCardPresent()) return;
  if (!leitor.PICC_ReadCardSerial()) return;

  String uid = "";

  for (byte i = 0; i < leitor.uid.size; i++) {
    uid += String(leitor.uid.uidByte[i], HEX);

    if (i < leitor.uid.size - 1) {
      uid += " ";
    }
  }

  uid.toUpperCase();

  StaticJsonDocument<200> doc;

  doc["uid"] = uid;

  if (uid == tagAutorizada) {
    doc["status"] = "Acesso liberado";
  } else {
    doc["status"] = "Acesso negado";
  }

  serializeJson(doc, Serial);
  Serial.println();

  leitor.PICC_HaltA();
  leitor.PCD_StopCrypto1();

  delay(1000);
}