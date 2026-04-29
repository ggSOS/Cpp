#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
byte cartaoCadastrado[] = { 0x99, 0x8C, 0xEE, 0x97 };
bool acessoGarantido = true;
String liberadoString = "";


void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Aproxime o cartão...");
}

void loop() {
  StaticJsonDocument<200> doc;
  String uidString = "";

  // Verifica se tem cartão
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Lê o cartão
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Mostra UID
  // Serial.print("UID do cartão: ");
  acessoGarantido = true;
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    if (rfid.uid.uidByte[i] != cartaoCadastrado[i]) {
      acessoGarantido = false;  // Sai do laço se um byte for diferente
    }
  }
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Add a leading zero if the byte is less than 0x10 (16)
    uidString.concat(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    // Convert the byte to a Hexadecimal string
    uidString.concat(String(rfid.uid.uidByte[i], HEX));
    // Optional: Add space or separator
    if (i < rfid.uid.size - 1) uidString.concat(" ");
  }
  uidString.toUpperCase();
  doc["id"] = uidString;

  
  // Serial.println();
  if (acessoGarantido) {
    // Serial.println("> Acesso Liberado.");
    liberadoString = "Liberado";
    doc["liberado"] = liberadoString;
    serializeJson(doc, Serial);
    Serial.println();
    delay(3000);
    // Serial.println("\nAproxime o cartão...");
  } else {
    // Serial.println("> Acesso Negado.");
    liberadoString = "Bloqueado";
    doc["liberado"] = liberadoString;
    serializeJson(doc, Serial);
    Serial.println();
    // Serial.println("\nAproxime o cartão...");
  }


  delay(1000);
}