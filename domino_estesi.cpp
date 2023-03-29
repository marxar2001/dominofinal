/*
	Juego de Domino 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define DOMINO 28

//Esctructura
struct ficha{
	int valores[2];
};

//Variables
int orden[28];
int ficha_actual=0;
int turno;
int buscar;
int indice;
struct ficha *aux=NULL;
struct mesa *jugador_actual=NULL;
bool posicion;
bool nadie_gano=true;
bool no_ficha=true;

//Funciones
struct ficha*crear_ficha(int a, int b);
void revolver();
struct mesa *nuevo_contenedor();
struct ficha *quitar_indice(struct mesa *m, int indice);
void agregar_ficha(struct mesa *m, struct ficha *f);
void imprimir_ficha(struct ficha *f);
int buscar_valor(struct mesa *m, int valor);
void imprimir_mesa(struct mesa *m);
int buscar_mula(struct mesa *m, int valor);
struct ficha *consultar_valores(struct mesa *m);
void agregar_ficha_orilla(struct mesa *m, struct ficha *f, bool posicion);		

//Contenedor de fichas
struct mesa{	
	struct ficha **fichas;
	struct ficha *primero, *ultimo;
	int total;
};

//Apuntador
struct mesa *M=NULL;		 
struct mesa *Jugador1=NULL;
struct mesa *Jugador2=NULL;
struct ficha *fichas=NULL;

int main(){
	
	struct ficha *temporal;
	int busqueda; 
	int contador=0;
	int i=0, j=0;
	while (i<=6) {
		j=0;
		while (j<=i){
			fichas=realloc(fichas,(contador+1)*sizeof(struct ficha));
			temporal=crear_ficha(i, j);
			memcpy(&fichas[contador], temporal, sizeof(struct ficha));
			free(temporal);
			contador++;
			j++;
		}
		i++;
	}
	printf("Total de fichas %i\n", contador);
	revolver();
	printf("Fichas desordenadas\n");
	i=0;
	while(i<contador){
		printf("Ficha: [%i|%i]\n", fichas[orden[i]].valores[0], fichas[orden[i]].valores[1]);
		i++;
	}
	
//Jugadores
	Jugador1=nuevo_contenedor();
	Jugador2=nuevo_contenedor();
	
//Repartir fichas
	printf("Ficha al Jugador 1\n");
	i=0;
	while(i<6){
		agregar_ficha(Jugador1, &fichas[orden[i]]);
		imprimir_ficha(&fichas[orden[i]]);
		i++;
	}
	printf("Ficha al Jugador 2\n");
	while (i<12){
		agregar_ficha(Jugador2, &fichas[orden[i]]);
		imprimir_ficha(&fichas[orden[i]]);
		i++;
	}
	busqueda=buscar_valor(Jugador1, 1);
	if(busqueda !=-1){
		printf("El Jugador 1 SI tiene el valor 1 en la posicion %i\n", busqueda + 1);
	} else {
		printf("El Jugador 1 NO tiene el valor 1 en la posicion %i\n", busqueda);
	}
	
//Turnos 0-> jugador1, 1-> jugador2
	turno=rand() %2;
	jugador_actual=((turno) % 2) ? Jugador2:Jugador1;
	printf("Inicia el jugador %i\n", turno+1);
	indice=-1;
	buscar=6;
	printf("Buscando mula\n");
	while(indice == -1 && buscar >= 0){
		indice=buscar_mula(jugador_actual, buscar);
		printf("Buscando mula %i: resultado %i\n", buscar, indice);
		buscar--;		
	}
	
	if(indice !=-1){	
		aux=quitar_indice(jugador_actual, indice);
	} else {	
		turno++;	
		printf("Cambio de jugador %i\n", (turno % 2) + 1);
		jugador_actual=((turno) % 2) ? Jugador2 : Jugador1;
		buscar=6;
		while(indice == -1 && buscar >= 0){
			indice=buscar_mula(jugador_actual, buscar);
			printf("Buscando mula %i: resultado %i\n", buscar, indice);
			buscar--;
		}
		
		if(indice !=-1){	
			aux=quitar_indice(jugador_actual, indice);
		} else {	
			printf("No hay mula: NADIE GANA\n");
			exit(0);
		}
	}
	
	M=nuevo_contenedor();
	agregar_ficha(M, aux);
	printf("Tiro del Jugador %i\n", (turno % 2) + 1);
	printf("Ficha jugada: ");
	imprimir_ficha(aux);
	printf("Mesa actual: ");
	imprimir_mesa(M);
	while(nadie_gano){
		turno ++;
		printf("Turno del Jugador %i\n", (turno%2) + 1 );
		jugador_actual=(turno % 2) ? Jugador2:Jugador1;
		aux=consultar_valores(M);
		
		do{
			indice=buscar_valor(jugador_actual, aux->valores[0]);
			if(indice == -1){   
                no_ficha=true;
                indice=buscar_valor(jugador_actual, aux->valores[1]);
                if(indice == -1){    
                    no_ficha=true;
                } else {    
                    buscar=aux->valores[1];
                    no_ficha=false;
                    posicion=false;   
                }
            } else {    
                buscar=aux->valores[0];
                no_ficha=false;
                posicion=true;    
                
            }
            if(no_ficha){
                if(i<28){
                    printf("El Jugador no tiene fichas, agregando una...");
                    imprimir_ficha(&fichas[orden[i]]);
                    agregar_ficha(jugador_actual, &fichas[orden[i]]);
                    i++;
                } else {
                    printf("Jugador %i PIERDE\n", (turno % 2)+1);
                    exit(0);
                }
            }
        } while(no_ficha);
        free(aux);  
        aux=quitar_indice(jugador_actual, indice);
        printf("Juega ");
        imprimir_ficha(aux);
        printf("%s\n", (posicion) ? "Principio" : "Final");
        agregar_ficha_orilla(M, aux, posicion);
        printf("Mesa actual: ");
        imprimir_mesa(M);
        if(jugador_actual->total == 0){
            nadie_gano=false;
        }
	}
    printf("GANA Jugador %i\n", (turno % 2) + 1);
	return 0;
}

struct ficha *crear_ficha (int a, int b){
	struct ficha *nueva=calloc(sizeof(struct ficha), 1);
	nueva->valores[0]=a;
	nueva->valores[1]=b;
	printf("Creando nueva ficha [%i|%i]\n", nueva->valores[0], nueva->valores[1]);
	return nueva;
}

void revolver(){
	int i=0, contador=0, valor_aleatorio;
	int arreglo_no_repetidos[28];
	memset(&arreglo_no_repetidos, 0, sizeof(int)*DOMINO);
	srand(time(NULL));
	while(i<DOMINO){
		valor_aleatorio=rand() % 28;
		if(arreglo_no_repetidos[valor_aleatorio] == 0){
			orden[i]=valor_aleatorio;			
			i++;
			arreglo_no_repetidos[valor_aleatorio] = 1;
			}
		}
	}

//Espacio para contenedor
struct mesa *nuevo_contenedor(){
	struct mesa *temp=NULL;
	temp=calloc(1, sizeof(struct mesa)); 
	return temp;
}

//Agregar una ficha a mesa
void agregar_ficha(struct mesa *m, struct ficha *f){
	if(m){
		if(f){
			m->fichas=realloc(m->fichas, sizeof(struct ficha *) * m->total + 1);
			m->fichas[m->total]=f;
			m->total++;
			m->primero=m->fichas[0];
			m->ultimo=m->fichas[m->total-1];
			
		} else {
			printf("Error en ficha\n");			
		}
	} else {
		printf("Error en contenedor");
	}
}

int buscar_valor(struct mesa *m, int valor){
	int v=-1;	
	struct ficha *actual=NULL;
	bool encontrado=false;
	int i=0;
	while(!encontrado && i < m->total){
		actual=m->fichas[i];
		if(actual->valores[0] == valor || actual->valores[1] == valor){
			v=i;
			encontrado=true;
		}
		i++;
	}
	return v;
}

struct ficha *quitar_indice(struct mesa *m, int indice){
	int i=indice;
	struct ficha *temporal=NULL;
	temporal=m->fichas[indice];
	while(i<m->total-1){
		m->fichas[i]=m->fichas[i+1];
		i++;
	}
	m->fichas[i]=NULL;
	m->fichas=realloc(m->fichas, sizeof(struct ficha*)*(m->total-1));
	m->total--;
	if(m->total>0){
		m->primero=m->fichas[0];
		m->ultimo=m->fichas[m->total-1];
	}
	return temporal;
}

//Ver ficha
void imprimir_ficha(struct ficha *f){
	printf("Ficha: [%i|%i]\n", f->valores[0], f->valores[1]);
}

struct ficha *consultar_valores(struct mesa *m){
    struct ficha *f=malloc(sizeof(struct ficha));
    f->valores[0]=m->primero->valores[0];
    f->valores[1]=m->ultimo->valores[1];
    return f;
}

//Buscar mula
int buscar_mula(struct mesa *m, int valor){
	int v= -1; 
	struct ficha *actual=NULL;
	bool encontrado=false;
	int i=0;
	printf("Buscamos mula %i en mesa", valor);
	imprimir_mesa(m);
	while(!encontrado && i < m->total){
		actual=m->fichas[i];
		if(actual->valores[0] == valor && actual-> valores[1] == valor){
			v=i;
			encontrado=true;
		}
		i++;
	}
	return v;
}

//Ver mesa
void imprimir_mesa(struct mesa *m){
    int i=0;
    while(i<m->total){
        printf("[%i|%i]", m->fichas[i]->valores[0], m->fichas[i]->valores[1]);
        i++;
    }
    printf("\n");
}

//Fichas en orillas
void agregar_ficha_orilla(struct mesa *m, struct ficha *f, bool posicion){
    if(m){
        if(f){
            m->fichas=realloc(m->fichas,sizeof(struct ficha *) * (m->total+1));
            if(posicion){   
                int i=m->total;
                while(i>0){
                    m->fichas[i]=m->fichas[i-1];
                    i--;
                }
                m->fichas[0]=f;
            } else {   
                m->fichas[m->total]=f;
            }
            m->total++;
            m->primero=m->fichas[0];
            m->ultimo=m->fichas[m->total-1];
        } else {
            printf("Error en ficha\n");
        }
    }
}

