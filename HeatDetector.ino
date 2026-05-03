#include <Wire.h>
#include <DFRobot_MLX90614.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// WiFi credentials
#define ssid "ATTFVUyTUI"
#define password "Division131!!"

// Telegram Bot Token
#define BOT_TOKEN "8681924182:AAF_jEAP68AYE7yk0w0JKczuPzv_vwNJEk0"

// Your Telegram Chat ID (get from @userinfobot)
#define CHAT_ID "8643071720"

// Temperature thresholds (Celsius)
#define TEMP_WARNING  60.0   // Warning level
#define TEMP_DANGER   85.0   // Danger level
#define TEMP_CRITICAL 120.0  // Critical / fire risk level

// How often to poll Telegram messages
#define BOT_MTBS 1000

// Cooldown between alerts (ms) — prevents spam (30 seconds)
#define ALERT_COOLDOWN 30000

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime    = 0;
unsigned long last_alert_time = 0;
bool alertSent = false;

DFRobot_MLX90614_I2C mlx;
bool mlxFound = false;

void sendFireAlert(float object, float ambient) {
  String msg = "";

  if (object >= TEMP_CRITICAL) {
    msg = "CRITICAL FIRE RISK!\n";
  } else if (object >= TEMP_DANGER) {
    msg = "DANGER: High temperature detected!\n";
  } else if (object >= TEMP_WARNING) {
    msg = "WARNING: Elevated temperature detected!\n";
  }

  msg += "Object Temp:  " + String(object)  + " C\n";
  msg += "Ambient Temp: " + String(ambient) + " C\n";
  msg += "Check your surroundings immediately!";

  bot.sendMessage(CHAT_ID, msg, "");
  Serial.println("Alert sent: " + msg);
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text    = bot.messages[i].text;
    String from    = bot.messages[i].from_name;

    Serial.println("Message from: " + from + " | Text: " + text);

    if (text == "/start") {
      String welcome = "Hello, " + from + "!\n";
      welcome += "Available commands:\n";
      welcome += "/status      - Get LED status\n";
      welcome += "/temp        - Read current temperature\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    else if (text == "/status") {
      String state = digitalRead(LED_BUILTIN) ? "ON" : "OFF";
      bot.sendMessage(chat_id, "LED is currently: " + state, "");
    }

    else if (text == "/temp") {
      if (!mlxFound) {
        bot.sendMessage(chat_id, "Sensor not found. Check wiring.", "");
      } else {
        float ambient = mlx.getAmbientTempCelsius();
        float object  = mlx.getObjectTempCelsius();
        if (isnan(ambient) || isnan(object)) {
          bot.sendMessage(chat_id, "Bad reading, try again.", "");
        } else {
          String msg = "Ambient: " + String(ambient) + " C\n";
          msg       += "Object:  " + String(object)  + " C";
          bot.sendMessage(chat_id, msg, "");
        }
      }
    }

    else {
      bot.sendMessage(chat_id, "Unknown command. Type /start for help.", "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(LED_BUILTIN, OUTPUT);

  // WiFi setup
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println("IP: " + WiFi.localIP().toString());

  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  Serial.println("Bot ready!");
  
  // MLX90614 setup
  mlxFound =(mlx.begin() == NO_ERR);
  while (!mlxFound) {
    Serial.println("Error: MLX90614 not found. Check SDA/SCL, power, and ground.");
    delay(5000);
  }
  Serial.println("Sensor found!");
}  

void loop() {
  // MLX90614 loop
  if (!mlxFound) {
    delay(1000);
  } else {
    float ambient = mlx.getAmbientTempCelsius();
    float object  = mlx.getObjectTempCelsius();

    if (isnan(ambient) || isnan(object)) {
      Serial.println("Bad reading, retrying...");
      delay(500);
    } else {
      Serial.print("Ambient: ");
      Serial.print(ambient);
      Serial.print(" C  |  Object: ");
      Serial.print(object);
      Serial.println(" C");

      // Fire alert check
      bool thresholdBreached = (object >= TEMP_WARNING);
      unsigned long now      = millis();
      bool cooldownExpired   = (now - last_alert_time > ALERT_COOLDOWN);

      if (thresholdBreached && cooldownExpired) {
        sendFireAlert(object, ambient);
        last_alert_time = now;
      }

      // Reset alertSent when temp returns to safe level
      if (object < TEMP_WARNING) {
        if (alertSent) {
          bot.sendMessage(CHAT_ID, "Temperature back to safe levels.", "");
          alertSent = false;
        }
      } else {
        alertSent = true;
      }
    }
  }

  // Telegram bot loop
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("New messages: " + String(numNewMessages));
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }

  delay(1000);
}