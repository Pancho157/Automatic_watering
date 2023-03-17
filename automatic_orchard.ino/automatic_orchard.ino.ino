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

int sensor_1_TIME_BETWEEN_WATERING = 3600000    // 1 hr //
  int sensor_2_TIME_BETWEEN_WATERING = 3600000  // 1 hr //

  int zone_1_LAST_WATERING = 0;
int zone_2_LAST_WATERING = 0;

int rele_1_STATUS = 0;
int rele_2_STATUS = 0;

int last_watering_zone_1 = 0 int last_watering_zone_2 = 0

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
  actual_time = millis();

  sensor_1_LECTURE = digitalRead(sensor_1_PIN);
  sensor_2_LECTURE = digitalRead(sensor_2_PIN);

  sensor_1_LECTURE_PERCENTAGE = map(sensor_1_LECTURE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);
  sensor_1_LECTURE_PERCENTAGE = map(sensor_1_LECTURE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);

  sensor_1_START_WATERING_PERCENTAGE = map(sensor_1_START_WATERING_VALUE, dirt_sensor_1_BOTTOM_VALUE, dirt_sensor_1_TOP_VALUE, 100, 0);
  sensor_2_START_WATERING_PERCENTAGE = map(sensor_2_START_WATERING_VALUE, dirt_sensor_2_BOTTOM_VALUE, dirt_sensor_2_TOP_VALUE, 100, 0);

  if (actual_time > interval_to_set_time_to_0 && rele_1_STATUS == 0 && rele_2_STATUS == 0) {
    setMillis(timer_new_value);  // coloca los millis en 0

    actual_time = millis();
    zone_1_LAST_WATERING = 0;
    zone_2_LAST_WATERING = 0;
  }

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
    case 0;
      // Sensors humidity display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Suelo 1:");
      lcd.setCursor(10, 0);
      lcd.print(porcentajeSueloUno);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Suelo 2:");
      lcd.setCursor(10, 1);
      lcd.print(porcentajeSueloUno);
      lcd.print("%");
      break;
  }
    case 1;
      // Sensors humidity display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Inicio Z1 +3%");
      lcd.setCursor(10, 0);
      lcd.print(porcentajeSueloUno);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Suelo 2:");
      lcd.setCursor(10, 1);
      lcd.print(porcentajeSueloUno);
      lcd.print("%");
      break;
    case 2;
      // Sensors humidity display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inicio Z1 +3%");
      lcd.setCursor(10, 0);
      lcd.print(porcentajeSueloUno);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("> Suelo 2:");
      lcd.setCursor(10, 1);
      lcd.print(porcentajeSueloUno);
      lcd.print("%");
      break;
  }
}

// -------------------------------------------------------------

void execute_function() {}

void verify_watering_conditions() {
  if (tiempo - ultimoRiegoZ1 >= tiempoEntreRiegosZ1 && valorInicialSueloUno > lecturaSueloUno) {
    estadoReleZ1 = 1;
    digitalWrite(releSueloUno, estadoReleZ1)
      ultimoRiegoZ1 = tiempo;
  }

  if (tiempo - ultimoRiegoZ2 >= tiempoEntreRiegosZ2 && valorInicialSueloDos > lecturaSueloDos) {
    estadoReleZ2 = 1;
    digitalWrite(releSueloDos, estadoReleZ2)
      ultimoRiegoZ2 = tiempo;
  }

  if (tiempo - ultimoRiegoZ1 >= estadoReleZ1) {
    estadoReleZ1 = 0;
    digitalWrite(releSueloUno, LOW)
  }

  if (tiempo - ultimoRiegoZ2 >= tiempoDeRiegoDos) {
    estadoReleZ2 = 0;
    digitalWrite(releSueloDos, estadoReleZ2)
  }
}

void setMillis(unsigned long new_millis) {
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
