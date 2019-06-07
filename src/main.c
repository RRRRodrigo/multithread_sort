#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct args{
    int* entrada;
    int ini;
    int fim;
};

typedef struct args Tipo_Args;

void merge(int* entrada, int inicio, int half, int fim) {
    int com1 = inicio, com2 = half+1, comAux = 0, tam = fim-inicio+1;
    int *vet2;
    vet2 = (int*)malloc(tam * sizeof(int));

    while(com1 <= half && com2 <= fim){
        if(entrada[com1] < entrada[com2]) {
            vet2[comAux] = entrada[com1];
            com1++;
        } else {
            vet2[comAux] = entrada[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= half){  //Caso ainda haja elementos na primeira metade
        vet2[comAux] = entrada[com1];
        comAux++;
        com1++;
    }

    while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
        vet2[comAux] = entrada[com2];
        comAux++;
        com2++;
    }

    for(comAux = inicio; comAux <= fim; comAux++){    //Move os elementos de volta para o entrada original
        entrada[comAux] = vet2[comAux-inicio];
    }

    free(vet2);
}

void mergeSort(int * entrada, int inicio, int fim){
    if (inicio < fim) {
        int half = (fim+inicio)/2;

        mergeSort(entrada, inicio, half);
        mergeSort(entrada, half+1, fim);
        merge(entrada, inicio, half, fim);
    }
}

void * merger(void* void_args){
  int inicio = ((Tipo_Args*)void_args)->ini;
  int fim = ((Tipo_Args*)void_args)->fim;
  int* entrada = ((Tipo_Args*)void_args)->entrada;

  mergeSort(entrada, inicio, fim);
}

Tipo_Args* init_struct(int* entrada, int ini, int fim){
  Tipo_Args* arg_return = malloc(sizeof(Tipo_Args));
  arg_return->entrada = entrada;
  arg_return->ini = ini;
  arg_return->fim = fim;
  return arg_return;
}

int entrada[10000];

int main(){
  pthread_t workers[2];
  int thread_id[5000];
  int x = 0, i = 0;

  scanf("%d", &(entrada[x]));
  while(scanf("%d", &(entrada[++x])) == 1){}
  //x é o tamanho do vetor
  x--;

  Tipo_Args* arg1 = init_struct(entrada, 0, x/2);
  Tipo_Args* arg2 = init_struct(entrada, (x/2)+1, x);

  pthread_create(&(workers[0]), NULL, merger, (void*)(arg1)); //cada thread ordena metade do vetor
  pthread_create(&(workers[1]), NULL, merger, (void*)(arg2));

  pthread_join(workers[0], NULL);
  pthread_join(workers[1], NULL);
  merge(entrada, 0, x/2, x);

  for(i=0;i<=x;i++){
    printf("%d", entrada[i]);
    if(!(i==x))
      printf(" ");
  }
  printf("\n");
  return 0;
}
