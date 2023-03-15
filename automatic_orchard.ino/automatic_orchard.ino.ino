#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);

// Constants
int dirt_sensor_1_TOP = 1023;
int dirt_sensor_1_BOTTOM = 0;
int dirt_sensor_2_TOP = 1023;
int dirt_sensor_2_BOTTOM = 0;

// Variables
    // Lecturas para riego de suelo
    int start_irrigation_


/*
  Conexiones de datos (físicas):
   Lectura sensor suelo N°1: D4
   Control del rele activado por sensor N°1: D6
   Lectura sensor suelo N°2: D5
   Control del rele activado por sensor N°2: D7
   Display LCD (con I2C): SDA: A4
                          SCL: A5
   Botón siguiente: D1
   Botón anterior: D2
   Botón seleccionar: D3
  (Los botones se conectan a gnd y vcc, sacando un cable que verifica cuando se cierra el circuito de los mismos y dicho cable se conecta a los puertos mencionados anteriormente)
*/

void setup() {
  // LCD init
  lcd.begin();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
}
