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

//*****struct para cola de mazos en juego*****
struct NodoCola {
    struct Carta carta;
    struct NodoCola* siguiente;
};
//******inicializacion de la cola en NULL*******
struct NodoCola* inicializarCola() {
    return NULL;
}


//**********seleccion aleatoria***********
struct Carta seleccionarCartaAleatoria(struct NodoCola** mazo_IA) {
    int numCartas = 0;
    struct NodoCola* iterador = *mazo_IA;
    while (iterador != NULL) {
        numCartas++;
        iterador = iterador->siguiente;
    }
    
    if (numCartas > 0) {
        int opcion = rand() % numCartas + 1; // Elegir una carta aleatoria
        int i = 1;
        iterador = *mazo_IA;
        while (i < opcion && iterador != NULL) {
            iterador = iterador->siguiente;
            i++;
        }

        if (iterador != NULL) {
            struct Carta cartaSeleccionada = iterador->carta;
            // Eliminar la carta del mazo de la IA
            *mazo_IA = iterador->siguiente;
            free(iterador);
            return cartaSeleccionada;
        } else {
            struct Carta cartaVacia;
            cartaVacia.Nombre[0] = '\0';
            cartaVacia.Tipo[0] = '\0';
            cartaVacia.LP = 0;
            cartaVacia.AP = 0;
            cartaVacia.DF = 0;
            return cartaVacia;
        }
    } else {
        struct Carta cartaVacia;
        cartaVacia.Nombre[0] = '\0';
        cartaVacia.Tipo[0] = '\0';
        cartaVacia.LP = 0;
        cartaVacia.AP = 0;
        cartaVacia.DF = 0;
        return cartaVacia;
    }
}
//*******SELECCION ALEATORIA DE CARTA IA*******
struct Carta elegirCarta(struct NodoCola** mazo_IA) {
    int numCartas = 0;
    struct NodoCola* iterador = *mazo_IA;
    while (iterador != NULL) {
        numCartas++;
        iterador = iterador->siguiente;
    }
    
    if (numCartas > 0) {
        int opcion = rand() % numCartas + 1; // Elegir una carta aleatoria
        int i = 1;
        iterador = *mazo_IA;
        while (i < opcion && iterador != NULL) {
            iterador = iterador->siguiente;
            i++;
        }

        if (iterador != NULL) {
            struct Carta cartaSeleccionada = iterador->carta;
            // Eliminar la carta del mazo de la IA
            *mazo_IA = iterador->siguiente;
            free(iterador);
            return cartaSeleccionada;
        } else {
            struct Carta cartaVacia;
            cartaVacia.Nombre[0] = '\0';
            cartaVacia.Tipo[0] = '\0';
            cartaVacia.LP = 0;
            cartaVacia.AP = 0;
            cartaVacia.DF = 0;
            return cartaVacia;
        }
    } else {
        struct Carta cartaVacia;
        cartaVacia.Nombre[0] = '\0';
        cartaVacia.Tipo[0] = '\0';
        cartaVacia.LP = 0;
        cartaVacia.AP = 0;
        cartaVacia.DF = 0;
        return cartaVacia;
    }
}
//************Turno de la IA****************
struct Carta turnoIA(struct NodoCola** mazo_IA) {
    int opcion = rand() % 2; // Elegir aleatoriamente entre 0 y 1
    if (opcion == 0) {
        // Elegir una carta aleatoria para jugar
        return seleccionarCartaAleatoria(mazo_IA);
    } else {
        // Atacar sin jugar carta
        struct Carta cartaVacia;
        cartaVacia.Nombre[0] = '\0';
        cartaVacia.Tipo[0] = '\0';
        cartaVacia.LP = 0;
        cartaVacia.AP = 0;
        cartaVacia.DF = 0;
        return cartaVacia;
    }
}

//************funcion para poner las cartas en cola*******************
void encolar(struct NodoCola** cola, struct Carta carta) {
    struct NodoCola* nuevoNodo = (struct NodoCola*)malloc(sizeof(struct NodoCola));
    if (nuevoNodo == NULL) {
        printf("Error al asignar memoria para el nodo de la cola.\n");
        return;
    }
    nuevoNodo->carta = carta;
    nuevoNodo->siguiente = NULL;

    if (*cola == NULL) {
        *cola = nuevoNodo;
    } else {
        struct NodoCola* iterador = *cola;
        while (iterador->siguiente != NULL) {
            iterador = iterador->siguiente;
        }
        iterador->siguiente = nuevoNodo;
    }
}
//*********struct para desencolar***********
struct Carta desencolar(struct NodoCarta** lista) {
    if (*lista == NULL) {
        struct Carta cartaVacia;
        cartaVacia.Nombre[0] = '\0';
        cartaVacia.Tipo[0] = '\0';
        cartaVacia.LP = 0;
        cartaVacia.AP = 0;
        cartaVacia.DF = 0;
        return cartaVacia;
    }

    struct NodoCarta* temp = *lista;
    struct Carta carta = temp->carta;
    *lista = temp->siguiente;
    free(temp);
    return carta;
}

//********funcion para mostrar las cartas*********
void mostrarCarta(struct Carta carta) {
    printf("Nombre: %s\n", carta.Nombre);
    printf("Tipo: %s\n", carta.Tipo);
    printf("LP: %d\n", carta.LP);
    printf("AP: %d\n", carta.AP);
    printf("DF: %d\n", carta.DF);
}

//***************JUEGO COMPLETO**********************
void juego(struct NodoCarta** listaCartas, struct Jugador* jugador, struct IA* ia, struct NodoCola** mazo_Jugador, struct NodoCola** mazo_IA) {
    //asignacion de vidas
    jugador->vida = 5;
    ia->vida = 5;

   //*******Se crean los mazos para el juego****************
    srand(time(NULL)); //inicio del rand
    for (int i = 0; i < 15; i++) {
        
        struct Carta cartaAleatoria = desencolar(listaCartas); 
        if (cartaAleatoria.Nombre[0] != '\0') { 
           
            int aleatorio = rand() % 2; 
            if (aleatorio == 0) {
                encolar(mazo_Jugador, cartaAleatoria);
            } else {
                encolar(mazo_IA, cartaAleatoria);
            }
        }
    }

    int turno = 1;
    while (jugador->vida > 0 && ia->vida > 0) {
        printf("\n--- Turno %d ---\n", turno);
        printf("Vida del Jugador: %d\n", jugador->vida);
        printf("Vida de la IA: %d\n", ia->vida);

        struct Carta carta_jugador; // Declaracion de variable

        for (int i = 0; i < 1; i++) {
            
            printf("Cartas disponibles para el Jugador:\n");
            struct NodoCola* iterador = *mazo_Jugador;
            int cartaDisponible = 1;
            while (iterador != NULL && cartaDisponible <= 3) { // muestra solo 3
                printf("(%d) ", cartaDisponible);
                mostrarCarta(iterador->carta);
                iterador = iterador->siguiente;
                cartaDisponible++;
            }
		}
            printf("\nJugador, elige tu accion:\n");
            printf("(1) Jugar una carta.\n");
            printf("(2) Atacar con una carta.\n");
            int accion;
            scanf("%d", &accion);

            if (accion == 1) {
                // Jugar una carta
                printf("Selecciona una carta para jugar (1, 2 o 3):\n");
                int opcion_carta;
                scanf("%d", &opcion_carta);

                if (opcion_carta >= 1 && opcion_carta <= 3) {
                    struct NodoCola* cartaSeleccionada = *mazo_Jugador;
                    for (int j = 1; j < opcion_carta; j++) {
                        cartaSeleccionada = cartaSeleccionada->siguiente;
                    }

                    carta_jugador = cartaSeleccionada->carta; // Asignar la carta seleccionada
                    
                    printf("Has jugado la siguiente carta:\n");
                    mostrarCarta(carta_jugador);
                } else {
                    printf("Opción no valida, se pasa el turno.\n");
                }
            } else if (accion == 2) {
                // Atacar con una carta
                printf("Atacar con una carta:\n");
                // Aquí puedes implementar lógica adicional para el jugador al atacar
            } else {
                printf("Opción no valida, se pasa el turno.\n");
            }

        struct Carta carta_ia_turno = turnoIA(mazo_IA);
        

        printf("\n--- Resultados del turno %d ---\n", turno);
        printf("Carta jugada por el Jugador:\n");
        mostrarCarta(carta_jugador); // Mostrar la carta jugada por el jugador
        printf("Carta jugada por la IA:\n");
        mostrarCarta(carta_ia_turno);

        turno++;
    }

    // Determinar el resultado del juego
    if (jugador->vida <= 0) {
        printf("\nLa IA ha ganado el juego.\n");
    } else if (ia->vida <= 0) {
        printf("\nEl jugador ha ganado el juego.\n");
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


	
int main() {
	
    struct NodoCarta* listaCartas = NULL; 
    int menu = -1;

    cargaInformacion(&listaCartas);
    
    struct NodoCola* Mazo_player = NULL;
    struct NodoCola* Mazo_IA = NULL;
    
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
                juego(&listaCartas, &jugador, &ia, &Mazo_player, &Mazo_IA);
                
                //al final del juego tendremos un menu de opciones volver a jugar, salir etc, dentro de case volver a jugar ejecutamos funcion de lista null+ funcion de carga de info+jugar
                
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
