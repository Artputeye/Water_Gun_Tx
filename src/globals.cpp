#include "globals.h"

AsyncWebServer server(80);

char DEVICE_NAME[28] = "Water Gun TX";  //"ARRTECH";
char DEVICE_PASS[28] = "12345678"; 

char WIFI_NAME[30]; //load from FS (network.json)
char PASSWORD[25]; //load from FS (network.json)

char HOST[30] = "guntx" ; //hostname
char IP_ADDR[16];
char SUBNET_MASK[16];
char DEFAULT_GATEWAY[16];

char MQTT_ADDR[16];     //= "192.168.101.100";//"192.168.1.247";
char MQTT_USERNAME[28]; //= "mqtt-user";//"inverter";  // replace with your credentials
char MQTT_PASSWORD[28]; //= "12345678";
char MQTT_PORT[5];      //= "1883";

char D_SoftwareVertion[15] = "1.5.8";
char D_Mfac[15] = "ARTTECH";
char D_Model[15] = "Water Gun";

char user[10] = "admin"; // OTA Authentication pass
char pass[10] = "12345678";

char MAC_RECEIVE[18];
uint8_t receiverMAC[6];

int ledState = LOW;

bool ipconfig;
bool wifimode;

String serialData;
