//=====[Libraries]=============================================================
#include "mbed.h"

//=====[Defines]===============================================================
#define TIME_INCREMENT_MS          10
#define TIME_BLINK_LED_SYSTEM     1000
#define TIME_CHECK_PRESSURE        1500
#define TIME_SEND_DATA             3000
#define DEBOUNCE_BUTTON_TIME_MS    40
#define KEYPAD_NUMBER_OF_ROWS      4
#define KEYPAD_NUMBER_OF_COLS      3

//=====[Declaration and initialization of public global objects]===============
DigitalIn onOffButton(BUTTON1);
DigitalIn upButton(D0);
DigitalIn downButton(D1);
DigitalIn levelHighSensor(D2);
DigitalIn levelMediumSensor(D3);
DigitalIn levelLowSensor(D4);

DigitalOut ledSystem(LED1);
DigitalOut pumpP1(LED2);
DigitalOut pumpP2(LED3);
DigitalOut buzzer(D7);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

AnalogIn potentiometer(A0);


//=====[Declaration of public data types]======================================

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_FALLING,
    BUTTON_RISING
} onOffButtonState_t;

typedef enum {
    MATRIX_KEYPAD_SCANNING,
    MATRIX_KEYPAD_DEBOUNCE,
    MATRIX_KEYPAD_KEY_HOLD_PRESSED
} matrixKeypadState_t;

//=====[Declaration and initialization of public global variables]=============
bool onOffStatus = false;
bool onOffStatusLed = false;
bool evento_boton = false;
int blinkLedSystem = 0;

int timeCheckPressure=0;
int counterEventsBtnOff = 0;
int accumulatedDebounceButtonTime     = 0;
int numberOfEnterButtonReleasedEvents = 0;
int timeAccumulatedSendData=0;
int timeAccumulatedCheckPressure=0;

int pressureCutIn_P1 = 5;
int pressureCutOff_P1 = 80;


onOffButtonState_t enterButtonState;


char buffer[50]; 

DigitalOut keypadRowPins[KEYPAD_NUMBER_OF_ROWS] = {PE_5, PE_6, PE_3, PF_8};
DigitalIn keypadColPins[KEYPAD_NUMBER_OF_COLS]  = {PF_7, PF_9, PG_1};

int accumulatedDebounceMatrixKeypadTime = 0;
char matrixKeypadLastKeyPressed = '\0';
char matrixKeypadIndexToCharArray[] = {
    '1', '2', '3',
    '4', '5', '6',
    '7', '8', '9', 
    '*', '0', '#',
};
matrixKeypadState_t matrixKeypadState;


//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void statusSystem();
void statusLed();
void debounceButtonInit();
bool debounceButtonUpdate();
float readPressureM1();
void checkpressure();
void statePump1();

void sendData();
void availableCommands();

void matrixKeypadInit();
char matrixKeypadScan();
char matrixKeypadUpdate();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        statusSystem();
        statusLed();
        checkpressure();
        sendData();
        HAL_Delay(TIME_INCREMENT_MS);
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    onOffButton.mode(PullDown);
    upButton.mode(PullDown);
    downButton.mode(PullDown);
    levelHighSensor.mode(PullDown);
    levelMediumSensor.mode(PullDown);
    levelLowSensor.mode(PullDown);
    debounceButtonInit();
    matrixKeypadInit();
}

void outputsInit()
{
    ledSystem = 0;
    pumpP1 = false;
    pumpP2 = 0;
    buzzer =0;
}

void statusSystem()
{   
    evento_boton=debounceButtonUpdate();
    if (evento_boton){
        onOffStatus=!onOffStatus;
    }
   
}

void statusLed()
{   
    if (onOffStatus)
    {
        if(blinkLedSystem <= TIME_BLINK_LED_SYSTEM){
            blinkLedSystem = blinkLedSystem+TIME_INCREMENT_MS;
        }
        else{
            blinkLedSystem=0;
            ledSystem=!ledSystem;
        }
    }
    else{
        ledSystem=0;
    }
}


void sendData()
{   
    if (timeAccumulatedSendData <= TIME_SEND_DATA){
        timeAccumulatedSendData=timeAccumulatedSendData+TIME_INCREMENT_MS;
    }
    else{
        int length = snprintf(buffer, sizeof(buffer), "La presión de entrada es: %.2f psi, ", readPressureM1());
        uartUsb.write(buffer, length);
        int length1 = snprintf(buffer, sizeof(buffer), "el Systema esta: %s, \n\r", pumpP1 ? "encendido" : "apagado");
        uartUsb.write(buffer, length1);
        timeAccumulatedSendData=0;
    }
}

void debounceButtonInit()
{
    if( onOffButton == 1) {
        enterButtonState = BUTTON_DOWN;
    } else {
        enterButtonState = BUTTON_UP;
    }
}

bool debounceButtonUpdate()
{
    bool enterButtonReleasedEvent = false;
    switch( enterButtonState ) {

    case BUTTON_UP:
        if( onOffButton ) {
            enterButtonState = BUTTON_FALLING;
            accumulatedDebounceButtonTime = 0;
        }
        break;

    case BUTTON_FALLING:
        if( accumulatedDebounceButtonTime >= DEBOUNCE_BUTTON_TIME_MS ) {
            if( onOffButton ) {
                enterButtonState = BUTTON_DOWN;
            } else {
                enterButtonState = BUTTON_UP;
            }
        }
        accumulatedDebounceButtonTime = accumulatedDebounceButtonTime +
                                        TIME_INCREMENT_MS;
        break;

    case BUTTON_DOWN:
        if( !onOffButton ) {
            enterButtonState = BUTTON_RISING;
            accumulatedDebounceButtonTime = 0;
        }
        break;

    case BUTTON_RISING:
        if( accumulatedDebounceButtonTime >= DEBOUNCE_BUTTON_TIME_MS ) {
            if( !onOffButton ) {
                enterButtonState = BUTTON_UP;
                enterButtonReleasedEvent = true;
            } else {
                enterButtonState = BUTTON_DOWN;
            }
        }
        accumulatedDebounceButtonTime = accumulatedDebounceButtonTime +
                                        TIME_INCREMENT_MS;
        break;

    default:
        debounceButtonInit();
        break;
    }
    return enterButtonReleasedEvent;

}


void matrixKeypadInit()
{
  matrixKeypadState = MATRIX_KEYPAD_SCANNING;
  int pinIndex = 0;
  for (pinIndex = 0; pinIndex < KEYPAD_NUMBER_OF_COLS; pinIndex++) {
    (keypadColPins[pinIndex]).mode(PullUp);
    }
}

char matrixKeypadScan()
{
    int row = 0;
    int col = 0;
    int i = 0;

    for( row=0; row<KEYPAD_NUMBER_OF_ROWS; row++ ) {

        for( i=0; i<KEYPAD_NUMBER_OF_ROWS; i++ ) {
            keypadRowPins[i] = 1;
        }

        keypadRowPins[row] = 0;

        for( col=0; col<KEYPAD_NUMBER_OF_COLS; col++ ) {
            if( keypadColPins[col] == 1 ) {
                return matrixKeypadIndexToCharArray[row*KEYPAD_NUMBER_OF_ROWS + col];
            }
        }
    }
    return '\0';
}

char matrixKeypadUpdate()
{
    char keyDetected = '\0';
    char keyReleased = '\0';

    switch( matrixKeypadState ) {

    case MATRIX_KEYPAD_SCANNING:
        keyDetected = matrixKeypadScan();
        if( keyDetected != '\0' ) {
            matrixKeypadLastKeyPressed = keyDetected;
            accumulatedDebounceMatrixKeypadTime = 0;
            matrixKeypadState = MATRIX_KEYPAD_DEBOUNCE;
        }
        break;

    case MATRIX_KEYPAD_DEBOUNCE:
        if( accumulatedDebounceMatrixKeypadTime >=
            DEBOUNCE_BUTTON_TIME_MS ) {
            keyDetected = matrixKeypadScan();
            if( keyDetected == matrixKeypadLastKeyPressed ) {
                matrixKeypadState = MATRIX_KEYPAD_KEY_HOLD_PRESSED;
            } else {
                matrixKeypadState = MATRIX_KEYPAD_SCANNING;
            }
        }
        accumulatedDebounceMatrixKeypadTime =
            accumulatedDebounceMatrixKeypadTime + TIME_INCREMENT_MS;
        break;

    case MATRIX_KEYPAD_KEY_HOLD_PRESSED:
        keyDetected = matrixKeypadScan();
        if( keyDetected != matrixKeypadLastKeyPressed ) {
            if( keyDetected == '\0' ) {
                keyReleased = matrixKeypadLastKeyPressed;
            }
            matrixKeypadState = MATRIX_KEYPAD_SCANNING;
        }
        break;

    default:
        matrixKeypadInit();
        break;
    }
    return keyReleased;
}


float readPressureM1() { 
    float analogValue = potentiometer.read();
    float voltagePsi= (analogValue * 3.3)*30;
    float pressureM1=voltagePsi;

    return pressureM1;
}

void checkpressure() {
    //verifica presion cada 1.5 seg
    if (timeAccumulatedCheckPressure <= TIME_CHECK_PRESSURE){
        timeAccumulatedCheckPressure=timeAccumulatedCheckPressure+TIME_INCREMENT_MS;
    }
    else{
        int valuePressureM1 = readPressureM1();

        if (valuePressureM1 <= pressureCutIn_P1) {
            pumpP1 = false;
        } else if (valuePressureM1 > pressureCutIn_P1 && valuePressureM1 <= pressureCutOff_P1) {
            pumpP1 = true;
        } else {
            pumpP1 = false;
        }
        timeAccumulatedCheckPressure=0;
    }
}
