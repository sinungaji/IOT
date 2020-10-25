const int merah = 5; //esp32 pin merah
const int kuning = 18; //esp32 pin kuning
const int hijau = 19; //esp32 pin hijau

const long interval = 5000;
long kuningBlink = 0;

unsigned long previousMillis = 0; 

const long durance[] = {4,1,4}; //berapa second di state

enum status {MERAH, KUNING, HIJAU};
status statusLED = MERAH;


void setup() {
  // put your setup code here, to run once:
   pinMode(merah, OUTPUT);
   pinMode(kuning, OUTPUT);
   pinMode(hijau, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

    unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= durance[statusLED] * 1000) {
    previousMillis = currentMillis;

    switch(statusLED){
      case MERAH:
        digitalWrite(merah,HIGH);
        digitalWrite(kuning,LOW);
        kuningBlink = 0;
        digitalWrite(hijau,LOW);
        statusLED = KUNING; //step case selanjutnya
        break;
      case KUNING:
        digitalWrite(merah,LOW);
        digitalWrite(kuning,HIGH);
        kuningBlink = 1;
        digitalWrite(hijau,LOW);
        statusLED = HIJAU;
        break;
      case HIJAU:
        digitalWrite(merah,LOW);
        digitalWrite(kuning,LOW);
        kuningBlink = 0;
        digitalWrite(hijau,HIGH);
        statusLED = MERAH;
        break;
        
    }
 
  }
  if (kuningBlink && (currentMillis - previousMillis >= interval)) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
  
      digitalWrite(kuning, !digitalRead(kuning));
    
  }
}
