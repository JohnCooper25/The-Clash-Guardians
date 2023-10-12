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

//***************JUEGO COMPLETO**********************
void juego(struct NodoCarta* listaCartas) {
    struct NodoCarta* mazo_jugador = NULL;
    struct NodoCarta* mazo_ia = NULL;

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

    // Mostrar los datos de ambas manos
    printf("\nMano del Jugador:\n");
    struct NodoCarta* iterador_mano_jugador = mano_Jugador;
    while (iterador_mano_jugador != NULL) {
        mostrarCarta(iterador_mano_jugador->carta);
        iterador_mano_jugador = iterador_mano_jugador->siguiente;
    }

    // Mostrar la vida de ambos jugadores
    printf("\nVida del Jugador: %d\n", jugador.vida);
    printf("Vida de la IA: %d\n", ia.vida);
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
