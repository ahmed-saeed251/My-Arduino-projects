#include <LiquidCrystal_I2C.h>   // include Arduino LCD library
// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define CH0  12
#define CH1  11
#define CH2  10
#define CH3  9
#define CH4  8

// variables
byte ch_number;
uint32_t res;
const uint32_t res_table[5] = {100, 1000, 10000, 100000, 1000000};
char _buffer[11];

void setup(void) {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("V = ");
  lcd.setCursor(0, 1);
  lcd.print("R = ");

  /* Continuity mode pin */
  pinMode(2, INPUT_PULLUP);

  pinMode(CH0, OUTPUT);
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(CH4, OUTPUT);

  ch_number = 4;
  ch_select(ch_number);
}

// main loop
void loop() {

  uint16_t volt_image = analogRead(A1) + 1;
  float volts = analogRead(A3) * (4.88 / 1023.0) * 11.0;

  if(volt_image >= 550 && ch_number < 4)
  {
    ch_number++;
    ch_select(ch_number);
    delay(50);
    return;
  }

  if(volt_image <= 90 && ch_number > 0) {
    ch_number--;
    ch_select(ch_number);
    delay(50);
    return;
  }

  if(volt_image < 900)
  {
    float value = (float)volt_image*res/(1023 - volt_image);
    if(value < 1000.0)
      sprintf(_buffer, "%03u.%1u Ohm ", (uint16_t)value, (uint16_t)(value*10)%10);
    else if(value < 10000.0)
           sprintf(_buffer, "%1u.%03u kOhm", (uint16_t)(value/1000), (uint16_t)value%1000);
    else if(value < 100000.0)
           sprintf(_buffer, "%02u.%02u kOhm", (uint16_t)(value/1000), (uint16_t)(value/10)%100);
    else if(value < 1000000.0)
           sprintf(_buffer, "%03u.%1u kOhm", (uint16_t)(value/1000), (uint16_t)(value/100)%10);
    else
      sprintf(_buffer, "%1u.%03u KOhm", (uint16_t)(value/1000000), (uint16_t)(value/1000)%1000);
    
  }

  else
    sprintf(_buffer, "Inf               ");

  lcd.setCursor(4, 1);
  lcd.print(_buffer);
  lcd.setCursor(4, 0);
  lcd.print(volts);
  Serial.println(_buffer);
  Serial.println();

  /* Continuity tester */
  if (digitalRead(2) == LOW){
    digitalWrite(3, HIGH);
    tone(3,3250);
    delay(150);
    //noTone(6);
    }
  else
    digitalWrite(3, LOW);
      tone(3,3250);
    delay(300);

}

void ch_select(byte n) {
  switch(n) {
    case 0:
      digitalWrite(CH0, LOW);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, HIGH);
      break;
    case 1:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, LOW);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, HIGH);
      break;
    case 2:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, LOW);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, HIGH);
      break;
    case 3:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, LOW);
      digitalWrite(CH4, HIGH);
      break;
    case 4:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, LOW);
  }
  res = res_table[n];
}
