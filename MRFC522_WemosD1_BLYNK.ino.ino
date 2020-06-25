/** OOOOOK FUNZIONAAA con WEMOS D1 R1 mini
 ** nell'APP Blynk occorre inserire una icone LCD collegata al Virtual pin V5
//OOK this sketch has been modified by adding the Blynk instruction in order to 
//post data card read from MRFC522 to Blynk application


Useful links
https://wiki.wemos.cc/products:d1:d1_mini
https://cdn-images-1.medium.com/max/1400/1*YKc8KpAfMrlhrOLmNjdRwQ.png (D1 full pinout)
https://github.com/Jorgen-VikingGod/ESP8266-MFRC522
https://github.com/miguelbalboa/rfid
d1 mini rc52 wiring
https://discourse-cdn-sjc1.com/business5/uploads/mydevices/original/2X/e/ecedba79dc05f2c0b02b7fba8b3da2681590a11a.jpg
RST  - D3
MISO - D6
MOSI - D7
SCK  - D5
SDA  - D8
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
constexpr uint8_t RST_PIN =  0;          // Configurable, see typical pin layout above 18
constexpr uint8_t SS_PIN =  15;         // Configurable, see typical pin layout above  16
String CardID ="00000000";
char CardID_char [8] = "";
String content = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "oZB3rY11S0Nr0cowojDpGiK9ddUFtx5g";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Vodafone-34162499";
char pass[] = "exvwmfckpc5bkav";

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  delay(1000);
  Serial.println("Setup");
  
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println("Setup done");
  
  // connect to BLYNK
  pinMode(LED_BUILTIN, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
    
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show some details of the PICC (that is: the tag/card)

  Serial.print(F("Card UID:"));

//lettura della card UID
  CardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
//    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    CardID += String(mfrc522.uid.uidByte[i], HEX);
  }
  CardID.toUpperCase();
  Serial.println();
  CardID.toCharArray(CardID_char, 32); //convert variable CardID (that has the format string) into variable CardID_char (that has the format char)
  Serial.print("Card char: ");
  Serial.println(CardID_char);
  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  delay(1000);
         
  Blynk.virtualWrite(V5,CardID_char);
  
}
