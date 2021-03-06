#ifndef _BIDE_H_
#define _BIDE_H_
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <pthread.h>
#define  G_SEQLEN 512
typedef long int int64_t;

using namespace std;
class BideThread
{
  public:
    int64_t ** m_pDatabase;//指向数据库
    set<int64_t> ** m_pWordProject; //item索引,记录每个item出现的序列位置
    int64_t m_nCountRows;
    int64_t m_nCountDifItem;
    double m_like;
    int64_t m_minSup;
    int64_t m_seq[G_SEQLEN]; //多线程版本废弃
    map<int64_t,double> m_mapIdf;
  private:
    int64_t m_nCountSeq; //输出多少序列
    int ThreadId;//线程id
    ofstream * m_pOut;
#ifdef _THREAD
  pthread_mutex_t m_mutex;
#endif
  public:
    bool bide(const set<int64_t> & pProDatabase,int64_t *seq,const bool & backExtensionCheck,const double &lr);
    void locallyFreQuentItems(const set<int64_t> & pData,const int64_t * seq,map<int64_t,int64_t> & set);
    void cmpProjectDatabase(const set<int64_t> & preojectDatabase,const int64_t * seq,set<int64_t> & projectData);
    static double likelyHood(const double &c1,const double & c2, const double &c12,const  double &N);
    bool backScan(const int64_t * seq,const set<int64_t> &pData)const;
    bool backExtensionCheck(const int64_t * seq,const set<int64_t> &pData)const;
    bool forwardExtensionCheck(map<int64_t,int64_t> & set,const int64_t & support);
    bool i_thSemiMaxPeriods(const int64_t * seq,const set<int64_t> &pData,const int64_t &ith)const;
    bool i_thMaxPeriods(const int64_t * seq,const set<int64_t> & pData,const int64_t &ith) const;
    int64_t firstInstanceOfSeq(const int64_t * array,const int64_t &ith,const int64_t * seq)const;
    int64_t lastInFirstInstanceOfSeq(const int64_t * array,const int64_t &ith,const int64_t * seq)const;
    int64_t lastInLastInstanceOfSeq(const int64_t * array,const int64_t &ith,const int64_t * seq)const;
    int64_t lastInstanceOfSq(const int64_t * array,const int64_t &ith,const int64_t * seq)const;
    void coutData(const int64_t * seq,const double & lr,const int64_t support);
    BideThread(int id,string logPath);
    ~BideThread(void);
    void runFromItem();
#ifdef _THREAD
    void run(int64_t * seq);
#endif
};
#endif
