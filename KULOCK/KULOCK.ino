#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define buttonPinLOCK PIN_PD6
#define buttonPinID PIN_PB3
#define IDLENGTH 7
const String ID[] = {"Aon", "Benz", "Kai", "Khem", "Tan", "Morgan", "C. Korrawit"};
const String IDCODE[] = {"1909", "0490", "2042", "6969", "0023", "2034", "5555"};
String thisID = "";
String OTP = String(random(1000, 999999));
String thisPWD = "";
bool lockState = false;
bool haveID = false;
byte lock00[8] = {B00000, B00000, B00000, B00011, B00100, B01000, B01000, B01000};
byte lock01[8] = {B00000, B00000, B00000, B11000, B00100, B00010, B00010, B00010};
byte unlock00[8] = {B00011, B00100, B01000, B01000, B01000, B00000, B00000, B00000};
byte unlock01[8] = {B11000, B00100, B00010, B00010, B00010, B00010, B00010, B00010};
byte lock10[8] = {B11111, B10000, B10000, B10011, B10001, B10001, B10000, B11111};
byte lock11[8] = {B11111, B00001, B00001, B11001, B10001, B10001, B00001, B11111};
int c = 0;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char key = NO_KEY;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {PIN_PB0, PIN_PB1, PIN_PB2, PIN_PB3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {PIN_PC2, PIN_PC1, PIN_PC0}; //connect to the column pinouts of the keypad
int testRun = -1;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);
int index(String key)
{
  if (key.length() == 0)
    return -1;
  for (int i = 0; i < IDLENGTH; i++)
  {
    if ( key == IDCODE[i])
    {
      return i;
    }
  }
  return -1;
}

void printLockState(bool lockState)
{
  //  lcd.clear();
  //  delay(50);
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
  // put your setup code here, to run once:
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2); // Set up the lcd to have 16 char on 2 lines
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
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  lcd.print("ID: " + thisID);
  key = keypad.getKey();
  if (key != NO_KEY && key != '#' && key != '*') {
    thisID += key;
    delay(50);
  }
  if (key == '#')
  {
    lcd.clear();
    thisID = thisID.substring(0, thisID.length() - 1);
    delay(50);
  }
  if (key == '*')
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (index(thisID) != -1)
      lcd.print("ID found");
    else
      lcd.print("ID not found");
    delay(2000);
    lcd.clear();
    thisID = "";
    delay(50);
  }
  printLockState(lockState);
  //  char key = keypad.getKey();
  //  if(haveID == false)
  //  {
  //
  //    printLockState(lockState);
  //    lcd.setCursor(0,0);
  //    lcd.print("ID: ");
  //    lcd.print(thisID);
  ////        lcd.setCursor(0,0);
  //  if(key!=NO_KEY)
  //    lcd.print(key);
  //    if(key != NO_KEY && key != '#' && key != '*')
  //    {
  //      thisID += key;
  //    }
  //    if(key != NO_KEY && key == '#')
  //    {
  //      lcd.clear();
  //      printLockState(lockState);
  //      lcd.setCursor(0,0);
  //      if(index(thisID) != -1)
  //      {
  //        haveID = true;
  //        lcd.print(index(thisID));
  ////        delay(1000);
  //      }
  //      else
  //      {
  //        lcd.print("Failed");
  //      }
  //      delay(1000);
  //      thisID = "";
  //      lcd.clear();
  //    }
  //    if(key != NO_KEY && key == '*')
  //    {
  //          lcd.setCursor(0,1);
  //        lcd.print("*");
  //      thisPWD = thisPWD.substring(0, thisPWD.length() - 1);
  //      lcd.clear();
  //    }
  //  }
  //  else
  //  {
  //    lcd.clear();
  //    lcd.setCursor(0,0);
  //    lcd.print("fuck");
  //  }
}
