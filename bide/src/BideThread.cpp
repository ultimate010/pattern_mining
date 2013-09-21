#include "BideThread.h"



BideThread::BideThread(int id,string logPath)
{
  ThreadId = id;
  this->m_pOut = new ofstream(logPath.c_str());
#ifdef _ERROR
  if(this->m_pOut->fail()){
    cerr <<"Can not open " <<logPath <<" for write\n";
  }
#endif
  memset(this->m_seq,-1,G_SEQLEN); //清空
  this->m_nCountSeq = 0;
}


BideThread::~BideThread(void)
{
  if(m_pOut != NULL){
    delete m_pOut;
  }
  for(int64_t i = 0;i < m_nCountRows;i++){
    delete[] m_pWordProject[i];
  }
  delete[] m_pWordProject;
  for(int64_t i = 0;i < m_nCountDifItem;i++){
    delete m_pWordProject[i];
  }
  delete[] m_pWordProject;
}
/*
 * 设置好m_seq后开始计算
 */
void BideThread::runFromItem()
{
  const set<int64_t>  projectData(*(this->m_pWordProject[m_seq[1]])); //所有投影出现
  if(!backScan(this->m_seq,projectData)){
    bool bei = backExtensionCheck(this->m_seq,projectData);
    bide(projectData,this->m_seq,bei,(double)-1.0f);
  }
}

bool BideThread::GetNext()
{
  /*bug
  */
}

/*
 * 判断是否可以向前生长
 * 通过检测返回true
 */
bool BideThread::forwardExtensionCheck(map<int64_t,int64_t> & myset,const int64_t & support){
  for(map<int64_t,int64_t>::const_iterator iter_set = myset.begin();
        iter_set != myset.end();iter_set++){
    if(iter_set->second == support){
      return false;
    }
  }
  return true;
}
bool BideThread::bide(const set<int64_t> & parmProDatabase,int64_t * seq,
      const bool & backCheck,const double & lr)
{
  int64_t currentSupport  = parmProDatabase.size();
  map<int64_t,int64_t> myset;
  locallyFreQuentItems(parmProDatabase,seq,myset);
  bool forwardExtension = forwardExtensionCheck(myset,currentSupport);
  if(forwardExtension == true && backCheck == false && lr > this->m_like){
    this->m_nCountSeq++;
    coutData(lr);
  }
  for(map<int64_t,int64_t>::const_iterator iter = myset.begin();
        iter != myset.end();iter++){
    seq[0]++; seq[seq[0]] = iter->first;
    set<int64_t> projectData;
    cmpProjectDatabase(parmProDatabase,seq,projectData);
    int64_t c2 = this->m_pWordProject[seq[seq[0]]]->size();
    int64_t c12 = projectData.size();
    double lrr = likelyHood(currentSupport,(double)c2,(double)c12,this->m_nCountRows);
    if(lrr < this->m_like){
    }else {
      if(!backScan(seq,projectData)){
        bool bei = backExtensionCheck(seq,projectData);
        bide(projectData,seq,bei,lrr);
      }
    }
    seq[seq[0]] = -1; seq[0]--; //回退
  }
  return true;
}

void  BideThread::locallyFreQuentItems(const set<int64_t> & pData,const int64_t * seq,map<int64_t,int64_t> & myset)
{
  set<int64_t> set_sentence; //每个句子中包含的单词的集合
  for(set<int64_t>::const_iterator iter = pData.begin();
        iter != pData.end();iter++){
    unsigned int pos = firstInstanceOfSeq(this->m_pDatabase[*iter],seq[0],seq);
    for(pos++;pos <= this->m_pDatabase[*iter][0];pos++){
      set_sentence.insert((this->m_pDatabase[*iter][pos]));
    }
    for(set<int64_t>::iterator iter_sentence = set_sentence.begin();
          iter_sentence!=set_sentence.end();iter_sentence++){
      map<int64_t,int64_t>::iterator iter = myset.find(*iter_sentence);
      if (iter!=myset.end()){
        (iter->second)++;
      }else{
        myset.insert(make_pair(*iter_sentence,1));
      }
    }
    set_sentence.clear();
  }
#ifdef _DEBUG
  cout <<"未处理前的项目集:" <<endl;
  for (map<int64_t,int64_t>::iterator iter = myset.begin();
        iter != myset.end();iter++){
    cout <<iter->first <<" ";
  }
  cout <<endl;
#endif
  for(map<int64_t,int64_t>::iterator iter = myset.begin();
        iter != myset.end();){
    if (iter->second < this->m_minSup){
      myset.erase(iter);
      iter = myset.begin();
    }else{
      iter++;
    }
  }
#ifdef _DEBUG
  cout <<"满足支持度的项目集合为：" <<endl;
  for (map<int64_t,int64_t>::iterator iter = myset.begin();
        iter != myset.end();iter++){
    cout <<iter->first <<" ";
  }
  cout <<endl;
#endif
}

void BideThread::cmpProjectDatabase(const set<int64_t> & preojectDatabase,const int64_t * seq,set<int64_t> & projectData){
  for(set<int64_t>::const_iterator iter = preojectDatabase.begin();
        iter != preojectDatabase.end();iter++){
    if(-1 != firstInstanceOfSeq(this->m_pDatabase[*iter],seq[0],seq)){
      projectData.insert(*iter);
    }
  }
}

int64_t BideThread::lastInLastInstanceOfSeq(const int64_t * array,const unsigned int &ith,const int64_t * seq)const{
  if(ith == seq[0]){
    return lastInstanceOfSq(array,ith,seq);
  }else{
    int64_t end = lastInLastInstanceOfSeq(array,ith + 1,seq);
    for(;end >= 1; end--){
      if(array[end] == seq[ith]){
        return end;
      }
    }
  }
#ifdef _DEBUG
  cerr <<"Error 3#: in the lastInLastInstanceOfSeq";
#endif
  return -1;
}
int64_t BideThread::lastInFirstInstanceOfSeq(const int64_t * array,const unsigned int &ith,const int64_t * seq)const{
  if(ith == seq[0]){
    return firstInstanceOfSeq(array,ith,seq);
  }else{
    int64_t end = lastInFirstInstanceOfSeq(array,ith + 1,seq);
    for(;end >= 1; end--){
      if(array[end] == seq[ith]){
        return end;
      }
    }
  }
#ifdef _DEBUG
  cerr <<"Error 2#: int he lastInFirstInstanceOfSeq";
#endif
  return -1;
}

double BideThread::likelyHood(const double &c1,const double & c2, const double &c12,const double &N){
  double ret = ((long)c2 == (long)c12) ? 2*c12*log((c12*N)/(c1*c2)) : 2*(c12*log((c12*N)/(c1*c2)) + (c2-c12)*log(((c2-c12)*N)/((N-c1)*c2)));
  if (c12 < c1*c2/N){
    return -ret;
  }else{
    return ret;
  }
}

bool BideThread::backScan(const int64_t * seq,const set<int64_t> &pData)const{
  for(unsigned int i = 1;i <= seq[0];i++){
    if (i_thSemiMaxPeriods(seq,pData,i) == true){
      return true;
    }
  }
  return false;
}
/*
 * 当有一个不满足就返回true
 */
bool BideThread::backExtensionCheck(const int64_t * seq,const set<int64_t> &pData)const{
  for(unsigned int i = 1;i <= seq[0];i++){
    if(i_thMaxPeriods(seq,pData,i) == true){
      return true;
    }
  }
  return false;
}

//seq[0]就是序列的长度，pData.size()就是序列的支持度
//利用scanskip优化技术，即是如果投影数据库中有一句
//没有满足backsacan，就永远不可能满足,这一点可以根据每句中的the i-th semi-maximum
// period of a prefix sequence 来看，如果一句中不存在这个，即是为空，那么不用查找了
//直接返回false
bool BideThread::i_thSemiMaxPeriods(const int64_t * seq,const set<int64_t> &pData ,const unsigned int &ith)const{
  bool firstRun = true;
  int64_t orgSeq[G_SEQLEN]; memset(orgSeq,-1,G_SEQLEN * sizeof(int64_t));
  int64_t interSeq[G_SEQLEN]; memset(interSeq,-1,G_SEQLEN * sizeof(int64_t));
  for (set<int64_t>::const_iterator iter = pData.begin();
        iter != pData.end();iter++){
    unsigned int begin;
    if (ith == 1){
      begin = 0;
    }else{
      begin = firstInstanceOfSeq(this->m_pDatabase[*iter],ith - 1,seq); //pData中的位置
    }
    int lastInFirst = lastInFirstInstanceOfSeq(this->m_pDatabase[*iter],ith,seq);
    unsigned int pSeq = 0;
    unsigned int pInterSeq = 0;
    if(firstRun){
      for(unsigned int pos = begin + 1;pos < lastInFirst;pos++){
        orgSeq[pSeq++] = this->m_pDatabase[*iter][pos];
      }
      if(pSeq == 0) return false; //空集
      firstRun = false;
    }else{
      memset(interSeq,-1,G_SEQLEN * sizeof(int64_t));
      for(unsigned int pos = begin + 1;pos < lastInFirst;pos++){
        for(pSeq = 0;pSeq < G_SEQLEN;pSeq++){
          if(orgSeq[pSeq] == this->m_pDatabase[*iter][pos]){
            break;
          }
        }
        if(pSeq == G_SEQLEN){
        }else{
          //find
          interSeq[pInterSeq++] = orgSeq[pSeq];
        }
      }
      if(pInterSeq == 0) return false;
      memcpy(orgSeq,interSeq,G_SEQLEN * sizeof(unsigned long));
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
bool BideThread::i_thMaxPeriods(const int64_t * seq,const set<int64_t> & pData,const unsigned int &ith)const{
  bool firstRun = true;
  int64_t orgSeq[G_SEQLEN]; memset(orgSeq,-1,G_SEQLEN * sizeof(int64_t));
  int64_t interSeq[G_SEQLEN]; memset(interSeq,-1,G_SEQLEN * sizeof(int64_t));
  for(set<long>::const_iterator iter = pData.begin();
        iter != pData.end();iter++){
    unsigned int begin;
    if (ith == 1){
      begin = 0;
    }else{
      begin = firstInstanceOfSeq(this->m_pDatabase[*iter],ith - 1,seq);
    }
    unsigned int end = lastInLastInstanceOfSeq(this->m_pDatabase[*iter],ith,seq);
    unsigned int pSeq = 0;
    unsigned int pInterSeq = 0;
    if(firstRun){
      for(unsigned int pos = begin + 1;pos < end;pos++){
        orgSeq[pSeq++] = this->m_pDatabase[*iter][pos];
      }
      if(pSeq == 0) return false; //空集
      firstRun = false;
    }else{
      memset(interSeq,-1,G_SEQLEN * sizeof(int64_t));
      for(unsigned int pos = begin + 1;pos < end; pos){
        for(pSeq = 0;pSeq < G_SEQLEN;pSeq++){
          if(orgSeq[pSeq] == this->m_pDatabase[*iter][pos]){
            break;
          }
        }
        if(pSeq == G_SEQLEN){
        }else{
          interSeq[pInterSeq++] = orgSeq[pSeq];
        }
      }
      if(pInterSeq == 0) return false;
      memcpy(orgSeq,interSeq,G_SEQLEN * sizeof(int64_t));
    }
  }
  return true;
}

int64_t BideThread::firstInstanceOfSeq(const int64_t * array,const unsigned int &ith,const int64_t * seq)const{
#ifdef _DEBUG
  //数组的第一个元素表示该数组的长度 //参数检测
  if(ith < 1 || array == NULL || seq == NULL || seq[0] == 0){
    cout <<"传入的参数错误，位置求first instance of a seq!" <<endl;
    return -1;
  }
  if(ith > seq[0]){
    cout <<"参数错误，i-th超出序列的范围!" <<endl;
    return -1;
  }
#endif
  unsigned int pos = 1;
  for(unsigned int i = 1;i <= array[0];i++){
    if(array[i] == seq[pos]){
      if(pos == ith){
        return i;
      }else{
        ++pos;
      }
    }
  }
#ifdef _DEBUG
  cerr <<"Warring 1#: Can not find seq in the array\n";
#endif
  return -1;
}

int64_t BideThread::lastInstanceOfSq(const int64_t * array,const unsigned int &ith,const int64_t * seq) const{
#ifdef _DEBUG
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

/*
 * 第0号位置存放长度
 */
void BideThread::coutData(const double & lr){
  int64_t * tempP = &(m_seq[1]);
  *m_pOut << lr <<"\t";
  while(*tempP != -1){
    *m_pOut << *tempP++ << "\t";
  }
  *m_pOut << endl;
}

