#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <Wire.h>                 //Thư viện giao tiếp I2C
#include <LiquidCrystal_I2C.h>  //Thư viện LCD
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //Thiết lập địa chỉ và loại LCD

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

#define DHTTYPE DHT11 // DHT 11

//const  DHTPin = D3;

DHT dht(D3, DHTTYPE);     // Initialize DHT sensor.
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];


#define FIREBASE_HOST "doan1-1d85d-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "FBwyjUcOuRBHa0Q8IkH1kg5mrNMEczwHsXK20D1h"

#define WIFI_SSID "CANH MINH"
#define WIFI_PASSWORD "anhkhongchodau1980"

FirebaseData firebaseData;
// WiFiClient client;
String path = "/";
FirebaseJson json;

// byte Led = 2;
//có thể comment
String led_blue, led_red, led_yellow;


void setup()
{
  Serial.begin(115200);
  dht.begin();
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  Wire.begin(D5, D4);              //Thiết lập chân kết nối I2C (SDA,SCL);
  lcd.init();                      //Khởi tạo LCD
  lcd.clear();                     //Xóa màn hình
  lcd.backlight();             //Bật đèn nền
 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // đợi kết nối wifi rồi mới kết nối firebase
  lcd.print("CONNECTING");
  lcd.setCursor(0, 1);
  lcd.print("TO WIFI");
  int i = 7;
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(i, 1);
    lcd.print(".");
    delay(200);
    i++;
    Serial.println(".");
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData, path))
  {
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
  }
  Serial.println("Connected with IP:");
  Serial.println(WiFi.localIP());
  Serial.println();
  lcd.clear();
  lcd.print("IP");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1200);
  lcd.clear();
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
  lcd.createChar(1, degree);
}

//Firebase.setInt(firebaseData, path + "/temperature_and_humidity/humidity", random(1, 100));

void loop()
{


  if (Firebase.getString(firebaseData, path + "/Led_status/Led_blue"))
  {
    led_blue = firebaseData.stringData();
  }
  if (led_blue == "1")
  {
    digitalWrite(D0, 0);
//    Serial.println("Đèn Xanh Bật");
  }
  else {
    digitalWrite(D0, 1);
//    Serial.println("Đèn Xanh Tắt");
  }


  if (Firebase.getString(firebaseData, path + "/Led_status/Led_red"))
  {
    led_red = firebaseData.stringData();
  }
  if (led_red == "1")
  {
    digitalWrite(D1, 0);
//    Serial.println("Đèn Đỏ Bật");
  }
  else {
    digitalWrite(D1, 1);
//    Serial.println("Đèn Đỏ Tắt");
  }

  if (Firebase.getString(firebaseData, path + "/Led_status/Led_yellow"))
  {
    led_yellow = firebaseData.stringData();
  }
  if (led_yellow == "1")
  {
    digitalWrite(D2, 0);
//    Serial.println("Đèn Vàng Bật");
  }
  else {
    digitalWrite(D2, 1);
//    Serial.println("Đèn Vàng Tắt");
  }


  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Firebase.setFloat(firebaseData, path + "/temperature_and_humidity/humidity", h);
  Firebase.setFloat(firebaseData, path + "/temperature_and_humidity/temperature", t);

  lcd.setCursor(6, 0);
  lcd.print(t);
  lcd.write(1);
  lcd.print("C");
  lcd.setCursor(6, 1);
  lcd.print(h);
  lcd.print("%");
//  Serial.print("Nhiệt Độ: ");
//  Serial.println(t);
//  Serial.print("Độ Ẩm: ");
//  Serial.println(h);
}