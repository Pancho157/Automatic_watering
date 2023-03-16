#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);

// Constants
const int dirt_sensor_1_TOP_VALUE = 1023;
const int dirt_sensor_1_BOTTOM_VALUE = 0;
const int dirt_sensor_2_TOP_VALUE = 1023;
const int dirt_sensor_2_BOTTOM_VALUE = 0;

// Conections
const int sensor_1_PIN = 4;
const int sensor_2_PIN = 5;

const int sensor_1_RELE = 6;
const int sensor_2_RELE = 7;

const int nextButton = 1;
const int backButton = 2;
const int selectButton = 3;

// -------------------------------------------------------
// Variables
int sensor_1_START_WATERING_VALUE = 600;
int sensor_2_START_WATERING_VALUE = 600;

int sensor_1_TIME_BETWEEN_WATERING = 3600000 // 1 hr
int sensor_2_TIME_BETWEEN_WATERING = 3600000 // 1 hr

int long interval_to_set_time_to_0 = 604800000;  // 1 week



void setup() {
  // LCD init
  lcd.begin();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
}
