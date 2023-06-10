#include "csvParser.h"
#include "myThreads.h"
#include "pi.h"




void normalThreadRoutine(NormalThreadAttributes* normalThreadAttributes){
  normalThreadAttributes->howManyTimesThreadWorked = normalThreadAttributes->howManyTimesThreadWorked +1;
  usleep(normalThreadAttributes->requisition.delay);
  char* pi = getPi(normalThreadAttributes->requisition.quantity);
  writeResultInFile(normalThreadAttributes,pi);
  free(pi);
  *normalThreadAttributes->threadPositionInArrayOfThreads = 0;
  
}






void dispatcherRoutine(Threads* threads){
  int i =0;
  Requisition* requisition = (Requisition*)malloc(sizeof(Requisition));
  NormalThreadAttributes* normalThreadAttributes = (NormalThreadAttributes*)malloc(sizeof(NormalThreadAttributes)*N);
  for(int k =0;k<N;k++){
    (normalThreadAttributes+i)->howManyTimesThreadWorked = 0;
  }
  NormalThreadAttributes* workerAttributes;
  FILE *file;
  file = fopen("data.csv","r");
  if (file == NULL) {
    printf("Erro ao criar o arquivo.\n");
    return;
  }


  while(readRequisitionFromCsv(file,requisition)!= 0){
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
    //este printf tem poderes fortes o suficiente para impedir o codigo de crashar;
    //printf("Quantidade: %d\nDelay: %d\n",workerAttributes->requisition.quantity,workerAttributes->requisition.delay);
    //usleep(temporeq);
    pthread_create((threads->normalthread + i),NULL,(void*)normalThreadRoutine,workerAttributes);
    threads->arrayOfThreads[i] = 1;
    usleep(temporeq);
  }
  for(int j =0;j<N;j++){
    if(threads->arrayOfThreads[j] == 1){
      pthread_join(threads->normalthread[j],NULL);
    }
  }
  free(requisition);
  free(normalThreadAttributes);
  fclose(file);
  
}

