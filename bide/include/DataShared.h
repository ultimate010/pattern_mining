#ifndef _DATASHARED_H_
#define _DATASHARED_H_
#include <stdlib.h>
#include <fstream>
#include <string>
#include <map>

using namespace std;
class DataShared
{
  /*protected func*/
  protected:
    bool LoadData();
    void loadConf(string path,map<string,string> & confMap);
    bool SetParameter(map<string,string> & mapConf);
    bool SetMinSup(const double &sup);
    bool SetLikelyHood(const double &x);
    void Freequent1Seq() ;

    /*public func*/
  public:
    DataShared(void);
    ~DataShared(void);
    /*public data*/
  public:
    int64_t ** m_pDatabase;//指向数据库
    vector<int64_t> ** m_pWordProject; //item索引,记录每个item出现的序列位置
    int64_t m_nCountItem;//句子中出现总item个数
    int64_t m_nCountDifItem;//不同item个数
    int64_t m_nCountRows;//item总个数
    ofstream * m_pOut;//输出文件指针
    double m_like;//相关性
    double m_minSup;//最小支持度
    vector<int64_t> *m_freq1Item;//记录频繁一项目集
    vector<int64_t>::iterator m_iter;//记录处理的序列的位置
  private:
    string m_inputFile; //输入文件路径
    string m_outFile;  //输出文件路径
    map<string,string> m_confMap; //配置参数
};
#endif
