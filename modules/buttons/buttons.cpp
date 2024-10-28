//=====[Libraries]=============================================================

#include "buttons.h"
#include "mbed.h"
#include "arm_book_lib.h"

#include "serial_communication.h"


//=====[Declaration of private defines]========================================
#define DEBOUNCE_KEY_TIME_MS 400


//=====[Declaration of private data types]=====================================
typedef enum {
    BUTTON_UP,
    BUTTON_DEBOUNCE,
    BUTTON_DOWN,
} buttonState_t;



//=====[Declaration and initialization of public global objects]===============
DigitalIn menuButton(BUTTON1);
DigitalOut Led_1(LED1);

//=====[Declaration of external public global variables]=======================
bool pressedButton = false;

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============
static buttonState_t buttonState = BUTTON_UP;
static int timeIncrement_ms = 0;

//=====[Declarations (prototypes) of private functions]========================
static void buttonReset();
static void buttonsUpdate();


//=====[Implementations of public functions]===================================
void buttonsInit( int updateTime_ms )
{
    timeIncrement_ms = updateTime_ms;
    menuButton.mode(PullDown);
    Led_1=0;
    buttonState= BUTTON_UP;
}

bool statusButton(){
    buttonsUpdate();
    return pressedButton;
}



//=====[Implementations of private functions]==================================

static void buttonsUpdate()
{
    static int accumulatedDebouncebuttonTime = 0;
    switch( buttonState ) { 

    case BUTTON_UP:
        pressedButton = false;
        if( menuButton == true) {
            buttonState = BUTTON_DEBOUNCE;
            accumulatedDebouncebuttonTime=0;
        }
        break;

    case BUTTON_DEBOUNCE:
        if( accumulatedDebouncebuttonTime >=
            DEBOUNCE_KEY_TIME_MS ) {
            if( menuButton == true ) {
                Led_1=1;
                buttonState = BUTTON_DOWN;
            } else {
                buttonState = BUTTON_UP;
            }
        }
            accumulatedDebouncebuttonTime =
            accumulatedDebouncebuttonTime + timeIncrement_ms;
        break;

    case BUTTON_DOWN:          
            if( menuButton == false ) {
                pressedButton = true;
                Led_1=0;
                buttonState =  BUTTON_UP;
            }
        break;
        
    default:
        buttonReset();
        break;

    }
}

static void buttonReset()
{
    buttonState =  BUTTON_UP;
}