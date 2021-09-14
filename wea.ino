#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include<Wire.h>
#include<LiquidCrystal.h>

#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x3F,16,2);


const char* ssid     = "E.D.I.T.H";     
const char* password = "vicky008";   
String APIKEY = "ebdd397729cb9d206ed954dcb6c9efac";
String CityID = "1259440"; 

WiFiClient client;
char servername[]="api.openweathermap.org";  
String result;
int  counter = 60;
String weatherDescription ="";
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;

 void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  int cursorPosition=0;
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting");
  }
}
void loop() {
    if(counter == 60) 
    {
      counter = 0;
      displayGettingData();
      delay(1000);
      getWeatherData();
    }else
    {
      counter++;
      displayConditions(Temperature,Humidity,Pressure,weatherLocation,Country,weatherDescription);
      
      delay(6000);
     
    }
}
void getWeatherData() //client function to send/receive GET request data.
{
  if (client.connect(servername, 80)) {  //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
      result = result+c;
    }

  client.stop(); //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);

char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';

StaticJsonBuffer<1024> json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);
if (!root.success())
{
  Serial.println("parseObject() failed");
}
String location = root["name"];
String country = root["sys"]["country"];
float temperature = root["main"]["temp"];
float humidity = root["main"]["humidity"];
String weather = root["weather"]["main"];
String description = root["weather"]["description"];
float pressure = root["main"]["pressure"];

weatherDescription = description;
weatherLocation = location;
Country = country;
Temperature = temperature;
Humidity = humidity;
Pressure = pressure;

delay(10000);
}
void displayConditions(float Temperature,float Humidity, float Pressure,String location,String Country,String description)
{
 Serial.print(Temperature,1);
 Serial.print((char)223);
 Serial.print(" ");
 Serial.print(Humidity,0);
 Serial.print(" ");
 Serial.print(Pressure,1);
 Serial.print(" ");
  Serial.print(location);
  Serial.print(" ");
  Serial.print(Country);
  Serial.print(" ");
  Serial.print(description);
  Serial.println("...               ");
  delay(100);

    lcd.clear();
    lcd.backlight();
    lcd.setCursor(1,0);
    lcd.print(location);
    Serial.print(location);
    Serial.print(" ");
    lcd.print(" , ");
    lcd.print(Country);
    Serial.print(Country);
    Serial.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(description);
    Serial.print(description);
    Serial.print(" ");
    delay(12000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T:"); 
    Serial.print(Temperature,1);
    Serial.print((char)223);
    lcd.print(Temperature);
    lcd.print((char)223);
    lcd.print("C ");
    lcd.setCursor(8,0);
    lcd.print(" H:");
    lcd.print(Humidity);
    lcd.print(" %");
    Serial.print(Humidity,0);
    Serial.print(" ");
    lcd.setCursor(1,1);
    lcd.print("P: ");
    lcd.print(Pressure);
    lcd.print(" hPa");
    Serial.print(Pressure,1);
    Serial.print(" ");
    delay(12000);
}



void displayGettingData()
{
  Serial.print("Getting data");
  delay(100);
}
