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

int sensor_1_config_menu_index = 0;
bool sensor_1_config_menu = false;

int sensor_2_config_menu_index = 0;
bool sensor_2_config_menu = false;

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

int sensor_1_TIME_BETWEEN_WATERING = 3600000;  // 1 hr //
int sensor_2_TIME_BETWEEN_WATERING = 3600000;  // 1 hr //

int zone_1_LAST_WATERING = 0;
int zone_2_LAST_WATERING = 0;

int rele_1_STATUS = 0;
int rele_2_STATUS = 0;

// Used to set milis to 0
extern volatile unsigned long timer0_millis;
unsigned long timer_new_value = 0;

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

  if (actual_time > interval_to_set_time_to_0 && rele_1_STATUS == 0 && rele_2_STATUS == 0) {
    setMillis(timer_new_value);  // coloca los millis en 0
    zone_1_LAST_WATERING = 0;
    zone_2_LAST_WATERING = 0;
  }

  actual_time = millis();

  sensor_1_LECTURE = digitalRead(sensor_1_PIN);
  sensor_2_LECTURE = digitalRead(sensor_2_PIN);

  sensor_1_LECTURE_PERCENTAGE = map(sensor_1_LECTURE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);
  sensor_1_LECTURE_PERCENTAGE = map(sensor_1_LECTURE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);

  sensor_1_START_WATERING_PERCENTAGE = map(sensor_1_START_WATERING_VALUE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);
  sensor_2_START_WATERING_PERCENTAGE = map(sensor_2_START_WATERING_VALUE, dirt_sensor_2_BOTTOM_VALUE, dirt_sensor_2_TOP_VALUE, 100, 0);

  verify_watering_conditions();

  if (nextButton == HIGH) {
    while (nextButton == HIGH) {}
    menu_index++;
    if (menu_index > 5) {
      menu_index = 0;  // Menu bottom
    }
  }

  if (backButton == HIGH) {
    while (backButton == HIGH) {}
    menu_index--;
    if (menu_index < 0) {
      menu_index = 5;  // Menu top
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
    case 0:
      // Sensors humidity display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Suelo 1:");
      lcd.setCursor(10, 0);
      lcd.print(sensor_1_LECTURE_PERCENTAGE);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Suelo 2:");
      lcd.setCursor(10, 1);
      lcd.print(sensor_2_LECTURE_PERCENTAGE);
      lcd.print("%");
      break;

    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Config. Zone 1");
      lcd.setCursor(0, 1);
      lcd.print("Config. Zone 2");
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Config. Zone 1");
      lcd.setCursor(0, 1);
      lcd.print("> Config. Zone 2");
      break;
  }
}

// -------------------------------------------------------------

void config_sensor_1_menu() {
  switch (menu_index) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Valores riego");
      lcd.setCursor(0, 1);
      lcd.print("Tiempo riego");
      break;

    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Valores riego");
      lcd.setCursor(0, 1);
      lcd.print("> Tiempo riego");
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Volver");
      break;
  }
}

// -------------------------------------------------------------

void config_sensor_1() {
  switch (menu_index) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Inicio Z.1 +3%");
      lcd.setCursor(0, 1);
      lcd.print("Inicio Z.1 -3%");
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inicio Z.1 +3%");
      lcd.setCursor(0, 1);
      lcd.print("> Inicio Z.1 -3%");
      break;

    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Fin Z.1 +3%");
      lcd.setCursor(0, 1);
      lcd.print("Fin Z.1 -3%");
      break;

    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fin Z.1 +3%");
      lcd.setCursor(0, 1);
      lcd.print("> Fin Z.1 -3%");
      break;
  }
}


// -------------------------------------------------------------

void execute_function() {
}

// -------------------------------------------------------------

void verify_watering_conditions() {
  // Stop watering:
  // - Time between waterings has not passed yet
  // - Humidity levels are above to configured ones

  // Start wateing:
  // - Humidity levels are bellow configured ones
  if (sensor_1_TIME_BETWEEN_WATERING > actual_time - zone_1_LAST_WATERING && sensor_1_START_WATERING_VALUE < sensor_1_LECTURE) {
    rele_1_STATUS = 1;
    digitalWrite(sensor_1_RELE, HIGH);
    zone_1_LAST_WATERING = actual_time;
  } else {
    rele_1_STATUS = 0;
    digitalWrite(sensor_1_RELE, LOW);
  }

  if (sensor_2_TIME_BETWEEN_WATERING > actual_time - zone_2_LAST_WATERING && sensor_2_START_WATERING_VALUE < sensor_2_LECTURE) {
    rele_2_STATUS = 1;
    digitalWrite(sensor_2_RELE, HIGH);
    zone_2_LAST_WATERING = actual_time;
  } else {
    rele_2_STATUS = 0;
    digitalWrite(sensor_2_RELE, LOW);
  }
}

// -------------------------------------------------------------

void setMillis(unsigned long new_millis) {
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
