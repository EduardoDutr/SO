#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>
#include "randomNumbersCsv.c"

#define N 100
#define temporeq 0


typedef struct _requisition_
{
  int quantity;
  int delay;

}Requisition;



typedef struct _threads_
{
  int* arrayOfThreads;
  pthread_t* normalthread;

}Threads;

typedef struct _normalThreadAttributes_
{
  int* threadPositionInArrayOfThreads;
  Requisition requisition;
  int threadId;
  int howManyTimesThreadWorked;

}NormalThreadAttributes;

char* getPi(int quantity){
  char* pi = (char*)malloc(sizeof(char)*quantity+3);
  //quantity+3 pois consideramos '3' ',' e '\n'
  sprintf(pi,"%d",quantity);
  pi[quantity+3] = '\n';
  return pi;
}

void writeResultInFile(NormalThreadAttributes* normalThreadAttributes, char* piDigits){
  FILE* file;

  char filename[40];
  sprintf(filename,"ThreadsOutput/ThreadNumber_%d",normalThreadAttributes->threadId);

  file = fopen(filename,"a+");
  if (file == NULL) {
    printf("Erro ao criar o arquivo.\n");
    return;
  }
  char* phraseToWrite = (char*)malloc(sizeof(char)*210);
  sprintf(phraseToWrite,"%d,%d,%d,%s\n",normalThreadAttributes->howManyTimesThreadWorked,normalThreadAttributes->requisition.quantity,normalThreadAttributes->requisition.delay,piDigits);
  fputs(phraseToWrite,file);
  free(phraseToWrite);
  fclose(file);
}

void normalThreadRoutine(NormalThreadAttributes* normalThreadAttributes){
  normalThreadAttributes->howManyTimesThreadWorked = normalThreadAttributes->howManyTimesThreadWorked +1;
  usleep(normalThreadAttributes->requisition.delay);
  char* pi = getPi(normalThreadAttributes->requisition.quantity);
  printf("%d\n",normalThreadAttributes->requisition.quantity);
  writeResultInFile(normalThreadAttributes,pi);
  
  *normalThreadAttributes->threadPositionInArrayOfThreads = 0;
  free(pi);
}



Requisition* readRequisitionFromCsv(FILE* file){
  Requisition* requisition = (Requisition*)malloc(sizeof(Requisition));
  char row[10];
  char* token;
  if(fgets(row,10,file) == NULL){
    return NULL;
  } 
  requisition->quantity = atoi(strtok(row,","));
  requisition->delay = atoi(strtok(NULL,"\n"));
  return requisition;
}


void dispatcherRoutine(Threads* threads){
  int i =0;
  Requisition* requisition;
  NormalThreadAttributes* normalThreadAttributes = (NormalThreadAttributes*)malloc(sizeof(NormalThreadAttributes)*N);
  for(int k =0;k<N;k++){
    (normalThreadAttributes+i)->howManyTimesThreadWorked = 0;
  }
  NormalThreadAttributes* workerAttributes;
  FILE *file;
  file = fopen("dados.csv","r");
  if (file == NULL) {
    printf("Erro ao criar o arquivo.\n");
    return;
  }


  while((requisition = readRequisitionFromCsv(file))!= NULL){
    while(*(threads->arrayOfThreads + i) == 1){
      i++;
      if(i>=N){
        sleep(1);
        i=0;
      }
    }
    workerAttributes = normalThreadAttributes + i;
    workerAttributes->requisition.delay = requisition->delay;
    workerAttributes->requisition.quantity = requisition->quantity;
    workerAttributes->threadId = i;
    workerAttributes->threadPositionInArrayOfThreads = threads->arrayOfThreads+i;
    
    pthread_create((threads->normalthread + i),NULL,(void*)normalThreadRoutine,workerAttributes);
    threads->arrayOfThreads[i] = 1;
    usleep(temporeq);
    free(requisition);
  }
  for(int j =0;j<N;j++){
    if(threads->arrayOfThreads[j] == 1){
      pthread_join(threads->normalthread[j],NULL);
    }
  }
  
  free(normalThreadAttributes);
  fclose(file);
  
}


int main(int argc, char* argv[]){
  Threads* threads = (Threads*)malloc(sizeof(Threads));
  threads->arrayOfThreads = (int*)malloc(sizeof(int)*N);
  threads->normalthread = (pthread_t*)malloc(sizeof(pthread_t)*N);
  pthread_t dispatcher;

  criarArquivoCSV( atoi(argv[1]));

  pthread_create(&dispatcher,NULL,(void*)&dispatcherRoutine,threads);
  pthread_join(dispatcher,NULL);


  free(threads->arrayOfThreads);
  free(threads->normalthread);
  free(threads);
  return 0;
}
