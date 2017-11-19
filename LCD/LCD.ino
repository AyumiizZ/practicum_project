#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define buttonPinLOCK PIN_PD6
#define IDLENGTH 7
const String ID[] = {"Aon", "Benz", "Kai", "Khem", "Tan", "Morgan", "C. Korrawit"};
const String IDCODE[] = {"1909", "0490", "2042", "6969", "0023", "2034", "5555"};
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);
String thisID = "5555";
String OTP = String(random(1000, 999999));
String thisPWD = OTP;
bool lockState = false;
bool haveID = false;
byte lock00[8] = {B00000, B00000, B00000, B00011, B00100, B01000, B01000, B01000};
byte lock01[8] = {B00000, B00000, B00000, B11000, B00100, B00010, B00010, B00010};
byte unlock00[8] = {B00011, B00100, B01000, B01000, B01000, B00000, B00000, B00000};
byte unlock01[8] = {B11000, B00100, B00010, B00010, B00010, B00010, B00010, B00010};
byte lock10[8] = {B11111, B10000, B10000, B10011, B10001, B10001, B10000, B11111};
byte lock11[8] = {B11111, B00001, B00001, B11001, B10001, B10001, B00001, B11111};
int c = 0;
int testRun = -1;
int index(String key)
{
  for (int i = 0; i < IDLENGTH; i++)
  {
    if ( key == IDCODE[i])
      return i;
  }
  return -1;
}

void printLockState(bool lockState)
{
  lcd.clear();
  delay(50);
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

void setup()
{
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
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("KUlock Starting");
  //  lcd.setCursor(0,1);
  //  for(int i = 0; i < 4; i++)
  //  {
  //    delay(750);
  //    lcd.print("#");
  //  }
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("dev: Aon & Benz");
  //  lcd.setCursor(0,1);
  //  lcd.print("####");
  //  for(int i = 0; i < 4; i++)
  //  {
  //    delay(750);
  //    lcd.print("#");
  //  }
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("Practicum class");
  //  lcd.setCursor(0,1);
  //  lcd.print("########");
  //  for(int i = 0; i < 4; i++)
  //  {
  //    delay(750);
  //    lcd.print("#");
  //  }
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("Kasetsart U.");
  //  lcd.setCursor(0,1);
  //  lcd.print("############");
  //  for(int i = 0; i < 4; i++)
  //  {
  //    delay(750);
  //    lcd.print("#");
  //  }
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("Getting start");
  //  lcd.setCursor(0,1);
  //  lcd.print("################");
  //  delay(1500);
  //end loading...
  printLockState(lockState);
  //  lockState = not lockState;
  lcd.setCursor(3, 0);
  lcd.print("ID: " + thisID);
  digitalWrite(PIN_PD0, HIGH);
  delay(300);
  digitalWrite(PIN_PD0, LOW);
}



void loop() {

  lcd.setCursor(0, 1);
  lcd.print(testRun);
  testRun *= -1;
  if (!haveID)
  {
    if (index(thisID) != -1)
    {
      printLockState(lockState);
      lcd.setCursor((8 - ID[index(thisID)].length()) / 2, 0);
      lcd.print(("Hello! " + ID[index(thisID)]).substring(0, 14));
      //      lockState = not lockState;
      haveID = true;
      delay(3000);
    }
    else
    {
      printLockState(lockState);
      lcd.setCursor(0, 0);
      lcd.print("ID not found");
      delay(3000);
    }
  }
  else
  {
    if (digitalRead(buttonPinLOCK) == LOW)
    {
      if (lockState)
      {
        printLockState(lockState);
        lcd.setCursor(0, 0);
        lcd.print("OTP: " + thisPWD);
        delay(3000);
        if (thisPWD == OTP)
        {
          lockState = not lockState;
          printLockState(lockState);
          lcd.setCursor(3, 0);
          lcd.print("Success");
          delay(3000);
          c = 0;
        }
        else
        {
          printLockState(lockState);
          lcd.setCursor(0, 0);
          lcd.print("Failed" + String(c) + "times");
          c++;
        }
        delay(3000);
      }
      else
      {
        lockState = not lockState;
        printLockState(lockState);
        lcd.setCursor(3, 0);
        lcd.print("Success");
        delay(3000);
        c = 0;
      }
      haveID = false;
      printLockState(lockState);
      //  lockState = not lockState;
      lcd.setCursor(3, 0);
      lcd.print("ID: " + thisID);
    }
    //    if(c > 3)
    //    {
    ////      printLockState(lockState);
    ////      lcd.setCursor(0,0);
    ////      lcd.print("Failed" + String(c) + "times");
    //      delay((c-3)*(c-3)*100);
    //    }
  }
  //  if (digitalRead(buttonPinLOCK) == LOW && !haveID) /* switch is pressed */
  //  {
  //    printLockState(lockState);
  //    lcd.setCursor((8 - ID[index(thisID)].length()) / 2, 0);
  //    lcd.print(("Hello! " + ID[index(thisID)]).substring(0, 14));
  //    lockState = not lockState;
  //    haveID = true;
  //    delay(3000);
  //  }
  //  if (digitalRead(buttonPinLOCK) == LOW && haveID)
  //  {
  //    lcd.clear();
  //    printLockState(lockState);
  //    lcd.setCursor(1,0);
  //    lcd.print("OTP");
  //    delay(3000);
  //    if(OTP == thisPWD)
  //    {
  //      lcd.clear();
  //      lcd.setCursor(0,0);
  //      lcd.print("Success");
  //      lockState = not lockState;
  //      delay(3000);
  //    }
  //    else
  //    {
  //      lcd.clear();
  //      lcd.setCursor(0,0);
  //      lcd.print("Failed");
  //      delay(3000);
  //    }
  //    haveID = false;
  //  }
  delay(50);
}
