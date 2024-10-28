//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "buttons.h"
#include "display.h"
#include "matrix_keypad.h"
#include "pressure_sensor.h"
#include "serial_communication.h"
#include "pumps.h"

//=====[Declaration of private defines]========================================
#define WAITING_TIME 300
#define WAITING_TIME_AJUST_FREQUENCY 7000
#define TIME_INCREMENT_MS 10

//=====[Declaration of private defines]========================================
typedef enum {
  INIT,
  PRESSURE_SP1,
  PRESSURE_SP2,
  FREQUENCY_AJUST,
  PUMP1,
  PUMP2,
  TEMPERATURE
} lcdState_t;

typedef enum { MENU, 
                READ_KEYPAD, 
                CONFIRM } ajustFrequency_t;

typedef enum {
  OPTIONS,
  CHANGE_FREQUENCY,
  STATUS_SYSTEM,
  STOP_SYSTEM
} options_menu_t;


//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============
static int transitionTime = 0;
static int timeChangeFrequency = 0;

static lcdState_t lcdState;
static ajustFrequency_t ajustFrequency;
static options_menu_t options_menu;

static char keyPressedList[3];
static bool confirmFrequency = false;
static bool statusMenuFrecuency= false;
static bool runPumps=true;

//=====[Declarations (prototypes) of private functions]========================
static void updateMenuDisplay();
static void ajustValueFrequency();
static void selectOptionMenu();

//=====[Implementations of public functions]===================================

void controlPanelInit() {
  matrixKeypadInit(10);
  serialComInit();
  buttonsInit(10);
  displayInit(DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER);
  pressureSensorInit();
  pumpsInit();
  lcdState = INIT;
  ajustFrequency = MENU;
  options_menu=OPTIONS;
}

void controlPanelUpdate() {
  pumpsInitUpdate(runPumps);
  if (statusButton()) {
    lcdState = FREQUENCY_AJUST;
  }
  updateMenuDisplay();
}

void strCounter(char *buffer, size_t bufferSize) {
  snprintf(buffer, bufferSize, "%d", transitionTime); // Convertir float a char
}

//=====[Implementations of private functions]==================================

static void updateMenuDisplay() {
  char buffer[9];
  switch (lcdState) {
  case INIT:
    if (transitionTime <= WAITING_TIME) {
      displayCharPositionWrite(0, 0);
      displayStringWrite("Iniciando");
      displayCharPositionWrite(0, 1);
      displayStringWrite("Sistema...");
      transitionTime = transitionTime + TIME_INCREMENT_MS;
    } else {
      transitionTime = 0;
      lcdState = PRESSURE_SP1;
    }
    break;

  case PRESSURE_SP1:
    if (transitionTime <= WAITING_TIME) {
      displayCharPositionWrite(0, 0);
      displayStringWrite("Presion SP1:");
      displayCharPositionWrite(0, 1);
      strPressureM1(buffer,
                    sizeof(buffer)); // Llamar con el tamaño correcto del buffer
      displayStringWrite(buffer);
      displayStringWrite(" psi");
      transitionTime = transitionTime + TIME_INCREMENT_MS;
    } else {
      transitionTime = 0;
      lcdState = PRESSURE_SP2;
    }
    break;

  case PRESSURE_SP2:
    if (transitionTime <= WAITING_TIME) {
      displayCharPositionWrite(0, 0);
      displayStringWrite("Presion SP2:");
      displayCharPositionWrite(0, 1);
      strPressureM2(buffer, sizeof(buffer));
      displayStringWrite(buffer);
      displayStringWrite(" psi  ");
      transitionTime = transitionTime + TIME_INCREMENT_MS;
    } else {
      transitionTime = 0;
      lcdState = PUMP1;
    }
    break;

  case FREQUENCY_AJUST:
    ajustValueFrequency();
    break;

  case PUMP1:
    if (transitionTime <= WAITING_TIME) {
      displayCharPositionWrite(0, 0);
      displayStringWrite("Bomba 1:    ");
      displayCharPositionWrite(0, 1);
      int length1 = snprintf(buffer, sizeof(buffer), " %s ", StatePump1Read()? "Encendida" : "Apagada  ");
      displayStringWrite(buffer);
      transitionTime = transitionTime + TIME_INCREMENT_MS;
    } else {
      transitionTime = 0;
      lcdState = PUMP2;
    }  
    break;

  case PUMP2:
    if (transitionTime <= WAITING_TIME) {
      displayCharPositionWrite(0, 0);
      displayStringWrite("Bomba 2:    ");
      displayCharPositionWrite(0, 1);
      int length1 = snprintf(buffer, sizeof(buffer), "%s", StatePump2Read()?  "Encendida" : "Apagada  ");
      displayStringWrite(buffer);
      transitionTime = transitionTime + TIME_INCREMENT_MS;
    } else {
      transitionTime = 0;
      lcdState = PRESSURE_SP1;
    }  
    break;

  case TEMPERATURE:
    // Aquí puedes agregar la lógica para TEMPERATURE
    break;

  default:
    // Manejo del estado no esperado
    break;
  }
}

static void ajustValueFrequency() {
  char buffer[50];
  char key;

  switch (ajustFrequency) {

  case MENU:
    serialMenu();
    ajustFrequency = READ_KEYPAD;
    break;

  case READ_KEYPAD:
    selectOptionMenu();
    break;

  case CONFIRM:
    key = matrixKeypadUpdate();
    if (key == '*') {
      int length = snprintf(buffer, sizeof(buffer), "Operación cancelada \n");
      serialComWrite(buffer, length);
      ajustFrequency = MENU;
      lcdState = PRESSURE_SP1;
    }
    if (key == '#') {
      int length = snprintf(buffer, sizeof(buffer), "Frecuencia cambiada! \n");
      serialComWrite(buffer, length);
      ajustFrequency = MENU;
      lcdState = PRESSURE_SP1;
    }
    break;
  }

}


static void selectOptionMenu(){
    char buffer[50];
    static int timesKeyPressed = 0;
    char key;
    int length;

    switch(options_menu){

    case OPTIONS:
        key = matrixKeypadUpdate();
        switch (key) {
            case '1':
                length = snprintf(buffer, sizeof(buffer), "Ingrese Frecuencia: \n");
                serialComWrite(buffer, length);
                options_menu=CHANGE_FREQUENCY;
                break;
            case '2':
                length = snprintf(buffer, sizeof(buffer), "El estado del sistema es: \n");
                serialComWrite(buffer, length);
                options_menu=STATUS_SYSTEM;
                break;
            case '3':
                options_menu=STOP_SYSTEM;
                break;
            default:
                options_menu=OPTIONS;
                break;
        }
        break;

    case CHANGE_FREQUENCY:
        key = matrixKeypadUpdate();
        if (key != '\0'){
            if (timesKeyPressed < 2) {
                keyPressedList[timesKeyPressed] = key;
                int length = snprintf(buffer, sizeof(buffer), "%c", key);
                serialComWrite(buffer, length);
                timesKeyPressed++;
            }
        }
        if (timesKeyPressed > 1) {
            int length = snprintf(buffer, sizeof(buffer),
                                    " Hz \nPara confirmar frecuencia presione # \n");
            serialComWrite(buffer, length);
            length = snprintf(buffer, sizeof(buffer),
                                "Para cancelar frecuencia presione * \n");
            serialComWrite(buffer, length);
            options_menu=OPTIONS;
            ajustFrequency = CONFIRM;
            timesKeyPressed = 0;
        }
        break;

    case STATUS_SYSTEM:
        length = snprintf(buffer, sizeof(buffer),
                            "\nPresión 1: %.2f \n", readPressureM1());
        serialComWrite(buffer, length);
        length = snprintf(buffer, sizeof(buffer),
                            "\nPresión 2: %.2f \n", readPressureM2());
        serialComWrite(buffer, length);
        length = snprintf(buffer, sizeof(buffer),
                            "\nBomba 1: %s \n", StatePump1Read()?  "Encendida" : "Apagada");
        serialComWrite(buffer, length);
        length = snprintf(buffer, sizeof(buffer),
                            "\nBomba 2: %s \n", StatePump2Read()?  "Encendida" : "Apagada");
        serialComWrite(buffer, length);
        options_menu=OPTIONS;
        lcdState = PRESSURE_SP1;
    break;

    case STOP_SYSTEM:
        runPumps=false;
        pumpsInitUpdate(runPumps);
        options_menu=OPTIONS;
        lcdState = PRESSURE_SP1;
    break;

    }
};