#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>

#define N 2
#define temporeq 1

typedef struct _requisition_
{
  int quantity;
  int delay;
  int* arrayOfThreads;
  int i;

}Requisition;



typedef struct _threads_
{
  int* arrayOfThreads;
  pthread_t* normalthread;
}Threads;



void getPi(int quantity){
  printf("Quantity: %d\n", quantity);
}


void normalThreadRoutine(Requisition* requisition){
  Requisition requisitionNormal;
  requisitionNormal.delay = requisition->delay;
  requisitionNormal.quantity = requisition->quantity;
  requisitionNormal.i = requisition->i;
  getPi(requisitionNormal.quantity);
  sleep(requisitionNormal.delay);
  requisition->arrayOfThreads[requisitionNormal.i] = 0;
}



Requisition* readRequisitionFromCsv(FILE* file){
  Requisition* requisition = (Requisition*)malloc(sizeof(Requisition));
  char row[10];
  char* token;
  if(fgets(row,10,file) == NULL){
    return NULL;
  }
  if(row == NULL) return NULL; 
  requisition->quantity = atoi(strtok(row,","));
  requisition->delay = atoi(strtok(NULL,"\n"));
  return requisition;
}


void dispatcherRoutine(Threads* threads){
  int i =0;
  Requisition* requisition = (Requisition*)malloc(sizeof(Requisition));
  FILE *file;
  file = fopen("test.txt","r");
  while((requisition = readRequisitionFromCsv(file))!= NULL){
    while(*(threads->arrayOfThreads + i) == 1){
      i++;
      if(i>=N){
        sleep(1);
        i=0;
      }
    }
    requisition->i = i;
    requisition->arrayOfThreads = threads->arrayOfThreads;
    pthread_create((threads->normalthread + i),NULL,(void*)normalThreadRoutine,requisition);
    threads->arrayOfThreads[i] = 1;
    sleep(temporeq);
    i=0;
  }
  
  fclose(file);
}


int main(){
  Threads* threads = (Threads*)malloc(sizeof(Threads));
  threads->arrayOfThreads = (int*)malloc(sizeof(int)*N);
  threads->normalthread = (pthread_t*)malloc(sizeof(pthread_t)*N);
  pthread_t dispatcher;

  pthread_create(&dispatcher,NULL,(void*)&dispatcherRoutine,threads);
  pthread_join(dispatcher,NULL);


  free(threads->arrayOfThreads);
  free(threads->normalthread);
  free(threads);
  return 0;
}
