#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"
#include <string.h>


/* 
 * Cria uma LEF vazia 
 */
lef_t * cria_lef (){
    lef_t *l;

    l = malloc(sizeof(lef_t));
    if (l == NULL)
        return NULL;
    l->Primeiro = NULL;
    return l;
}

/* 
 * Destroi a LEF e retorna NULL. 
 */ 
lef_t *destroi_lef (lef_t *l){
    nodo_lef_t *aux;

    while( l->Primeiro != NULL){
        aux = l->Primeiro;
        l->Primeiro = l->Primeiro->prox;
        free(aux->evento);
        free(aux);
    }

    l->Primeiro = NULL;
    free(l);
    l = NULL;

    return l;
}

/* 
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita. 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */ 
int adiciona_inicio_lef (lef_t *l, evento_t *evento){
    nodo_lef_t *n;

    n = malloc(sizeof(nodo_lef_t));
    if (n == NULL){
        free(n);
        return 0;
    }
       
    
    n->evento = malloc(sizeof(evento_t));
    if (n->evento == NULL){
        free(n->evento);
        return 0;
    }
        

    memcpy(n->evento, evento, sizeof(evento_t));

    if (l->Primeiro == NULL){
        l->Primeiro = n;
        n->prox = NULL;
        return 1;
    }
    
    n->prox = l->Primeiro;
    l->Primeiro = n;

    return 1;
}

/* 
 * Adiciona um evento de acordo com o valor evento->tempo na LEF. 
 * Uma nova copia da estrutura evento sera feita 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef (lef_t *l, evento_t *evento){
    nodo_lef_t *novo, *aux;

    novo = malloc(sizeof(nodo_lef_t));
    if (novo == NULL){
        free(novo);
        return 0;
    }

    novo->evento = malloc(sizeof(evento_t));
    if (novo->evento == NULL){
        free(novo->evento);
        return 0;
    }

    memcpy(novo->evento, evento, sizeof(evento_t));   

    if (l->Primeiro == NULL){
        l->Primeiro = novo;
        novo->prox = NULL;
        return 1;
    }

    aux = l->Primeiro;

    if (novo->evento->tempo < aux->evento->tempo){
        novo->prox = l->Primeiro;
        l->Primeiro = novo;
        return 1;
    }
    
    while ((aux->prox != NULL) && (aux->prox->evento->tempo < novo->evento->tempo)){
        aux = aux->prox;
    }

    novo->prox = aux->prox;
    aux->prox = novo;
    return 1;

}

/* 
 * Retorna e retira o primeiro evento da LEF. 
 * A responsabilidade por desalocar
 * a memoria associada eh de quem chama essa funcao.
 */
evento_t *obtem_primeiro_lef (lef_t *l){
    evento_t *evento;
    nodo_lef_t *aux;

    aux = l->Primeiro;
    evento = l->Primeiro->evento;
    l->Primeiro = l->Primeiro->prox;

    free(aux);
    
    return evento;
}
