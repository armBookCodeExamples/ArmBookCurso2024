# ArmBookCurso2024


**Título:** Control de Sistema de Tratamiento de Agua

**Alumno:** Jesús García

**Objetivo:** Desarrollar un sistema que monitorice y controle un proceso de tratamiento de agua.

**Descripción:**

El sistema controlará el suministro y la producción de agua ultrapura a través de un sistema de tratamiento mediante ósmosis inversa.

El usuario podrá:
- Encender y apagar el sistema.
- Controlar las presiones de corte mínimas y máximas para el encendido de las bombas de abastecimiento principal.
- Ajustar la frecuencia de funcionamiento de la bomba que proporciona presión a las membranas de filtración.

Cuando el sistema está encendido, las bombas P1 y P2 funcionarán en condiciones normales.
- La bomba P1 está controlada por presiones mínima y máxima de funcionamiento.
- La bomba P2 está controlada por una presión mínima para evitar que funcione en vacío.

La bomba P2 impulsa agua a presión a través de membranas para purificarla, y el agua tratada se almacena en un tanque con tres interruptores de nivel:
- **Level high:** Cuando se activa, indica que el tanque está lleno y apaga todas las bombas hasta que el nivel disminuya.
- **Level medium:** Permite iniciar la producción de agua desde este nivel.
- **Level low:** También permite iniciar la producción de agua desde este nivel.

**Plataforma de desarrollo:** NUCLEO-144

**Periféricos a utilizar:**
- **USER BUTTON:** Para iniciar o apagar el sistema.
- **LED 1:** Indica el estado de funcionamiento del sistema.
- **ANALOG IN 1:** Emula un sensor de presión.
- **UART:** Se utiliza para enviar información sobre el estado del sistema a una PC.
- **DIGITAL IN:** Emula el interruptor de nivel alto ("level high").
- **DIGITAL IN:** Emula el interruptor de nivel medio ("level medium").
- **DIGITAL IN:** Emula el interruptor de nivel bajo ("level low").

