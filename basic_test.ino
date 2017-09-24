/* Terminal sensor adapter (DFR0055) [S093] : http://rdiot.tistory.com/47 [RDIoT Demo] */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
 
LiquidCrystal_I2C lcd(0x27,20,4);  // LCD2004
int SensorPin = 10; //Our sensor is connected to digital pin 10
 
//Initiate OneWire communication
OneWire ds(SensorPin);
 
void setup()
{
  lcd.init();  // initialize the lcd 
  lcd.backlight();
  lcd.print("start LCD2004");
 
  delay(1000);
 
  lcd.clear();
}
 
void loop()
{
 
 float T = readT(); // call function to read from sensor
 
 
  lcd.setCursor(0,0);
  lcd.print("S093:TerminalSenAdt");
 
  lcd.setCursor(0,1);
  lcd.print("temper=" + (String)T + " 'C");
 
  lcd.setCursor(0,2);
  if(T<10)
    lcd.print("stat=cold");
  else if(T>50)
    lcd.print("stat=hot-");
  else
    lcd.print("stat=soso");  
 
  delay(750);
}
 
 
float readT(){
//returns the temperature from our sensor
 
byte data[12];
byte addr[8];
 
if ( !ds.search(addr)) {
ds.reset_search();
return -300; // if there is no sensor on OneWire Bus, return -300 value
}
 
ds.reset();
ds.select(addr);
ds.write(0x44,1); // tell sensor to start converting
ds.reset();
ds.select(addr);
ds.write(0xBE); // tell sensor to start sending data
 
for (int i = 0; i < 9; i++) { // we receive data in this loop
data[i] = ds.read();
}
 
ds.reset_search();
 
byte MSB = data[1];
byte LSB = data[0];
 
float raw = ((MSB << 8) LSB); // move MSB left for 8 spaces, join that with LSB
float realTempC = raw / 16; // move decimal point left for 4 spaces, result our temperature
return realTempC;
 
}
