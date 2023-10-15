# The-Clash-Guardians
El juego The Clash Guardians se basa en un juego de cartas. El juego consta de 1 jugador que tendra que utilizar la logica y la estrategia para lograr ganar el juego.
A continuacion se mostrara el objetivo a alcanzar y como se abordo la solucion a este proyecto.

## Objetivo a alcanzar.
Tal y como se menciono The Clash Guardians es un juego de cartas que tiene como objetivo conseguir la victoria por sobre la "IA" que combate en contra de el jugador. 
El juego consta de lo siguiente, en un comienzo se le solicita al jugador ingresar su nombre. Posteriormente tiene disponible un menu de opciones en el cual puede escoger opciones como:
"Crear una Carta".
"Jugar".
"Ver Historial".
"Salir".

### Crear una Carta.
Se debia implementar una logica que permitiera al jugador ingresar una carta completamente nueva a la lista de cartas a repartir, para posteriormente jugar con ella.
Obviamente tenia limitaciones, las estadisticas eran limitadas para que no excedieran los rangos de las cartas que ya habian en juego.

### Juego.
Era la parte mas importante de todas. Se debian crear los mazos,manos y tableros para cada jugador 'Jugador e IA'. Teniendo esto en cuenta y una vez listos comenzaba el juego.
Este mismo estaba compuesto por un sistema de turnos, en los cuales el jugador tenia opciones para realizar acciones, mientras que la IA jugaba por su parte teniendo en cuenta sus ventajas y desventajas.
Bajo este concepto ambos jugadores tenian la capacidad tanto de **Jugar una Carta* como tambien **Atacar a una Carta* las cuales se detallan a continuacion.

**Jugar Una Carta* : El jugador tenia la opcion de poner una de sus cartas disponibles en su mano dentro de su tablero, con esta misma en un siguiente turno podria atacar a una carta del tablero enemigo.
**Atacar a una Carta*:El jugador selecciona una carta de su tablero para atacar, luego se escogia una del tablero enemigo para ser atacada. Una vez con esta proceso listo.
                      Mediante una logica de ataque se restaban puntos de vida de las cartas, si una carta es derrotada. El dueño de esta misma pierde una vida

### Ver Historial.
Una vez finalizada la partida el jugador tenia acceso al historial de esta misma en la cual podia ver todas las acciones realizadas a lo largo del juego.

## Solucion.
Este proyecto se abordo en 4 grandes pasos:
1. Identificacion.
2. Implementacion.
3. Ejecucion.

### Identificacion.
En esta parte, como su nombre indica se tenia que analizar e identificar el proyecto. Pensar en los tipos de datos a utilizar, funciones a realizar, algoritmos, logica, orden, secuencia.
En esta etapa se llego a la idea de utilizar una lista como estructura de datos general para recibir la base de datos de las cartas. Posterior a esto se llego a la determinacion de utilizar **PILAS**.
Las pilas serian de lo mas importante pues estas mismas nos permitirian mover datos entre una y otra para poder llevar a cabo el juego y poder implementar la logica,la secuencia y el orden.
Por otra parte las funciones a realizar eran muchas desde agregar y quitar cartas hasta realizar operaciones entre ellas para llevar a cabo los ataques.

### Implementacion.
La parte mas importante de la **solucion* En terminos de tiempo esta misma fue la que mas se trabajo, debido a que se debio llevar a cabo todo lo descrito en la etapa anterior.
Se crearon las estructuras de datos en conjunto con las funciones que permitirian un correcto desarrollo del juego. Pero con esto tambien se debio abordar el orden logico del programa.
Se utilizaron funciones como **Push* y **Pop* para la manipulacion de datos al igual que iteradores que recorrian los mazos, manos y tableros dependiendo el caso.

### Ejecucion.
Por su parte la ejecucion fue la etapa en donde los errores se identificaban y provocaba cambios en todo el programa. Cambios de logica, implementaciones y hasta tipos de datos.
Todo esto se iba probando y ejecutando para verificar el correcto funcionamiento del juego al igual que las logicas implementadas. Se logro completar el juego con todas sus funciones y opciones disponibles.
Sin duda alguna esta fue la parte mas complicada de este largo proceso.

## Material extra
A continuacion adjuntamos un link de acceso a un video en youtube en el cual se detalla todo lo implementado. Tambien se adjunta un link de acceso a el repositorio de **Github* que contiene todo el material del proyecto:

(https://github.com/JohnCooper25/The-Clash-Guardians/tree/main)


                      
