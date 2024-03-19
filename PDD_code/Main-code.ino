#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Setup Wifi
const char *ssid = "Iqdamganz";
const char *password = "Godam123";


const long utcOffsetInSeconds = 25200;

// Setting tanggal menjadi nama hari
String weekDays[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
String months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };


int dataConduct, datapH, dataNitrogen, dataPhosphorus, dataPotassium;
float dataSoil, dataTemp;


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);
SoftwareSerial mySerial(D3, D4);    // RX, TX NPK
SoftwareSerial serial_gps(D6, D7);  // TX, RX GPS

// The TinyGPSPlus object
TinyGPSPlus gps;

#include <FirebaseESP8266.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define API_KEY "AIzaSyB8tP_ZnhvRD-TMTbCzQPg1NK82VH78mSQ"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://skripsi-5f7b4-default-rtdb.asia-southeast1.firebasedatabase.app/"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "alfatihiqdam17@gmail.com"
#define USER_PASSWORD "God@m170800"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  serial_gps.begin(9600);
  mySerial.begin(4800);
  WiFi.begin(ssid, password);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("OLED Initializing OK!");
  display.display();
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  config.wifi.clearAP();
  config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
}


void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (serial_gps.available() > 0)
    if (gps.encode(serial_gps.read())) {
      displayInfo();

      FirebaseJson jsonSensor;
      FirebaseJson jsonData;

      jsonData.set("/EC", dataConduct);
      jsonData.set("/pH", datapH);
      jsonData.set("/Nitrogen", dataNitrogen);
      jsonData.set("/Phosphorus", dataPhosphorus);
      jsonData.set("/Potassium", dataPotassium);
      jsonData.set("/Soil", dataSoil);
      jsonData.set("/Temperature", dataTemp);


      jsonSensor.set("data", jsonData);
      timeClient.update();

      String weekDay = weekDays[timeClient.getDay()];
      Serial.print("Week Day: ");
      Serial.println(weekDay);

      time_t epochTime = timeClient.getEpochTime();

      //Get a time structure
      struct tm *ptm = gmtime((time_t *)&epochTime);

      int monthDay = ptm->tm_mday;
      Serial.print("Month day: ");
      Serial.println(monthDay);

      int currentMonth = ptm->tm_mon + 1;
      Serial.print("Month: ");
      Serial.println(currentMonth);

      String currentMonthName = months[currentMonth - 1];
      Serial.print("Month name: ");
      Serial.println(currentMonthName);

      int currentYear = ptm->tm_year + 1900;
      Serial.print("Year: ");
      Serial.println(currentYear);

      String latitude = String(gps.location.lat(), 6);
      String longitude = String(gps.location.lng(), 6);
      String coordinates = latitude + "," + longitude;
      jsonSensor.set("Location", coordinates);

      //Print complete date:
      String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
      jsonSensor.set("Date", currentDate + " " + timeClient.getFormattedTime());

      if (Firebase.pushJSON(fbdo, "/sensors/Alat 1", jsonSensor)) { //pushJSON untuk menambah data dan menyimpan

        Serial.println(fbdo.dataPath());

        Serial.println(fbdo.pushName());

        Serial.println(fbdo.dataPath() + "/" + fbdo.pushName());

      } else {
        Serial.println(fbdo.errorReason());
      }
    }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true)
      ;
  }
}

void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }
  NTPtime();
  NPKread();
  oledShow();

  Serial.println();
}
