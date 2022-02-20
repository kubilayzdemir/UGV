#include <esp_now.h>
#include <WiFi.h>
#define AIN1 D4
#define AIN2 D1
#define PWMA D0
#define BIN1 D9
#define BIN2 D15
#define PWMB D14
//#define STBY D8
#define HIZ 255
#define PWM_FREQ  5000       
#define PWM_CH_A 0  
#define PWM_CH_B 1
#define PWM_RES   8    
#define JoyMin 500
#define JoyMax 7500
void sag_geri(int hiz);
void sag_ieri(int hiz);
void sag_geri(int hiz);
void sag_ileri(int hiz);
void dur();

typedef struct struct_message { // karsi tarafa yollanacak veri paketi 
  int joyR_X;
  int joyR_Y;
  int joyL_X;
  int joyL_Y;
} struct_message;

struct_message myData;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&myData, incomingData, sizeof(myData));

  
  
}

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  //pinMode(STBY, OUTPUT);
  // Set device as a Wi-Fi Station
  Serial.begin(115200);
  
  ledcSetup(PWM_CH_A, PWM_FREQ, PWM_RES);                // PWM zamanlayicisi ayarlandi 
  ledcAttachPin(PWMA, PWM_CH_A);       // Motor PWM pini ile zamanlayici iliskilendirildi
  ledcSetup(PWM_CH_B, PWM_FREQ, PWM_RES);                // PWM zamanlayicisi ayarlandi 
  ledcAttachPin(PWMB, PWM_CH_B);       // Motor PWM pini ile zamanlayici iliskilendirildi  

  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW tanımlama hatası");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("joyR_X;: ");
  Serial.println(myData.joyR_X);
  Serial.print("joyR_Y received: ");
  Serial.println(myData.joyR_Y);
  Serial.print("joyL_X received: ");
  Serial.println(myData.joyL_X);
  Serial.print("joyL_Y received: ");
  Serial.println(myData.joyL_Y);
  if( myData.joyR_X > JoyMax && myData.joyL_Y > JoyMin && myData.joyL_Y < JoyMax ){ // sadece ileri 
    sag_ileri(HIZ);
    sol_ileri(HIZ);
  }

  if(myData.joyR_X < JoyMin && myData.joyL_Y > JoyMin && myData.joyL_Y < JoyMax ){ // sadece geri 
    sag_geri(HIZ);
    sol_geri(HIZ);
  }

   if(myData.joyL_Y > JoyMax && myData.joyR_X > JoyMin && myData.joyR_X < JoyMax ){ // sadece SAĞA 
    sag_ileri(HIZ);
    sol_geri(HIZ);
  }

   if(myData.joyL_Y < JoyMin && myData.joyR_X > JoyMin && myData.joyR_X < JoyMax ){ // sadece sola 
    sol_ileri(HIZ);
   sag_geri(HIZ);
  }
  delay(10);
  dur();  
  

}

void sag_ileri(int hiz){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWM_CH_A, hiz); 
}


void sol_ileri(int hiz){
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(PWM_CH_B, hiz); 
}

void sag_geri(int hiz){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(PWM_CH_A, hiz); 
}

void sol_geri(int hiz){
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  ledcWrite(PWM_CH_B, hiz); 
}

void dur(){
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}
