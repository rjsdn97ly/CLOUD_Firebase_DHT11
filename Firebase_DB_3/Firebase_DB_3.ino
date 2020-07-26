#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define DHTPIN D4
#define DHTTYPE DHT11
#include <DHT.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

DHT dht(DHTPIN, DHTTYPE);

#define FIREBASE_HOST "hello-firebase-5b868.firebaseio.com"
#define FIREBASE_AUTH "eFySO8UGLQRNCitGC51r3J779SDyW2YgobShIwZr"
#define WIFI_SSID "test"
#define WIFI_PASSWORD "webdong1"

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID);
  dht.begin(9600);

  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //NTP클라이언트 시작
  timeClient.begin();
  //한국시간은 +9시간 = 32400초
  timeClient.setTimeOffset(32400);
  
  Serial.println();
  Serial.println("Now: " + NowString());

  // Firebase 실시간 데이터베이스 인증
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {

  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  Serial.print("Temperature = ");
  Serial.println(temp);
  Serial.print("Humidity = ");
  Serial.println(humi);

  StaticJsonBuffer<200> jsonbuffer;
  JsonObject& root = jsonbuffer.createObject();
  root["temperature"] = temp;
  root["humidity"] = humi;
  root["time"] = NowString();
  String name = Firebase.push("logDHT", root);

  if(Firebase.failed()){
    Serial.print("pushing /logs failed");
    Serial.println(Firebase.error());
    return;
  }
  Serial.println("sent data to firebase");
  delay(5000);

}

// 현재 시간 받아오는 함수
String NowString() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  //구조체 선언
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + " " + String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  return currentDate;
}
