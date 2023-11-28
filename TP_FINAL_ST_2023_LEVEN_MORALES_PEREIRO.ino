/**********************************
  TP FINAL
  Integrantes: Octavio Leven - Santiago Morales - Thiago Pereiro
 **********************************/

//Librerías
#include "U8g2lib.h"
#include "string.h"
//#include "DHT.h"
#include "DHT_U.h"
#include "Adafruit_Sensor.h"
#include <Preferences.h>

Preferences preferences;

//Constructores y variables globales
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//Pantalla Display en pixeles
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int estado = 0;

unsigned int VALOR_UMBRAL;

#define PIN_BOTON_1 35
#define PIN_BOTON_2 34

#define PANTALLA_1 0
#define ESTADO_SUMA 1
#define ESTADO_RESTA 2

void setup()
{
  Serial.begin(115200);
  Serial.println(F("OLED test"));
  u8g2.begin();
  dht.begin();

  pinMode(PIN_BOTON_1, INPUT_PULLUP);
  pinMode(PIN_BOTON_2, INPUT_PULLUP);

  preferences.begin("my-app", false);
  VALOR_UMBRAL = preferences.getUInt("UMBRAL", 0);

}

void loop()
{
  float t = dht.readTemperature();
  //Serial.println(t);
  char temp[5];
  sprintf(temp, "%2.1f", t);
  // Serial.println(temp);
  char umbral[2];
  sprintf(umbral, "%i", VALOR_UMBRAL);
  Serial.println(VALOR_UMBRAL);
  Serial.println(umbral);

  switch (estado) {
    case PANTALLA_1:
      u8g2.setFont(u8g_font_5x7);
      u8g2.drawStr(0, 12, "Temperatura: ");
      u8g2.drawStr(5, 27, temp);
      u8g2.drawStr(90, 27, "C");
      u8g2.drawStr(0, 47, "Valor umbral:");
      u8g2.drawStr(0, 62, umbral);
      u8g2.sendBuffer();

      if (digitalRead(PIN_BOTON_1) == LOW) {
        estado = ESTADO_SUMA;
      }

      if (digitalRead(PIN_BOTON_2) == LOW) {
        estado = ESTADO_RESTA;
      }
      
      break;

    case ESTADO_SUMA:
    if (digitalRead(PIN_BOTON_1) == HIGH) {
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      VALOR_UMBRAL++;
      estado = PANTALLA_1;
      }

      break;

    case ESTADO_RESTA:
    if (digitalRead(PIN_BOTON_2) == HIGH) {
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      VALOR_UMBRAL--;
      estado = PANTALLA_1;
      }
      break;
  }

}
