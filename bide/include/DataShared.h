#pragma once
#include <stdlib.h>
#include <fstream>
#include <string>
#include <map>

using namespace std;
class DataShared
{
  protected:
    void loadConf(string path,map<string,string> & confMap);

  public:
    bool SetParameter(map<string,string> & mapConf);
    bool LoadData();
    bool SetMinSup(const double &sup);
    bool SetLikelyHood(const double &x);
    void Freequent1Seq() ;
    DataShared(void);
    ~DataShared(void);
    int64_t ** m_pDatabase;//指向数据库
    vector<int64_t> ** m_pWordProject; //单词索引
    int64_t m_nCountItem;//句子中出现总单词个数
    int64_t m_nCountDifItem;//不同单词个数
    double m_minSup;//最小支持度
    int64_t m_nCountRows;//数据库中句子总行数
    ofstream * m_pOut,*m_pOut1,*m_pOutParameter;//用于输出
    string* m_Index;//单词索引，下标表示单词的映射
    double m_like;//相关性
    int64_t * m_pWordCout;//记录每个单词出现的总次数
    int64_t *m_pZh;//记录中文的映射
    int64_t *m_pEn;//记录英文的映射
    vector<int64_t>::iterator m_iter;//记录处理的序列的位置
    vector<int64_t> * m_freq1Item;//记录所有要处理的项目
    int64_t m_outPutZh;//记录输出的中文单词数
  private:
    string m_inputFile; //输入文件
    string m_outFile;  //输出文件
    map<string,string> m_confMap; //配置参数
};

