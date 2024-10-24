# ArmBookCurso2024


**Título:** Control de Sistema de Tratamiento de Agua

**Alumno:** Jesús García

**Objetivo:** Desarrollar un sistema que monitorice y controle un proceso de tratamiento de agua.

**Descripción:**

El sistema controlará el suministro y la producción de agua ultrapura a través de un sistema de tratamiento mediante ósmosis inversa.

Entrega TP1
- Encender y apagar el sistema: en este caso ya se habia implementado maquina de estado para ese control por que se evidenció problema de rebote y era muy dificil poder prender o apagar el sistema. el Led1 es el indicador de este servicio.
- Controlar las presiones de corte mínimas y máximas para el encendido de las bombas de abastecimiento principal: este item se esta emulando con un potenciometro y se realiza una conversión sencilla que me permite obtener la presión de entrada en PSI y prender o apagar la bomba P1 según la presiones altas y bajas configuradas para apagar el sistema, el monitoreo se realiza cada 1.5 segundos, para evitar que sea bloqueante se uso un retardo de 10 milisegundos y se usó una variable de acumulación de tiempo hasta legar a los 1.5 segundos.
- Ajustar la frecuencia de funcionamiento de la bomba que proporciona presión a las membranas de filtración: se ha implementado una maquina de estado para el manejo de un teclado 4x3, todavia no se ha implementado.
- Envío por UART: de momento se esta enviando cada 3 segundos el estado y la presión actual del sistema.


**Plataforma de desarrollo:** NUCLEO-F767ZI

**Periféricos a utilizar:**
- **USER BUTTON:** Para iniciar o apagar el sistema.
- **LED 1:** Indica el estado de funcionamiento del sistema.
- **ANALOG IN 1:** Emula un sensor de presión.
- **UART:** Se utiliza para enviar información sobre el estado del sistema a una PC.


