#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>

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

}NormalThreadAttributes;

char* getPi(int quantity){
  char* pi = (char*)malloc(sizeof(char)*quantity+3);
  //quantity+3 pois consideramos '3' ',' e '\n'
  sprintf(pi,"%d",quantity);
  pi[quantity+3] = '\n';
  return pi;
}

void writeResultInFile(int threadId, char* piDigits){
  FILE* file;

  char filename[20];
  sprintf(filename,"ThreadNumber_%d",threadId);

  file = fopen(filename,"a+");
  if (file == NULL) {
    printf("Erro ao criar o arquivo.\n");
    return;
  }
  char* phraseToWrite = (char*)malloc(sizeof(char)*110);
  sprintf(phraseToWrite,"Value of pi: %s \n",piDigits);
  fputs(phraseToWrite,file);
  free(phraseToWrite);
  fclose(file);
}

void normalThreadRoutine(NormalThreadAttributes* normalThreadAttributesPointer){
  NormalThreadAttributes normalThreadAttributes;
  normalThreadAttributes.requisition.delay = normalThreadAttributesPointer->requisition.delay;
  normalThreadAttributes.requisition.quantity = normalThreadAttributesPointer->requisition.quantity;
  normalThreadAttributes.threadId = normalThreadAttributesPointer->threadId;
  normalThreadAttributes.threadPositionInArrayOfThreads = normalThreadAttributesPointer->threadPositionInArrayOfThreads;
  
  sleep(normalThreadAttributes.requisition.delay);
  printf("%d\n",normalThreadAttributes.requisition.quantity);
  char* pi = getPi(normalThreadAttributes.requisition.quantity);
  writeResultInFile(normalThreadAttributes.threadId,pi);
  //sleep(normalThreadAttributes.requisition.delay);
  *normalThreadAttributes.threadPositionInArrayOfThreads = 0;
  free(pi);
  printf("Debug %d\n",normalThreadAttributes.threadId);
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
  NormalThreadAttributes* normalThreadAttributes = (NormalThreadAttributes*)malloc(sizeof(NormalThreadAttributes));
  FILE *file;
  file = fopen("test.txt","r");
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
    normalThreadAttributes->requisition.delay = requisition->delay;
    normalThreadAttributes->requisition.quantity = requisition->quantity;
    normalThreadAttributes->threadId = i;
    normalThreadAttributes->threadPositionInArrayOfThreads = threads->arrayOfThreads+i;
    
    pthread_create((threads->normalthread + i),NULL,(void*)normalThreadRoutine,normalThreadAttributes);
    threads->arrayOfThreads[i] = 1;
    sleep(temporeq);
    i=0;
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
