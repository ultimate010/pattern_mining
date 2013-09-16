#ifndef _BIDE_H_
#define _BIDE_H_
#include <vector>
#include <map>
#include "DataShared.h"
#include <math.h>
#define  seqLength 100
using namespace ZThread;
using namespace std;
class BideThread :
	public ZThread::Runnable
{
private:
	CountedPtr<DataShared> ptrData;
	Mutex lock;
	//记录第几个线程
	int ThreadId;
	//序列
	long m_seq[seqLength+1];
public:
	bool Bide(  vector<long>   * pProDatabase,const long *seq,const bool & backExtensionCheck,const double &lr,const long c1);
	map<long,long> LocallyFreQuentItems(const vector<long> &pData,const long *seq)const ;
	vector<long>  ProjectDatabase( const vector<long> &preojectDatabase,const long *seq ) const;
	static double likelyHood(const double &c1,const  double & c2, const double &c12,const  double &N) ;
	bool BackScan(const long * seq,const vector<long> &pData ) const ;
	bool BackExtensionCheck(const long *seq,const vector<long> &pData ) const ;
	bool i_ThSemiMaxPeriods(const long * seq,const vector<long> &pData ,const int &ith) const ;
	bool i_ThMaxPeriods(const long *seq,const  vector<long> & pData ,const long &ith ) const;
	long fistInstanceOfSq(const long * array,const long &ith,const long *seq )const;
	long lastInstanceOfSq(const long * array,const long &ith,const long * seq )const;
	BideThread(int id,const CountedPtr<DataShared> & pdata);
	~BideThread(void);
	void run();
	bool GetNext();


};
#endif
