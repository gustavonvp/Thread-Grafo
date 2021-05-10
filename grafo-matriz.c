
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 0
#define true 1
#define false 0


#define PRINT_LEGENDA\
	printf("\n\n");\
	printf("Legenda da Matriz de acordo com os Vertices: \n");\
	printf("v0 = A, v1 = B, v2 = C , v3 = D ,  v4 = E ,  v5 = F");\
	printf("\n\n\n");	

#define PRINT_RESPOSTA\
  printf("Os vertices que possuem maior numero de arestas incidentes sao: \n");\
    printf("B , D , E");\
    printf("\n\n");

#define PRINT_INCIDENCIAS\
  printf("Incidencias no grafo nao direcionado:\n\n");\
  printf("Vertice B:");\
  printf("\n\n");\
  printf("B - A (Peso = 1)  ,  B - C (Peso = 3)  , B - E (Peso = 2)");\
  printf("\n\n");\
  printf("Vertice D:");\
  printf("\n\n");\
  printf("D - C (Peso = 6)  ,  D - E (Peso = 4)  , D - F (Peso = 3)");\
  printf("\n\n");	\
  printf("Vertice E:");\
  printf("\n\n");\
  printf("E - A (Peso = 2)  ,  E - B (Peso = 2)  , E - D (Peso = 4)");\
  printf("\n\n");


typedef int bool;
typedef int TIPOPESO;

pthread_mutex_t mutexcomp; // mutex que irá controlar o acesso a variável varComp
long varComp; // variável a ser compartilhada

typedef struct adjacencia {
    int vertice; // vertice de destino 
    TIPOPESO peso; // peso associado a aresta que leva ao vertice de destino
    struct adjacencia *prox; // O próximo elemento da lista de adjacencias
}ADJACENCIA;

typedef struct vertice {
    /* Dados armazenados vão aqui */
    ADJACENCIA *cab; //possui apenas a cabeça da lista de adjacencia
}VERTICE;

typedef struct grafo { //lembrando que cada grafo possui:
    int vertices; // numero de vertice total do grafo
    int arestas; // numero de arestas totais do grafo
    VERTICE *adj; // Arranjo de vertices da estrutura
}GRAFO;


/**função para criar um GRAFO**/
GRAFO *criaGrafo (void *v) {
	long tid = 10;
	int vertice = (int *) v;
	int i;
	pthread_mutex_lock (&mutexcomp); // inicio da seção crítica
	printf ("\nThread da funcao criaGrafo iniciando MUTEX e modificando variavel compartilhada\n\n");
	varComp = 1;
	varComp = tid * varComp;
	
		printf ("Thread da funcao criaGrafo computou o valor %ld para varComp\n\n", varComp);
		printf ("Thread da funcao criaGrafo finalizou MUTEX e modificando variavel compartilhada\n\n");
	
	GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO)); //aloca espaço para estrtura grafo
	g->vertices = v; //atualizo o numero de vertice
	g->arestas = 0;  //atualizo o numero de vertice
	g->adj = (VERTICE *)malloc(vertice*sizeof(VERTICE)); //ler abaixo
	//Dentro da estrturua tem só o arranjo para o ponteiro de vertice, não o arranjo em si 	
	// então aloco o arranjo com (v) o numero de vertice desejado
	for (i=0; i<v; i++){ 
		g->adj[i].cab=NULL; //coloco NULL em todas arestas
	}

	return(g);
	
	pthread_mutex_unlock (&mutexcomp); // fim da seção crítica
	pthread_exit((void*) v);
}

/**função para adicionar arestas no GRAFO**/

ADJACENCIA *criaAdj(int v, int peso){ 
	ADJACENCIA *temp = (ADJACENCIA *) malloc (sizeof(ADJACENCIA)); //aloca espaço para um nó
	temp->vertice =v; //vertice alvo da adjacencia
	temp->peso = peso; //peso da aresta
	temp->prox = NULL; 
	return(temp); //retorno endereço da adjacencia
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p) { //vai de vi a vf
	if(!gr) return (false);  //validações se o grafo existe 
	if((vf<0)||(vf >= gr->vertices))return(false); //validações se os valores não são neg
	if((vi<0)||(vf >= gr->vertices))return(false); //ou maiores que o numero de vértice do grafo
	
	ADJACENCIA *novo = criaAdj(vf,p); //crio adjacencia com o vértice final e o peso
	//coloco a adjacencia na lista do vértice inicial
	novo->prox = gr->adj[vi].cab; //o campo prox da adjacencia vai receber a cabeça da lista
	gr->adj[vi].cab=novo; // e a cabeça da lista passa a ser o novo elemento
	gr->arestas++; // atualizo o numero de aresta no grafo
	return (true);
}

void imprime(GRAFO *gr){
	//validações se o grafo existe 
	
	printf("Vertices: %d. Arestas: %d. \n",gr->vertices,gr->arestas); //imprime numero de vértice e arestas
	int i;
	
	for(i=0; i<gr->vertices; i++){
		printf("v%d: ",i); //Imprimo em qual aresta estou
		ADJACENCIA *ad = gr->adj[i].cab; //chamo a cabeça da lista de adjacencia desta aresta
			while(ad){ //enquanto as adjacencias não forem nula
				printf("v%d(%d) ",ad->vertice,ad->peso); //imprimo a adjacencia e seu peso
				ad=ad->prox; //passo para proxima adjacencia
			}
		printf("\n");	
	}
}

int main()
{
	
	pthread_t threads[NUM_THREADS];
	pthread_attr_t  attr;
	pthread_mutex_init (&mutexcomp, &attr);
	char *status; // Ponteiro utilizado por cada thread para "retornar" os seus status
	int rc;
  	long t;
  	int v;
  	
  	 /* Initialize and set thread detached attribute */
 		 pthread_attr_init (&attr); // inicializar o atributo attr
 		 pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE); // configurar o atributo attr
  	
  	for (t = 0; t < 1; t++){
  		
  		
		  		pthread_mutex_lock (&mutexcomp); // início da sessão crítica imposta pela main
				printf ("###\tPrincipal iniciando MUTEX e modificando variavel compartilhada\t###\n");
		   		varComp = 1;
		    	varComp = -1*varComp;
		
		    	printf ("\t#\tPrincipal computou o valor %ld para varComp\t\t#\n", varComp);
		    	printf ("###\tPrincipal finalizou MUTEX e modificou variavel compartilhada\t###\n");
		    	pthread_mutex_unlock (&mutexcomp); // fim da sessão crítica imposta pela main
		  		
     			rc = pthread_create(&threads[t], &attr, criaGrafo, (int *) &v);
     	
      			GRAFO * gr = criaGrafo(6);
      			criaAresta(gr, 0, 1, 1);
				criaAresta(gr, 0, 4, 2);
				
				criaAresta(gr, 1, 0, 1);
				criaAresta(gr, 1, 2, 3);
				criaAresta(gr, 1, 4, 2);
				
				criaAresta(gr, 2, 1, 3);
				criaAresta(gr, 2, 3, 6);
				
				criaAresta(gr, 3, 2, 6);
				criaAresta(gr, 3, 4, 4);
				criaAresta(gr, 3, 5, 3);
	
				criaAresta(gr, 4, 0, 2);
				criaAresta(gr, 4, 1, 2);
				criaAresta(gr, 4, 3, 4);
	
				criaAresta(gr, 5, 3, 3);
				
				imprime(gr);
			
      	    if (rc) {
       		  printf("ERROR; return code from pthread_create() is %d\n", rc);
        	  exit(-1);
      	    }
	    }
     
  		for (t = 0; t < 1; t++) {
    		rc = pthread_join (threads[t], &status);
    		
    		char verifica[] = "\nStatus da Thread = OK  - Atraves do Join";
    		status = verifica;
    		
			PRINT_LEGENDA;
    		PRINT_INCIDENCIAS;
    		PRINT_RESPOSTA;
    		
    		if (status != verifica) {
      			printf ("ERROR; return code from pthread_join() is %d\n", rc);
      			exit (-1);
    		}
    		
    		printf ("Main: completed join with thread %ld and 'MACRO' Data = %c  having a status of:  %s\n", t, status, status);
  		}

  		printf ("Main: program completed. Exiting.\n");
	
	/* Free attribute and wait for the other threads */
  	pthread_attr_destroy(&attr);
  	
  	pthread_mutex_destroy (&mutexcomp);
  	
    pthread_exit(NULL);
    
    return 0;
}


