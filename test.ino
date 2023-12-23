#include<LiquidCrystal.h>  // include the LCD library
#define Relay PA6
const int rs = PB11, en = PB10, d4 = PA4, d5 = PA3, d6 = PA2, d7 = PA1; //STM32 Pins to which LCD is connected 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize the LCD
int tthap=27;
int tcao=28;
#include<DHT.h>                     //Library for using DHT sensor 
#define DHTPIN PA0 
#define DHTTYPE DHT11
 
DHT dht(DHTPIN, DHTTYPE);     //initilize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11

void setup()
{ 
 
  pinMode (PC13, OUTPUT);
   pinMode(Relay, OUTPUT);
   digitalWrite(Relay, LOW);
  // initialize the LCD
  lcd.begin(16,2);
  dht.begin();   
  lcd.setCursor(0,0);                               
  lcd.print("bai tap lon ");

  lcd.setCursor(0,1);                               
  lcd.print("he thong nhung");
  delay(3000);
  lcd.clear();
}
 
void loop()
{
  float h = dht.readHumidity();       //Gets Humidity value
  float t = dht.readTemperature();    //Gets Temperature value
  
  delay(500);

if (t < tthap) {
  digitalWrite (PC13,HIGH);
  delay(300);
  digitalWrite (PC13,LOW);
  delay(300);
  digitalWrite (Relay,LOW);
  
}

delay (100);

if (t > tcao) {
  digitalWrite (Relay,HIGH);
  delay(300);
  
}


  lcd.setCursor(0,0);
  lcd.print("Nhietdo: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Doam: ");
  lcd.print(h);
  lcd.print(" %");
}
