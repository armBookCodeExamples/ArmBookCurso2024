//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "serial_communication.h"
#include "matrix_keypad.h"


//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============


//=====[Declaration and initialization of private global variables]============


//=====[Declarations (prototypes) of private functions]========================
void static serialComRead();

//=====[Implementations of public functions]===================================

void serialComInit()
{   
}

void serialMenu()
{   
    uartUsb.write( "\n",1);
    uartUsb.write( "MENU DE CONFIGURACION \n",24);
    uartUsb.write( "\n",1);
    uartUsb.write( "Seleccione el item que desea configurar: \n",43);
    uartUsb.write( "1. Ajustar Frecuencia de la bomba    \n",39);
    uartUsb.write( "2. Verificar estado del sistema      \n",39);
    uartUsb.write( "3. Detener sistema                   \n",39);
    uartUsb.write( "\n",1);
}

void serialComWrite( char* message, int length)
{
    uartUsb.write(message, length );
}


//=====[Implementations of private functions]==================================


