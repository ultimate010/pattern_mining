#include "DataShared.h"

DataShared::DataShared(void)
{
	SetParameter();	LoadData();
	m_pOut = new ofstream(outFile);
	m_pOut1 = new ofstream(outFile1);
	m_pOutParameter = new ofstream(outFileParameter);
	if (m_pOut==NULL||m_pOut1==NULL){
		cout<<"生成文件错误" << endl; return; }
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

bool DataShared::SetParameter()
{
	cout <<"请输入支持度：" <<endl;
	long sup;
	cin >>sup;
	SetMinSup(sup);
	double lr;
	cout <<"请输入lr值: " <<endl; cin>> lr;
	SetLikelyHood(lr);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 把输入数据做映射，输出临时文件
// 格式为：
// row1：每个单词对应的映射数字
// row2：上一行的总单词数目
//////////////////////////////////////////////////////////////////////////
bool DataShared::ChuliShuju(const int & min_sup){
	long nCountItem = 0;	//计算单词数目
	long nCountRow = 0; //计算行数
	int nCountWordIn1Sentence = 0; //每行中的单词数目
	m_nCountEn = 0;
	m_nCountZh = 0;
	ifstream f_in(rawFile);
	ofstream f_out(tempFile);
	if(f_in==NULL||f_out==NULL){
		cout <<"文件未找到!" <<endl; return false;
	}
	multimap<string,long> * map_All_Word = new multimap<string,long>; //存储所有的单词
	map<string,long>* m_map =new map<string,long>; string str_Input; //存储不同的单词
	while(getline(f_in,str_Input)){ //读出每一行
		if (str_Input.length()==0)	continue;//如果空行，跳过
		nCountRow++; //总行数+1
#if _MyDebug
		cout <<"读取原始数据第" <<nCountRow <<"行!" <<endl;
#endif
		nCountWordIn1Sentence = 0; //每行中单词数目初始化为0
		int nSpacePos = 0; //位置
		nSpacePos = str_Input.find(" ");
		while(nSpacePos>=0){
			while (nSpacePos == 0 ){
				str_Input = str_Input.erase(0,1);nSpacePos=str_Input.find(" ");//删掉句首的无用字符
			}
			string subString = str_Input.substr(0,nSpacePos); //取出所有的单词
			str_Input.erase(0,nSpacePos+1);
			if (subString==" "||subString=="")
			{
			nSpacePos = str_Input.find(' '); continue ; //空格跳过
			}
			char c = subString.data()[0];
			if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
				//处理的是英文
				if(m_map->find(subString)==m_map->end())
					m_nCountEn++;
			}else{
				//其他类型的都记为英文
				if(m_map->find(subString)==m_map->end())
				m_nCountZh++;
			}
			nCountWordIn1Sentence++; //处理一个单词，单词数+1
			multimap<string,long>::iterator myIter = map_All_Word->find(subString);  //查找是否已经记录过这个单词
			if (myIter != map_All_Word->end())
			{ //找到存在的
				f_out <<myIter->second <<" "; //输出映射的数字数据
				map_All_Word->insert(make_pair(subString,myIter->second));  //把当前单词映射再次插入map中
#if _MYDEBUG
				cout <<myIter->second <<" ";
#endif
			}else{ //没有记录这个单词，插入这个新的映射
				f_out << nCountItem <<" ";
#if _MYDEBUG
				cout <<nCountItem <<"	";
#endif
				map_All_Word->insert(make_pair(subString,nCountItem));
				m_map->insert(make_pair(subString,nCountItem)); nCountItem++;   	//总单词计数+1
			}
			nSpacePos = str_Input.find(' ');
		}
		f_out <<endl; f_out << nCountWordIn1Sentence << endl; //换行
	}
	/*
	f_out <<nCountRow <<" " << map_All_Word.size() <<" " << min_sup <<" " <<m_map->size() <<endl; //输出第一行
	*/
	//赋值
	m_minSup = min_sup; m_nCountRows = nCountRow; m_nCountItem = map_All_Word->size();
	m_nCountDifItem = m_map->size(); m_Index = new string[m_nCountDifItem];
	for (map<string,long>::iterator iter = m_map->begin();
		iter!=m_map->end();iter++)
		m_Index[iter->second]=iter->first; //初始化
	delete m_map;
	delete map_All_Word;
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
	if(ChuliShuju(m_minSup)==false){
		//读取数据
		return false;
	}
	ifstream in(tempFile);
	if (in==NULL)
	{
		cout <<"文件未找到!" <<endl;
		return false;
	}
	string str_firstInput;
	string str_secondInput;
	int pos;
	//初始化单词索引
	m_pDatabase = new long *[m_nCountRows];
	m_pWordCout = new long[m_nCountDifItem];
	m_pWordProject = new vector<long> *[m_nCountDifItem];
	m_pEn = new long[m_nCountEn];
	m_pZh = new long[m_nCountZh];
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

long DataShared::a2long( const string &s ) const
{
	return atol(s.data());
}

bool DataShared::SetMinSup( const long &sup )
{
	m_minSup = sup;
	return true;
}

bool DataShared::SetLikelyHood( const double &x )
{
	m_like = x;
	return true;
}
