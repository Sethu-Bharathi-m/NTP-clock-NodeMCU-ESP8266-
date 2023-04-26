#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>  // Library link:     https://github.com/taranais/NTPClient
#include <WiFiUdp.h>
ESP8266WebServer server(80);

const char* ssid = "Redmi 6 Pro";        // SSSID means the hotspot name
const char* password = "SethuBharathi";  // wifi Password

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org", 19802.4, 60000);  //19800 i.e, time zone for India +5:30 hrs, = 330minutes, 330minutes*60seconds = 19800seconds (converting hours to sec)
char dayWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  String name = ssid;
  Serial.print("Connected to WiFi (");
  Serial.print(name);
  Serial.println(")");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  timeClient.update();
  Serial.println();

  String formattedTime = timeClient.getFormattedTime();
  // Serial.println(formattedTime);

  int hh = timeClient.getHours();
  int mn = timeClient.getMinutes();
  int sec = timeClient.getSeconds();

  int day = timeClient.getDay();
  // Serial.println(dayWeek[day]);

  String date_time = timeClient.getFormattedDate();  //output: 2022-04-10T16:23:10Z
  // Serial.println(date_time);

  int index_date = date_time.indexOf("T");
  String dd = date_time.substring(8, index_date);
  String mm = date_time.substring(5, index_date - 3);
  String yyyy = date_time.substring(0, index_date - 6);
  String date = (dd + "-" + mm + "-" + yyyy);
  // Serial.println("Date: " + date);

  Serial.println("Date: " + date);
  Serial.println("Day: " + String(dayWeek[day]));
  Serial.println("Time: " + formattedTime);

  delay(1000);
}



void handleRoot() {

  String time = String("The Time is ") + String(timeClient.getFormattedTime()) + "\n";
  String day = String("The Day is ") + String(String(dayWeek[timeClient.getDay()])) + "\n";

  String date_time = timeClient.getFormattedDate();
  int index_date = date_time.indexOf("T");
  String dd = date_time.substring(8, index_date);
  String mm = date_time.substring(5, index_date - 3);
  String yyyy = date_time.substring(0, index_date - 6);
  String date = "The Date is " + (dd + "-" + mm + "-" + yyyy);


  String cast = "<html> <meta http-equiv=\"refresh\" content=\"2\"> <body bgcolor=\"#aaff80\"> <h3></h3>" + date + "<br>" + day + "<br>" + time + "<br>" + "</body></html>";

  server.send(200, "text/html", cast);
}
