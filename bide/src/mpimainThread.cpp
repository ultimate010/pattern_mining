/*
 * Master Slave Framework
 * Zaks Wang
 * 2013-9-18
 */
#include "mpi.h"
#include "DataShared.h"
#include "BideThread.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

#define COM_TAG 0
#define MES_TAG 1
#define MES_FINISH 2
#define THREAD_NUM  2

void bcast2BideThread(DataShared * data);
void showData(BideThread * bide);
void iniBideThread(BideThread * bide);
void * run(void * arg);

int G_COUNT = 0;
int master();
int slave();
BideThread * pBide = NULL;
pthread_mutex_t g_mutex;
int main(int argc,char **argv)
{
  int node,nodeSize;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&node);
  MPI_Comm_size(MPI_COMM_WORLD,&nodeSize);
  if(node == 0){
    master();
  }else{
    slave();
  }
  MPI_Finalize();
  return 0;
}
int master(){
  int size,rank,nslave,buf;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  nslave = (size - 1) * THREAD_NUM;
#ifdef _INFO
  cout <<"****************************************\n";
  cout <<"\tMaster:\t Slave size: " <<nslave <<endl;
  cout <<"****************************************\n";
#endif
  /*
   * 同步数据
   */
  DataShared * pData = new DataShared("../run.conf");
  bcast2BideThread(pData);
  int64_t count = 0;
  while(nslave>0){
#ifdef _INFO
    cout <<"Master:\t**************Waiting for request**************\n";
#endif
    MPI_Recv(&rank,1,MPI_INT,MPI_ANY_SOURCE,COM_TAG,MPI_COMM_WORLD,&status);
#ifdef _INFO
    cout <<"Master:\tReceive " <<rank <<" request\n";
#endif
    if(pData->hasNext()){
      int64_t next = pData->getNextId();
      count++;
#ifdef _INFO
      cout <<"Master:\tTry to assign task to " <<rank <<endl;
#endif
      MPI_Send(&rank,1,MPI_INT,rank,COM_TAG,MPI_COMM_WORLD);
      MPI_Send((void *)&G_COUNT,sizeof(G_COUNT),MPI_CHAR,rank,MES_TAG,MPI_COMM_WORLD);
      MPI_Send((void *)&next,sizeof(int64_t),MPI_CHAR,rank,MES_TAG,MPI_COMM_WORLD);
      cout <<"Master: " <<count <<" of " <<pData->m_taskCount <<endl;
#ifdef _INFO
      cout <<"Master:\tAlready assign task to " <<rank <<endl;
#endif
    }else{
#ifdef _INFO
      cout <<"Master:\t**********No task, send exit message to " <<rank <<"***********" <<endl;
#endif
      MPI_Send(&rank,1,MPI_INT,rank,MES_FINISH,MPI_COMM_WORLD);
      nslave--;
    }
  }
  delete pData;
  return 0;
}
int slave(){
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  char temp[20] = {0};
  sprintf(temp,"%d",rank);
  string path(temp);
  path = "log.node" + path;
#ifdef _INFO
  ofstream out(path.c_str());
#endif
  path = path + ".out";
  pBide = new BideThread(rank,path);
  iniBideThread(pBide);
  /*
   * 多线程
   */
  pthread_t m_thread[THREAD_NUM];
  pthread_mutex_init(&g_mutex, NULL);
  for(int i = 0;i < THREAD_NUM;i++){
    pthread_create(&m_thread[i], NULL,run,NULL);
  }
  for(int i = 0;i < THREAD_NUM;i++){
    pthread_join(m_thread[i], NULL);
  }
  pthread_mutex_destroy(&g_mutex);
  delete pBide;
  return 0;
}

void bcast2BideThread(DataShared * data){
  int64_t minsup = (int64_t)(data->m_minSup * data->m_nCountRows);
  MPI_Bcast(&(data->m_like),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Bcast(&(minsup),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Bcast(&(data->m_nCountRows),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Bcast(&(data->m_nCountDifItem),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  for(int64_t i = 0;i < data->m_nCountDifItem;i++){
    int64_t size = (data->m_pWordProject[i])->size();
    MPI_Bcast(&(size),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
    int64_t temp;
    for(set<int64_t>::iterator iter = data->m_pWordProject[i]->begin();
          iter != data->m_pWordProject[i]->end();iter++){
      temp = *iter;
      MPI_Bcast(&(temp),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
    }
  }
  for(int64_t i = 0;i < data->m_nCountRows;i++){
    int64_t size = data->m_pDatabase[i][0];
    MPI_Bcast(&(size),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
    for(int64_t j = 0;j <= data->m_pDatabase[i][0];j++){
      MPI_Bcast(&(data->m_pDatabase[i][j]),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
    }
  }
}
void iniBideThread(BideThread * bide){
  MPI_Bcast(&(bide->m_like),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Bcast(&(bide->m_minSup),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Bcast(&(bide->m_nCountRows),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  MPI_Bcast(&(bide->m_nCountDifItem),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
  bide->m_pWordProject = new set<int64_t> * [bide->m_nCountDifItem];
  for(int64_t i = 0;i < bide->m_nCountDifItem;i++){
    int64_t size = 0;
    MPI_Bcast(&(size),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
    //cout <<size <<endl;
    bide->m_pWordProject[i] = new set<int64_t>;
    int64_t temp;
    for(int64_t j = 0;j < size;j++){
      MPI_Bcast(&(temp),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
      bide->m_pWordProject[i]->insert(temp);
    }
  }
  bide->m_pDatabase = new int64_t * [bide->m_nCountRows];
  for(int64_t i = 0;i < bide->m_nCountRows;i++){
    int64_t size = 0;
    MPI_Bcast(&(size),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
    //cout <<size <<endl;
    bide->m_pDatabase[i] = new int64_t[size + 1];
    int64_t temp;
    for(int64_t j = 0;j <= size;j++){
      MPI_Bcast(&(temp),sizeof(int64_t),MPI_CHAR,0,MPI_COMM_WORLD);
      bide->m_pDatabase[i][j] = temp;
    }
  }
  //  showData(bide);
}


void showData(BideThread * bide){
  cout << bide->m_nCountRows <<"\t" <<bide->m_nCountDifItem <<"\t" <<bide->m_minSup <<endl;
  cout <<"Database;\n";
  for(int64_t i = 0;i < bide->m_nCountRows;i++){
    for(int64_t j = 0;j <= bide->m_pDatabase[i][0];j++){
      cout <<bide->m_pDatabase[i][j] <<"\t";
    }
    cout <<endl;
  }
}

void * run(void *){
  MPI_Status m_status;
  int m_rank;
  char buf[256];
  int64_t seq[G_SEQLEN];
  MPI_Comm_rank(MPI_COMM_WORLD,&m_rank);
  while(true){
    /*轮询获取消息*/
    pthread_mutex_lock(&g_mutex);
    MPI_Send(&m_rank,1,MPI_INT,0,COM_TAG,MPI_COMM_WORLD);
#ifdef _INFO
    out <<"Slave:\t" <<rank <<" send request to master\n";
#endif
    MPI_Recv(&m_rank,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&m_status);
#ifdef _INFO
    out <<"Slave:\t" <<rank <<" get response from master\n";
#endif
    if(m_status.MPI_TAG == MES_FINISH){
#ifdef _INFO
      out <<"Slave:\t" <<rank <<" receive exit message\n";
#endif
      pthread_mutex_unlock(&g_mutex);
      break;
    }else{
      MPI_Recv(buf,256,MPI_CHAR,0,MES_TAG,MPI_COMM_WORLD,&m_status);
      int64_t task;
      MPI_Recv(&task,sizeof(int64_t),MPI_CHAR,0,MES_TAG,MPI_COMM_WORLD,&m_status);
#ifdef _INFO
      out <<"Slave:\t" <<rank <<" receive task " <<(int)*buf << endl;
      out <<"Slave:\t" <<rank <<" doing task " <<(int)*task<<"\n";
#endif
      memset(seq,-1,G_SEQLEN * sizeof(int64_t));
      seq[0] = 1; seq[1] = task;
      pthread_mutex_unlock(&g_mutex);
      pBide->run(seq);
    }
  }
}
