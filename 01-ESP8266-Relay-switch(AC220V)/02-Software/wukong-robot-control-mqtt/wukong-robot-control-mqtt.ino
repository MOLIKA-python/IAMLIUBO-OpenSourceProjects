/* wukong-robot control the NodeMCU by MQTT
   Compile with Arduino IDE
   Author: IAMLIUBO
   Github: https://github.com/imliubo/IAMLIUBO-OpenSourceProjects/01-ESP8266-Relay-switch(AC220V)
*/
#include "EspMQTTClient.h"

#define RELAY_PIN D1
#define LED_PIN   D6

void onConnectionEstablished();

EspMQTTClient client(
  "CMCC-7uHL",                 // Wifi ssid
  "zxcvbnmlk",                 // Wifi password
  onConnectionEstablished,     // MQTT connection established callback
  "192.168.1.57"               // MQTT broker ip
);

long lastTime = 0;
uint8_t pin_status = 0;

void LED_Control_Callback(const String & payload) {
  const char* p = payload.c_str();
  //  Serial.println(p);
  if (strstr(p, "开灯")) {
    digitalWrite(RELAY_PIN, HIGH);
    client.publish("/wukong/mqtt", "主人，灯已打开！");// "wukong/mqtt" 是在config.yml中定义的 topic_s 字段，用来回复wukong_robot的自定义消息。
  }
  if (strstr(p, "关灯")) {
    digitalWrite(RELAY_PIN, LOW);
    client.publish("/wukong/mqtt", "主人，灯已关闭！");
  }
}

void onConnectionEstablished()
{
  client.subscribe("开发板一", LED_Control_Callback);// "开发板一" 是在action.json文件中定义的，用来接收wukong-robot下发的命令。
}

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop()
{
  client.loop();

  long now = millis();
  if (now - lastTime > 1000) {
    lastTime = now;
    digitalWrite(LED_PIN, !pin_status);
    pin_status = !pin_status;
  }
}
