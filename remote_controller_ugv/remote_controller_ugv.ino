#include <esp_now.h>
#include <WiFi.h>

const uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0xD9, 0x31, 0x30};  // deneyap kart MAC adresi

#define joyRX A4   // Joystick 1 x pini A4 ileri geri yapacak 
#define joyRY A5   // Joystick 1 y pini A5 problem var 
#define joyLX A0   // Joystick 1 x pini A4
#define joyLY A1   // Joystick 1 y pini A5 sağ sol yapacak 

/* Karsi tarafa yollanacak veri paketi */
typedef struct struct_message { // karsi tarafa yollanacak veri paketi 
  int joyR_X;
  int joyR_Y;
  int joyL_X;
  int joyL_Y;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;   // Eslesilen cihaz bilgisi

/* paket yollandiginda cagirilacak callback fonksiyonu*/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup(){
 
  Serial.begin(115200);
  
  /* Set device as a Wi-Fi Station */
  WiFi.mode(WIFI_STA);

  /* kullanilan pin tanimlamari yapilir */
 pinMode(joyRX, INPUT);
 pinMode(joyRY, INPUT);
 pinMode(joyLX, INPUT);
 pinMode(joyLY, INPUT);
 dacWrite(D12,210);
  
  /* ESP-NOW init edilir */
  if (esp_now_init() != 0) {
      Serial.println("Error initializing ESP-MOW");
      return;
  }

  /* Once ESPNow is successfully Init, we will register for Send CB to
     get the status of Trasnmitted packet */
     // esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
       esp_now_register_send_cb(OnDataSent);

  // Register peer
  //esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

    // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
   return;
  }

}
 
void loop(){
  /* joystick'ten okuma islemi yapilir */
  myData.joyR_X = analogRead(joyRX);
  myData.joyR_Y = analogRead(joyRY);
  myData.joyL_X = analogRead(joyLX);
  myData.joyL_Y = analogRead(joyLY);
  Serial.print("joyR_X;: ");
  Serial.println(myData.joyR_X);
  Serial.print("joyR_Y received: ");
  Serial.println(myData.joyR_Y);
  Serial.print("joyL_X received: ");
  Serial.println(myData.joyL_X);
  Serial.print("joyL_Y received: ");
  Serial.println(myData.joyL_Y);
  /* buraya bazi islemler gelebilir okunan veri 0-45
     45 -90 arasına göre farkli degerler yazabiliriz */
  
  if( esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData)) == ESP_OK){
    Serial.println("Veri gönderimi başarılı.");
  }else{
    Serial.println("Veri gönderimi başarısız!");
  }
  delay(10);
  }
