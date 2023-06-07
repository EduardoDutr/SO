#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>

#define N 2
#define temporeq 0.5

typedef struct _requisition_
{
    int quantity;
    int delay;
    int* arrayOfThreads;
    int i;
}Requisition;

void getPi(int quantity){
    printf("Quantity: %d\n", quantity);
}
void normalThreadRoutine(Requisition* requisition){
    Requisition requisitionNormal;
    requisitionNormal.delay = requisition->delay;
    requisitionNormal.quantity = requisition->quantity;
    requisitionNormal.i = requisition->i;
    getPi(requisitionNormal.i);
    sleep(requisitionNormal.delay);
    requisition->arrayOfThreads[requisitionNormal.i] = 0;
}

Requisition* readRequisitionFromCsv(FILE* file){
    Requisition* requisition;
    char row[10];
    char* token;
    if(fgets(row,10,file) == NULL){
        return NULL;
    } 
    requisition->quantity = atoi(strtok(row,","));
    requisition->delay = atoi(strtok(NULL,"\n"));
    return requisition;
}

//dar um jeito de setar o arrayOfThreads[i] para 0

void dispatcherRoutine(int* arrayOfThreads){
    int i =0;
    pthread_t normalthread[N];
    Requisition* requisition;
    FILE *file;
    file = fopen("test.txt","r");
    while((requisition = readRequisitionFromCsv(file))!= NULL){
        while(arrayOfThreads[i] == 1){
            i++;
            if(i>N){
                sleep(1);
                i=0;
            }
        }
        requisition->i = i;
        requisition->arrayOfThreads = arrayOfThreads;
        pthread_create(normalthread + i,NULL,(void*)normalThreadRoutine,requisition);
        arrayOfThreads[i] = 1;
        sleep(temporeq);
        i=0;
    }
    
    fclose(file);
}


//Dispatcher lera do Csv e buscara alguma thread para executar a funcao,
int main(){
    int* arrayOfThreads = (int*)malloc(sizeof(int)*N);
    pthread_t dispatcher;
    pthread_create(&dispatcher,NULL,(void*)&dispatcherRoutine,arrayOfThreads);
    pthread_join(dispatcher,NULL);
    return 0;
    free(arrayOfThreads);
}
