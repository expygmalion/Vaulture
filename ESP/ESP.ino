#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EMailSender.h>

const char* APssid="TEST ";
const char* APpassword="12345678";

const char* ssid="12345678";
const char* password="11223344";

#define myemail "123@xyz.com" // replace with your email
#define pass "abcd" // replace with password
#define recv_Email "abc@xyz.com" // replace with email to recieve message
#define port 465

EMailSender sender(myemail,pass,recv_Email,serversmtp,port);

int inCorrectCount = 0;
String lockerPassword = "1234";

ESP8266WebServer server(80);
IPAddress ip(192,168,4,1),gateway(192,168,1,1),subnet(255,255,255,0);

SoftwareSerial mySerial(D6, D5); // RX, TX



void setup() {
  Serial.begin(115200);
  // mySerial.begin(115200);

  //  WiFi.softAPConfig(ip,gateway,subnet);
  WiFi.softAP(APssid,APpassword);

   WiFi.begin(ssid,password);

  IPAddress IP =WiFi.softAPIP();
   while (WiFi.status() != WL_CONNECTED) 
   {
         delay(1000);
         Serial.println("Connecting to WiFi...");
   }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleOnConnect);
  server.on("/submit", checkPassword);
  //server.onNotFound(handleOnInCorrect);

  server.begin();
  Serial.println("Server has started");
}
void handleOnConnect()
{
  String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Password Form</title></head><body>";
    html += "<h1>Password Submission Form</h1>";
    html += "<form action=\"/submit\" method=\"get\">";
    html += "<label for=\"password\">Enter Password:</label>";
    html += "<input type=\"password\" id=\"password\" name=\"password\" required>";
    html += "<button type=\"submit\">Submit</button>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
}



void sendmail()
{
    EMailSender::EMailMessage msg;
    msg.subject="warning";
    msg.message=" the incorrectcount more than 3 ";
    EMailSender::Response resp = sender.send(recv_Email,msg);
    Serial.println("sending the email: ");
    Serial.println(resp.desc);
  
}

void sendmail2()
{
    EMailSender::EMailMessage msg;
    msg.subject="New password!";
    msg.message="passwod changed!";
    EMailSender::Response resp = sender.send(recv_Email,msg);
    Serial.println("sending the email: ");
    Serial.println(resp.desc);
}

void loop() 
{
  server.handleClient();
  if(Serial.available() > 0)
  {
    String data = Serial.readStringUntil('s');
    int index = data.indexOf("originalPassword:");

    if(data == "limit")
    {
      sendmail();
    }
    else if(index != -1)
    {
      lockerPassword = data.substring(index+17);
      sendmail2();
      Serial.print("changeee");
    }
  }

}

void checkPassword()
{
  String password = server.arg("password");
  if(password==lockerPassword)
  {
    Serial.print("oks");
    server.send(200,"text/plain","Password is correct");
    delay(200);
    
  }
  else
  {
    inCorrectCount++;
    Serial.print("nos");
    server.send(400,"text/plain","Password is not correct");
 delay(200);
}
}