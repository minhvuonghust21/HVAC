#include <Keypad.h>
#include <Adafruit_BMP085.h>

#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, A1, A2);//RS,EN,D4,D5,D6,D7
#define Relay A0

const String password_1 = "1"; // change your password here
const String password_2 = "0";  // change your password here
const String password_3 = "9";   // change your password here
String input_password;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// initializing a character of size 4 for showing the result
int apsuatchophep=1020;
Adafruit_BMP085 bmp;
 
void setup() {
  
input_password.reserve(32);

pinMode(Relay, OUTPUT);
digitalWrite(Relay, LOW);
lcd.begin(16, 2);



lcd.setCursor(0, 0);
lcd.print("bai tap lon");
lcd.setCursor(0, 1);
lcd.print("he thong nhung");
delay (3000);
 
lcd.clear();//clear display

Serial.begin(9600);
if (!bmp.begin())
{
Serial.println("ERROR");///if there is an error in communication
while (1) {}
}
}
void loop()
{
  
lcd.setCursor(0, 0);//set the cursor to column 0, line1
lcd.print("Ap suat="); // print name
lcd.print(bmp.readPressure()/100);
lcd.print("hPa ");

delay(500);


/*if (bmp.readPressure()/100 > apsuatchophep) {
  digitalWrite (Relay,HIGH);
  
}
if (bmp.readPressure()/100 < apsuatchophep) {
  digitalWrite (Relay,LOW);

}*/

  char key = keypad.getKey();

 if (key) {
    Serial.println(key);

    if (key == '*') {
      input_password = ""; // reset the input password
    } 
    else if (key == '#') {
      if (input_password == password_1) {
        Serial.println("The password is correct, turning ON relay");
        digitalWrite(Relay, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("bat thong gio");
      } else if (input_password == password_2) {
        Serial.println("The password is correct, turn OFF relay");
        digitalWrite(Relay, LOW);
        lcd.setCursor(0, 1);
        lcd.print("tat thong gio");

      } else 

      input_password = ""; // reset the input password
    } 
    else {
      input_password += key; // append new character to input password string
    }
  }
}


