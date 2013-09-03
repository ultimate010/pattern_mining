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
  protected:
    void loadConf(string path,map<string,string> & confMap);

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
    //指向数据库
    long ** m_pDatabase ;
    //单词索引
    vector<long> ** m_pWordProject;
    //句子中出现总单词个数
    long m_nCountItem;
    //不同单词个数
    long m_nCountDifItem;
    //中文单词数目
    long m_nCountZh;
    //英文单词数目
    long m_nCountEn;
    //最小支持度
    long m_minSup;
    //数据库中句子总行数
    long m_nCountRows;
    //用于输出
    ofstream * m_pOut,*m_pOut1,*m_pOutParameter;
    //单词索引，下标表示单词的映射
    string* m_Index;
    //相关性
    double m_like;
    //记录每个单词出现的总次数
    long * m_pWordCout;
    //记录中文的映射
    long *m_pZh;
    //记录英文的映射
    long *m_pEn;
    //记录处理的序列的位置
    vector<long>::iterator m_iter;
    //记录所有要处理的项目
    vector<long> * m_freq1Item;
    //记录输出的中文单词数
    long m_outPutZh;
};

