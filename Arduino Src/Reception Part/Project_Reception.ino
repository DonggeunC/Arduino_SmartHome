#include <MsTimer2.h>
#include <Stepper.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial P_Serial(6, 7);
char acc[50];
char* sArr[4] = { NULL };
int a[4] = { 0 };
Servo servo;
const int spr = 2048;
Stepper stepper(spr, 11, 9, 10, 8);
unsigned long cur = 0;
unsigned long pre = 0;

int speaker = 3;
int Do = 262;
int Re = 294;
int Mi = 330;
int Sol = 392;
int Do2 = 523;


void isr() {
  cur = millis();
  static int count = 0;
  int send = 4;
  if (cur - pre > 300) {
    P_Serial.print((String)send);

    Serial.println(count++);
    pre = cur;
  }
}


void setup() {
  Serial.begin(9600);
  P_Serial.begin(9600);

  servo.attach(4);
  pinMode(2, INPUT_PULLUP);

  pinMode(speaker, OUTPUT);

  lcd.init();
  lcd.backlight();
  stepper.setSpeed(14);
  attachInterrupt(0, isr, RISING);
}

void loop() {
  int index = 0;
  int i = 0;
  char* ptr = NULL;

  while (P_Serial.available()) {
    acc[index++] = P_Serial.read();  //어떻게 배열로 저장?
  }
  acc[index] = '\0';

  ptr = strtok(acc, "#");

  while (ptr != NULL) {
    sArr[i++] = ptr;
    ptr = strtok(NULL, "#");
  }

  for (int k = 0; k < 4; k++)
    a[k] = atoi(sArr[k]);  // 문자를 정수로 바꿈  그럼 4번 할 필요 있나? 정수 하나 오는데

  if (a[0] == 999) {
    lcd.setCursor(0, 0);
    lcd.print("Access");

    tone(speaker, Do, 250);
    delay(250);
    tone(speaker, Mi, 250);
    delay(250);
    tone(speaker, Sol, 250);
    delay(250);
    tone(speaker, Do2, 250);
    delay(250);

    for (int i = 0; i < 180; i++) {
      servo.write(i);
      delay(20);
    }
    lcd.clear();
  } else if (a[0] == 888) {
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");

    tone(speaker, Re, 250);
    delay(500);
    tone(speaker, Re, 250);
    delay(500);

    delay(3000);
    lcd.clear();

  } 
  
  else if (a[0] == 500) {

    lcd.setCursor(0, 0);
    lcd.print("FIRE");

    for (int i = 0; i < 5; i++) {
      tone(speaker, Do2, 250);
      delay(250);
      tone(speaker, Do, 250);
      delay(250);
    }

    delay(3000);
    lcd.clear();

  } else if (a[0] == 12495) {
    lcd.setCursor(0, 0);
    lcd.print("HUMIDITY : ");
    lcd.setCursor(11, 0);
    lcd.print((String)a[1] + "%");
    delay(4000);
    lcd.clear();

  } else if (a[0] == 6375) {
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.setCursor(13, 0);
    lcd.print((String)a[1] + "C");
    delay(4000);
    lcd.clear();
  }

  else if (a[0] > 50 && a[0] < 100) {
 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidity 50!");
    lcd.setCursor(0, 1);
    lcd.print("Dehumidification");
    stepper.step(spr);
    lcd.clear();
  }

  else {
    delay(200);
  }
}