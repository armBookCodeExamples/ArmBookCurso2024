//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "pressure_sensor.h"
#include "control_system.h"

//=====[Declaration of private defines]========================================
#define TIME_CHECK_PRESSURE        200
#define PRESSURE_CUT_IN_P1          20
#define PRESSURE_CUT_OFF_P1         80
#define TIME_INCREMENT_MS           10

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============
DigitalOut pump1(LED2);
DigitalOut pump2(LED3);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============
float valuePressure1=0;
float valuePressure2=0;
bool statePump1 = false;
bool statePump2 = false;

//=====[Declaration and initialization of private global variables]============
static int timeAccumulatedCheckPressure=0;

//=====[Declarations (prototypes) of private functions]========================
static bool statusPump1();
static bool statusPump2();
static void checkPressure();

//=====[Implementations of public functions]===================================
void pumpsInit(){
    pump1=false;
    pump2=false;
}

void pumpsInitUpdate(bool run){
    if (run){
        checkPressure();
    }else{
        pump1=false;
        pump2=false;
    }  
}


bool StatePump1Read()
{
    return statePump1;
}

bool StatePump2Read()
{
    return statePump2;
}
//=====[Implementations of private functions]==================================

static void checkPressure() {
    if (timeAccumulatedCheckPressure <= TIME_CHECK_PRESSURE){
        timeAccumulatedCheckPressure=timeAccumulatedCheckPressure+TIME_INCREMENT_MS;
    }
    else{
        statePump1=statusPump1();
        statePump2=statusPump2();
    }
}

static bool statusPump1(){
    valuePressure1 = readPressureM1();
    if (valuePressure1 <= PRESSURE_CUT_IN_P1) {
        pump1 = false;
        pump2 = false;
    } else if (valuePressure1 > PRESSURE_CUT_IN_P1 && valuePressure1 <= PRESSURE_CUT_OFF_P1) {
        pump1 = true;
    } else {
        pump1 = false;
    }
    return pump1;
} 

static bool statusPump2(){
    valuePressure2 = readPressureM1();
    if (valuePressure2 <= PRESSURE_CUT_IN_P1) {
        pump2 = false;
    } else if (valuePressure2 > PRESSURE_CUT_IN_P1 && valuePressure2 <= PRESSURE_CUT_OFF_P1) {
        pump2 = true;
    } else {
        pump2 = false;
    }
    return pump2;
}
