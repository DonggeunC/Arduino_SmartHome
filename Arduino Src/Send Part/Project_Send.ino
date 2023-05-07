#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <DHT_U.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#define flame A1
#define RST_PIN 9
#define SS_PIN 10

DHT dht(3, DHT11);
SoftwareSerial P_Serial(6, 7);
MFRC522 mfrc(SS_PIN, RST_PIN);
Servo servo;
int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;
char temp[50];
char* tArr[4];
int rec[4];

void setup() {
  Serial.begin(9600);
  P_Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
  servo.attach(5);
  irrecv.enableIRIn();
  dht.begin();
}

void loop() {
  int index = 0, i = 0;
  while (P_Serial.available()) {
    temp[index++] = P_Serial.read();
  }
  temp[index] = '\0';
  if (temp[0] == '4') {
    for (int angle = 0; angle < 180; angle++) {
      servo.write(angle);
      delay(10);
    }
  }

  int rest = (int)dht.readTemperature();
  int resh = (int)dht.readHumidity();
  delay(200);
  if (resh > 50) {
    P_Serial.print((String)resh);
    delay(100);
  }

  int remote;
  if (irrecv.decode(&results)) {
    remote = results.value;
    Serial.println(remote);
    irrecv.resume();
    if (remote == 12495) {
      String se11 = (String)12495;
      String se12 = (String)resh;
      String se1 = se11 + "#" + se12 + "#";
      P_Serial.print(se1);
      delay(100);
    } else if (remote == 6375) {
      String se21 = (String)6375;
      String se22 = (String)rest;
      String se2 = se21 + "#" + se22 + "#";
      P_Serial.println(se2);
      delay(100);
    }
  }
  delay(100);

  int a = 500;
  int val = analogRead(flame);
  delay(200);
  }
  if (!mfrc.PICC_IsNewCardPresent())
    return;
  if (!mfrc.PICC_ReadCardSerial())
    return;
  if (mfrc.uid.uidByte[0] == 85 &&
      mfrc.uid.uidByte[1] == 217 &&
      mfrc.uid.uidByte[2] == 147 &&
      mfrc.uid.uidByte[3] == 172) {
    int ac = 999;
    P_Serial.print((String)ac);
    delay(100);
  } else {
    int de = 888;
    P_Serial.print((String)de);
    delay(100);
  }
  Serial.print("card UID : ");
  for (byte i = 0; i < 4; i++) {
    Serial.print((String)mfrc.uid.uidByte[i] + "#");
    delay(100);
  }
  Serial.println();
}