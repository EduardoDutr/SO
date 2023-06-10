#include "csvParser.h"
#include "myThreads.h"


int readRequisitionFromCsv(FILE* file, Requisition* requisition){
  char row[15];
  
  if(fgets(row,15,file) == NULL){
    return 0;
  } 
  requisition->quantity = atoi(strtok(row,","));
  requisition->delay = atoi(strtok(NULL,"\n"));
  return 1;
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
  //printf("%d\n",normalThreadAttributes->requisition.quantity);
  char* phraseToWrite = (char*)malloc(sizeof(char)*210);
  sprintf(phraseToWrite,"%d,%d,%d,%s\n",normalThreadAttributes->howManyTimesThreadWorked,normalThreadAttributes->requisition.quantity,normalThreadAttributes->requisition.delay,piDigits);
  fputs(phraseToWrite,file);
  free(phraseToWrite);
  fclose(file);
}

void createRandomRequisitionsInFile(int quantityOfRequisitions){
  FILE* archive = fopen("data.csv", "w");
    if (archive == NULL) {
      printf("Erro ao criar o arquivo.");
      return;
    }

  srand(time(NULL));


  for (int i = 0; i < quantityOfRequisitions; i++) {
    int quantityOfDigits = rand() % 91 + 10; // Gera um número aleatório entre 10 e 100
    int delay = rand() % 1001 + 500; // Gera um número aleatório entre 500 e 1500

    fprintf(archive, "%d,%d\n", quantityOfDigits, delay);
  }

  fclose(archive);
}
