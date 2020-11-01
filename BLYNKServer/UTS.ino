#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHTpin 4  //pin dht saya di esp32
#define DHTtipe DHT11 //tipe dht saya

#define LDR 23 //pin LDR di esp32

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"

DHT dhtSuhu(DHTpin, DHTtipe);
BlynkTimer timer;

//TOKEN, SSID, PASSWORD
char auth[] = "VIIc8LjyExOczCG1O6RBunpAYkOnvmlf"; //token auth   
char ssid[] = "BERSINAR"; //ssid wifi
char pass[] = "tanyajeki"; // pass wifi

//PIN LED FISIK
const int merahCahaya = 5; //esp32 pin merah menandakan cahaya
const int hijauSuhu = 19; //esp32 pin hijau menandakan suhu

//button
int button = 0;

//status suhu
int mydhtsuhu = LOW;
String aman = String("aman");
String bahaya = String("bahaya");

//slider cahaya
int setpointCahaya = 0;

//slider suhu
int setpointSuhu = 0;

//slider cahaya
BLYNK_WRITE(V4){
  setpointCahaya = param.asInt();  
  Serial.print("LUX: ");
  Serial.println(setpointCahaya);
}

//slider suhu
BLYNK_WRITE(V5){
  setpointSuhu = param.asInt();  
  Serial.print("SUHU: ");
  Serial.println(setpointSuhu);
}

WidgetLED ledMerahCahaya(V6);
WidgetLED ledHijauSuhu(V7);

BLYNK_WRITE(V8) {
  button = param.asInt();
}

void sendSensorSuhu(){
 //cek dht berfungsi
  unsigned long temperature = dhtSuhu.readTemperature();
  if( isnan(temperature)){
    Serial.println("MAAF DHT RUSAK!");
  }

  if(setpointSuhu <= temperature){ 
    digitalWrite(hijauSuhu, LOW);
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V3, temperature);
    Serial.println("Suhu berada pada : ");
    Serial.println(temperature);
    Serial.println("C");
    ledHijauSuhu.setColor(BLYNK_GREEN);
    Serial.println("TEMPERATURE LED HIJAU");
  }  
  else{
    digitalWrite(hijauSuhu, HIGH); 
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V3, temperature);
    Serial.println("Suhu berada pada : ");
    Serial.println(temperature);
    Serial.println("C");
    ledHijauSuhu.setColor(BLYNK_RED);
    Serial.println("TEMPERATURE LED MERAH");
  }
}

void sendSensorCahaya(){
 //cek LDR berfungsi
 int input = analogRead(LDR);
 int lux = (0.009768 * input) + 10;
 if(lux <= setpointCahaya) {
    Serial.println(lux);
    Blynk.virtualWrite(V0,lux);
    Blynk.virtualWrite(V2,lux);
    Serial.println("LUX berada pada : ");
    Serial.println(lux);
    Serial.println("LUX");
    digitalWrite(merahCahaya, LOW);
    digitalWrite(hijauSuhu,HIGH);
    ledMerahCahaya.setColor(BLYNK_GREEN);
    Serial.println("LUX LED HIJAU");
  }
  else {
    Serial.println(lux);
    Blynk.virtualWrite(V0,lux);
    Blynk.virtualWrite(V2,lux);
    Serial.println("LUX berada pada : ");
    Serial.println(lux);
    Serial.println("LUX");
    digitalWrite(merahCahaya, HIGH);
    digitalWrite(hijauSuhu,LOW);
    ledMerahCahaya.setColor(BLYNK_RED);
    Serial.println("LUX LED MERAH");
  }
}

void setup() {
  // Debug console
  Serial.begin(9600);
 
  //Blynk.begin(auth, ssid, pass);
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);

  //IP server lokal
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,100,82), 8080);
  dhtSuhu.begin();

  //Output pin
  pinMode(merahCahaya, OUTPUT);
  pinMode(hijauSuhu,OUTPUT);

  //waktu sensor suhu
  timer.setInterval(1000L, sendSensorSuhu);
  //waktu sensor cahaya
  timer.setInterval(1000L, sendSensorCahaya);
  
  ledMerahCahaya.on();
  ledHijauSuhu.on(); //led virtual blink
  

}

void loop() {
  Blynk.run();
  if(button == 1){
    timer.run();
    Serial.println("System ON");
  }else {
    Serial.println("System OFF");
    digitalWrite(merahCahaya, LOW);
    digitalWrite(hijauSuhu,LOW);
  }
}
