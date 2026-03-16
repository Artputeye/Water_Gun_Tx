#include "@Operation.h"

#define PIN_STEER 35
#define PIN_THROTTLE 39
#define PIN_FIRE 25

#define DEADZONE 180
#define SEND_INTERVAL 30     // max rate ~33Hz

typedef struct
{
  int8_t throttle;
  int8_t steering;
  bool fire;
} ControlPacket;

ControlPacket data;
ControlPacket lastData;

unsigned long lastSend = 0;

// แยก filter ของแต่ละ pin
float steerFiltered = 0;
float throttleFiltered = 0;

int readFiltered(int pin, float &filtered)
{
  int raw = analogRead(pin);
  filtered = filtered * 0.7 + raw * 0.3;
  return (int)filtered;
}

int8_t convertAxis(int value)
{
  int center = 2048;
  int diff = value - center;

  if (abs(diff) < DEADZONE)
    return 0;

  diff = constrain(diff, -2048, 2047);
  return map(diff, -2048, 2047, -100, 100);
}

void setupIO()
{
  pinMode(PIN_FIRE, INPUT_PULLUP);

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_STEER, ADC_11db);
  analogSetPinAttenuation(PIN_THROTTLE, ADC_11db);
}

void setupEspNow()
{
  Serial.println("⚡ ESP-NOW init");

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);

  peer.channel = WiFi.channel();

  peer.encrypt = false;
  peer.ifidx = (wifimode == 0) ? WIFI_IF_AP : WIFI_IF_STA;

  if (esp_now_add_peer(&peer) != ESP_OK)
  {
    Serial.println("❌ Peer add failed");
    return;
  }

  Serial.println("✅ ESP-NOW ready");
}

void operation()
{
  // อ่านค่า
  data.steering = convertAxis(readFiltered(PIN_STEER, steerFiltered));
  data.throttle = convertAxis(readFiltered(PIN_THROTTLE, throttleFiltered));
  data.fire = (digitalRead(PIN_FIRE) == LOW);

  // ส่งเฉพาะเมื่อข้อมูลเปลี่ยน
  if (memcmp(&data, &lastData, sizeof(data)) != 0 &&
      millis() - lastSend >= SEND_INTERVAL)
  {
    esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));

    lastData = data;
    lastSend = millis();
  }
}

