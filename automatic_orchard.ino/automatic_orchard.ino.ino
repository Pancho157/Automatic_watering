#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);

// Conections
const int sensor_1_PIN = 4;
const int sensor_2_PIN = 5;

const int sensor_1_RELE = 6;
const int sensor_2_RELE = 7;

const int nextButton = 1;
const int backButton = 2;
const int selectButton = 3;

// Constants
const int dirt_sensor_1_TOP_VALUE = 1023;
const int dirt_sensor_1_BOTTOM_VALUE = 0;
const int dirt_sensor_2_TOP_VALUE = 1023;
const int dirt_sensor_2_BOTTOM_VALUE = 0;

const int long interval_to_set_time_to_0 = 604800000;  // 1 week //

// -------------------------------------------------------
// Variables
int menu_index = 0;

int actual_time = 0;

int sensor_1_LECTURE = 0;
int sensor_2_LECTURE = 0;

int sensor_1_LECTURE_PERCENTAGE = 0;
int sensor_2_LECTURE_PERCENTAGE = 0;

int sensor_1_START_WATERING_VALUE = 600;
int sensor_2_START_WATERING_VALUE = 600;

int sensor_1_START_WATERING_PERCENTAGE = 0;
int sensor_2_START_WATERING_PERCENTAGE = 0;

int sensor_1_TIME_WATERING = 0;
int sensor_2_TIME_WATERING = 0;

int sensor_1_TIME_BETWEEN_WATERING = 3600000 // 1 hr //
int sensor_2_TIME_BETWEEN_WATERING = 3600000 // 1 hr //

int rele_1_STATUS = 0;
int rele_2_STATUS = 0;

// -------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  // LCD init
  lcd.begin();
  lcd.backlight();

  pinMode(nextButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
}

// -------------------------------------------------------------

void loop() {
  actual_time = millis();

  sensor_1_LECTURE = digitalRead(sensor_1_PIN);
  sensor_2_LECTURE = digitalRead(sensor_2_PIN);

  sensor_1_LECTURE_PERCENTAGE = map(sensor_1_LECTURE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);
  sensor_1_LECTURE_PERCENTAGE = map(sensor_1_LECTURE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);

  sensor_1_START_WATERING_PERCENTAGE = map(sensor_1_START_WATERING_VALUE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);
  sensor_2_START_WATERING_PERCENTAGE = map(sensor_2_START_WATERING_VALUE, dirt_sensor_2_BOTTOM_VALUE, dirt_sensor_2_TOP_VALUE, 100, 0);

  if (nextButton == HIGH) {
    while (nextButton == HIGH) {}
    menu_index++;
    if (menu_index > 5) {
      menu_index = 0;
    }
  }

  if (backButton == HIGH) {
    while (backButton == HIGH) {}
    menu_index--;
    if (menu_index < 0) {
      menu_index = 5;
    }
  }

  if (selectButton == HIGH) {
    while (selectButton == HIGH) {}
    execute_function();
  }
}

// -------------------------------------------------------------

void menu() {
  switch (menu_index) {
    case 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lectura suelo 1");
      lcd.setCursor(0, 1);
      lcd.print("Valor:");
      lcd.setCursor(8, 1);
      lcd.print(lecturaSueloUno);
      lcd.setCursor(11, 1);
      lcd.print(porcentajeSueloUno);
      lcd.setCursor(14, 1);
      lcd.print("%");
      break;
  }
}

// -------------------------------------------------------------

void execute_function() {}
