#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define FIM 4
#define ACABA 34944

struct herois {
    int id; /* inteiro unico para cada heroi que o identifica   */
    int paciencia; /* inteiro de 0 a 100 que interfere nas decisoes de permanencia em locais e filas  */
    int idade; /* inteiro de 18 a 100 que iterfere no tempo de deslocamento */
    int experiencia; /* inteiro maior ou igual a 0 que indica o numero de missoes que o heroi participou  */
    conjunto_t *habilidades; /* conjunto de habilidades que o heroi possui */
};
typedef struct herois herois_st;

struct local {
    int id; /* inteiro unico para cada heroi que o identifica */
    int lot_max; /* numero maximo de herois no local */
    fila_t *fila; /* vetor de inteiros com no maximo max elementos  */
    int coord_x; /* uma coordenada x que indica o local, cada unidade corresponde a 1 metro*/
    int coord_y; /* uma coordenada x que indica o local, cada unidade corresponde a 1 metro*/
    conjunto_t *herois; /* conjunto com os ids dos herois presentes */
};
typedef struct local local_st;

struct mundo {
    int tempo_atual; /* tempo atual da simulação, cada unidade corresponde a 15 minutos do mundo real*/
    int tam_mundo; /* valor máximo da coordenada */
    int n_habilidades; /* numero de habilidades existentes no mundo */
    int num_herois; /* numero de herois existentes no mundo*/
    int num_locais; /* numero de locais existnetes no mundo */
    int fim; /* valor do instante fnal da simualação*/
    int n_missoes; /* numero de missoes do mundo */
    herois_st *n_herois; /* vetor de ponteiros para  herois */
    local_st *n_locais; /* vetor de ponteiros para para ponteiros de locais*/
    conjunto_t *ch_habilidades; /* conjunto cujo max é o valor n_habilidades, ou seja, o numero de habilidades existentes no mundo*/
};
typedef struct mundo mundo_st;

int aleat(int a, int b){
    return ((rand() % (b - a  + 1)) + a); /* cria rand com minimo e maximo */
}

/* Cria o vetor de heróis */
herois_st *cria_heroi(conjunto_t *c, int quant){
    herois_st *n_herois;
    int i;
    n_herois = malloc(sizeof(herois_st)*quant);

    for(i = 0; i < quant; i++){
        n_herois[i].id = i;
        n_herois[i].paciencia = aleat(0, 100);
        n_herois[i].idade = aleat(18, 100);
        n_herois[i].experiencia = 0;

        n_herois[i].habilidades = cria_subcjt_cjt(c, aleat(2,5));

    }

    return n_herois;
}

/* Cria o vetor de locais */
local_st *cria_local(int tam_mundo, int quant){
    local_st *n_locais;
    int i;

    n_locais = malloc(sizeof(local_st)*quant);

    for(i = 0; i < quant; i++){
        n_locais[i].id = i;
        n_locais[i].lot_max = aleat(5, 30);
        n_locais[i].fila = cria_fila();
        n_locais[i].coord_x = aleat(0, tam_mundo-1);
        n_locais[i].coord_y = aleat(0, tam_mundo-1);
        n_locais[i].herois = cria_cjt(n_locais[i].lot_max);
    }

    return n_locais;
}

/*Cria conjunto de habilidades do mundo*/
conjunto_t *cria_hab(int quant){
    conjunto_t *ch_habilidades;
    int i;

    ch_habilidades = cria_cjt(quant);

    for(i = 0; i < quant; i++){
        insere_cjt(ch_habilidades, aleat(0, quant-1)); 
    }

    return ch_habilidades;
}

evento_t *cria_evento(){
    evento_t *evento;

    evento = malloc(sizeof(evento_t));

    evento->tempo = 0;
    evento->tipo = 0;
    evento->dado1 = 0;
    evento->dado2 = 0;

    return evento;
}

void chegada(int id, int tempo, evento_t *e, int id_local_dest) {

    e->tempo = tempo ;
    e->tipo = CHEGADA;
    e->dado1 = id;
    e->dado2 = id_local_dest;

}

/* Função para criar as primeiras chegadas dos heróis */
void cria_chegada(mundo_st *m, evento_t *e, lef_t *lista){
    int i;

    for(i=0; i < m->num_herois; i++){
        chegada(i, aleat(0, 96*7), e, aleat(0, m->num_locais-1));
        adiciona_ordem_lef(lista, e);
    }
}

void saida(int id_heroi, int id_local, int tempo, evento_t *e){

    e->tempo = tempo;
    e->tipo = SAIDA;   
    e->dado1 = id_heroi; 
    e->dado2 = id_local;
}

void cria_missao(int n_missoes, lef_t *lista, evento_t *e, int tempo, int id) {

    e->tempo = tempo;
    e->tipo = MISSAO;
    e->dado1 = id;
    e->dado2 = 0;

    adiciona_ordem_lef(lista, e);
}

void fim_do_mundo(evento_t *e, lef_t *lista){

    e->tempo = ACABA;
    e->tipo = FIM;
    e->dado1 = 0;
    e->dado2 = 0;

    adiciona_ordem_lef(lista, e);
    free(e);
}

conjunto_t *equipes(int i, mundo_st *m){
    int j;
    conjunto_t *aux, *uniao;

    uniao = cria_cjt(m->n_habilidades);

     /* une todas as habilidades dos herois no local i*/
    for(j = 0; j < m->n_locais[i].herois->card; j++){
        aux = uniao;
        uniao = uniao_cjt(uniao, m->n_herois[m->n_locais[i].herois->v[j]].habilidades);
        destroi_cjt(aux);
    }

    return uniao;
}

int encontra (conjunto_t **uniao, conjunto_t *missao_cjt, mundo_st *m, int *indice){
    int j=0, aux = 0;

    while((j < m->num_locais) && (aux == 0)){
        if ((contido_cjt(missao_cjt, uniao[j]) && (!vazio_cjt(uniao[j]))))
            aux = 1;
        j++;
    }

    *indice = j;
    return aux;
}


void destroiuniao(conjunto_t **uniao, int tam){
    int i;

    for(i=0; i < tam; i++){
        destroi_cjt(uniao[i]);
    }
}

void tratar_missao(evento_t *missao, mundo_st *m, lef_t *l){
    int i, k, j, indice=0, aux;
    conjunto_t *missao_cjt, **uniao;

    missao_cjt = cria_subcjt_cjt(m->ch_habilidades, aleat(3,6));
    uniao = malloc(sizeof(conjunto_t)*m->num_locais);

    printf("%6d: MISSAO %2d HAB REQ: ", m->tempo_atual, missao->dado1);
    imprime_cjt(missao_cjt);

    for(i = 0; i < m->num_locais; i++){ /* percorre os locais do mundo */
        uniao[i] = equipes(i, m);
        printf("%6d: MISSAO %2d HER_EQL %2d: ", m->tempo_atual, missao->dado1, i);
        imprime_cjt(uniao[i]);
    }

    if (!(encontra(uniao, missao_cjt, m, &j))){
        printf("%6d: MISSAO %2d IMPOSSIVEL \n", m->tempo_atual, missao->dado1);

        aux = missao->tempo + aleat(missao->tempo, ACABA);
        cria_missao(m->n_missoes, l, missao, aux, missao->dado1);

        destroiuniao(uniao, m->num_locais);
        destroi_cjt(missao_cjt);
        free(uniao);

        return;

    } else {
        indice = j - 1;

        for(; j < m->num_locais; j++){
            if ((contido_cjt(missao_cjt, uniao[j]) && (!vazio_cjt(uniao[j])))){
                if ((cardinalidade_cjt(m->n_locais[j].herois) < cardinalidade_cjt(m->n_locais[indice].herois))){
                    indice = j;
                }
            }
        }
    }

    printf("%6d: MISSAO %2d HER_EQS %2d: ", m->tempo_atual, missao->dado1, indice);
    imprime_cjt(m->n_locais[indice].herois);

    for(k = 0; k < cardinalidade_cjt(m->n_locais[indice].herois); k++){
        m->n_herois[m->n_locais[indice].herois->v[k]].experiencia++;
    }

    destroiuniao(uniao, m->num_locais);
    destroi_cjt(missao_cjt);
    free(uniao);
}

int paciente(int paciencia, int tamanho){
    if (paciencia/4 - tamanho > 0)
        return 1;
    return 0;
}

int max(int a, int b){
    if (a > b)
        return a;
    return b;
}

void tratar_chegada(evento_t *e, mundo_st *m, lef_t *l){

    int tpl;

    printf("%6d: CHEGA HEROI %2d LOCAL %d,", e->tempo, e->dado1, e->dado2);
    printf("(%2d/%2d) ", cardinalidade_cjt(m->n_locais[e->dado2].herois), m->n_locais[e->dado2].lot_max);

    if (cardinalidade_cjt(m->n_locais[e->dado2].herois) == m->n_locais[e->dado2].lot_max){ /* verifica se o local esta cheio */
        if(paciente(m->n_herois[e->dado1].paciencia, tamanho_fila(m->n_locais[e->dado2].fila))){ /* verifica a paciencia do heroi */
            insere_fila(m->n_locais[e->dado2].fila, e->dado1);/* se o heroi for paciente o insere na fila */
            printf(" FILA %2d \n", m->n_locais[e->dado2].fila->tamanho);
        }
        else{
            saida(e->dado1, e->dado2, e->tempo, e); /* se o heroi nao for paciente cria um evento de saida */
            adiciona_ordem_lef(l, e);
            printf(", DESISTE \n");
        }
    }
    else {
        printf(", ENTRA \n");

        insere_cjt(m->n_locais[e->dado2].herois, e->dado1); /* se o local nao estiver cheio insere o heroi */

        tpl = max(1, (m->n_herois[e->dado1].paciencia/10 + aleat(-2, 6))); /* calcula o tempo de permanencia */

        saida(e->dado1, e->dado2, tpl + e->tempo, e); /* cria evento de saida */
        adiciona_ordem_lef(l, e);
    }
}

double distancia(int x1, int x2, int y1, int y2){ /* calcula distancia dos pontos */
    int soma = ((pow((x2-x1),2)) + pow((y2-y1),2));
    return sqrt(soma);
}

void tratar_saida(mundo_st *m, evento_t *e, lef_t *l){

    int tdl, id_local_dest, x1, x2, y1, y2, local = e->dado2;

    retira_cjt(m->n_locais[e->dado2].herois, e->dado1); /* retira do conjunto de herois presentes no local */

    printf("%6d: SAIDA HEROI %2d LOCAL %d, ", e->tempo, e->dado1, e->dado2);
    printf("(%2d/%2d) ", cardinalidade_cjt(m->n_locais[e->dado2].herois), m->n_locais[e->dado2].lot_max);

    id_local_dest = aleat(0, m->num_locais-1); /* cria o local de destino depos da saida */

    x1 = m->n_locais[id_local_dest].coord_x;
    x2 = m->n_locais[e->dado2].coord_x;
    y1 = m->n_locais[id_local_dest].coord_y;
    y2 = m->n_locais[e->dado2].coord_y;

    tdl = distancia(x1, x2, y1, y2)/(100 - max(0, m->n_herois[e->dado1].idade-40)); /* calcula o tempo de deslocamento */
    chegada(e->dado1, e->tempo + tdl/15, e, id_local_dest); /* cria um novo evento de chegada */
    adiciona_ordem_lef(l, e);

    /* verifica se a fila do local existe */
    if ((!vazia_fila(m->n_locais[local].fila))){
        retira_fila(m->n_locais[local].fila, &e->dado1);
        chegada(e->dado1, m->tempo_atual, e, local); /*cria um evento de chegada para o heroi que estava na fila */
        adiciona_inicio_lef(l, e); /* insere com prioridade na lef */
        printf("REMOVE FILA HEROI %2d",  e->dado1);
    }

    printf("\n");
}

mundo_st *cria_mundo(){
    mundo_st *m;

    m = malloc(sizeof(mundo_st));

    m->tempo_atual = 0;
    m->tam_mundo = 2000;
    m->n_habilidades = 10;
    m->num_herois = m->n_habilidades*5;
    m->num_locais = m->num_herois/6;
    m->n_missoes = ACABA/100;
    m->ch_habilidades = cria_hab(m->n_habilidades);
    m->n_herois = cria_heroi(m->ch_habilidades, m->num_herois);
    m->n_locais = cria_local(m->tam_mundo, m->num_locais);

    return m;
}

void destroi(mundo_st *m, lef_t *l){

    int j;

    for(j = 0; j < m->num_locais; j++){
        destroi_cjt(m->n_locais[j].herois);
        destroi_fila(m->n_locais[j].fila);
    }
    int i;
    for(i = 0; i < m->num_herois; i++){
        destroi_cjt(m->n_herois[i].habilidades);
    }

    free(m->n_herois);
    free(m->n_locais);
    destroi_cjt(m->ch_habilidades);
    destroi_lef(l);
    free(m);
}

int main() {
    mundo_st *mundo;
    evento_t *e;
    int j, k;
    lef_t *lista;

    srand(0);

    mundo = cria_mundo();

    lista = cria_lef();
    e = cria_evento();

    cria_chegada(mundo, e, lista);

    for(j = 0; j < mundo->n_missoes; j++){
        cria_missao(mundo->n_missoes, lista, e, aleat(0, ACABA), j);
    }

    fim_do_mundo(e, lista); /* cria um evento de fim do mundo */

    while(lista->Primeiro != NULL){
        e = obtem_primeiro_lef(lista);
        mundo->tempo_atual = e->tempo;
        switch (e->tipo)
        {
        case CHEGADA:
            tratar_chegada(e, mundo, lista);
            break;
        case SAIDA:
            tratar_saida(mundo, e, lista);
            break;
        case MISSAO:
            tratar_missao(e, mundo, lista);
            break;
        case FIM:
            printf("%6d: FIM", e->tempo);
            for(k = 0; k < mundo->num_herois; k++){ /* imprime a experiencia dos herois em ordem de id */
                printf("\nHEROI %2d EXPERIENCIA %2d", k, mundo->n_herois[k].experiencia);
            }
            printf("\n\n");
            free(e);
            destroi(mundo, lista);
            return 0;
            break;
        }
    free(e);
    }

    return 0;
}