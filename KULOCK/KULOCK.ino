#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <avr/pgmspace.h>
#define buttonPinLOCK PIN_PD6
#define buttonPinID PIN_PB3
#define IDLENGTH 10
const char ID_0[] PROGMEM = "Aon";
const char IDCODE_0[] PROGMEM = "1909";
const char ID_1[] PROGMEM = "Benz";
const char IDCODE_1[] PROGMEM = "0490";
const char ID_2[] PROGMEM = "Kai";
const char IDCODE_2[] PROGMEM = "2034";
const char ID_3[] PROGMEM = "Khem";
const char IDCODE_3[] PROGMEM = "4724";
const char ID_4[] PROGMEM = "Tan";
const char IDCODE_4[] PROGMEM = "0023";
const char ID_5[] PROGMEM = "Morgan";
const char IDCODE_5[] PROGMEM = "2042";
const char ID_6[] PROGMEM = "C. Korrawit";
const char IDCODE_6[] PROGMEM = "9012";
const char ID_7[] PROGMEM = "W. Worachai";
const char IDCODE_7[] PROGMEM = "5555";
const char ID_8[] PROGMEM = "Nuty";
const char IDCODE_8[] PROGMEM = "3782";
const char ID_9[] PROGMEM = "Cheerana";
const char IDCODE_9[] PROGMEM = "12";
const char* const ID[] PROGMEM = {ID_0, ID_1, ID_2, ID_3, ID_4, ID_5, ID_6, ID_7, ID_8, ID_9};
const char* const IDCODE[] PROGMEM = {IDCODE_0, IDCODE_1, IDCODE_2, IDCODE_3, IDCODE_4, IDCODE_5, IDCODE_6, IDCODE_7, IDCODE_8, IDCODE_9};
char buffer[30];
char thisID[11];
char OTP[7];
char thisPWD[11];
bool lockState = false;
bool haveID = false;
uint8_t lock00[8] = {B00000, B00000, B00000, B00011, B00100, B01000, B01000, B01000};
uint8_t lock01[8] = {B00000, B00000, B00000, B11000, B00100, B00010, B00010, B00010};
uint8_t unlock00[8] = {B00011, B00100, B01000, B01000, B01000, B00000, B00000, B00000};
uint8_t unlock01[8] = {B11000, B00100, B00010, B00010, B00010, B00010, B00010, B00010};
uint8_t lock10[8] = {B11111, B10000, B10000, B10011, B10001, B10001, B10000, B11111};
uint8_t lock11[8] = {B11111, B00001, B00001, B11001, B10001, B10001, B00001, B11111};
uint8_t c = 0;
const uint8_t ROWS = 4; //four rows
const uint8_t COLS = 3; //three columns
char key = NO_KEY;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
uint8_t rowPins[ROWS] = {PIN_PB0, PIN_PB1, PIN_PB2, PIN_PB3}; //connect to the row pinouts of the keypad
uint8_t colPins[COLS] = {PIN_PC2, PIN_PC1, PIN_PC0}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);
int index(char key[])
{
  for (int i = 0; i < IDLENGTH; i++)
  {
    strcpy_P(buffer, (char*)pgm_read_word(&(IDCODE[i])));
    if (strcmp(key, buffer) == 0)
    {
      return i;
    }
  }
  return -1;
}

char getOTP()
{
  int OTP_length = random(4, 6);
  for (int i = 0; i < OTP_length ; i++)
    OTP[i] = (char)(random(0, 9) + 48);

}

void printLockState(bool lockState)
{
  if (lockState)
  {
    lcd.setCursor(3, 1);
    lcd.print("Locked!!");
    lcd.setCursor(14, 0);
    lcd.print((char)0);
    lcd.print((char)1);
  }
  else
  {
    lcd.setCursor(2, 1);
    lcd.print("Unlocked!!");
    lcd.setCursor(14, 0);
    lcd.print((char)4);
    lcd.print((char)5);
  }
  lcd.setCursor(14, 1);
  lcd.print((char)2);
  lcd.print((char)3);
}

void setup() {
  pinMode(PIN_PB5, OUTPUT);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.createChar(0, lock00);
  lcd.createChar(1, lock01);
  lcd.createChar(2, lock10);
  lcd.createChar(3, lock11);
  lcd.createChar(4, unlock00);
  lcd.createChar(5, unlock01);
  lcd.setCursor(0, 0);
  for (int i = 0; i < 16; i++)
  {
    lcd.setCursor(i, 0);
    lcd.print("#");
    lcd.setCursor(i, 1);
    lcd.print("#");
    delay(100);
  }
  lcd.clear();
  printLockState(lockState);
  lcd.setCursor(0, 0);
  lcd.print("ID: ");
  digitalWrite(PIN_PB5, HIGH);
  Serial.begin(9600);
  Serial.println("Device started...");
}

void loop() {
  key = keypad.getKey();
  if (!haveID)
  {
    if (key != NO_KEY && key != '#' && key != '*')
    {
      if (c < 10)
        thisID[c++] = key;
    }
    if (key == '#')
    {
      if (c >= 1)
        thisID[--c] = '\0';
      lcd.clear();
    }
    if (key == '*')
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      if (index(thisID) != -1)
      {
        lcd.print("ID found");
        printLockState(lockState);
        lcd.clear();
        strcpy_P(buffer, (char*)pgm_read_word(&(ID[index(thisID)])));
        lcd.setCursor((8 - String(buffer).length()) / 2, 0);
        lcd.print("Hello! ");
        lcd.print(String(buffer).substring(0, 7));
        printLockState(lockState);
        haveID = true;
        getOTP();
        Serial.println(OTP);
        delay(1000);
      }
      else
      {
        lcd.print("ID not found");
        printLockState(lockState);
        delay(1000);
      }
      for (int i = 0; i < 11; i++)
        thisID[i] = '\0';
      c = 0;
      lcd.clear();
    }
    printLockState(lockState);
    lcd.setCursor(0, 0);
    lcd.print("ID: ");
    lcd.print(thisID);
    if (haveID)
      lcd.clear();
  }
  else
  {
    if (key != NO_KEY && key != '#' && key != '*')
    {
      if (c < 9)
        thisPWD[c++] = key;
    }
    if (key == '#')
    {
      if (c >= 1)
        thisPWD[--c] = '\0';
      lcd.clear();
    }
    if (key == '*')
    {
      Serial.println(strcmp(thisPWD, OTP));
      if (strcmp(thisPWD, OTP) == 0)
      {
        lcd.clear();
        lockState = !lockState;
        lcd.print("Success");
        if (lockState)
        {
          digitalWrite(PIN_PB5, LOW);
        }
        else
        {
          digitalWrite(PIN_PB5, HIGH);
        }
        delay(1000);
        haveID = false;
        lcd.clear();
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Failed");
        printLockState(lockState);
        delay(1000);
        lcd.clear();
      }
      for (int i = 0; i < 11; i++)
        thisPWD[i] = '\0';
      c = 0;
    }
    printLockState(lockState);
    lcd.setCursor(0, 0);
    lcd.print("PWD: ");
    lcd.print(thisPWD);
  }
  delay(50);
}
