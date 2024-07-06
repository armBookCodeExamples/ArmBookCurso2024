#include "mbed.h"

// Declarar pines de input
AnalogIn soilHumiditySensor(A0); // Sensor de humedad
AnalogIn lightSensor(A1); // Sensor de luz
DigitalIn manualButton(D8); // Boton para accionar la bomba manualmente

// Declarar pines de output
DigitalOut pump(D7); // Bomba de agua
DigitalOut statusLED(D9); // LED que indica si el sistema está funcionando o no
DigitalOut alertLED(D10); // LED que indica si el reservorio de agua está bajo
DigitalOut manualLED(D11); // LED que indica si el sistema esta en modo manual

// Para evitar delays que bloquean el flujo de codigo, utilizo estos temporizadores
Ticker statusTicker;
Ticker pauseTimeout;
Timeout pumpTimeout;

// Variables globales de inicializacion
bool manualMode = false;
bool pumpActive = false;
bool waterReservoirEmpty = false;
float soilThreshold = 0.3;
float lightThreshold = 0.3;

void activatePump() {
    // Encender la bomba
    pump.write(1);
    // La bomba se enciende por 5 segundos
    pumpTimeout.attach(callback([] {
        // Apagar la bomba
        pump.write(0);
    }), 5s);
}

void checkSoilHumidity() {
    // Solo revisar el sensor si el sistema no esta en modo manual
    if (!manualMode) {
        float soilHumidity = soilHumiditySensor.read();
        if (soilHumidity < soilThreshold) {
            activatePump();
        }
    }
}

void manualPumpActivation() {
    // Si se presiona el boton por primera vez, se activa el modo manual y se acciona la bomba
    // Si se presiona el boton por segunda vez, se activa el modo automatico
    if (manualButton.read() == 1) {
        manualMode = !manualMode;
        if (manualMode) {
            activatePump();
        }
    }
}

void alertBlink() {
    // Si el reservorio esta vacio, enciende la luz (alarma)
    if (waterReservoirEmpty) {
        alertLED = !alertLED;
    } else {
        alertLED = 0;
    }
}

void checkWaterReservoir() {
    // Leer el sensor de luz
    float lightLevel = lightSensor.read();
    // Si la luz es baja, significa que el sensor esta tapado y el reservorio esta vacio
    if (lightLevel < lightThreshold) {
        waterReservoirEmpty = true;
    // Si la luz es alta, el sensor no esta tapado, y el reservorio todavia tiene agua
    } else {
        waterReservoirEmpty = false;
    }
    alertBlink();
}

void statusBlink() {
    // Parpadear si el sistema esta operativo y en modo automatico
    if (!manualMode) {
        statusLED = !statusLED;
    }
}


int main() {
    statusLED = 0;
    alertLED = 0;

    // Esta funcionn callback sucede cada segundo y manda la señal de encender y apagar el LED que indica que el sistema está operativo
    statusTicker.attach(callback(statusBlink), 1s);

    // Esta funcion hace que el sensor de humedad se lea cada minuto, para permitir que el agua se disperse en la maceta antes de mangar a regar de nuevo
    pauseTimeout.attach(callback(checkSoilHumidity), 60s);

    // El sistema se va a mantener funcionando para siempre
    while (true) {
        manualPumpActivation(); // Permite operar la bomba manualmente en caso de emergencia
        checkSoilHumidity(); // Leer el sensor de humedad para detectar si la planta requiere ser regada
        checkWaterReservoir(); // Leer el sensor de luz para detectar si el reservorio debe ser llenado

        ThisThread::sleep_for(1s); // Manda a dormir el dispositivo por 1 segundo para ahorrar consumo
    }
}
