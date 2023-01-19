#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

evento_t *cria_evento(){
    evento_t *e;

    e = malloc(sizeof(evento_t));
    if(e == NULL)
        return NULL;
    e->dado1 = 20;
    e->dado2 = 10;
    e->tempo = 3;
    e->tipo = 4;

    return e;
}
evento_t *cria_evento_novo(){
    evento_t *e;

    e = malloc(sizeof(evento_t));
    if(e == NULL)
        return NULL;
    e->dado1 = 20;
    e->dado2 = 10;
    e->tempo = 2;
    e->tipo = 4;

    return e;
}



int main(){
    lef_t *l;
    evento_t *e, *n;
    
    e = cria_evento();
    n = cria_evento_novo();

    l = cria_lef();

    adiciona_inicio_lef(l, e);
    adiciona_ordem_lef(l, n);

    destroi_lef(l);
    free(e);
    free(n);

    return 0;

}
