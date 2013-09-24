#include "DataShared.h"
#include "BideThread.h"
#include <iostream>
#include <fstream>

using namespace std;

#define THREAD_NUM  4
pthread_t g_thread[THREAD_NUM];
pthread_mutex_t g_mutex;

void iniBideThread(DataShared * data,BideThread * bide);
void * run(void * arg);

DataShared * pData = NULL;
BideThread * pBide = NULL;
int64_t count = 0;
int main(int argvs,char ** argv){
  //DataShared * pData = new DataShared(argv[0]);
  pData = new DataShared("../run.conf");
  pBide = new BideThread(0,"node0.log");
  iniBideThread(pData,pBide);
  pthread_mutex_init(&g_mutex, NULL);
  for(int i = 0;i < THREAD_NUM;i++){
    pthread_create(&g_thread[i], NULL,run,NULL);
  }
  for(int i = 0;i < THREAD_NUM;i++){
    pthread_join(g_thread[i], NULL);
  }
  pthread_mutex_destroy(&g_mutex);
  delete pData;
  delete pBide;
  return 0;
}

void * run(void *){
  int64_t seq[G_SEQLEN];
  while(true){
    pthread_mutex_lock(&g_mutex);
    if(pData->hasNext()){
      cout <<"Task:" <<++count <<" of " <<pData->m_taskCount <<endl;
      int64_t item = pData->getNextId();
      pthread_mutex_unlock(&g_mutex);
      memset(seq,-1,G_SEQLEN * sizeof(int64_t));
      seq[0] = 1; seq[1] = item;
      pBide->run(seq);
    }else{
      pthread_mutex_unlock(&g_mutex);
      break;
    }
  }
}

void iniBideThread(DataShared * data,BideThread * bide){
  bide->m_like = data->m_like;
  bide->m_minSup = data->m_minSup * data->m_nCountRows;
  bide->m_nCountRows = data->m_nCountRows;
  bide->m_nCountDifItem = data->m_nCountDifItem;
  bide->m_pWordProject = new set<int64_t> * [bide->m_nCountDifItem];
  for(int64_t i = 0;i < bide->m_nCountDifItem;i++){
    bide->m_pWordProject[i] = new set<int64_t>(*(data->m_pWordProject[i]));
  }
  bide->m_pDatabase = new int64_t *[bide->m_nCountRows];
  for(int64_t i = 0;i < bide->m_nCountRows;i++){
    bide->m_pDatabase[i] = new int64_t[data->m_pDatabase[i][0] + 1];
    for(int64_t j = 0;j <= data->m_pDatabase[i][0];j++){
      bide->m_pDatabase[i][j] = data->m_pDatabase[i][j];
    }
  }
}

