# Regador de plantas automático

Nombre: Nicolás Tertusio.

Correo: nicotertu@gmail.com

El objetivo de este programa consiste en regar automaticamente plantas empleando sensores de humedad. El programa obedece, a grandes rasgos, el siguiente diagrama de bloques:

![bloqdiag](https://github.com/Nicotertu/ArmBookCurso2024/assets/39504627/20b203e4-0505-4b1e-b71c-89ac4d5aaa40)

El sistema cuenta con dos sensores: uno de humedad y uno de luz.

 - El sensor de humedad se instala dentro de la maceta y permite saber que tan seca está la tierra.
 - El sensor de luz se instala dentro del reservorio, en la parte inferior del mismo.
   - El reservorio tiene un pequeño tubo con una masa flotante que sube y baja dependiendo del nivel del agua.
   - Cuando el reservorio tiene poca agua, la masa flotante está a nivel del sensor de luz, bloqueando la luz e indicando la falta de agua.

El sistema cuenta tambien con dos luces LED indicadoras:

  - Una luz se enciende y apaga cada segundo indicando que el sistema está operativo y en modo automatico.
  - Una luz se enciende cuando el reservorio tiene poca agua.

El funcionamiento del programa es el siguiente:

 1. Se inician dos callbacks: uno para indicar que el programa esta funcionando y otro para mandar a medir el sensor de humedad.
 2. El primer callback solo enciende y apaga un led cada segundo.
 3. El segundo callback manda a leer el sensor de humedad cada 60 segundos.
    - Los 60 segundos son para dar tiempo a la tierra de esparcirse.
 5. Si el programa esta en modo automatico, el sensor de humedad es leido.
 6. Si el sensor de humedad detecta que la planta está seca, manda a activar la bomba de agua.
 7. La bomba de agua se enciende por 5 segundos.
 8. Luego de todo eso inicia el loop que va a estar activandose cada segundo.
 9. Se comienza leyendo el boton para ver si el usuario desea encender la bomba manualmente.
 10. Y por ultimo se revisa el nivel de agua en el reservorio por medio del sensor de luz.

