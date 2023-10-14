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

struct Pila {
    struct NodoCarta* tope;
    int tamano;
};

struct NodoCarta* pop(struct Pila *pila) {
    if (pila == NULL || pila->tope == NULL) {
        return NULL; // Si la pila esta vacia, no hay nada para eliminar
    }

    struct NodoCarta* temp = pila->tope;
    pila->tope = pila->tope->siguiente;
    pila->tamano--;

    return temp;
}

struct NodoCarta* seleccionarMejorCarta(struct Pila *tablero) {
    struct NodoCarta* temp = tablero->tope;
    struct NodoCarta* mejorCarta = temp;
    int maxLP = temp->carta.LP - temp->carta.DF;
    temp = temp->siguiente;

    while (temp != NULL) {
        int LP = temp->carta.LP - temp->carta.DF;
        if (LP > maxLP) {
            maxLP = LP;
            mejorCarta = temp;
        }
        temp = temp->siguiente;
    }

    return mejorCarta;
}

// Funcion para inicializar una pila
void inicializarPila(struct Pila *pila) {
    pila->tope = NULL;
    pila->tamano = 0;
}

// Funcion para agregar un elemento a la pila
void push(struct Pila *pila, struct NodoCarta* carta) {
    if (pila == NULL) {
        printf("La pila no esta inicializada.\n");
        return;
    }

    struct NodoCarta* nuevoNodo = (struct NodoCarta*)malloc(sizeof(struct NodoCarta));
    if (nuevoNodo == NULL) {
        printf("Error al asignar memoria para el nuevo nodo de la pila.\n");
        return;
    }

    nuevoNodo->carta = carta->carta; // Copiar la informacion de la carta

    if (pila->tope == NULL) {
        pila->tope = nuevoNodo;
    } else {
        nuevoNodo->siguiente = pila->tope;
        pila->tope = nuevoNodo;
    }

    pila->tamano++;
}

void atacarConCartaIA(struct Pila *tableroIA, struct Pila *tableroJugador, struct Jugador *jugador) {
    // Seleccionar la mejor carta para atacar
    struct NodoCarta* atacante = seleccionarMejorCarta(tableroIA);

    // Seleccionar la mejor carta para atacar en el tablero del jugador
    struct NodoCarta* atacado = seleccionarMejorCarta(tableroJugador);

    // Realizar cálculos de ataque
    int damage = atacante->carta.AP - atacado->carta.DF;
    if (damage > 0) {
        atacado->carta.LP -= damage;
        if (atacado->carta.LP <= 0) {
            // Eliminar la carta atacada del tablero del jugador y restarle un punto de vida al jugador
            pop(tableroJugador);
            jugador->vida--;
            printf("Una de tus cartas ha sido derrotada.\n");
        } else {
            printf("Tu carta ha recibido %d puntos de danio.\n", damage);
            printf("Vida restante de la carta atacada: %d\n", atacado->carta.LP);
        }
    } else {
        printf("La carta atacada ha bloqueado todo el daño.\n");
    }
}



//******Carga de info desde txt********
void cargaInformacion(struct NodoCarta** listaCartas) {
    FILE* file = fopen("Cartas.txt", "r");

    if (file == NULL) {
        perror("No existe archivo .txt");
        return;
    }

    struct NodoCarta* iterador = NULL; // Iterador para recorrer la lista
    
    // Asignacion de variables a la lista
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
                *listaCartas = nuevoNodo; // Si la lista esta vacia, el nuevo nodo es el primer elemento
                iterador = *listaCartas; // Iterador apunta al primer nodo
            } else {
                iterador->siguiente = nuevoNodo; // Conecta el nuevo nodo al ultimo nodo
                iterador = nuevoNodo; // El nuevo nodo es ahora el ultimo elemento
            }

            iterar++;
        } else {
            break; // Si no se pudo leer correctamente, salir del bucle
        }
    }

    fclose(file);
}

// Funcion para obtener un numero aleatorio entre min y max
int obtenerNumeroAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
}


void moverCartas(struct Pila* origen, struct Pila* destino, int n) {
    for (int i = 0; i < n; i++) {
        struct NodoCarta* carta = pop(origen);
        if (carta != NULL) {
            push(destino, carta);
        } else {
            printf("No se pueden mover más cartas. La pila de origen está vacía.\n");
            break;
        }
    }
}

void mostrarCartasMano(struct Pila *mano) {
    struct NodoCarta* temp = mano->tope;
    printf("Cartas en tu mano:\n");
    int count = 1;
    while (temp != NULL) {
        printf("Carta %d:\n", count);
        printf("Nombre: %s\n", temp->carta.Nombre);
        printf("Tipo: %s\n", temp->carta.Tipo);
        printf("LP: %d\n", temp->carta.LP);
        printf("AP: %d\n", temp->carta.AP);
        printf("DF: %d\n", temp->carta.DF);
        printf("\n");
        temp = temp->siguiente;
        count++;
    }
}

void atacarConCarta(struct Pila *tableroJugador, struct Pila *tableroIA, struct IA *ia) {
    printf("Cartas en tu tablero:\n");
    if (tableroJugador->tope == NULL) {
        printf("No hay cartas en tu tablero para atacar.\n");
        return;
    } else {
        struct NodoCarta* temp = tableroJugador->tope;
        int count = 1;
        while (temp != NULL) {
            printf("%d. Nombre: %s, Tipo: %s, LP: %d, AP: %d, DF: %d\n", count, temp->carta.Nombre, temp->carta.Tipo, temp->carta.LP, temp->carta.AP, temp->carta.DF);
            temp = temp->siguiente;
            count++;
        }
    }

    int atacanteIndex;
    printf("Selecciona el numero de carta que deseas atacar desde tu tablero (1-%d): ", tableroJugador->tamano);
    scanf("%d", &atacanteIndex);

    if (atacanteIndex < 1 || atacanteIndex > tableroJugador->tamano) {
        printf("Seleccion de carta invalida.\n");
        return;
    }

    // Encontrar la carta atacante
    struct NodoCarta* atacante = tableroJugador->tope;
    int count = 1;
    while (atacante != NULL && count < atacanteIndex) {
        atacante = atacante->siguiente;
        count++;
    }

    printf("Cartas en el tablero de la IA:\n");
    if (tableroIA->tope == NULL) {
        printf("No hay cartas en el tablero de la IA para atacar.\n");
        return;
    } else {
        struct NodoCarta* temp = tableroIA->tope;
        count = 1;
        while (temp != NULL) {
            printf("%d. Nombre: %s, Tipo: %s, LP: %d, AP: %d, DF: %d\n", count, temp->carta.Nombre, temp->carta.Tipo, temp->carta.LP, temp->carta.AP, temp->carta.DF);
            temp = temp->siguiente;
            count++;
        }
    }

    int atacadoIndex;
    printf("Selecciona el numero de carta que deseas atacar en el tablero de la IA (1-%d): ", tableroIA->tamano);
    scanf("%d", &atacadoIndex);

    if (atacadoIndex < 1 || atacadoIndex > tableroIA->tamano) {
        printf("Seleccion de carta invalida.\n");
        return;
    }

    // Encontrar la carta atacada
    struct NodoCarta* atacado = tableroIA->tope;
    count = 1;
    while (atacado != NULL && count < atacadoIndex) {
        atacado = atacado->siguiente;
        count++;
    }
    count = 1;
    while (atacado != NULL && count < atacadoIndex) {
        atacado = atacado->siguiente;
        count++;
    }

    if (atacante == NULL || atacado == NULL) {
        printf("No se encontro la carta seleccionada.\n");
        return;
    }

    // Realizar los calculos de ataque
    int damage = atacante->carta.AP - atacado->carta.DF;
    if (damage > 0) {
        atacado->carta.LP -= damage;
        if (atacado->carta.LP <= 0) {
            // Eliminar la carta atacada del tablero de la IA y restarle un punto a la vida de la IA
            pop(tableroIA);
            ia->vida--;
            printf("La carta de la IA ha sido derrotada.\n");
        } else {
            printf("La carta de la IA ha recibido %d puntos de danio.\n", damage);
            printf("Vida restante de la carta atacada: %d\n", atacado->carta.LP);
        }
    } else {
        printf("La carta atacada ha bloqueado todo el danio.\n");
    }

    // Actualizar las estadisticas de las cartas si es necesario
    if (atacado->carta.LP < 0) {
        atacado->carta.LP = 0; // Asegurarse de que la vida no sea negativa
    }
}

void jugarCarta(struct Pila *mano, struct Pila *tablero) {
    int cartaIndex;
    
    printf("Selecciona el numero de carta que deseas jugar (1-3): ");
    scanf("%d", &cartaIndex);

    if (cartaIndex < 1 || cartaIndex > 3) {
        printf("Seleccion de carta invalida.\n");
        return;
    }

    // Encontrar la carta seleccionada
    struct NodoCarta* temp = mano->tope;
    struct NodoCarta* prev = NULL;
    int count = 1;
    while (temp != NULL && count < cartaIndex) {
        prev = temp;
        temp = temp->siguiente;
        count++;
    }

    if (temp == NULL) {
        printf("No se encontro la carta seleccionada.\n");
        return;
    }

    // Mover la carta seleccionada de la mano al tablero
    if (prev == NULL) {
        mano->tope = temp->siguiente;
    } else {
        prev->siguiente = temp->siguiente;
    }
    temp->siguiente = NULL;
    push(tablero, temp);

    printf("Carta jugada con exito.\n");
    
    
}

void jugarCartaIA(struct Pila *mano, struct Pila *tablero) {
    int manoSize = mano->tamano;
    if (manoSize == 0) {
        printf("La mano de la IA esta vacia. No se puede jugar ninguna carta.\n");
        return;
    }

    int cartaIndex = obtenerNumeroAleatorio(1, manoSize);

    // Encontrar la carta seleccionada
    struct NodoCarta* temp = mano->tope;
    struct NodoCarta* prev = NULL;
    int count = 1;
    while (temp != NULL && count < cartaIndex) {
        prev = temp;
        temp = temp->siguiente;
        count++;
    }

    if (temp == NULL) {
        printf("No se encontro la carta seleccionada para jugar.\n");
        return;
    }

    // Mover la carta seleccionada de la mano al tablero
    if (prev == NULL) {
        mano->tope = temp->siguiente;
    } else {
        prev->siguiente = temp->siguiente;
    }
    temp->siguiente = NULL;
    push(tablero, temp);

    printf("La IA ha jugado una carta al tablero.\n");
}

void mostrarResumenTurno(struct Pila *tableroJugador, struct Pila *tableroIA, struct Jugador *jugador, struct IA *ia) {
    printf("\nResumen de movimientos del turno:\n");

    // Mostrar la vida de ambos jugadores
    printf("Vida del Jugador: %d\n", jugador->vida);
    printf("Vida de la IA: %d\n", ia->vida);

    // Mostrar las cartas en el tablero del jugador
    printf("\nTablero del Jugador:\n");
    if (tableroJugador->tope == NULL) {
        printf("No hay cartas en el tablero del Jugador.\n");
    } else {
        struct NodoCarta* temp = tableroJugador->tope;
        while (temp != NULL) {
            printf("Nombre: %s, Tipo: %s, LP: %d, AP: %d, DF: %d\n", temp->carta.Nombre, temp->carta.Tipo, temp->carta.LP, temp->carta.AP, temp->carta.DF);
            temp = temp->siguiente;
        }
    }

    // Mostrar las cartas en el tablero de la IA
    printf("\nTablero de la IA:\n");
    if (tableroIA->tope == NULL) {
        printf("No hay cartas en el tablero de la IA.\n");
    } else {
        struct NodoCarta* temp = tableroIA->tope;
        while (temp != NULL) {
            printf("Nombre: %s, Tipo: %s, LP: %d, AP: %d, DF: %d\n", temp->carta.Nombre, temp->carta.Tipo, temp->carta.LP, temp->carta.AP, temp->carta.DF);
            temp = temp->siguiente;
        }
    }
}

void turno_IA(struct Pila* Mano_IA, struct Pila* Tablero_IA, struct Pila* Tablero_Jugador, struct Jugador* jugador) {
    if (Tablero_IA->tamano == 0) {
        jugarCartaIA(Mano_IA, Tablero_IA);
    } else {
        int decision = obtenerNumeroAleatorio(1, 3); // Ajuste de la probabilidad para atacar
        if (decision == 1 || decision == 2) {
            jugarCartaIA(Mano_IA, Tablero_IA);
        } else {
            atacarConCartaIA(Tablero_IA, Tablero_Jugador, jugador);
        }
    }
}

//***************JUEGO COMPLETO**********************
void juego(struct NodoCarta* listaCartas) {
	
	// Inicializar la vida de ambos jugadores
    struct Jugador jugador;
    struct IA ia;
    jugador.vida = 5;
    ia.vida = 5;
    int turno = 1;
	
    struct Pila Mazo_Jugador, Mazo_IA, Mano_Jugador, Mano_IA, Tablero_Jugador, Tablero_IA;
    
    // Inicializar las pilas
    inicializarPila(&Mazo_Jugador);
    inicializarPila(&Mazo_IA);
    inicializarPila(&Mano_Jugador);
    inicializarPila(&Mano_IA);
    inicializarPila(&Tablero_Jugador);
    inicializarPila(&Tablero_IA);
    
    // Agregar 15 cartas aleatorias de listaCartas a Mazo_Jugador y Mazo_IA respectivamente
    struct NodoCarta* iterador = listaCartas;
    int i;
    srand(time(0)); // Inicializar la semilla para obtener valores aleatorios

    for (i = 0; i < 15; i++) {
	    // Obtener un numero aleatorio para seleccionar una carta
	    int randomIndex = obtenerNumeroAleatorio(0, 59);
	    int j;
	    for (j = 0; j < randomIndex; j++) {
	        if (iterador->siguiente != NULL) {
	            iterador = iterador->siguiente;
	        } else {
	            iterador = listaCartas; // Volver al inicio si llegamos al final de la lista
	        }
	    }
	    // Aqui puedes agregar la carta seleccionada a Mazo_Jugador o Mazo_IA, dependiendo de a quien le toque
	    if (i < 7) {
	        push(&Mazo_Jugador, iterador);
	    } else {
	        push(&Mazo_IA, iterador);
	    }
	}
	
	
	
	//entregamos 3 cartas a las manos
	moverCartas(&Mazo_Jugador, &Mano_Jugador, 3);
	moverCartas(&Mazo_IA, &Mano_IA, 3);
	
	do 
	{
		printf("Primer Turno.\n");
		
		//mostramos las cartas
		mostrarCartasMano(&Mano_Jugador);
	        
	    jugarCarta(&Mano_Jugador, &Tablero_Jugador);	
		
		jugarCartaIA(&Mano_IA, &Tablero_IA);
		
		mostrarResumenTurno(&Tablero_Jugador, &Tablero_IA, &jugador, &ia);
		
		turno++;
		
	}while((turno==1));
	
	if(turno>=2)
	{
		while (jugador.vida > 0 && ia.vida > 0)
		{
    		
    		printf("\nInicio del %d Turno.",turno);
    		
    		moverCartas(&Mazo_Jugador, &Mano_Jugador, 1);
            moverCartas(&Mazo_IA, &Mano_IA, 1);
    		
    		//mostramos las cartas
			mostrarCartasMano(&Mano_Jugador);
		        
		    int opcion;
            printf("Escoja la opcion que desea realizar:\n");
            printf("(1) Jugar una carta.\n(2) Atacar con una carta.\n");
            scanf("%d", &opcion);

            switch (opcion) {
                case 1:
                    jugarCarta(&Mano_Jugador, &Tablero_Jugador);
                    /*jugarCartaIA(&Mano_IA, &Tablero_IA);
                    mostrarResumenTurno(&Tablero_Jugador, &Tablero_IA, &jugador, &ia);*/
                    //turno++;
                    break;
                case 2:
                    if (Tablero_Jugador.tamano > 0) {
                        atacarConCarta(&Tablero_Jugador, &Tablero_IA, &ia);
                        /*atacarConCartaIA(&Tablero_IA, &Tablero_Jugador, &jugador); // Llamado a la función atacarConCartaIA
                        mostrarResumenTurno(&Tablero_Jugador, &Tablero_IA, &jugador, &ia);*/
                    } else {
                        printf("Necesitas al menos una carta en tu tablero para atacar.\n");
                    }
                    //turno++;
                    break;
                default:
                    printf("Opcion no valida.\n");
                    break;
            }
            turno_IA(&Mano_IA, &Tablero_IA, &Tablero_Jugador, &jugador);
            mostrarResumenTurno(&Tablero_Jugador, &Tablero_IA, &jugador, &ia);
			turno++;		
		}
		
		if(jugador.vida >0 && ia.vida<=0)
		{
			printf("\nDuelo Terminado.");
			printf("\n%s, es el ganador.",jugador.Nombre);
		}
		
		if(ia.vida>0 && jugador.vida<=0)
		{
			printf("\nDuelo Terminado.");
			printf("\nLa IA es la ganadora.");
		}
    	    
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
        printf("Error al asignar memoria para el nuevo nodo de la carta.\n");
        return;
    }
    
    // Pide al usuario que ingrese la informacion de la carta
    printf("Ingresa el nombre de la carta: ");
    scanf("%s", nuevoNodo->carta.Nombre);
    
    // Pide al usuario que ingrese el tipo de carta hasta que ingrese un tipo valido
    do {
        printf("Ingresa el tipo de carta (mage, vikings, necromancer, beast): ");
        scanf("%s", nuevoNodo->carta.Tipo);
    } while (!EsTipoValido(nuevoNodo->carta.Tipo));
    
    printf("Ingresa el valor de LP de la carta: ");
    scanf("%d", &nuevoNodo->carta.LP);
    if(nuevoNodo->carta.LP>150)
    {
    	printf("El valor es mas alto de lo que se acepta (150). favor reingresar:\n");
    	scanf("%d", &nuevoNodo->carta.LP);
	}
    
    printf("Ingresa el valor de AP de la carta: ");
    scanf("%d", &nuevoNodo->carta.AP);
    if(nuevoNodo->carta.AP>130)
    {
    	printf("El valor es mas alto de lo que se acepta (130). favor reingresar:\n");
    	scanf("%d", &nuevoNodo->carta.AP);
	}
    
    printf("Ingresa el valor de DF de la carta: ");
    scanf("%d", &nuevoNodo->carta.DF);
    if(nuevoNodo->carta.DF>110)
    {
    	printf("El valor es mas alto de lo que se acepta (110). favor reingresar:\n");
    	scanf("%d", &nuevoNodo->carta.DF);
	}
    
    nuevoNodo->siguiente = NULL;
    
    // Agrega la nueva carta al final de la lista
    if (*listaCartas == NULL) {
        *listaCartas = nuevoNodo;
    } else {
        struct NodoCarta* temp = *listaCartas;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
}

//******Imprimir Historial******
void imprimirHistorial(struct Historial historial[], int n) {
    printf("Historial de partidas:\n");
    for (int i = 0; i < n; i++) {
        printf("%s\n", historial[i].resena);
    }
}

//*******Escritura de Historial*******
void escribirHistorial(struct Historial historial[], int *n, const char* resena) {
    if (*n < 20) {
        strcpy(historial[*n].resena, resena);
        (*n)++;
    } else {
        for (int i = 1; i < 20; i++) {
            strcpy(historial[i - 1].resena, historial[i].resena);
        }
        strcpy(historial[19].resena, resena);
    }
}
//*******Funcion main******
int main() {
    struct NodoCarta* listaCartas = NULL;
    struct Historial historial[20];
    int n = 0;
    int opcion;
    
    struct Jugador jugador;
    
    printf("Hola, a continuacion ingrese el nombre del jugador:\n");
    
    scanf("%s",jugador.Nombre);
    
    do {
        printf("\nSeleccione una opcion:\n");
        printf("(1) Cargar informacion desde archivo.\n");
        printf("(2) Crear una carta.\n");
        printf("(3) Jugar.\n");
        printf("(4) Ver historial.\n");
        printf("(5) Salir.\n");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                cargaInformacion(&listaCartas);
                printf("Se ha cargado la informacion desde el archivo.\n");
                break;
            case 2:
                CrearCarta(&listaCartas);
                printf("Se ha creado una nueva carta.\n");
                break;
            case 3:
                juego(listaCartas);
                break;
            case 4:
                imprimirHistorial(historial, n);
                break;
            case 5:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while (opcion != 5);
    
    return 0;
}
