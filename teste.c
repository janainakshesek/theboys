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


int main(){
    lef_t *l;
    evento_t *e;
    
    e = cria_evento();


    l = cria_lef();

    adiciona_inicio_lef(l, e);

    destroi_lef(l);
    free(e);

    return 0;

}
