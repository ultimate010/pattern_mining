#include "DataShared.h"

DataShared::DataShared(string confPath){
  loadConf(confPath,m_confMap);
  SetParameter(m_confMap);
  loadData();
  /*
     m_pOut = new ofstream(m_outFile.c_str());
     if (m_pOut==NULL){
#ifdef _ERROR
cerr <<"Can not open" <<m_outFile <<endl;
#endif
return;
}
*/
  m_iter = m_freq1Item.begin();
  m_taskCount = m_freq1Item.size();
  }
bool DataShared::hasNext(){
  if(m_iter == m_freq1Item.end()){
    return false;
  }
  return true;
}
int64_t DataShared::getNextId(){
  if(hasNext()){
    return *m_iter++;
  }
  return -1;
}

void DataShared::loadConf(string path,map<string,string> & confMap){
#ifdef  _INFO
  cout <<"参数 :" <<endl;
#endif
  ifstream file(path.c_str());
  if(file.fail()){
#ifdef _ERROR
    cerr <<"Can not open " <<path <<endl;
#endif
    return ;
  }
  string str;
  while(getline(file,str)){
    if(str.size() == 0){
      continue;
    }
    confMap.insert(pair<string,string>(str.substr(0,str.find('=')),str.substr(str.find('=')+1)));
  }
#ifdef _INFO
  map<string,string>::iterator iter;
  for(iter = confMap.begin();iter != confMap.end();iter++){
    cout <<iter->first <<"\t" << iter->second <<endl;
  }
#endif
}


DataShared::~DataShared(void){
  //释放资源
  for(int i = 0;i < m_nCountRows;i++){
    delete [] m_pDatabase[i];
  }
  delete [] m_pDatabase;
  for(int i = 0;i < m_nCountDifItem;i++){
    delete m_pWordProject[i];
  }
  delete [] m_pWordProject;
  // delete m_pOut;
}

bool DataShared::SetParameter(map<string,string> & mapConf)
{
  string str = "minisup";
  map<string,string>::iterator iter = mapConf.find(str);
  str = iter->second;
  SetMinSup(atof(str.c_str()));
  str = "lr";
  iter = mapConf.find(str);
  str = iter->second;
  SetLikelyHood(atof(str.c_str()));
  str = "inputfile";
  iter = mapConf.find(str);
  m_inputFile = iter->second;
  str = "outputfile";
  iter = mapConf.find(str);
  m_outFile = iter->second;
  str = "zhfile";
  iter = mapConf.find(str);
  m_zhFile = iter->second;
  return true;
}



//////////////////////////////////////////////////////////////////////////
// 读取数据到内存中，并且初始话pDatabase指针指向数据库
// 数据库的格式为：
// pDatabase中数据存储格式为：
// 每行：本行的单词数 依次每个单词
//////////////////////////////////////////////////////////////////////////
bool DataShared::loadData()
{
  ifstream zhFile(m_zhFile.c_str());
  if (zhFile.fail()){
#ifdef _ERROR
    cerr <<"Can not open" <<m_zhFile <<endl;
#endif
    return false;
  }
  string lineStr;
  while(!(zhFile.eof())){
    getline(zhFile,lineStr);
    if(lineStr.size() == 0){
      continue;
    }
    int64_t temp = atoi(lineStr.substr(lineStr.find('\t')+1,lineStr.size()).c_str());
    m_ZhSet.insert(temp);
  }
#ifdef _DEBUG
  cout <<"Han zi:\n";
  for(set<int64_t>::iterator iter = m_ZhSet.begin();iter != m_ZhSet.end();iter++){
    cout << *iter <<"\t";
  }
#endif
  ifstream txtFile(m_inputFile.c_str());
  if(txtFile.fail()){
#ifdef _ERROR
    cerr <<"Can not open" <<m_inputFile <<endl;
#endif
    return false;
  }
  getline(txtFile,lineStr); //读取第一行信息
  vector<string> vstr = getInfo(lineStr.c_str(),"\t");
  m_nCountRows = atoi(vstr[0].c_str());//总行数
  m_nCountDifItem = atoi(vstr[1].c_str());//总不同单词数
  m_nCountItem = atoi(vstr[2].c_str());//总单词数
  int64_t min = (int)(m_minSup * m_nCountRows);
  m_pDatabase = new int64_t *[m_nCountRows];
  m_pWordProject = new set<int64_t> *[m_nCountDifItem];
  for(int i = 0;i < m_nCountDifItem;i++){
    m_pWordProject[i] = new set<int64_t>();
  }
  int64_t lineCount = 0;
  while(!(txtFile.eof())){
    getline(txtFile,lineStr);
    if(lineStr.size() == 0){
      continue;
    }
    vector<string> vstr = getInfo(lineStr.c_str(),"\t");
    m_pDatabase[lineCount] = new int64_t[vstr.size() + 1];
    m_pDatabase[lineCount][0] = vstr.size();
    for(int i = 0;i < vstr.size();i++){
      int64_t id = atoi(vstr[i].c_str());
      m_pDatabase[lineCount][i + 1] = id;
      m_pWordProject[id]->insert(lineCount);
    }
    lineCount++;
  }
  //Compute Freequent1Seq
  for(int64_t i = 0;i < m_nCountDifItem;i++){
    if(m_pWordProject[i]->size() >= min){
      if(m_ZhSet.end() != m_ZhSet.find(i)){
        m_freq1Item.push_back(i);
      }
    }
  }
#ifdef _DEBUG
  cout <<endl;
  cout <<"Database:\n";
  for(int i = 0;i < m_nCountRows;i++){
    for(int j = 0;j < m_pDatabase[i][0];j++){
      cout << m_pDatabase[i][j+1] <<"\t";
    }
    cout <<endl;
  }
  cout <<"Freq iterms:\n";
  vector<int64_t>::iterator iter = m_freq1Item.begin();
  while(iter != m_freq1Item.end()){
    cout << *iter++ <<" ";
  }
  cout <<endl;
#endif
  return true;
}

bool DataShared::SetMinSup(const double &sup)
{
  this->m_minSup = sup;
  return true;
}

bool DataShared::SetLikelyHood(const double &x)
{
  this->m_like = x;
  return true;
}
