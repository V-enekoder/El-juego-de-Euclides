#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<windows.h>
#include<unistd.h>
#include<ctype.h>

#define FILAS 8
#define COLUMNAS 8
#define CASILLAS 64
#define CADENA 100
#define TIEMPO_PARTIDA 900
#define TIEMPO_JUGADA 15
#define COMBINACIONES 2016

enum Colors { 
 	BLACK = 0,
 	BLUE = 1,
 	GREEN = 2,
 	CYAN = 3,
 	RED = 4,
 	MAGENTA = 5,
 	BROWN = 6,
 	LGREY = 7,
 	DGREY = 8,
 	LBLUE = 9,
 	LGREEN = 10,
 	LCYAN = 11,
 	LRED = 12,
 	LMAGENTA = 13,
 	YELLOW = 14,
 	WHITE = 15
};

struct jugador{
	char nombre[40];
	int puntuacion;
	int color;
};

int esValido(char cadena[]);
int convertir(char c[]);
void iniciar();
struct jugador llenar();
int sortear_color();
int sortear_turno(struct jugador p[], int bot);
void crear(int tablero[FILAS][COLUMNAS],int jugadas_en_tablero[], struct jugador p[], int diferencias[]);
void imprimir(int tablero[FILAS][COLUMNAS], struct jugador p[], int turno, int segundos);
void Color(int Background, int Text);
int jugada(int tablero[FILAS][COLUMNAS],struct jugador p, int jugadas_en_tablero[], int diferencias[], int bot, int turno);
int aleatorio_en_rango(int minimo, int maximo);
int jugada_bot(int jugadas_en_tablero[], int longitud, int mayor);
int contar(int v[]);
long combinaciones (int m, int n);
long double factorial(int n);
void calcular(int jugadas_en_tablero[], int restas[], long longitud);
void agregar(int x, int restas[], long size);
int encontrar(int x, int restas[],long size);
int casilla_llena(int tablero[FILAS][COLUMNAS], int fila, int columna, struct jugador p);
void Manual_de_juego();
void Acerca_del_juego();
void Cargarjuego();

int main(){
	
	char opcion[CADENA];
	int x, continuar;
	
	srand(time(NULL));
	
	do{	
		do{
			system("cls");
			system("COLOR 0F");
			printf("1-Iniciar el juego\n");
			printf("2-Cargar juego\n");
			printf("3-Acerca del juego\n");
			printf("4-Ayuda\n");
			printf("0-Salir\n");
			printf("\nDigite una opcion valida: ");
	
			fgets(opcion, sizeof(opcion), stdin);  
		
		}while(!esValido(opcion));
		
		x = convertir(opcion);
		
		switch(x){
			
			case 0: 
				printf("\n**Gracias por usar el programa**\n"); 	
				break;
			
			case 1: 
				iniciar();
				break;
			
			case 2: 
				Cargarjuego();
				break;
			
			case 3: 
				Acerca_del_juego();
				break;
			
			case 4: 
				system("cls");
				Manual_de_juego();
				break;
		}
		
		printf("\n\nSi desea continuar en el programa, presione 1.\n");
		printf("Sino, presione otra tecla.\n");
		printf("Seleccion: ");
		scanf("%d",&continuar);
		
	}while (continuar);
	
	return 0;
}

int esValido(char cadena[]){
    int longitud = strlen(cadena);
    
    while (longitud > 0 && isspace(cadena[longitud - 1]))
        longitud--;
    if (longitud <= 0) 
		return 0;
    int i;
    
    for (i = 0; i < longitud; ++i) {
        if ((cadena[i] == '-' && i > 0) || (!isdigit(cadena[i]) && cadena[i] != '-')) 
            return 0;
    }
    return 1;
}

int convertir(char c[]){
	return atoi(c);
}

void iniciar(){
	
	int i, jugador,ultimo, bot;
	int tablero[FILAS][COLUMNAS], jugadas_en_tablero[CASILLAS], diferencias[COMBINACIONES];
	int tiempo_transcurrido = 0, tiempo_jugada = 0;
	clock_t begin = 0, end = 0;
	struct jugador p[2];
	
	system("cls");
	printf("¿Desea jugar contra un bot?\n");
	printf("1. Si.\n0. No.\n");
	do{
		printf("Opcion: ");
		scanf("%d",&bot);
	}while( bot < 0 || bot > 1);
	
	for(i = 0; i < 2; i++){
		system("cls");
		printf("Jugador %d.\n", i + 1);
		p[i] = llenar();
	}
	
	p[0].color = sortear_color();
	
	if(p[0].color == 0)
		p[1].color = -1;
	else
		p[1].color = 0;
	
	jugador = sortear_turno(p,bot);
	
	if(jugador == 1)
		ultimo = 0;
	
	crear(tablero,jugadas_en_tablero,p,diferencias);	
	system("cls");
	
	for(i = 0; i < CASILLAS - 2; i++){
		
		tiempo_transcurrido += tiempo_jugada;

		if(ultimo == 0) 
			jugador = 1;
		else 
			jugador = 0;
		
		begin = clock();
		imprimir(tablero,p,jugador,tiempo_transcurrido);
 		p[jugador].puntuacion += jugada(tablero,p[jugador],jugadas_en_tablero, diferencias, bot, jugador);
		end = clock();
		
		system("cls");
		tiempo_jugada = (int)(end - begin)/CLOCKS_PER_SEC;
		if(tiempo_jugada >= TIEMPO_JUGADA || tiempo_transcurrido >= TIEMPO_PARTIDA)
			break;
		ultimo = jugador;
		jugador++;	
	}
	
	if(tiempo_jugada >= TIEMPO_JUGADA)
		printf("%s pierde por tiempo.\n",p[jugador].nombre);
	
	else if (tiempo_transcurrido >= TIEMPO_PARTIDA){
		printf("EL juego ha terminado. Veamos esas puntuaciones:\n\n");
		printf("%s vs %s\n",p[0].nombre,p[1].nombre);
		printf("   %d   vs    %d\n",p[0].puntuacion,p[1].puntuacion);
		
		if(p[0].puntuacion > p[1].puntuacion)
			printf("Gana %s. Felicitaciones.\n",p[0].nombre);
		
		else if(p[1].puntuacion > p[0].puntuacion)
			printf("Gana %s. Felicitaciones.\n",p[1].nombre);
		
		else
			printf("\nHa sido un empate.\n");
	}
		
	else{
		printf("%s vs %s\n",p[0].nombre,p[1].nombre);
		printf("   %d   vs    %d\n",p[0].puntuacion,p[1].puntuacion);
		
		if(p[0].puntuacion > p[1].puntuacion)
			printf("Gana %s. Felicitaciones.\n",p[0].nombre);
		
		else if(p[1].puntuacion > p[0].puntuacion)
			printf("Gana %s. Felicitaciones.\n",p[1].nombre);
		
		else
			printf("\nHa sido un empate.\n");
	}
	
	printf("Gracias por jugar.\n");
}

struct jugador llenar(){ 
	
	struct jugador p;
	
	printf("Nombre: ");
	scanf("%s",&p.nombre);
	p.puntuacion = 0;
	p.color = 0;
	
	return p;
}

int sortear_color(){ 
	char opcion[CADENA];
	int blanco;
	system("cls");

	do{
		do{
			system("cls");
			printf("En el juego hay 2 colores de casillas.");
			printf("\nLas casillas blancas corresponden al jugador: ");
			fgets(opcion, sizeof(opcion), stdin);    	
		}while(!esValido(opcion));
		blanco = convertir(opcion);	
	}while(blanco <= 0|| blanco >=3);
	
	if(blanco == 1)
		return 0;
	else
		return -1;
}

int sortear_turno(struct jugador p[], int bot){ 

	char opcion[CADENA];
	int turno, jugador1, jugador2, seleccion;
	
	system("cls");
	
	if(bot == 1){
		printf("!! Primero juega %s!!\n",p[0].nombre);
		system("pause");
		return 0;
	}
	
	do{
		printf("**SORTEO PARA ABRIR POSICION DE PRIMERO**\n");
		printf("\nDigite un numero de 1 a 3.\n");
		turno = rand() % 3 + 1;
		
		do{
			printf("Jugador 1: ");
			fgets(opcion, sizeof(opcion), stdin);  
		}while(!esValido(opcion));
		jugador1 = convertir(opcion);
		
		do{
			printf("Jugador 2: ");
			fgets(opcion, sizeof(opcion), stdin);  
		}while(!esValido(opcion));
		jugador2 = convertir(opcion);
		
		if(jugador1 == turno){	
			printf("\n!! Primero juega %s!!\n",p[0].nombre);
			system("pause");
			return 0;
			
		}else if(jugador2 == turno){
			printf("!! Primero juega %s!!\n",p[1].nombre);
			system("pause");
			return 1;
		}
		system("cls");
	}while(true);
}

void crear(int tablero[FILAS][COLUMNAS],int jugadas_en_tablero[], struct jugador p[], int diferencias[]){
	
	int i, j, k, a = 0, fila, columna;

	for(i = 0;i < FILAS;i++){	
		for(j = 0;j < COLUMNAS;j++)
			tablero[i][j] = 0;
		
		if(i == a){
			for(k = 1; k < FILAS; k+= 2)
				tablero[i][k] = -1;	
		}
		else{
			for(k = 0; k < FILAS; k += 2)
				tablero[i][k] = -1;	
			a+=2;
		}	
	}
	
	for(i = 0; i < CASILLAS; i++)
		jugadas_en_tablero[i] = 0;
	
	for(i = 0; i < COMBINACIONES; i++)
		diferencias[i] = -1;
	
	do{
		fila = rand() % 8;
		columna = rand() % 8;	
	}while(tablero[fila][columna] == -1);
	
	tablero[fila][columna] = rand() % 100 + 1;
	jugadas_en_tablero[0] = tablero[fila][columna];
	
	do{
			fila = rand() % 8;
			columna = rand() % 8;
			jugadas_en_tablero[1] = rand() % 100 + 1;
	}while(tablero[fila][columna] == 0 || (jugadas_en_tablero[1] == jugadas_en_tablero[0]));
	
	tablero[fila][columna] = jugadas_en_tablero[1];
}

void imprimir(int tablero[FILAS][COLUMNAS], struct jugador p[], int turno, int segundos){ 
	
	int i = 0,j, x; 
	int min = segundos / 60, seg = segundos % 60;	
	
	printf("\nTiempo transcurrido %d:%d\n",min,seg);
	printf("\nJuega %s.\n",p[turno].nombre);
	
	for(i = 0; i < 2; i++){
		
		if(p[0].puntuacion > p[1].puntuacion)
			x = i;
		else
			x = 1 - i;		
		
		printf("%d.%s tiene %d punto(s)\n",i + 1,p[x].nombre, p[x].puntuacion);	
	}
	
	printf("\n");
	
	for(int i = 0;i < FILAS; i++){
		for(int j = 0;j < COLUMNAS; j++){		
			
	      	if(tablero[i][j] == 0){
	      		Color(BLACK,WHITE);
			  	printf("_B__|");	      		
			} 
	  
			else if(tablero[i][j] == -1){
				Color(BLACK,BLUE);
				printf("_A__|");	
			} 
			
			else if(tablero[i][j] == -2){
				Color(BLACK,RED);
				printf("_X__|");
			}
			
			else{
				Color(BLACK,BROWN);
				printf("_%d__|",tablero[i][j]);			
			}	
 		}
 		printf("\n");
	}
	Color(BLACK,WHITE);
}

void Color(int Background, int Text){ 

	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE); 

	int New_Color = Text + (Background * 16);

	SetConsoleTextAttribute(Console, New_Color);          

}
		
int jugada(int tablero[FILAS][COLUMNAS],struct jugador p, int jugadas_en_tablero[], int diferencias[], int bot, int turno){

	int jugada, i,j,contador, mayor;	
	long size;
	char opcion[CADENA];
	
	printf("\n\n");	
	contador = contar(jugadas_en_tablero);
	
	if(jugadas_en_tablero[0] > jugadas_en_tablero[1])
		mayor = jugadas_en_tablero[0];
	else 
		mayor = jugadas_en_tablero[1];
	
	if(bot == 1 && turno == 1){
		jugada = jugada_bot(jugadas_en_tablero, contador, mayor);	
		printf("\nLa jugada fue: %d\n",jugada);
		system("pause");
	}		
	
	else{
		do{
			printf("Introduzca la diferencia entre 2 numeros: ");
			fgets(opcion, sizeof(opcion), stdin);  
		}while(!esValido(opcion));		
		jugada = fabs(convertir(opcion));		
	}
	printf("\n");
	
	size = combinaciones(contador + 1,2);	
	for(i = 0; i < size; i++)
		diferencias[i] = -1;
	
	calcular(jugadas_en_tablero,diferencias,size);
	jugadas_en_tablero[contador] = jugada;

	do{
			i = rand() % 8;
			j = rand() % 8;		
	}while(casilla_llena(tablero,i,j,p));
	
	if(encontrar(jugada,diferencias,size) == 1 && encontrar(jugada,jugadas_en_tablero,contador) == 0){
		tablero[i][j] = jugada;
		return 1;	
	}
	
	jugadas_en_tablero[contador] = 0;
	tablero[i][j] = -2;
	return 0;	
}

int jugada_bot(int jugadas_en_tablero[], int longitud, int mayor){
	
	int i, menor = mayor, jugada;
	
	for(i = 0; i < longitud; i++){
		if(jugadas_en_tablero[i] < menor)
			menor = jugadas_en_tablero[i];
	}
	
	do{
		if(jugadas_en_tablero[0] % 2 == 0 && jugadas_en_tablero[1] % 2 == 0){
			do{
				jugada = aleatorio_en_rango(menor,mayor);
			}while(jugada % 2 == 1);
		}
		else
			jugada = aleatorio_en_rango(menor,mayor);	
		Sleep(1);	
	}while(encontrar(jugada,jugadas_en_tablero,longitud) == 1);
	
	return jugada;	
}

int aleatorio_en_rango(int minimo, int maximo){
   return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

int encontrar(int x, int restas[],long size){
	for(int i = 0; i < size; i++){
		if(restas[i] == x)
			return 1;
	}
	return 0;
}

int contar(int v[]){

	int elemento, contador = 0;

	for(elemento = 0; elemento < CASILLAS; elemento++){
		if(v[elemento] != 0)
			contador++;	
	}
	return contador;	
}

long combinaciones (int m, int n){
	long res = factorial(m) / (factorial(n) * factorial(m - n));
	return res;
}

long double factorial(int n){
	
    long double factorial = 1;
    
	for(int i = 1; i <= n; ++i) 
        factorial *= i;

    return factorial;	
}

void calcular(int jugadas_en_tablero[], int restas[], long longitud){
	
	int i,j;
	int primero, actual, diferencia;
	
	for(i = 0; i < longitud - 1; i++){
	
		primero = jugadas_en_tablero[i];
		for (j = 1; j < longitud; j++){
	
			actual = jugadas_en_tablero[j];
			diferencia = fabs(primero - actual);			
			if(diferencia != 0)
				agregar(diferencia, restas,longitud);
		}
	}
}

void agregar(int x, int restas[], long size){
	for(int i = 0; i < size; i++){
		if(restas[i] == -1 && encontrar(x,restas,size) == 0){
			restas[i] = x;
			break;			
		}
	}
}

int casilla_llena(int tablero[FILAS][COLUMNAS], int fila, int columna, struct jugador p){

	if(p.color == 0 && tablero[fila][columna] == 0 )
		return 0;
	
	else if( p.color == -1 && tablero[fila][columna] == -1)
		return 0;
	
	return 1;
}

void Manual_de_juego(){
	
	printf("\n--------bienvenido al manual de juego--------\n");
	
	printf("\n 1.El juego comienza con solo dos numeros en el tablero \n");
	
	
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | |29| | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | |15| | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	
	system("pause");
	system("cls");
	
	printf("\n 2.El jugador selecionado en el sorteo le tocara escribir la diferencia de esos dos numeros \n y se pondra en su respectiva casilla \n");


	printf("| | |14| | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | |29| | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | |15| | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	printf("| | | | | | | | | \n");
	printf("----------------- \n");
	
		printf("\n 3.despues le tocara al siguiente jugador escribir la diferencia de algunos de esos numero, \n si el jugador escribe un numero de una diferencia que ya se encuentra en la tabla  o se equivoca, \n se pone una X en su casilla correspondiente \n");

	system("pause");
	system("cls");
	
	printf("\n 4.si todas las casillas se encuentran llenas de numero ganara el jugador que tenga mas punto que el otro, los puntos se ganan al escribir correctamente la diferencia \n");
	
	printf("\n 5.sí en el turno del jugador 1 o el jugador 2 pasa de 15 segundos pierde automaticamente y se tomo como ganador a el otro jugador \n");
	
	printf("\n 6. gana el jugador que tenga mayor cantidad de puntos \n");	
	system("pause");
	system("cls");
}

void Acerca_del_juego(){
	printf("\nEste es un juego destinado al piblico de primaria, aunque puede ser jugado y disfrutado por personas\n");
	printf("de cualquier edad. Su principal atractivo es la capacidad de crear un ambiente lúdico para sus jugadores.\n");
	printf("Mejora la agilidad mental de los participantes, la capacidad para desarrollar actividades bajo estrés y\n");
	printf("su destreza matemática, pues requiere de la realización de sumas y restas de manera rápida.\n");
	system("pause");
}

void Cargarjuego(){                                                                                      
	printf("\nOpcion disponible para la versión 2.0 del juego.\n");
	system("pause");
	
}

