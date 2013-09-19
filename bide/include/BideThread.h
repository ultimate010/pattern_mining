#ifndef _BIDE_H_
#define _BIDE_H_
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#define  G_SEQLEN 100
using namespace std;
class BideThread
{
  public:
    int64_t ** m_pDatabase;//指向数据库
    set<int64_t> ** m_pWordProject; //item索引,记录每个item出现的序列位置
    int64_t m_nCountRows;
    int64_t m_nCountDifItem;
    double m_like;
  private:
    int64_t m_nCountSeq; //输出多少序列
    int ThreadId;//线程id
    int64_t m_seq[G_SEQLEN+1];
    ofstream * m_pOut;
  public:
    bool Bide(const set<int64_t> & pProDatabase,const int64_t *seq,const bool & backExtensionCheck,const double &lr);
    map<int64_t,int64_t> LocallyFreQuentItems(const set<int64_t> &pData,const int64_t * seq)const;

    vector<long>  ProjectDatabase( const set<long> &preojectDatabase,const long *seq)const;
    static double likelyHood(const double &c1,const double & c2, const double &c12,const  double &N);
    bool BackScan(const int64_t * seq,const set<int64_t> &pData)const;
    bool BackExtensionCheck(const int64_t * seq,const set<int64_t> &pData)const;
    bool i_ThSemiMaxPeriods(const int64_t * seq,const set<int64_t> &pData,const int &ith)const;
    bool i_ThMaxPeriods(const int64_t * seq,const  set<int64_t> & pData,const int &ith) const;
    int64_t fistInstanceOfSq(const int64_t * array,const int64_t &ith,const int64_t *seq)const;
    long lastInstanceOfSq(const int64_t * array,const int64_t &ith,const int64_t * seq)const;
    void coutData(const double & lr);
    BideThread(int id,string logPath);
    ~BideThread(void);
    void runFromItem();
    bool GetNext();
};
#endif
