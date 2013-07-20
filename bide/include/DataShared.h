#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#define  outFile "shuchu.txt"
#define  outFile1 "shuchu1.txt"
#define  outFileParameter "canshu.txt"
#define  rawFile "raw.txt"
#define  tempFile "tempFile.txt"
#define  debugFile "debug.txt"
#define  _MyDebug 0

using namespace std;
class DataShared
{
public:
	bool SetParameter();
	bool ChuliShuju(const int & min_sup);
	bool LoadData();
	bool SetMinSup(const long &sup);
	bool SetLikelyHood(const double &x);
	long a2long(const string &s)const ;
	void Freequent1Seq() ;
	DataShared(void);
	~DataShared(void);
	//ָ�����ݿ�
	long ** m_pDatabase ;
	//��������
	vector<long> ** m_pWordProject;
	//�����г����ܵ��ʸ���
	long m_nCountItem;
	//��ͬ���ʸ���
	long m_nCountDifItem;
	//���ĵ�����Ŀ
	long m_nCountZh;
	//Ӣ�ĵ�����Ŀ
	long m_nCountEn;
	//��С֧�ֶ�
	long m_minSup;
	//���ݿ��о���������
	long m_nCountRows;
	//�������
	ofstream * m_pOut,*m_pOut1,*m_pOutParameter;
	//�����������±��ʾ���ʵ�ӳ��
	string* m_Index;
	//�����
	double m_like;
	//��¼ÿ�����ʳ��ֵ��ܴ���
	long * m_pWordCout;
	//��¼���ĵ�ӳ��
	long *m_pZh;
	//��¼Ӣ�ĵ�ӳ��
	long *m_pEn;
	//��¼��������е�λ��
	vector<long>::iterator m_iter;
	//��¼����Ҫ�������Ŀ
	vector<long> * m_freq1Item;
	//��¼��������ĵ�����
	long m_outPutZh;
};

