#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define buttonPinID PIN_PD6
#define buttonPinLOCK PIN_PD6
#define IDLENGTH 7
const String ID[] = {"Aon", "Benz", "Kai", "Khem", "Tan", "Morgan", "C. Korrawit"};
const String IDCODE[] = {"1909", "0490", "2042", "6969", "0023", "2034", "5555"};
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);
String thisID = "5555";
bool lock_state = false;
byte lock00[8] = {B00000, B00000, B00000, B00011, B00100, B01000, B01000, B01000};
byte lock01[8] = {B00000, B00000, B00000, B11000, B00100, B00010, B00010, B00010};
byte unlock00[8] = {B00011, B00100, B01000, B01000, B01000, B00000, B00000, B00000};
byte unlock01[8] = {B11000, B00100, B00010, B00010, B00010, B00010, B00010, B00010};
byte lock10[8] = {B11111, B10000, B10000, B10011, B10001, B10001, B10000, B11111};
byte lock11[8] = {B11111, B00001, B00001, B11001, B10001, B10001, B00001, B11111};

int index(String key)
{
  for (int i = 0; i < IDLENGTH; i++)
  {
    if ( key == IDCODE[i])
      return i;
  }
}

void print_lock_state(bool lock_state)
{
  lcd.clear();
  delay(50);
  if (lock_state)
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

void setup()
{
  //    pinMode(buttonPinID,INPUT_PULLUP);
  pinMode(buttonPinLOCK, INPUT_PULLUP);
  pinMode(PIN_PD0, OUTPUT); // red LED
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2); // Set up the lcd to have 16 char on 2 lines
  lcd.createChar(0, lock00);
  lcd.createChar(1, lock01);
  lcd.createChar(2, lock10);
  lcd.createChar(3, lock11);
  lcd.createChar(4, unlock00);
  lcd.createChar(5, unlock01);
  //loading...
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("KUlock Starting");
  lcd.setCursor(0,1);
  for(int i = 0; i < 4; i++)
  {
    delay(750);
    lcd.print("#");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("dev: Aon & Benz");
  lcd.setCursor(0,1);
  lcd.print("####");
  for(int i = 0; i < 4; i++)
  {
    delay(750);
    lcd.print("#");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Practicum class");
  lcd.setCursor(0,1);
  lcd.print("########");
  for(int i = 0; i < 4; i++)
  {
    delay(750);
    lcd.print("#");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kasetsart U.");
  lcd.setCursor(0,1);
  lcd.print("############");
  for(int i = 0; i < 4; i++)
  {
    delay(750);
    lcd.print("#");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Getting start");
  lcd.setCursor(0,1);
  lcd.print("################");
  delay(1500);
  //end loading...
  print_lock_state(lock_state);
  lock_state = not lock_state;
  lcd.setCursor(3, 0);
  lcd.print("ID: " + thisID);
  digitalWrite(PIN_PD0, HIGH);
  delay(300);
  digitalWrite(PIN_PD0, LOW);
}



void loop() {

  if (digitalRead(buttonPinLOCK) == LOW) /* switch is pressed */
  {
    print_lock_state(lock_state);
    lcd.setCursor((8 - ID[index(thisID)].length()) / 2, 0);
    lcd.print(("Hello! " + ID[index(thisID)]).substring(0, 14));
    lock_state = not lock_state;
    delay(500);

  }
//  if (digitalRead(buttonPinID) == LOW)
//  {
//    printIcon(lock_state);
//    lcd.setCursor((8 - ID[index(thisID)].length()) / 2, 0);
//    lcd.print(("Hello! " + ID[index(thisID)]).substring(0, 14));
//    delay(500);
//  }
  delay(50);
}
