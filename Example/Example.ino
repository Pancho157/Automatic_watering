#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);

// Utilizados para volver a 0 la función millis()
extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;

// Calibración de los topes de los sensores
int topeSensorUno = 1023;
int topeSensorDos = 1023;

// Valores de sensores para iniciar el riego
int valorInicialSueloUno = 600;
int valorInicialSueloDos = 600;
int porcentajeInicialUno = 0;
int porcentajeInicialDos = 0;

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

// Pines de conexión de los diferentes módulos
int lecturaSueloUno = 4;
int porcentajeSueloUno = 0;
const int releSueloUno = 6;

int lecturaSueloDos = 5;
int porcentajeSueloDos = 0;
const int releSueloDos = 7;

// Botones
const int nextButton = 1;
const int backButton = 2;
const int selectButton = 3;

// Tiempos
int long tiempo = 0;

int long ultimoRiegoZ1 = 0;
int long ultimoRiegoZ2 = 0;

int long tiempoDeRiegoUno = 0;
int long tiempoDeRiegoDos = 0;

int long tiempoEntreRiegosZ1 = 3600000;  // 1h por defecto
int long tiempoEntreRiegosZ2 = 3600000;  // 1h por defecto

int long tiempoMinimoParaReinicioDeMillis = 604800000;  // 1 semana por defecto

// Indice del menú que renderiza las opciones y el menú que realiza las acciones
int indiceMenu = 0;

// Variables que se utilizan para ver el estado de los reles
int estadoReleZ1 = 0;
int estadoReleZ2 = 0;


// ---------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  pinMode(nextButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
}

void loop() {
  tiempo = millis();

  // TODO: Pasar el estado de los reles a variables para verificar que se encuentren en LOW
  if (tiempo > tiempoMinimoParaReinicioDeMillis && estadoReleZ1 == 0 && estadoReleZ2 == 0) {
    setMillis(new_value);  // coloca los millis en 0

    tiempo = millis();
    ultimoRiegoZ1 = 0;
    ultimoRiegoZ2 = 0;
  }

  //      Controles del LCD
  //  lcd.print("     Hello"); // You can make spaces using well... spaces
  //  lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
  //  lcd.print("World!");
  //  lcd.clear();


  // ----------------- Lee los valores de los sensores -------------------
  nextButton = digitalRead(nextButton, INPUT);

  lecturaSueloUno = digitalRead(lecturaSueloUno);
  lecturaSueloDos = digitalRead(lecturaSueloDos);
  porcentajeSueloUno = map(lecturaSueloUno, 0, topeSensorUno, 100, 0);
  porcentajeSueloDos = map(lecturaSueloDos, 0, topeSensorDos, 100, 0);

  int porcentajeInicialUno = map(valorInicialSueloUno, 0, topeSensorUno, 100, 0);
  int porcentajeInicialDos = map(valorInicialSueloDos, 0, topeSensorUno, 100, 0);

  // --------------------------------------------------------------------


  //  --------------- Verifica si se presionó algún botón ---------------
  // Los bucles while son para realizar el procedimiento una vez que se deje de presionar el botón
  // En caso de que los botones salgan del rango de acciones el valor de indice se va al otro extremo de la lista
  if (nextButton == HIGH) {
    while (nextButton == HIGH) {}
    indiceMenu++;
    if (indiceMenu > 5) {
      indiceMenu = 0;
    }
  }

  if (backButton == HIGH) {
    while (backButton == HIGH) {}
    indiceMenu--;
    if (indiceMenu < 0) {
      indiceMenu = 5;
    }
  }

  if (selectButton == HIGH) {
    while (selectButton == HIGH) {}
    hacerAccion();
  }
  //  ------------------------------------------------------------------

  // Verificar suelo se encuentra vacío todavía, tengo que desarrollar esa funcionalidad
  verificarSuelo();

  menu();
}


// Esta función es la que muestra las opciones en el LCD
// Funciona ubicandosé segun el valor de una variable con un switch
void menu() {
  switch (indiceMenu) {

    // Muestra las lecturas de los sensores de humedad
    case 0:
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

    case 1:
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

    // Modificar la humedad de inicio de los riegos
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inicio Zona1 +50");
      lcd.setCursor(0, 1);
      lcd.print("Valor:");
      lcd.setCursor(8, 1);
      lcd.print(valorInicialSueloUno);
      lcd.setCursor(11, 1);
      lcd.print(porcentajeInicialUno);
      lcd.setCursor(14, 1);
      lcd.print("%");
      break;

    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inicio Zona1 -50");
      lcd.setCursor(0, 1);
      lcd.print("Valor:");
      lcd.setCursor(8, 1);
      lcd.print(valorInicialSueloUno);
      lcd.setCursor(11, 1);
      lcd.print(porcentajeInicialUno);
      lcd.setCursor(14, 1);
      lcd.print("%");
      break;

    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inicio Zona2 +50");
      lcd.setCursor(0, 1);
      lcd.print("Valor:");
      lcd.setCursor(8, 1);
      lcd.print(valorInicialSueloDos);
      lcd.setCursor(11, 1);
      lcd.print(porcentajeInicialDos);
      lcd.setCursor(14, 1);
      lcd.print("%");
      break;

    case 5:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Inicio Zona2 -50");
      lcd.setCursor(0, 1);
      lcd.print("Valor:");
      lcd.setCursor(8, 1);
      lcd.print(valorInicialSueloDos);
      lcd.setCursor(11, 1);
      lcd.print(porcentajeInicialDos);
      lcd.setCursor(14, 1);
      lcd.print("%");
      break;

    // Modificar tiempos de riegos
    case 6:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tiempo riego Z1");
      lcd.setCursor(0, 1);
      lcd.print("+5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoDeRiegoUno);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    case 7:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tiempo riego Z1");
      lcd.setCursor(0, 1);
      lcd.print("-5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoDeRiegoUno);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    case 8:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tiempo riego Z2");
      lcd.setCursor(0, 1);
      lcd.print("+5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoDeRiegoDos);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    case 9:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tiempo riego Z2");
      lcd.setCursor(0, 1);
      lcd.print("-5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoDeRiegoDos);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    // Modificar tiempo entre riegos
    case 10:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Espera riego Z1");
      lcd.setCursor(0, 1);
      lcd.print("+5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoEntreRiegosZ1);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    case 11:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Espera riego Z1");
      lcd.setCursor(0, 1);
      lcd.print("-5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoEntreRiegosZ1);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    case 12:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Espera riego Z2");
      lcd.setCursor(0, 1);
      lcd.print("+5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoEntreRiegosZ2);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;

    case 13:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Espera riego Z2");
      lcd.setCursor(0, 1);
      lcd.print("-5 segs");
      lcd.setCursor(10, 1);
      lcd.print(tiempoEntreRiegosZ2);
      lcd.setCursor(14, 1);
      lcd.print("s");
      break;
  }
}


// Esta función modifica los parametros con los que se maneja el riego
// Utiliza la misma variable que el menú para su switch
void hacerAccion() {
  switch (indiceMenu) {
    // Modifica los valores de inicio de riego
    // 2: valorInicialSueloUno +50
    // 3: valorInicialSueloUno -50
    // 4: valorInicialSueloDos +50
    // 5: valorInicialSueloDos -50
    case 2:
      valorInicialSueloUno += 50;
      break;

    case 3:
      valorInicialSueloUno -= 50;
      break;

    case 4:
      valorInicialSueloDos += 50;
      break;

    case 5:
      valorInicialSueloDos -= 50;
      break;

    // Modificar tiempos de riegos
    // 6: tiempoDeRiegoUno +5 s
    // 7: tiempoDeRiegoUno -5 s
    // 8: tiempoDeRiegoDos +5 s
    // 9: tiempoDeRiegoDos -5 s
    case 6:
      tiempoDeRiegoUno += 50;
      break;

    case 7:
      tiempoDeRiegoUno -= 50;
      break;

    case 8:
      tiempoDeRiegoDos += 5;
      break;

    case 9:
      tiempoDeRiegoDos -= 5;
      break;

      // Modificar tiempo entre riegos
      // 10: tiempoEntreRiegosZ1 +5 s
      // 11: tiempoEntreRiegosZ1 -5 s
      // 12: tiempoEntreRiegosZ2 +5 s
      // 13: tiempoEntreRiegosZ2 -5 s

    case 10:
      tiempoEntreRiegosZ1 -= 5;
      break;

    case 11:
      tiempoEntreRiegosZ1 -= 5000;
      break;

    case 12:
      tiempoEntreRiegosZ2 -= 5000;
      break;

    case 13:
      tiempoEntreRiegosZ2 -= 5000;
      break;
  }
}


void verificarSuelo() {
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



// Vuelve los millis a 0
void setMillis(unsigned long new_millis) {
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
