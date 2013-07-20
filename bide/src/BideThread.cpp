#include "BideThread.h"


BideThread::BideThread(int id,const CountedPtr<DataShared> & pdata)
{
  ThreadId = id;
  ptrData = pdata;
}


BideThread::~BideThread(void)
{
}

void BideThread::run()
{
  //从data中取出序列用于扩展
  while(GetNext()){
    vector<long>  projectData(*(ptrData->m_pWordProject[m_seq[1]]));
    if (!BackScan(seq,projectData)){
      bool bei = BackExtensionCheck(newSeq,projectDatabase);
      Bide(projectData,m_seq,bei,0);
    }
  }
  *(ptrData->m_pOutParameter) <<"输出了:" << ptrData->m_outPutZh <<"个单词"<<endl;
}

bool BideThread::GetNext()
{
  lock.acquire();
  if(ptrData->m_iter == ptrData->m_freq1Item->end()){
    //处理完了
    cout <<"线程： "<<ThreadId <<"处理完成!"<<endl;
    lock.release();
    return false;
  }else{
    //存储序列，第0号位置存储本序列长度,最长为seqLength
    m_seq[1] = *(ptrData->m_iter);
    (ptrData->m_iter)++;
    m_seq[0] = 1;
    cout <<"线程:" <<ThreadId <<": 初始序列:" <<ptrData->m_Index[m_seq[1]] <<endl;
    lock.release();
    return true;
  }
}

bool forwardExtensionCheck(map<long,long & set,const unsigned int & support){
  for (map<long,long>::const_iterator iter_set = set.begin();
        iter_set != set.end();iter_set++){
    if (iter_set->second == currentSupport){
      return false;
    }
  }
  return true;
}
bool BideThread::Bide(vector<long> & pProDatabase,const long * seq,
      const bool & backExtensionCheck,const double & lr)
{
  long currentSupport  = pProDatabase.size();
  map<long,long> set;
  LocallyFreQuentItems(pProDatabase,seq,set);
  bool forwardExtension = forwardExtensionCheck(set,currentSupport);
  if(forwardExtension == true && backExtensionCheck == false && lr > ptrData->m_like){
    lock.acquire();
    ptrData->m_outPutZh += 1;
    if (seq[0] > 1){
      string tempFirst = ptrData->m_Index[seq[1]];
      string tempEnd = ptrData->m_Index[seq[seq[0]]];
      *(ptrData->m_pOut) << lr <<": ";
      for(int i = 1;i<=seq[0];i++){
        *(ptrData->m_pOut) <<ptrData->m_Index[seq[i]] <<" ";
        *(ptrData->m_pOut1) <<ptrData->m_Index[seq[i]] <<" ";
      }
      *(ptrData->m_pOut) <<currentSupport ;
      *(ptrData->m_pOut) <<endl;
      *(ptrData->m_pOut1) <<endl;
    }
    lock.release();
  }
  for(map<long,long>::const_iterator iter_set = set.begin();
        iter_set != set.end();iter_set++){
    seq[0] = seq[0]+1; seq[seq[0]] = iter_set->first;
    vector<long> projectData;
    ProjectDatabase(pProDatabase,seq,projectData);
    long c2 = ptrData->m_pWordProject[seq[seq[0]]]->size();
    long c12 = projectData.size();
    double lrr = likelyHood(currentSupport,c2,c12,ptrData->m_nCountRows);
    if(lrr<=ptrData->m_like){
      //新的序列不满足lr
    }else {
      bool bbb = BackScan(newSeq,projectDatabase);
      double xxx = (float)projectDatabase.size()/(float)(ptrData->m_pWordProject[newSeq[1]])->size();
      if (!bbb)
      {
        bool bei = BackExtensionCheck(newSeq,projectDatabase);
        Bide(projectDatabase,newSeq,bei,lrr,c12);
      }
    }
  }
  return true;
}

void  BideThread::LocallyFreQuentItems(const vector<long> & pData,const long * seq,map<long,long> & set)
{
  map<long,bool> set_sentence; //每个句子中包含的单词的集合
  for (vector<long>::const_iterator iter = pData.begin();
        iter != pData.end();iter++){
    unsigned int pos = firstInstanceOfSeq(ptrData->m_pDatabase[*iter],seq[0],seq);
    for (pos++;pos <= ptrData->m_pDatabase[*iter][0];pos++){
      set_sentence.insert(make_pair(ptrData->m_pDatabase[*iter][pos],true));
    }
    for (map<long,bool>::iterator iter_sentence = set_sentence.begin();
          iter_sentence!=set_sentence.end();iter_sentence++){
      map<long,long>::iterator iter = set.find(iter_sentence->first);
      if (iter!=set.end()){
        (iter->second)++;
      }else{
        set.insert(make_pair(iter_sentence->first,1));
      }
    }
    set_sentence.clear();
  }
#if _MyDebug
  cout <<"未处理前的项目集:" <<endl;
  for (map<long,long>::iterator i_set1 = set.begin();
        i_set1!=set.end();i_set1++){
    cout <<i_set1->first <<" ";
  }
  cout <<endl;
#endif
  for(map<long,long>::iterator i_set = set.begin();
        i_set!=set.end();){
    if (i_set->second<ptrData->m_minSup)
    {
      i_set = set.erase(i_set);
    }else{
      i_set++;
    }
  }
#if _MyDebug
  cout <<"满足支持度的项目集合为：" <<endl;
  for (map<long,long>::iterator i_set12 = set.begin();
        i_set12!=set.end();i_set12++){
    cout <<i_set12->first <<" ";
  }
  cout <<endl;
#endif
}

void BideThread::ProjectDatabase(const vector<long> & preojectDatabase,const long * seq,vector<long> & projectData) const
{
  for(vector<long>::const_iterator iter = preojectDatabase.begin();
        iter != preojectDatabase.end();iter++){
    if(-1 != firstInstanceOfSeq(ptrData->m_pDatabase[*iter],seq[0],seq)){
      projectData.push_back(*iter);
    }
  }
}


double BideThread::likelyHood( const double &c1,const double & c2, const double &c12,const double &N )
{
  double ret = ((long)c2 == (long)c12) ? 2*c12*log((c12*N)/(c1*c2)) : 2*(c12*log((c12*N)/(c1*c2)) + (c2-c12)*log(((c2-c12)*N)/((N-c1)*c2)));
  if (c12 < c1*c2/N){
    return -ret;
  }else{
    return ret;
  }
}

bool BideThread::BackScan( const long * seq,const vector<long> &pData ) const
{
  for(unsigned int i = 1;i <= seq[0];i++){
    if (i_ThSemiMaxPeriods(seq,pData,i) == true){
      return true;
    }
  }
  return false;
}

bool BideThread::BackExtensionCheck( const long *seq,const vector<long> &pData ) const
{
  for(unsigned int i = 1;i <= seq[0];i++){
    if(i_ThMaxPeriods(seq,pData,i) == true){
      return true;
    }
  }
  return false;
}

//seq.size();就是序列的长度，pData.size()就是序列的支持度
//利用scanskip优化技术，即是如果投影数据库中有一句
//没有满足backsacan，就永远不可能满足,这一点可以根据每句中的the i-th semi-maximum
// period of a prefix sequence 来看，如果一句中不存在这个，即是为空，那么不用查找了
//直接返回false
bool BideThread::i_ThSemiMaxPeriods(const long * seq,const vector<long> &pData ,const unsigned int &ith) const
{
  bool fisrtRun = true;
  unsigned long orgSeq[seqLength]; memset(orgSeq,-1,seqLength * sizeof(unsigned long));
  unsigned long interSeq[seqLength]; memset(interSeq,-1,seqLength * sizeof(unsigned long));
  for (vector<long>::const_iterator iter = pData.begin();
        iter != pData.end();iter++){
    int firstInstance;
    if (ith == 1){
      firstInstance = 1;
    }else{
      firstInstance = firstInstanceOfSeq(ptrData->m_pDatabase[*iter],ith-1,seq); //pData中的位置
    }
    int lastInFirst = firstInstanceOfSeq(ptrData->m_pDatabase[*iter],ith,seq);
    if(firstRun){
      unsigned int pSeq = 0;
      for(unsigned int pos = firstInstance + 1;pos < lastInFirst; pos){
        while(pSeq < seqLength && orgSeq[pSeq] != -1){
          pSeq++;
        }
        orgSeq[pSeq] = ptrData->m_pDatabase[*iter][pos];
      }
      if(pSeq == 0) return false; //空集
      firstRun = false;
    }else{
      unsigned int pSeq = 0;
      unsigned int pInterSeq = 0;
      memset(interSeq,-1,seqLength * sizeof(unsigned long));
      for(unsigned int pos = firstInstance + 1;pos < lastInFirst; pos){
        for(pSeq = 0;pSeq < seqLength;pSeq++){
          if(orgSeq[pSeq] == ptrData->m_pDatabase[*iter][pos]){
            break;
          }
        }
        if(pSeq == seqLength){
          //not find
        }else{
          //find
          while(pInterSeq < seqLength && instance[pInterSeq] != -1){
            pInterSeq++;
          }
          interSeq[pInterSeq] = orgSeq[pSeq];
        }
      }
      if(pInterSeq == 0) return false;
      memcpy(orgSeq,interSeq,seqLength * sizeof(unsigned long));
    }
  }
  return true;
}
//seq[0]就是序列的长度，pData.size()就是序列的支持度
//利用scanskip优化技术，即是如果投影数据库中有一句没
//有满足back extensioncheck，就永远不可能满足,这一点
//可以根据每句中的the i-th maximum period of a prefix
//sequence 来看，如果一句中不存在这个，即是为空，那么
//不用查找了 直接返回false
bool BideThread::i_ThMaxPeriods(const long *seq,const vector<long> & pData ,const unsigned int &ith) const
{
  bool firstRun = true;
  unsigned long orgSeq[seqLength]; memset(orgSeq,-1,seqLength * sizeof(unsigned long));
  unsigned long interSeq[seqLength]; memset(interSeq,-1,seqLength * sizeof(unsigned long));
  for (vector<long>::const_iterator iter = pData.begin();
        iter != pData.end();iter++){
    //对每一序列计算出此句中第一次出现的位置
    //*iter 表示投影数据库中显示的seq在总数据库中所在的位置
    unsigned int = firstInstance;
    if (ith == 1){
      firstInstance = 1;
    }else{
      firstInstance = firstInstanceOfSeq(ptrData->m_pDatabase[*iter],ith-1,seq);
    }
    unsigned int lastPosOfTheSeq = lastInstanceOfSq(ptrData->m_pDatabase[*iter],ith,seq);
    if(firstRun){
      unsigned int pSeq = 0;
      for(unsigned int pos = firstInstance + 1;pos < lastInstanceOfSq; pos){
        while(pSeq < seqLength && orgSeq[pSeq] != -1){
          pSeq++;
        }
        orgSeq[pSeq] = ptrData->m_pDatabase[*iter][pos];
      }
      if(pSeq == 0) return false; //空集
      firstRun = false;
    }else{
      unsigned int pSeq = 0;
      unsigned int pInterSeq = 0;
      memset(interSeq,-1,seqLength * sizeof(unsigned long));
      for(unsigned int pos = firstInstance + 1;pos < lastInstanceOfSq; pos){
        for(pSeq = 0;pSeq < seqLength;pSeq++){
          if(orgSeq[pSeq] == ptrData->m_pDatabase[*iter][pos]){
            break;
          }
        }
        if(pSeq == seqLength){
          //not find
        }else{
          //find
          while(pInterSeq < seqLength && instance[pInterSeq] != -1){
            pInterSeq++;
          }
          interSeq[pInterSeq] = orgSeq[pSeq];
        }
      }
      if(pInterSeq == 0) return false;
      memcpy(orgSeq,interSeq,seqLength * sizeof(unsigned long));
    }
  }
  return true;
}

long BideThread::firstInstanceOfSeq(const long * array,const long &ith,const long * seq) const
{
#ifdef __DEBUG__
  //数组的第一个元素表示该数组的长度 //参数检测
  if(ith < 1 || array == NULL || seq == NULL || seq[0] == 0){
    cout <<"传入的参数错误，位置求first instance of a seq!" <<endl;
    return -1;
  }
  if(ith >= seq[0]){
    cout <<"参数错误，i-th超出序列的范围!" <<endl;
    return -1;
  }
#endif
  unsigned int fisrt = 1;
  for(unsigned int i = 1;i <= array[0];i++){
    if(array[i] == seq[fisrt])
    {
      if(fisrt == ith)){
        return i;
      }else{
        ++fisrt;
      }
    }
  }
  return -1;
}

long BideThread::lastInstanceOfSq(const long * array,const long &ith,const long * seq) const
{
#ifdef __DEBUG__
  //数组的第一个元素表示该数组的长度 //参数检测
  if(ith < 1 || array == NULL || seq == NULL || seq[0] == 0){
    cout <<"传入的参数错误，位置求last instance of a seq!" <<endl;
    return -1;
  }
  if (ith > seq[0]){
    cout <<"参数错误，i-th超出序列的范围!" <<endl;
    return -1;
  }
#endif
  unsigned int last = seq[0];
  for (unsigned int i = array[0];i >= 1;i--)
  {   //未检测array中是否包含seq
    if(array[i] == seq[last]){
      if(last == ith){
        return last;
      }else{
        last--;
      }
    }
  }
  return -1; //没找到
}


