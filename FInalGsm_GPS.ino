#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(10,11);
SoftwareSerial gsmSerial(8,9);
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);
  long lat, lon;
  float flat, flon;
  int year;
  unsigned long age, date, time, chars;
  byte month, day, hour, minute, second, hundredths;
  String message="";
  String latFin="";
  String lonFin="";
  String coordDec="";
  String decimal="";
  int trunc_lat=0;
  int trunc_lon=0;
  int i=1;

void setup()  
{
  // Oploen serial communications and wait for port to open:
  Serial.begin(9600);
  gsmSerial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() // run over and over
{
 
 mySerial.listen();
    
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 5000) 
  {
    if (mySerial.available()) 
    
    {
      char c = mySerial.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) 
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  
  if (newdata) 
  {
    gpsdump(gps);
    Serial.println("lat:");
   printFloat(flat, 5);
   trunc_lat=trunc(flat);
   latFin+=trunc_lat;
   latFin+=".";
   latFin+=coordDec;
   Serial.print(latFin);
  
   
     Serial.println();
    Serial.println("long:");
    printFloat(flon, 5);
    trunc_lon=trunc(flon);
   lonFin+=trunc_lon;
   lonFin+=".";
   lonFin+=coordDec;
   Serial.print(lonFin);
  
   
    
    delay(5000);
    Serial.println();
  }
  
if(flat>0 && flon>0){
 SendMessage();
}
if(gsmSerial.available()>0){
  Serial.write(gsmSerial.read());
  }
}

void SendMessage(){
  gsmSerial.listen();
//Serial.println(i,"This is i :");
while(i>0){
   
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+919619129212\"\r");
  delay(1000);
          message+="Accident Occured!Click the link below https://www.google.com/maps/search/?api=1&query=";
          message+=latFin;
          message+=",";
          message+=lonFin;
  gsmSerial.println(message);
  delay(100);
  gsmSerial.println((char)26);
  delay(1000);
  
 i--;
  latFin="";
   lonFin="";
   coordDec="";
  }
}

void gpsdump(TinyGPS &gps)
{
  gps.f_get_position(&flat, &flon, &age);
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) 
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
// Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
 // Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
   // Serial.print(toPrint);
    decimal+=toPrint;
    remainder -= toPrint;
  }

//Serial.print(decimal);
coordDec=decimal;
 decimal="";
 
}
