#include "csvParser.h"
#include "myThreads.h"
#include "pi.h"



void normalThreadRoutine(NormalThreadAttributes* normalThreadAttributes){
  normalThreadAttributes->howManyTimesThreadWorked = normalThreadAttributes->howManyTimesThreadWorked +1;
  usleep(normalThreadAttributes->requisition.delay);
  char* pi = getPi(normalThreadAttributes->requisition.quantity);
  writeResultInFile(normalThreadAttributes->threadId,normalThreadAttributes->howManyTimesThreadWorked,normalThreadAttributes->requisition.quantity,normalThreadAttributes->requisition.delay,pi);
  free(pi);
  *normalThreadAttributes->threadPositionInArrayOfThreads = 0;
  
}






// void dispatcherRoutine(Threads* threads){
//   int threadId =0;
//   int requisitionId= 0;
//   Requisition* requisition = (Requisition*)malloc(sizeof(Requisition)*N);
//   NormalThreadAttributes* normalThreadAttributes = (NormalThreadAttributes*)malloc(sizeof(NormalThreadAttributes)*N);
//   for(int k =0;k<N;k++){
//     (normalThreadAttributes+threadId)->howManyTimesThreadWorked = 0;
//   }
//   NormalThreadAttributes* workerAttributes;
//   FILE *file;
//   file = fopen("data.csv","r");
//   if (file == NULL) {
//     printf("Erro ao criar o arquivo.\n");
//     return;
//   }

//   //Coleta todas as requisicoes e as poem em requisition;
//   while(requisitionId<N){
//     readRequisitionFromCsv(file,requisition+requisitionId);
//     requisitionId++;
//   }

//   requisitionId=0;
//   while(requisitionId<N){
//     while(*(threads->arrayOfThreads + threadId) == 1){
//       threadId++;
//       if(threadId>=N){
//         sleep(1);
//         threadId=0;
//       }
//     }
//     workerAttributes = normalThreadAttributes + threadId;
//     workerAttributes->requisition.delay = (requisition[requisitionId]).delay;
//     workerAttributes->requisition.quantity = (requisition[requisitionId]).quantity;
//     workerAttributes->threadId = threadId;
//     workerAttributes->threadPositionInArrayOfThreads = threads->arrayOfThreads+threadId;
//     //este printf tem poderes fortes o suficiente para impedir o codigo de crashar;
//     //printf("Quantidade: %d\nDelay: %d\n",workerAttributes->requisition.quantity,workerAttributes->requisition.delay);
//     //usleep(temporeq);
//     pthread_create((threads->normalthread + threadId),NULL,(void*)normalThreadRoutine,workerAttributes);
//     threads->arrayOfThreads[threadId] = 1;
//     usleep(temporeq);
//     requisitionId++;
//   }

//   for(int j =0;j<N;j++){
//     if(threads->arrayOfThreads[j] == 1){
//       pthread_join(threads->normalthread[j],NULL);
//     }
//   }
//   free(requisition);
//   free(normalThreadAttributes);
//   fclose(file);
  
// }



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
