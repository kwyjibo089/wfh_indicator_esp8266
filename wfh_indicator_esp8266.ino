/******************************************************************************************************************
*                                                                  
*                                                                  
*                                                                  
* Based on the example sketch on github:                                                                 
* https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/EchoBot/EchoBot.ino
*******************************************************************************************************************/
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = SECRET_SSID;     // your network SSID (name)
char password[] = SECRET_PASS; // your network key

// Initialize Telegram BOT
#define BOTtoken SECRET_BOT_TOKEN  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

const int  R_PIN = D8;
const int  G_PIN = D7;
const int  B_PIN = D6;

const int MIN_ANALOG = 1023;
const int MAX_ANALOG = 0;
const int MIN_COLOR  = 0;
const int MAX_COLOR  = 255;
  
void setup() {
  
  Serial.begin(115200);

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  
  analogWrite(R_PIN, 1023);  
  analogWrite(G_PIN, 0);
  analogWrite(B_PIN, 0);
  
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  client.setInsecure();  // TLS problem

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  analogWrite(R_PIN, 0);  
  analogWrite(G_PIN, 1023);
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      for (int i=0; i<numNewMessages; i++) {
        bot.sendMessage(bot.messages[i].chat_id, bot.messages[i].text, "");
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
