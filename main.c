#include "csvParser.h"
#include "myThreads.h"




int main(int argc, char* argv[]){
  Threads* threads = (Threads*)malloc(sizeof(Threads));
  threads->arrayOfThreads = (int*)malloc(sizeof(int)*N);
  threads->normalthread = (pthread_t*)malloc(sizeof(pthread_t)*N);
  pthread_t dispatcher;
  if(threads == NULL) return 1;
  if (threads->arrayOfThreads == NULL) return 2;
  if (threads->normalthread == NULL) return 3;
  
  int numReq = atoi(argv[1]);
  
  createRandomRequisitionsInFile( numReq);

  pthread_create(&dispatcher,NULL,(void*)&dispatcherRoutine,threads);
  pthread_join(dispatcher,NULL);


  free(threads->arrayOfThreads);
  free(threads->normalthread);
  free(threads);
  return 0;
}
