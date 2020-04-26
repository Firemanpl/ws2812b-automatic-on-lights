#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FastLED.h>
#define NUM_LEDS 60   // 60 LEDs numbered [0..59]
#define NUM_LEDSs 100 //
#define LED_PIN 5     // LED data pin-1
#define LED_PIN1 6    // LED data pin-2
#define relay1 D3
#define relay D7
#define ttp223 D1
#define pcstate D2
int i = 0;
int g = 0;
int red, green, blue, mode;
int brightness = 250;
int led_end = 40;           //end animation led
int led_end1 = 80;          //end animation led
unsigned long delq = 10;    // start delay led_end
unsigned long delq1 = 0;    // start delay led_end1
unsigned long deldel = 30;  //delay delayq
unsigned long deldel1 = 30; //delay delayq
unsigned long deloff = 5;   //off led animation delay
unsigned long deloff1 = 0;  //off led animation delay
unsigned long delay_rainbow;
unsigned long delayfade;

CRGB leds[NUM_LEDS];
CRGB ledss[NUM_LEDSs];
uint8_t hue = 0;
uint8_t gHue = 0;
uint8_t gHue1 = 0;

char auth[] = "1clhGNPZP3vpiu26OINn8OsdYTbdIpsS";
char ssid[] = "Tenda";
char pass[] = "kamil2k19";
char server[] = "192.168.0.105"; // IP for Local Cloud Server
//char server[] = "blynk-cloud.com";  // URL for Blynk Cloud Server

unsigned long actualtime = 0;
unsigned long savedtime1 = 0;  //rgb
unsigned long savedtime2 = 0;  //rgb
unsigned long savedtime3 = 0;  //set rgb
unsigned long savedtime4 = 0;  //fade
unsigned long savedtime7 = 0;  //off mode1
unsigned long savedtime8 = 0;  //rainbow
unsigned long savedtime9 = 0;  //rainbow
unsigned long savedtime10 = 0; //rainbow- delay
unsigned long savedtime11 = 0; //onoff sync

unsigned long savedtime12 = 0; //rgb
unsigned long savedtime13 = 0; //rgb
unsigned long savedtime14 = 0; //set rgb
unsigned long savedtime15 = 0; //fade
unsigned long savedtime16 = 0; //fade main hue++
unsigned long savedtime18 = 0; //off mode1
unsigned long savedtime19 = 0; //rainbow
unsigned long savedtime20 = 0; //rainbow
unsigned long savedtime21 = 0; //rainbow- delay

bool lock, lock1, lock2, lock3, lock6, lock7, lock8, lock11, lock12, lock13, lock14, lock15, lock16, lock17, lock18, lock19, lock20;
bool lock4 = 1;
bool lock_button1, lock_button2;
bool onoff1, onoff;
int rVal = 254;
int gVal = 1;
int bVal = 127;
int rDir = -1;
int gDir = 1;
int bDir = -1;
void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, server, 8080);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN1, GRB>(ledss, NUM_LEDSs);
  pinMode(ttp223, INPUT);  //ttp223
  pinMode(relay, OUTPUT);  //relay
  pinMode(relay1, OUTPUT); //relay
  pinMode(pcstate, INPUT); //pcstate
  digitalWrite(relay, HIGH);
  digitalWrite(relay1, HIGH);
  Blynk.setProperty(V11, "offColor", "#ff1744");
  Blynk.setProperty(V11, "onColor", "#00ff00");
  Blynk.setProperty(V11, "offLabel", "YES");
  Blynk.setProperty(V11, "onLabel", "NO");
  Blynk.setProperty(V11, "onBackColor", "#000000");
  Blynk.setProperty(V11, "offBackColor", "#000000");
  Blynk.setProperty(V10, "offColor", "#ff1744");
  Blynk.setProperty(V10, "onColor", "#00ff00");
  Blynk.setProperty(V10, "offLabel", "YES");
  Blynk.setProperty(V10, "onLabel", "NO");
  Blynk.setProperty(V10, "onBackColor", "#000000");
  Blynk.setProperty(V10, "offBackColor", "#000000");
  Blynk.setProperty(V5, "color", "#ffff00");
  Blynk.setProperty(V6, "color", "#ffff00");
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V9);
  Blynk.syncVirtual(V8);
  Blynk.syncVirtual(V11);
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V14);
  Serial.println("Connected.");
}
BLYNK_WRITE(V1)
{
  red = param.asInt();
}
BLYNK_WRITE(V2)
{
  green = param.asInt();
}
BLYNK_WRITE(V3)
{
  blue = param.asInt();
}
BLYNK_WRITE(V4)
{
  brightness = param.asInt();
}
BLYNK_WRITE(V5)
{
  delayfade = param.asInt();
}

BLYNK_WRITE(V7)
{
  switch (param.asInt())
  {
  case 1: // Item 1
    mode = 1;
    Serial.println("custom-mode selected");
    break;
  case 2: // Item 2
    mode = 2;
    Serial.println("fade-mode selected");
    break;
  case 3: // Item 3
    mode = 3;
    Serial.println("Rainbow-mode selected");
    break;
  default:
    Serial.println("Unknown mode selected");
  }
}
BLYNK_WRITE(V6)
{
  delay_rainbow = param.asInt();
}
BLYNK_WRITE(V9)
{
  onoff = param.asInt();
}
BLYNK_WRITE(V8)
{
  onoff1 = param.asInt();
}
BLYNK_WRITE(V11)
{
  lock_button1 = param.asInt();
}
BLYNK_WRITE(V10)
{
  lock_button2 = param.asInt();
}
void custommode_on1()
{
  if (lock11 == 1)
  {
    g = 0;
    delq1 = 10;
    lock11 = 0;
  }
  if (actualtime - savedtime12 >= delq1)
  {
    if (g <= NUM_LEDSs - 2)
    {
      g++;
      ledss[g] = CRGB(red, green, blue);
    }
    savedtime12 = actualtime;
  }
  for (int c = 0; c <= g; c++)
  {
    ledss[c] = CRGB(red, green, blue);
  }
  if (actualtime - savedtime13 >= deldel1)
  {
    if (g >= led_end1)
    {
      delq1 = delq1 + 5UL;
    }
    savedtime13 = actualtime;
  }
}
void fade_on1()
{
  if (lock12 == 1)
  {
    g = 0;
    delq1 = 10;
    lock12 = 0;
  }
  if (mode == 2)
  {
    if (actualtime - savedtime14 >= delq1)
    {
      if (g <= NUM_LEDSs - 2)
      {
        ledss[g] = CRGB(rVal, gVal, bVal);
        g++;
      }
      savedtime14 = actualtime;
    }
    for (int b = 0; b <= g; b++)
    {
      ledss[b] = CRGB(rVal, gVal, bVal);
    }
    if (actualtime - savedtime15 >= deldel1)
    {
      if (g >= led_end1)
      {
        delq1 = delq1 + 5UL;
      }
      savedtime15 = actualtime;
    }
  }
}
void rainbow_on1()
{
  if (lock13 == 1)
  {
    g = 0;
    delq1 = 10;
    lock13 = 0;
  }
  if (actualtime - savedtime19 >= delq1)
  {
    if (g <= NUM_LEDSs - 1)
    {
      g++;
    }
    savedtime19 = actualtime;
  }
  fill_rainbow(ledss, g, gHue1, 7);
  if (actualtime - savedtime20 >= deldel1)
  {
    if (g >= led_end1)
    {
      delq1 = delq1 + 5UL;
    }
    savedtime20 = actualtime;
  }
  if (actualtime - savedtime21 >= delay_rainbow)
  {
    gHue1++;
    savedtime21 = actualtime;
  }
}
void custommode_off1()
{
  if (lock14 == 0)
  {
    g = NUM_LEDSs - 1;
    lock14 = 1;
  }
  if (actualtime - savedtime18 >= deloff1)
  {
    if (g >= 0)
    {
      ledss[g] = CRGB(0, 0, 0);
      g--;
    }
    savedtime18 = actualtime;
  }
  for (int b = 0; b <= g; b++)
  {
    ledss[b] = CRGB(red, green, blue);
  }
  delq1 = 0;
}
void fade_off1()
{
  if (lock15 == 0)
  {
    g = NUM_LEDSs - 1;
    lock15 = 1;
  }
  if (actualtime - savedtime14 >= deloff1)
  {
    if (g >= 0)
    {
      ledss[g] = CRGB(0, 0, 0);
      g--;
    }
    savedtime14 = actualtime;
  }
    for (int b = 0; b <= g; b++)
  {
    ledss[b] = CRGB(rVal, gVal, bVal);
  }
  delq1 = 0;
}
void rainbow_off1()
{
  if (lock16 == 0)
  {
    g = NUM_LEDSs - 1;
    lock16 = 1;
  }
  if (actualtime - savedtime19 >= deloff1)
  {
    if (g >= 0)
    {
      ledss[g] = CRGB(0, 0, 0);
      g--;
    }
    savedtime19 = actualtime;
  }
  fill_rainbow(ledss, g, gHue1, 7);
  if (actualtime - savedtime21 >= delay_rainbow)
  {
    gHue1++;
    savedtime21 = actualtime;
  }
  delq1 = 0;
}

void custommode_on()
{
  if (lock == 1)
  {
    i = 0;
    delq = 10;
    lock = 0;
  }
  if (actualtime - savedtime1 >= delq)
  {
    if (i <= NUM_LEDS - 2)
    {
      i++;
      leds[i] = CRGB(red, green, blue);
    }
    savedtime1 = actualtime;
  }
  for (int b = 0; b <= i; b++)
  {
    leds[b] = CRGB(red, green, blue);
  }
  if (actualtime - savedtime2 >= deldel)
  {
    if (i >= led_end)
    {
      delq = delq + 5UL;
    }
    savedtime2 = actualtime;
  }
}
void fade_on()
{
  if (lock1 == 1)
  {
    i = 0;
    delq = 10;
    lock1 = 0;
  }
  if (mode == 2)
  {
    if (actualtime - savedtime3 >= delq)
    {
      if (i <= NUM_LEDS - 2)
      {
        leds[i] = CRGB(rVal, gVal, bVal);
        i++;
      }
      savedtime3 = actualtime;
    }
    for (int b = 0; b <= i; b++)
    {
      leds[b] = CRGB(rVal, gVal, bVal);
    }
    if (actualtime - savedtime4 >= deldel)
    {
      if (i >= led_end)
      {
        delq = delq + 5UL;
      }
      savedtime4 = actualtime;
    }
  }
}
void rainbow_on()
{
  if (lock2 == 1)
  {
    i = 0;
    delq = 10;
    lock2 = 0;
  }
  if (actualtime - savedtime8 >= delq)
  {
    if (i <= NUM_LEDS - 1)
    {
      i++;
    }
    savedtime8 = actualtime;
  }
  fill_rainbow(leds, i, gHue, 7);
  if (actualtime - savedtime9 >= deldel)
  {
    if (i >= led_end)
    {
      delq = delq + 5UL;
    }
    savedtime9 = actualtime;
  }
  if (actualtime - savedtime10 >= delay_rainbow)
  {
    gHue++;
    savedtime10 = actualtime;
  }
}
void custommode_off()
{
  if (lock6 == 0)
  {
    i = NUM_LEDS - 1;
    lock6 = 1;
  }
  if (actualtime - savedtime7 >= deloff)
  {
    if (i >= 0)
    {
      leds[i] = CRGB(0, 0, 0);
      i--;
    }
    savedtime7 = actualtime;
  }
  for (int b = 0; b <= i; b++)
  {
    leds[b] = CRGB(red, green, blue);
  }
  delq = 10;
}
void fade_off()
{
  if (lock7 == 0)
  {
    i = NUM_LEDS - 1;
    lock7 = 1;
  }
  if (actualtime - savedtime3 >= deloff)
  {
    if (i >= 0)
    {
      leds[i] = CRGB(0, 0, 0);
      i--;
    }
    savedtime3 = actualtime;
  }
  for (int b = 0; b <= i; b++)
  {
    leds[b] = CRGB(rVal, gVal, bVal);
  }
  delq = 10;
}
void rainbow_off()
{
  if (lock8 == 0)
  {
    i = NUM_LEDS - 1;
    lock8 = 1;
  }
  if (actualtime - savedtime8 >= deloff)
  {
    if (i >= 0)
    {
      leds[i] = CRGB(0, 0, 0);
      i--;
    }
    savedtime8 = actualtime;
  }
  fill_rainbow(leds, i, gHue, 7);
  if (actualtime - savedtime10 >= delay_rainbow)
  {
    gHue++;
    savedtime10 = actualtime;
  }
  delq = 10;
}

void loop()
{
  actualtime = millis();

  if (Blynk.connected())
  {
    Blynk.run();
  }
  else
  {
    Serial.print("Attempting reconnection #");
    Blynk.connect();
    digitalWrite(relay, HIGH);
    digitalWrite(relay1, HIGH);
  }
  if (onoff == 1)
  {
    lock6 = 0;
    lock7 = 0;
    lock8 = 0;
  }
  if (onoff1 == 1)
  {
    lock14 = 0;
    lock15 = 0;
    lock16 = 0;
  }
  if (onoff == 1 && mode == 1)
  {
    custommode_on();
  }
  else
  {
    lock = 1;
  }
  if (onoff == 1 && mode == 2)
  {
    fade_on();
  }
  else
  {
    lock1 = 1;
  }
  if (onoff == 1 && mode == 3)
  {
    rainbow_on();
  }
  else
  {
    lock2 = 1;
  }
  if (onoff == 0 && mode == 1)
  {
    custommode_off();
  }
  if (onoff == 0 && mode == 2)
  {
    fade_off();
  }
  if (onoff == 0 && mode == 3)
  {
    rainbow_off();
  }

  if (onoff1 == 1 && mode == 1)
  {
    custommode_on1();
  }
  else
  {
    lock11 = 1;
  }
  if (onoff1 == 1 && mode == 2)
  {
    fade_on1();
  }
  else
  {
    lock12 = 1;
  }
  if (onoff1 == 1 && mode == 3)
  {
    rainbow_on1();
  }
  else
  {
    lock13 = 1;
  }
  if (onoff1 == 0 && mode == 1)
  {
    custommode_off1();
  }
  if (onoff1 == 0 && mode == 2)
  {
    fade_off1();
  }
  if (onoff1 == 0 && mode == 3)
  {
    rainbow_off1();
  }

  if (analogRead(A0) <= 20 && lock3 == 0 && digitalRead(pcstate) == 1) //max 1024 && photoresistor
  {
    if (lock_button1 == 1)
    {
      onoff = 1;
    }
    if (lock_button2 == 1)
    {
      onoff1 = 1;
    }
    lock3 = 1;
    lock4 = 0;
  }
  else if (analogRead(A0) >= 100 && lock4 == 0 || digitalRead(pcstate) == 0 && lock4 == 0)
  {
    onoff = 0;
    onoff1 = 0;
    lock3 = 0;
    lock4 = 1;
  }

  if (digitalRead(ttp223) == HIGH)
  {
    if (lock_button1 == 1 || onoff == 1)
    {
      onoff = !onoff;
      while (digitalRead(ttp223) == HIGH)
        ;
    }
    if (lock_button2 == 1 || onoff1 == 1)
    {
      onoff1 = !onoff1;
      while (digitalRead(ttp223) == HIGH)
        ;
    }
  }
  if (onoff1 == 1)
  {
    digitalWrite(relay, LOW);
  }
  else if (g == -1)
  {
    digitalWrite(relay, HIGH);
  }
  if (onoff == 1)
  {
    digitalWrite(relay1, LOW);
  }
  else if (i == -1)
  {
    digitalWrite(relay1, HIGH);
  }

  if (onoff == 1 && lock17 == 0)
  {
    Blynk.virtualWrite(V9, HIGH);
    lock17 = 1;
    lock18 = 0;
  }
  else if (onoff == 0 && lock18 == 0)
  {
    Blynk.virtualWrite(V9, LOW);
    lock18 = 1;
    lock17 = 0;
  }
  if (onoff1 == 1 && lock19 == 0)
  {
    Blynk.virtualWrite(V8, HIGH);
    lock19 = 1;
    lock20 = 0;
  }
  else if (onoff1 == 0 && lock20 == 0)
  {
    Blynk.virtualWrite(V8, LOW);
    lock20 = 1;
    lock19 = 0;
  }
  if (actualtime - savedtime11 >= 1000UL)
  {/*
    Serial.print("Photoresistor: ");
    Serial.print(analogRead(A0));
    Serial.println(" ");
    Serial.print("PC_state: ");
    Serial.print(digitalRead(pcstate));
    Serial.println(" ");
    */
    FastLED.setBrightness(brightness);
    savedtime11 = actualtime;
  }
  if (actualtime - savedtime16 >= delayfade && mode == 2)
  {
    //hue++;
    rVal = rVal + rDir;
    gVal = gVal + gDir;
    bVal = bVal + bDir;
   
    // for each color, change direction if
    // you reached 0 or 255
    if (rVal >= 255 || rVal <= 0)
    {
      rDir = rDir * -1;
    }

    if (gVal >= 255 || gVal <= 0)
    {
      gDir = gDir * -1;
    }

    if (bVal >= 255 || bVal <= 0)
    {
      bDir = bDir * -1;
    }
        Serial.print("Red: ");
    Serial.print(rVal);
    Serial.println(" ");
    Serial.print("Green: ");
    Serial.print(gVal);
    Serial.println("");
    Serial.print("Blue: ");
    Serial.print(bVal);
    Serial.println(" ");
    //Blynk.virtualWrite(V1,rVal);
    //Blynk.virtualWrite(V2,gVal);
    //Blynk.virtualWrite(V3,bVal);
    savedtime16 = actualtime;
  }
  FastLED.show();
}