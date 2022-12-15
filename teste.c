#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"


int main(){
    fila_t *f;

    f = cria_fila();
    destroi_fila(f);

    return 0;

}