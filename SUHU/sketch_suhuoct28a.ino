#include <DHT.h>
#define DHTpin 4  //pin dht saya di esp32
#define DHTtipe DHT11 //tipe dht saya

DHT dhtSuhu(DHTpin, DHTtipe);

//pin
const int merah = 5; //esp32 pin merah
const int kuning = 18; //esp32 pin kuning
const int hijau = 19; //esp32 pin hijau
const int tombol = 21;

//status
int mydhtsuhu = LOW;

String aman = String("aman");
String hatihati = String("hati-hati");
String bahaya = String("bahaya");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(10000);

  dhtSuhu.begin();
  pinMode(merah, OUTPUT);
  pinMode(kuning, OUTPUT);
  pinMode(hijau, OUTPUT); 
  pinMode(tombol, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(mydhtsuhu == HIGH){
    Serial.println("Program Pendeteksi Suhu: ");
    if(digitalRead(tombol) == LOW){
      mydhtsuhu = LOW;
      delay(2500);
    }
    unsigned long temperature = dhtSuhu.readTemperature();
    if( isnan(temperature)){
      Serial.println("MAAF DHT RUSAK!");
    }
    Serial.println("Suhu berada pada : ");
    Serial.println(temperature);
    Serial.println("C\n");
    if(temperature<=37){
      Serial.println(aman);
      digitalWrite(merah,LOW);
      digitalWrite(kuning,LOW);
      digitalWrite(hijau,HIGH);
    }else if(temperature>37 && temperature<=50){
      Serial.println(hatihati);
      digitalWrite(merah,LOW);
      digitalWrite(kuning,HIGH);
      digitalWrite(hijau,LOW);
    }else{
      Serial.println(bahaya);
      digitalWrite(merah,HIGH);
      digitalWrite(kuning,LOW);
      digitalWrite(hijau,LOW);
    }
  }else{
    Serial.println("Silahkan Tekan Tombol Dahulu");
    if(digitalRead(tombol) == LOW){
      mydhtsuhu = HIGH;
      delay(2500);
    }
      digitalWrite(merah,LOW);
      digitalWrite(kuning,LOW);
      digitalWrite(hijau,LOW);
      delay(2500);
  }
}
