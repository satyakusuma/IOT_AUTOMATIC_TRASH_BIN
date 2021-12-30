#include <NewPing.h> //Library ultrasonik
#include <WiFi.h>  //Library Wifi
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h> //Library LCD 1602
#include <ThingSpeak.h> //Library Thingspeak
#include <BlynkSimpleEsp32.h> //Library blynk
#include <ESP32Servo.h> //LIbrary servo esp32

#define BLYNK_PRINT Serial //Define blynk  
int lcdColumns = 16; //Inisisasi Coloumn
int lcdRows = 2; //Inisiasi rows

char auth[] = "BoYpcly0NijeEh3j-T3mKnAHmBJ--sM0"; //Dapat dari email
char ssid[] = "Galaxy A50"; //Nama wifi
char pass[] = "ra entuk";  //Password wifi
unsigned long myChannelNumber = 1603040; //ID akun ThingSpeak
const char * myWriteAPIKey = "03UEPG70AGCPWLZA"; //memasukan apikey
WiFiClient client;

#define TRIGGER_PIN 33 //Trig pin Ultrasonik
#define ECHO_PIN 32 //Echo pin Ultrasonik
#define MAX_DISTANCE 200 //Max Jarak Ultrasonik
#define infraredPin 26 //Pin Sensor PIR (Deteksi Gerakan) 
int keyIndex = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int infraredValue = HIGH; //Define pirValue
int jarak; //Define jarak
Servo myservo; //Define servo
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //Define LCD 1602
int waktu_sebelumnya = 0;

void setup(){  //Pengaturan Pin dan Variabel
  lcd.init();//Inisiasi LCD
  lcd.backlight(); //Menyalakan Backlight
  Serial.begin(115200);
  delay(10);
  Blynk.begin(auth, ssid, pass, "blynk.azizfath.my.id", 8080); 
  pinMode(infraredPin, INPUT); //pirPin as INPUT
  myservo.attach(13); //Pin Servo
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop()
{  //Perulangan Program
  infraredValue = digitalRead(infraredPin);
  jarak = sonar.ping_cm();
  getInfraredValue();
  getJarak(); //Memanggil fungsi getJarak
  Blynk.run(); //Run blynk
  Blynk.virtualWrite(V1, infraredValue); // V1 for infraredValue
  Blynk.virtualWrite(V0, jarak); // V0 for ultrasonik
  lcd.setCursor(0, 0);//Set letak tulisan
  lcd.print("JAGA KEBERSIHAN");
  lcd.setCursor(0, 1);//Set letak tulisan
  lcd.print("Terimakasih");
  ThingSpeak.setField(1, jarak);
  ThingSpeak.setField(2, infraredValue);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
}

void getInfraredValue(void)
{
  unsigned long waktu_sekarang = millis();
  if(waktu_sekarang - waktu_sebelumnya >= 5000)
  {
    waktu_sebelumnya = waktu_sekarang;
    if (infraredValue == LOW) //Bila ada gerakan terbaca
    {
       Blynk.notify("Ada yang membuang sampah"); //Mengirim notifikasi
       myservo.write(90);
       
    }
    else 
    {
      myservo.write(0);
    }
  } 
}

void getJarak(void)
{
  if (jarak < 5)
  {
    Blynk.notify("Tempat sampah penuh"); //Mengirim notifikasi
  }
}
