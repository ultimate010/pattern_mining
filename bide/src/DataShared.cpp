#include "DataShared.h"

void DataShared::loadConf(string path,map<string,string> & confMap){
  cout <<"参数 :" <<endl;
  ifstream file(path.c_str());
  string str;
  while(getline(file,str)){
    confMap.insert(pair<string,string>(str.substr(0,str.find('=')),str.substr(str.find('=')+1)));
  }
  map<string,string>::iterator iter;
  for(iter = confMap.begin();iter != confMap.end();iter++){
    cout <<iter->first <<"\t" << iter->second <<endl;
  }
}

DataShared::DataShared(void)
{
  loadConf("../run.conf",m_confMap);
  SetParameter(m_confMap);
  LoadData();
  m_pOut = new ofstream(outFile);
  m_pOut1 = new ofstream(outFile1);
  m_pOutParameter = new ofstream(outFileParameter);
  if (m_pOut==NULL||m_pOut1==NULL){
#ifdef _ERROR
    cout<<"打开输出文件错误" << endl;
#endif
    return;
  }
  Freequent1Seq();
  *m_pOutParameter <<"lr: " <<m_like <<endl;
  *m_pOutParameter <<"最小支持度: " <<m_like <<endl;
  *m_pOutParameter <<"总单词数: " <<m_nCountItem <<endl;
  *m_pOutParameter <<"不同单词数: " <<m_nCountDifItem <<endl;
  *m_pOutParameter <<"英文单词数: " <<m_nCountEn <<endl;
  *m_pOutParameter <<"中文单词数: " <<m_nCountZh <<endl;
  *m_pOutParameter <<"总行数: " <<m_nCountRows <<endl;
  m_iter = m_freq1Item->begin();
  m_outPutZh = 0;
}

//////////////////////////////////////////////////////////////////////////
// 返回频繁1序列
//////////////////////////////////////////////////////////////////////////
void DataShared::Freequent1Seq()  {
  m_freq1Item = new vector<long>;
  for(long i = 0;i<m_nCountEn;i++){
    if((m_pWordProject[m_pEn[i]])->size() >= m_minSup){
      m_freq1Item->push_back(m_pEn[i]);
    }
  }
}



DataShared::~DataShared(void)
{
  //释放资源
  delete m_pDatabase[];
  delete m_pEn;
  delete m_pZh;
  delete m_pWordCout;
  delete m_pWordProject;
  delete m_Index;
}

bool DataShared::SetParameter(map<string,string> & mapConf)
{
  string str = "minisup";
  map<string,string>::iterator iter = mapConf.find(str);
  str = iter->second;
  SetMinSup(atof(str));
  str = "lr";
  iter = mapConf.find(str);
  str = iter->second;
  SetLikelyHood(atof(str));
  return true;
}



//////////////////////////////////////////////////////////////////////////
// 读取数据到内存中，并且初始话pDatabase指针指向数据库
// 数据库的格式为：
// 第i行为：转换后的句子
// 第i+1行为：上一行句子一共有多少个单词
// pDatabase中数据存储格式为：
// 每行：本行的单词数 依次每个单词
// 初始m_pWordProject索引，格式为：
// 第i行的数据存储单词i所有出现的行数
//////////////////////////////////////////////////////////////////////////
bool DataShared::LoadData()
{
  string str = "inputfile";
  map<string,string>::iterator iter = m_confMap.find(str);
  str = iter->second;
  ifstream in(str->c_str());
  if (in == NULL){
#ifdef _ERROR
    cout <<"输入文件未找到!" <<endl;
#endif
    return false;
  }
  string str_firstInput,str_secondInput;
  int pos;
  //初始化单词索引
  m_pDatabase = new int64_t *[m_nCountRows];
  m_pWordCout = new int64_t[m_nCountDifItem];
  m_pWordProject = new vector<int64_t> *[m_nCountDifItem];
  m_pEn = new int64_t[m_nCountEn];
  m_pZh = new int64_t[m_nCountZh];
  map<string,bool> setEn,setZh;
  long countEn = 0;
  long countZh = 0;
  for (long myi = 0 ; myi<m_nCountDifItem;myi++){
    m_pWordProject[myi] = new vector<long>;
    m_pWordCout[myi] = 0;
  }
  //进入数据读取
  long nCountWord = 0;  //存储每一句中的单词数目
  long nRow = 0; //存储当前第几行
  while(getline(in,str_firstInput)){ //str_firstInput中存放一句  str_secondInput中存放这句总单词数目
    getline(in,str_secondInput); nCountWord = a2long(str_secondInput.data());
    m_pDatabase[nRow] = new long[nCountWord+1]; //初始化
    long * pWriter = m_pDatabase[nRow]; //指向pDatabase中的第n行数据
    *pWriter = nCountWord; //写入本行总单词
    pWriter++;
    while(1){
      pos = str_firstInput.find(' ');
      if(pos == -1) break; //当前行已经处理完了
      str_secondInput = str_firstInput.substr(0,pos);  str_firstInput.erase(0,pos+1);
      *pWriter = a2long(str_secondInput.data());
      m_pWordCout[*pWriter]++;
      //判断当前映射的中英，加入相应的映射数组
      string temp = m_Index[*pWriter];
      char c = temp.data()[0];
      if((c<='z'&&c>='a')||(c<='Z'&&c>='A')){
        //英文
        //先判断set中是否记录，也就是是否第一次出现
        if(setEn.find(temp)==setEn.end()){
          m_pEn[countEn++]= *pWriter;
          setEn.insert(make_pair(temp,false));
        }
      }else{

        if(setZh.find(temp)==setZh.end()){
          m_pZh[countZh++]= *pWriter;
          setZh.insert(make_pair(temp,false));
        }
      }
      //写入索引,注意，一句话中出现两个同同样的单词，只写入一次行数
      vector<long> *pTemp = m_pWordProject[*pWriter];
      if (pTemp->size()==0){ //如果第一次插入，那么直接插入
        pTemp->push_back(nRow);
      }else{
        if (pTemp->back()!=nRow){ 	//如果已经记录当前行，那么不再记录
          pTemp->push_back(nRow);
        }
      }
      pWriter++;
    }
    //进入下一行
    nRow++;
  }
  return true;
}

bool DataShared::SetMinSup( const double &sup )
{
  this->m_minSup = sup;
  return true;
}

bool DataShared::SetLikelyHood( const double &x )
{
  this->m_like = x;
  return true;
}
