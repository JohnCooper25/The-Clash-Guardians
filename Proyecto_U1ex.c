#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

struct Jugador {
    char Nombre[60];
    int vida;
};

struct IA {
    char Nombre[60];
    int vida;
};

struct Carta {
    char Nombre[80];
    char Tipo[50];
    int LP;
    int AP;
    int DF;
};

struct Historial {
    char resena[80];
};

//******struct para lista enlazada********
struct NodoCarta {
    struct Carta carta;
    struct NodoCarta* siguiente;
};

//********funcion para mostrar las cartas*********
void mostrarCarta(struct Carta carta) {
    printf("Nombre: %s\n", carta.Nombre);
    printf("Tipo: %s\n", carta.Tipo);
    printf("LP: %d\n", carta.LP);
    printf("AP: %d\n", carta.AP);
    printf("DF: %d\n", carta.DF);
}

//******Carga de info desde txt********
void cargaInformacion(struct NodoCarta** listaCartas) {
    FILE* file = fopen("Cartas.txt", "r");

    if (file == NULL) {
        perror("No existe archivo .txt");
        return;
    }

    struct NodoCarta* iterador = NULL; // Iterador para recorrer la lista
    
    // Asignación de variables a la lista
    int iterar = 0; // Inicializamos a 0

    while (iterar < 60) {
        struct NodoCarta* nuevoNodo = (struct NodoCarta*)malloc(sizeof(struct NodoCarta));
        if (fscanf(file, "%79[^,], %49[^,],%d,%d,%d\n",
            nuevoNodo->carta.Nombre,
            nuevoNodo->carta.Tipo,
            &nuevoNodo->carta.LP,
            &nuevoNodo->carta.AP,
            &nuevoNodo->carta.DF) == 5) {
            nuevoNodo->siguiente = NULL;
            if (*listaCartas == NULL) {
                *listaCartas = nuevoNodo; // Si la lista está vacia, el nuevo nodo es el primer elemento
                iterador = *listaCartas; // Iterador apunta al primer nodo
            } else {
                iterador->siguiente = nuevoNodo; // Conecta el nuevo nodo al último nodo
                iterador = nuevoNodo; // El nuevo nodo es ahora el ultimo elemento
            }

            iterar++;
        } else {
            break; // Si no se pudo leer correctamente, salir del bucle
        }
    }

    fclose(file);
}

// Función para agregar una carta a un tablero
void agregarCarta(struct NodoCarta** tablero, struct NodoCarta* carta) {
    if (carta != NULL) {
        carta->siguiente = *tablero;
        *tablero = carta;
    }
}

void jugarCarta(struct NodoCarta** mano_Jugador, struct NodoCarta** tablero) {
    if (*mano_Jugador == NULL) {
        printf("No tienes cartas en la mano para jugar.\n");
        return;
    }

    printf("\nElige una carta para jugar:\n");
    int i = 1;
    struct NodoCarta* iterador_mano_jugador = *mano_Jugador;
    while (iterador_mano_jugador != NULL) {
        printf("%d. ", i);
        mostrarCarta(iterador_mano_jugador->carta);
        iterador_mano_jugador = iterador_mano_jugador->siguiente;
        i++;
    }

    int eleccion;
    printf("Elige el número de la carta que deseas jugar (1-%d): ", i - 1);
    scanf("%d", &eleccion);

    if (eleccion < 1 || eleccion >= i) {
        printf("Selección no válida. Inténtalo de nuevo.\n");
        return;
    }

    struct NodoCarta* carta_a_jugar = *mano_Jugador;
    struct NodoCarta* carta_anterior = NULL;

    // Encuentra la carta seleccionada en la lista de mano del jugador
    for (int j = 1; j < eleccion; j++) {
        carta_anterior = carta_a_jugar;
        carta_a_jugar = carta_a_jugar->siguiente;
    }

    // Muestra la carta que se va a jugar
    /*printf("Has jugado la siguiente carta:\n");
    mostrarCarta(carta_a_jugar->carta);*/

    // Elimina la carta de la mano del jugador
    if (carta_anterior != NULL) {
        carta_anterior->siguiente = carta_a_jugar->siguiente;
    } else {
        *mano_Jugador = carta_a_jugar->siguiente;
    }

    // Agrega la carta al tablero
    carta_a_jugar->siguiente = *tablero;
    *tablero = carta_a_jugar;

    printf("La carta se ha jugado con exito.\n");
}

int contarCartas(struct NodoCarta* lista) {
    int contador = 0;
    struct NodoCarta* iterador = lista;

    while (iterador != NULL) {
        contador++;
        iterador = iterador->siguiente;
    }

    return contador;
}

void turnoJugador(struct NodoCarta** mano_Jugador, struct NodoCarta** tablero) {
    printf("\n----- Turno del Jugador -----\n");
    printf("Tus cartas en mano:\n");
    /*struct NodoCarta* iterador_mano_jugador = *mano_Jugador;
    while (iterador_mano_jugador != NULL) {
        mostrarCarta(iterador_mano_jugador->carta);
        iterador_mano_jugador = iterador_mano_jugador->siguiente;
    }*/

    // Opciones para el jugador
    int opcion;
    printf("Elige una opción:\n");
    printf("1. Jugar una carta\n");
    printf("2. Terminar el turno\n");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            jugarCarta(mano_Jugador, tablero);
            break;
        case 2:
            printf("Terminas tu turno.\n");
            break;
        default:
            printf("Opción no válida.\n");
            break;
    }
}

// Función para el turno de la IA (selecciona una carta aleatoriamente)
void turnoIA(struct NodoCarta** mano_IA, struct NodoCarta** tablero) {
    if (*mano_IA == NULL) {
        printf("\nLa IA no tiene cartas en la mano para jugar.\n");
        return;
    }

    // Seleccionar una carta aleatoriamente
    int num_cartas_mano_IA = contarCartas(*mano_IA);
    int carta_seleccionada = rand() % num_cartas_mano_IA + 1;

    struct NodoCarta* carta_a_jugar = *mano_IA;
    struct NodoCarta* carta_anterior = NULL;

    for (int i = 1; i < carta_seleccionada; i++) {
        carta_anterior = carta_a_jugar;
        carta_a_jugar = carta_a_jugar->siguiente;
    }

    // Muestra la carta que se va a jugar
    /*printf("\nLa IA ha jugado la siguiente carta:\n");
    mostrarCarta(carta_a_jugar->carta);*/

    // Elimina la carta de la mano de la IA
    if (carta_anterior != NULL) {
        carta_anterior->siguiente = carta_a_jugar->siguiente;
    } else {
        *mano_IA = carta_a_jugar->siguiente;
    }

    // Agrega la carta al tablero
    carta_a_jugar->siguiente = *tablero;
    *tablero = carta_a_jugar;

    printf("La IA ha jugado su carta.\n");
}

void mostrarDetalleTurno(struct NodoCarta* tablero, const char* nombreJugador, int vidaJugador, const char* nombreIA, int vidaIA) {
    printf("\nDetalle del turno:\n");

    struct NodoCarta* iterador = tablero;
    int jugador = 1; // Variable para alternar entre el jugador y la IA

    while (iterador != NULL) {
        if (jugador) {
            printf("Carta jugada por %s:\n", nombreJugador);
            mostrarCarta(iterador->carta);
        } else {
            printf("Carta jugada por %s:\n", nombreIA);
            mostrarCarta(iterador->carta);
        }

        jugador = !jugador; // Cambia entre jugador e IA en cada iteración
        iterador = iterador->siguiente;
    }

    // Muestra la vida del jugador y la IA
    printf("Vida de %s: %d\n", nombreJugador, vidaJugador);
    printf("Vida de %s: %d\n", nombreIA, vidaIA);

    // Puedes agregar aquí más detalles, como las acciones realizadas por el jugador o la IA
}

//***************JUEGO COMPLETO**********************
void juego(struct NodoCarta* listaCartas) {
    struct NodoCarta* mazo_jugador = NULL;
    struct NodoCarta* mazo_ia = NULL;
    struct NodoCarta* tablero = NULL;

    // Llenar mazo del jugador con las primeras 15 cartas de listaCartas
    for (int i = 0; i < 15 && listaCartas != NULL; i++) {
        struct NodoCarta* carta = listaCartas;
        listaCartas = listaCartas->siguiente;
        carta->siguiente = mazo_jugador;
        mazo_jugador = carta;
    }

    // Llenar mazo de la IA con las siguientes 15 cartas de listaCartas
    for (int i = 0; i < 15 && listaCartas != NULL; i++) {
        struct NodoCarta* carta = listaCartas;
        listaCartas = listaCartas->siguiente;
        carta->siguiente = mazo_ia;
        mazo_ia = carta;
    }

    // Inicializar la vida de ambos jugadores
    struct Jugador jugador;
    struct IA ia;
    jugador.vida = 5;
    ia.vida = 5;

    // Crear las listas "mano_Jugador" y "mano_IA" y llenarlas con 3 cartas cada una
	struct NodoCarta* mano_Jugador = NULL;
	struct NodoCarta* mano_IA = NULL;
	
	for (int i = 0; i < 3; i++) {
	    if (mazo_jugador != NULL) {
	        struct NodoCarta* carta = mazo_jugador;
	        mazo_jugador = mazo_jugador->siguiente;
	        carta->siguiente = mano_Jugador;
	        mano_Jugador = carta;
	    }
	
	    if (mazo_ia != NULL) {
	        struct NodoCarta* carta = mazo_ia;
	        mazo_ia = mazo_ia->siguiente;
	        carta->siguiente = mano_IA;
	        mano_IA = carta;
	    }
	}
	
	// Eliminar las cartas del mazo del jugador y de la IA
	for (int i = 0; i < 3; i++) {
	    if (mazo_jugador != NULL) {
	        struct NodoCarta* temp = mazo_jugador;
	        mazo_jugador = mazo_jugador->siguiente;
	        free(temp);
	    }
	
	    if (mazo_ia != NULL) {
	        struct NodoCarta* temp = mazo_ia;
	        mazo_ia = mazo_ia->siguiente;
	        free(temp);
	    }
	}
	
	while (jugador.vida > 0 && ia.vida > 0) {
        // Turno del jugador
        turnoJugador(&mano_Jugador, &tablero);
        

        // Turno de la IA
        turnoIA(&mano_IA, &tablero);
        mostrarDetalleTurno(tablero, "IA", ia.vida, "Jugador", jugador.vida);
        // A continuación, puedes implementar la lógica del combate entre las cartas
        // jugadas por el jugador e IA, actualizar la vida de los jugadores, etc.

        // ... (resto de la lógica del juego) ...
    }

}

//*******Verificacion de tipo de carta*********
bool EsTipoValido(const char* tipo) {
    // Verifica si el tipo de carta es uno de los tipos validos
    return strcmp(tipo, "mage") == 0 ||
           strcmp(tipo, "vikings") == 0 ||
           strcmp(tipo, "necromancer") == 0 ||
           strcmp(tipo, "beast") == 0;
}
//*********Creacion de carta**************
void CrearCarta(struct NodoCarta** listaCartas) {
    struct NodoCarta* nuevoNodo = (struct NodoCarta*)malloc(sizeof(struct NodoCarta));
    
    if (nuevoNodo == NULL) {
        printf("Error al asignar memoria para la carta.\n");
        return;
    }

    printf("Ingrese el nombre de la carta: ");
    fflush(stdin); // Limpia el bufer de entrada
    fgets(nuevoNodo->carta.Nombre, sizeof(nuevoNodo->carta.Nombre), stdin);
    nuevoNodo->carta.Nombre[strlen(nuevoNodo->carta.Nombre) - 1] = '\0'; // Elimina el salto de linea

    char tipo[50];
    do {
        printf("Ingrese el tipo de la carta (mage, vikings, necromancer, beast): ");
        scanf("%s", tipo);

        if (!EsTipoValido(tipo)) {
            printf("Tipo de carta no valido. Intente de nuevo.\n");
        }
    } while (!EsTipoValido(tipo));

    strcpy(nuevoNodo->carta.Tipo, tipo);

    printf("Ingrese los puntos de vida (LP) de la carta: ");
    scanf("%d", &nuevoNodo->carta.LP);
    
    if(nuevoNodo->carta.LP>150)
    {
    	printf("La cantidad de puntos de vida (LP) excede el maximo (150), favor re ingresar: ");
    	scanf("%d", &nuevoNodo->carta.LP);
	}
	
    printf("Ingrese los puntos de ataque (AP) de la carta: ");
    scanf("%d", &nuevoNodo->carta.AP);
    
    if(nuevoNodo->carta.AP>130)
    {
    	printf("La cantidad de puntos de ataque (AP) excede el maximo (130), favor re ingresar: ");
    	scanf("%d", &nuevoNodo->carta.AP);
	}

    printf("Ingrese los puntos de defensa (DF) de la carta: ");
    scanf("%d", &nuevoNodo->carta.DF);
    
    if(nuevoNodo->carta.DF>110)
    {
    		printf("La cantidad de puntos de defensa (DF) excede el maximo (110), favor re ingresar: ");
    		scanf("%d", &nuevoNodo->carta.DF);
	}

    nuevoNodo->siguiente = NULL;

    if (*listaCartas == NULL) {
        *listaCartas = nuevoNodo; // Si la lista esta vacia el nuevo nodo es el primer elemento
    } else {
        struct NodoCarta* iterador = *listaCartas;
        while (iterador->siguiente != NULL) {
            iterador = iterador->siguiente;
        }
        iterador->siguiente = nuevoNodo; // Conecta el nuevo nodo al último nodo
    }
    
    printf("Carta creada y agregada a la lista.\n");
}
	
int main() {
    struct NodoCarta* listaCartas = NULL;
    int menu = -1;
    
    cargaInformacion(&listaCartas);

    struct Jugador jugador;
    struct IA ia;

    printf("Ingrese el nombre del jugador: ");
    scanf("%s", jugador.Nombre);

    while (menu != 0) {
        printf("\nBienvenido, a continuacion seleccione la accion a realizar:\n");
        printf("(1) Crear Carta.\n(2) Jugar.\n(3) Ver Historial.\n(0) Cerrar el programa.\n");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                CrearCarta(&listaCartas);
                break;

            case 2:
                juego(listaCartas);
                break;

            case 0:
                printf("Cerrando el programa.");
                break;

            default:
                printf("Opcion no valida.");
                break;
        }
    }

    // Limpieza de la lista enlazada (liberar memoria)
    while (listaCartas != NULL) {
        struct NodoCarta* temp = listaCartas;
        listaCartas = listaCartas->siguiente;
        free(temp);
    }

    return 0;
}
