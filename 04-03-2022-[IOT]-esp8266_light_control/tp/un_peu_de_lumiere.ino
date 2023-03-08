#include <ESP8266WiFi.h>

// declare variables
const String wifiName = "Redmi Note 9T";
const String wifiPass = "Aqwaba2000";
const int    ledPin   = 13;
int value = LOW;
WiFiServer server(80);


void setup() {
  Serial.begin(9600);
  // initialize led
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // let try to connect to WIFI
  Serial.print("ESP8266 TRYING TO CONNECT NETWORK =>  ");
  Serial.println(wifiName);
  WiFi.begin(wifiName, wifiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(". ");
  }
  Serial.print("\nConnected\n");
 
  // let start our web server
  server.begin();
  Serial.println("================= Server started =================");
  Serial.print("Use this url to connect :\t\thttp://");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  // get new connexion
  WiFiClient client = server.available();
  
  if (client) {
    // give view to client
    renderHtml(client, wifiName);
    
    //waiting for user send something
    Serial.println("user = new Action()");
    while(!client.available()){
      delay(1);
    }
   
    //get url to find user action
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
   
    //traitment
    if (request.indexOf("/device1=on") != -1)  {
      digitalWrite(ledPin, HIGH);
      value = HIGH;
    }
    if (request.indexOf("/device1=off") != -1)  {
      digitalWrite(ledPin, LOW);
      value = LOW;
    }
  }
  client.stop();
}

void renderHtml(WiFiClient client, String wifiName) {
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'/>");
  client.println("<title>IoT - MANAGER PAGE</title>");
  client.println("<style>");
  client.println("a{ text-decoration: none; font-weight: bold;} h2{font-size: 1em; text-align: center;} table{border-collapse: collapse; width: 100%;}");
  client.println("td{border: 1px solid #f08713; padding: 0.2em;} table tr td:last-of-type{width: 200px;}");
  client.println("table tr:first-of-type{font-weight: bold;}button:hover{background-color: #f08713; border: 1px solid #ddd;border-radius: 5px;color: white;cursor: pointer;}</style>");
  client.println("</head>");
  client.println("<body style='background-color: #2c94c4; margin: 0; font-family: system-ui;'>");
  client.println("<header style='color: black; padding: 0.8em; background-color: #0068b8; margin: auto;'>");
  client.println("<h1 style='text-align: center; color: black; padding: 0.5em; background-color: #fff; display: inline-block; margin: auto; border-radius: 10px; font-size: 1.1em;'>AIoT Club</h1>");
  client.println("<h1 style='text-align: center; color: black; padding: 0.5em; background-color: #fff; display: inline-block; margin: auto; border-radius: 10px; font-size: 1.1em; float: right; background-color: #f08713;'>WiFi : ");
  client.print(wifiName);
  client.println("</h1></header>");
  client.println("<div id='mainPage' style='width: 90%; height: 90vh; margin: auto; position: relative; top: 1.5em; display:flex; align-items:center; justify-content:center;'>");

  client.println("<a href='/device1=on'><button style='height: 60px; width: 120px; margin-right:0.5em;'> ON </button></a>");
  client.println("<a href='/device1=off'><button style='height: 60px; width: 120px;'> OFF </button></a>");

  client.println("</div>");
  
  client.println("</body>");
  client.println("</html>");
}
