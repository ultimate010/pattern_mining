#include "DataShared.h"
#include "BideThread.h"
#include <iostream>
#include <fstream>

using namespace std;

void iniBideThread(DataShared * data,BideThread * bide);
void setItem(int64_t item,BideThread * bide);


int main(int argvs,char ** argv){
  //DataShared * pData = new DataShared(argv[0]);
  DataShared * pData = new DataShared("../run.conf");
  BideThread bide(0,"node0.log");
  iniBideThread(pData,&bide);
  int64_t count = 0;
  while(pData->hasNext()){
    int64_t next = pData->getNextId();
    if(pData->m_ZhSet.find(next) != pData->m_ZhSet.end()){
      cout <<"Task:" <<++count <<" of " <<pData->m_taskCount <<endl;
      setItem(next,&bide);
      bide.runFromItem();
    }
  }
  delete pData;
  return 0;
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

void setItem(int64_t item,BideThread * bide){
  bide->m_seq[0] = 1;
  bide->m_seq[1] = item;
}
