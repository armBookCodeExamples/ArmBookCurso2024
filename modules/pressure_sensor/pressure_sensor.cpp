//=====[Libraries]=============================================================

#include "arm_book_lib.h"

//=====[Declaration of private defines]========================================
#define TIME_CHECK_PRESSURE 2000
#define TIME_INCREMENT_MS 10

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============
AnalogIn potentiometer(A0);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============
float valuePressureM1 = 0.0;
float valuePressureM2 = 0.0;

//=====[Declaration and initialization of private global variables]============
static int timeCheckPressure = 0;
static int timeAccumulatedCheckPressure=0;

//=====[Declarations (prototypes) of private functions]========================
float readPressureM1();
float readPressureM2();

//=====[Implementations of public functions]===================================

void pressureSensorInit() {}


void pressureSensorUpdate(){
    /*
    //verifica presion cada 2 seg
    if (timeAccumulatedCheckPressure <= TIME_CHECK_PRESSURE){
        timeAccumulatedCheckPressure=timeAccumulatedCheckPressure+TIME_INCREMENT_MS;
    }
    else{
        timeAccumulatedCheckPressure = 0;
    }
    */
};

void strPressureM1(char* buffer, size_t bufferSize){
    snprintf(buffer, bufferSize, "%.2f", readPressureM1()); // Convertir float a char
}

void strPressureM2(char* buffer, size_t bufferSize){
    snprintf(buffer, bufferSize, "%.2f", readPressureM2()); // Convertir float a char
}

float readPressureM1() { 
    float analogValue = potentiometer.read();
    float voltagePsi= (analogValue * 3.3)*30;
    float pressureM1=voltagePsi;
    return pressureM1;
}

float readPressureM2() { 
    float analogValue = potentiometer.read();
    float voltagePsi= ((analogValue * 3.3)*30)-5;
    float pressureM2=voltagePsi;
    return pressureM2;
}

//=====[Implementations of private functions]==================================

