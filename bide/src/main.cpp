#include "DataShared.h"
#include <iostream>
#include <fstream>

using namespace std;


int main(int argvs,char ** argv){
  //DataShared * pData = new DataShared(argv[0]);
  DataShared * pData = new DataShared("../run.conf");
  while(pData->hasNext()){
    int64_t next = pData->getNextId();
    if(pData->m_ZhSet.find(next) != pData->m_ZhSet.end()){
      cout << next;
    }
  }
}
