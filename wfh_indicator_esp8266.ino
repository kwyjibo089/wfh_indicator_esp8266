/************************************************************************************************************************



  Based on the example sketches on github:
  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/EchoBot/EchoBot.ino
  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/ChatAction/ChatAction.ino
*************************************************************************************************************************/
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

const int  D1_RED = D8;
const int  D1_GREEN = D7;
const int  D1_BLUE = D6;

const int  D2_RED = D4;
const int  D2_GREEN = D3;
const int  D2_BLUE = D2;

const int MIN_ANALOG = 1023;
const int MAX_ANALOG = 0;
const int MIN_COLOR  = 0;
const int MAX_COLOR  = 255;

void setup() {

  Serial.begin(115200);

  pinMode(D1_RED, OUTPUT);
  pinMode(D1_GREEN, OUTPUT);
  pinMode(D1_BLUE, OUTPUT);

  pinMode(D2_RED, OUTPUT);
  pinMode(D2_GREEN, OUTPUT);
  pinMode(D2_BLUE, OUTPUT);

  analogWrite(D1_RED, 1023);
  analogWrite(D1_GREEN, 0);
  analogWrite(D1_BLUE, 0);

  analogWrite(D2_RED, 1023);
  analogWrite(D2_GREEN, 0);
  analogWrite(D2_BLUE, 0);

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

  analogWrite(D1_RED, 0);
  analogWrite(D2_RED, 0);

  bool toggle = true;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    if (toggle == true) {
      analogWrite(D1_BLUE, 1023);
      analogWrite(D2_BLUE, 0);
    }
    else {
      analogWrite(D1_BLUE, 0);
      analogWrite(D2_BLUE, 1023);
    }
    toggle = !toggle;

    delay(500);
  }

  analogWrite(D1_BLUE, 0);
  analogWrite(D1_GREEN, 1023);
  analogWrite(D2_BLUE, 0);
  analogWrite(D2_GREEN, 1023);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(2000);
  analogWrite(D1_GREEN, 0);
  analogWrite(D2_GREEN, 0);
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/send_test_action") {
      bot.sendChatAction(chat_id, "typing");
      delay(4000);
      bot.sendMessage(chat_id, "Did you see the action message?");

      // You can't use own message, just choose from one of bellow

      //typing for text messages
      //upload_photo for photos
      //record_video or upload_video for videos
      //record_audio or upload_audio for audio files
      //upload_document for general files
      //find_location for location data

      //more info here - https://core.telegram.org/bots/api#sendchataction
    }

    else if (text == "/start") {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Chat Action Bot example.\n\n";
      welcome += "/send_test_action : to send test chat action message\n";
      bot.sendMessage(chat_id, welcome);
    }

    else if (text == "/red") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      delay(500);
      analogWrite(D1_RED, 1023);
      delay(4000);
      analogWrite(D1_RED, 0);
    }

    else if (text == "/blue") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      delay(500);
      analogWrite(D2_BLUE, 1023);
      delay(4000);
      analogWrite(D2_BLUE, 0);
    }
  }
}
