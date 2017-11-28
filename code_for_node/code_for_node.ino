d a
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

IPAddress server(128,199,163,14);

const char* ssid = "AonMerauqs";
const char* password = "fuckyou!";

int url_to_int(String url){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      return payload.toInt();
    } 
    else {
      return httpCode;
    }
    http.end();   //Close connection
  }
  return -1;
}

String url_to_string(String url){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      return payload;
    } 
    else {
      return "";
    }
    http.end();   //Close connection
  }
  return "";
}

int call_url(String url){
  if (WiFi.status() == WL_CONNECTED) {
   HTTPClient http;
   http.begin(url);
   int httpCode = http.GET();
   if (httpCode > 0) {
     String payload = http.getString();
     if(payload != "") {
       return 1;
     }
     else{
       return 0;
     }
   }
   else {
     return 0;
   }
   http.end();
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String usr, otp;
int h, m;
void loop() {
  WiFiClient client;
  const int httpPort = 22;
  //get data from server
  otp = url_to_string("http://128.199.163.14/OTP.txt");
  Serial.println(otp);
//  check = url_to_string("http://128.199.163.14/usr.txt");
  usr = "aon";//get usr from practicum board
  if(call_url("http://128.199.163.14/pracGetUsr.php?usr="+usr)){
    Serial.println("done");  
  }
  else{
    Serial.println("failed");  
  }

  delay(1000);
}
