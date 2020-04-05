# 1 "c:\\Users\\roman\\OneDrive\\Documents\\Arduino\\wfh_indicator_esp8266\\Arduino\\wfh_indicator_esp8266\\wfh_indicator_esp8266.ino"
/************************************************************************************************************************

  Work from home indicator.

  Using a NodeMCU with two RGB leds to indicate status on a picture frame. Status updates via telegram app on smartphone

  or laptop.



  See http://blog.roman-mueller.ch/index.php/2020/04/05/dad-status-indicator/ for full instructions and hardware needed.



  Original idea by Steve Forde (@cairn4)

  https://twitter.com/cairn4/status/1245539977993355265



  Based on the example sketches on github:

  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/EchoBot/EchoBot.ino

  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/ChatAction/ChatAction.ino

*************************************************************************************************************************/
# 15 "c:\\Users\\roman\\OneDrive\\Documents\\Arduino\\wfh_indicator_esp8266\\Arduino\\wfh_indicator_esp8266\\wfh_indicator_esp8266.ino"
# 16 "c:\\Users\\roman\\OneDrive\\Documents\\Arduino\\wfh_indicator_esp8266\\Arduino\\wfh_indicator_esp8266\\wfh_indicator_esp8266.ino" 2
# 17 "c:\\Users\\roman\\OneDrive\\Documents\\Arduino\\wfh_indicator_esp8266\\Arduino\\wfh_indicator_esp8266\\wfh_indicator_esp8266.ino" 2
# 18 "c:\\Users\\roman\\OneDrive\\Documents\\Arduino\\wfh_indicator_esp8266\\Arduino\\wfh_indicator_esp8266\\wfh_indicator_esp8266.ino" 2
# 19 "c:\\Users\\roman\\OneDrive\\Documents\\Arduino\\wfh_indicator_esp8266\\Arduino\\wfh_indicator_esp8266\\wfh_indicator_esp8266.ino" 2

// Initialize Wifi connection to the router
char ssid[] = "UPC0340975"; // your network SSID (name)
char password[] = "nsj7znhyRxvg"; // your network key

// Initialize Telegram BOT


WiFiClientSecure client;
UniversalTelegramBot bot("943011215:AAGgOYuSHucvHTozNFg6FIgY289lAFTsvE0" /* your Bot Token (Get from Botfather)*/, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime; //last time messages' scan has been done

// the two rgb leds
const int D1_RED = D8;
const int D1_GREEN = D7;
const int D1_BLUE = D6;

const int D2_RED = D4;
const int D2_GREEN = D3;
const int D2_BLUE = D2;

const int MAX_ANALOG = 1023;

// for the rainbow effect
int r = 0;
int g = 0;
int b = 0;

void setup() {

  Serial.begin(115200);

  pinMode(D1_RED, 0x01);
  pinMode(D1_GREEN, 0x01);
  pinMode(D1_BLUE, 0x01);

  pinMode(D2_RED, 0x01);
  pinMode(D2_GREEN, 0x01);
  pinMode(D2_BLUE, 0x01);

  analogWrite(D1_RED, MAX_ANALOG);
  analogWrite(D1_GREEN, 0);
  analogWrite(D1_BLUE, 0);

  analogWrite(D2_RED, MAX_ANALOG);
  analogWrite(D2_GREEN, 0);
  analogWrite(D2_BLUE, 0);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  client.setInsecure(); // TLS problem

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
      analogWrite(D1_BLUE, MAX_ANALOG);
      analogWrite(D2_BLUE, 0);
    }
    else {
      analogWrite(D1_BLUE, 0);
      analogWrite(D2_BLUE, MAX_ANALOG);
    }
    toggle = !toggle;

    delay(100);
  }

  analogWrite(D1_BLUE, 0);
  analogWrite(D1_GREEN, MAX_ANALOG);
  analogWrite(D2_BLUE, 0);
  analogWrite(D2_GREEN, MAX_ANALOG);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(2000);
  ledsOff();
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      analogWrite(D1_BLUE, MAX_ANALOG);
      analogWrite(D2_BLUE, MAX_ANALOG);
      delay(100);
      Serial.println("got response");
      analogWrite(D1_BLUE, 0);
      analogWrite(D2_BLUE, 0);
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

    if (text == "/hello" || text == "/start") {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Chat Action Bot example.\n\n";
      welcome += "/send_test_action : to send test chat action message\n";
      bot.sendMessage(chat_id, welcome);
    }

    else if (text == "/meetingon" || text == "/1on") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      analogWrite(D1_RED, MAX_ANALOG);
    }

    else if (text == "/meetingoff" || text == "/1off") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      analogWrite(D1_RED, 0);
    }

    else if (text == "/headphoneson" || text == "/2on") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      analogWrite(D2_GREEN, MAX_ANALOG);
    }

    else if (text == "/headphonesoff" || text == "/2off") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      analogWrite(D2_GREEN, 0);
    }


    else if (text == "/rainbow") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);

      setColor(MAX_ANALOG, 0, 0); // red
      setColor(0, MAX_ANALOG, 0); // green
      setColor(0, 0, MAX_ANALOG); // blue
      setColor(MAX_ANALOG, MAX_ANALOG, 0); // yellow
      setColor(320, 0, 320); // purple
      setColor(0, MAX_ANALOG, MAX_ANALOG); // aqua
      ledsOff();
    }

    else if (text == "/off") {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      ledsOff();
    }

    else {
      String msg = "Received message " + text + " from " + from_name + ".\n";
      msg += "Don\'t know what to do...\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, msg);
      ledsOff();
    }
  }
}

void setColor(int red, int green, int blue) {
  while ( r != red || g != green || b != blue ) {
    if ( r < red ) r += 1;
    if ( r > red ) r -= 1;

    if ( g < green ) g += 1;
    if ( g > green ) g -= 1;

    if ( b < blue ) b += 1;
    if ( b > blue ) b -= 1;

    _setColor();
    delay(10);
  }
}

void ledsOff() {
  r = 0;
  g = 0;
  b = 0;
  _setColor();
}

void _setColor() {
  analogWrite(D1_RED, r);
  analogWrite(D2_RED, r);
  analogWrite(D1_GREEN, g);
  analogWrite(D2_GREEN, g);
  analogWrite(D1_BLUE, b);
  analogWrite(D2_BLUE, b);
}
