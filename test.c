#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>

#define N 2
#define temporeq 0.750

typedef struct _requisition_
{
    int quantity;
    int delay;

}Requisition;

void getPi(int quantity){
    printf("Quantity: %d\n", quantity);
}
void normalThreadRoutine(Requisition* requisition){
    Requisition requisitionNormal;
    requisitionNormal.delay = requisition->delay;
    requisitionNormal.quantity = requisition->quantity;
    getPi(requisitionNormal.quantity);
    sleep(requisitionNormal.delay);
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

//todas as threads utilizam a mesma requisicao pois e um ponteiro

void dispatcherRoutine(){
    int i =0;
    int arrayOfThreads[N];
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
        pthread_create(normalthread + i,NULL,(void*)normalThreadRoutine,requisition);
        arrayOfThreads[i] = 1;
        sleep(temporeq);
        i=0;
    }
    fclose(file);
}


//Dispatcher lera do Csv e buscara alguma thread para executar a funcao,
int main(){
    pthread_t dispatcher;
    pthread_create(&dispatcher,NULL,(void*)&dispatcherRoutine,NULL);
    pthread_join(dispatcher,NULL);
    return 0;
}
