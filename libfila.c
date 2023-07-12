#include <stdlib.h>
#include <stdio.h>
#include "libfila.h"


/* Cria uma fila vazia e a retorna, se falhar retorna NULL. */
fila_t *cria_fila (){
    fila_t *f;

    f = malloc(sizeof(fila_t));
    if (f == NULL)
        return NULL;

    f->tamanho = 0;
    f->ini = NULL;
    f->fim = NULL;

    return f;
}

/*  Remove todos os elementos da fila, libera espaco e devolve NULL.*/
fila_t * destroi_fila (fila_t *f){
    
    nodo_f_t *aux;
    
    while(!vazia_fila(f)){
        aux = f->ini;
        f->ini = f->ini->prox;
        free(aux);
    }

    f->tamanho = 0;
    f->ini = f->fim = NULL;
    free(f);

    return NULL;
}

/* Retorna 1 se a fila esta vazia e 0 caso contrario. */
int vazia_fila (fila_t *f){

    if (f->ini == NULL)
        return 1;
    return 0;
}

/* Retorna o numero de elementos da fila. */
int tamanho_fila (fila_t *f){

    return f->tamanho;
}

/* Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario. */
int insere_fila (fila_t *f, int elemento){
    nodo_f_t *n, *aux;

    n = malloc(sizeof(nodo_f_t));
    if(n == NULL)
        return 0;

    n->elem = elemento;
    n->prox = NULL;

    if(vazia_fila(f)){
        f->ini = n;
        f->fim = n;
        f->tamanho++;

        return 1;
    }

    aux = f->fim;
    aux->prox = n;
    f->fim = n;
    f->tamanho++;

    return 1;
}

/* Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro *elemento. Retorna 1 se a operacao foi bem sucedida
 * e 0 caso contrario. */
int retira_fila (fila_t *f, int *elemento){
    nodo_f_t *aux;

     if(vazia_fila(f))
        return 0;

    *elemento = f->ini->elem;

    aux = f->ini;
    f->ini = f->ini->prox;
    f->tamanho--;

    free(aux);
    return 1;
}
