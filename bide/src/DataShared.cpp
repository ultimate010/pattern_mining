#include "DataShared.h"

DataShared::DataShared(void)
{
	SetParameter();	LoadData();
	m_pOut = new ofstream(outFile);
	m_pOut1 = new ofstream(outFile1);
	m_pOutParameter = new ofstream(outFileParameter);
	if (m_pOut==NULL||m_pOut1==NULL){
		cout<<"�����ļ�����" << endl; return; }
	Freequent1Seq();
	*m_pOutParameter <<"lr: " <<m_like <<endl;
	*m_pOutParameter <<"��С֧�ֶ�: " <<m_like <<endl;
	*m_pOutParameter <<"�ܵ�����: " <<m_nCountItem <<endl;
	*m_pOutParameter <<"��ͬ������: " <<m_nCountDifItem <<endl;
	*m_pOutParameter <<"Ӣ�ĵ�����: " <<m_nCountEn <<endl;
	*m_pOutParameter <<"���ĵ�����: " <<m_nCountZh <<endl;
	*m_pOutParameter <<"������: " <<m_nCountRows <<endl;
	m_iter = m_freq1Item->begin();
	m_outPutZh = 0;
}

//////////////////////////////////////////////////////////////////////////
// ����Ƶ��1����
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
	//�ͷ���Դ
	delete m_pDatabase[];
	delete m_pEn;
	delete m_pZh;
	delete m_pWordCout;
	delete m_pWordProject;
	delete m_Index;
}

bool DataShared::SetParameter()
{
	cout <<"������֧�ֶȣ�" <<endl;
	long sup;
	cin >>sup;
	SetMinSup(sup);
	double lr;
	cout <<"������lrֵ: " <<endl; cin>> lr;
	SetLikelyHood(lr);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ������������ӳ�䣬�����ʱ�ļ�
// ��ʽΪ��
// row1��ÿ�����ʶ�Ӧ��ӳ������
// row2����һ�е��ܵ�����Ŀ
//////////////////////////////////////////////////////////////////////////
bool DataShared::ChuliShuju(const int & min_sup){
	long nCountItem = 0;	//���㵥����Ŀ
	long nCountRow = 0; //��������
	int nCountWordIn1Sentence = 0; //ÿ���еĵ�����Ŀ
	m_nCountEn = 0;
	m_nCountZh = 0;
	ifstream f_in(rawFile);
	ofstream f_out(tempFile);
	if(f_in==NULL||f_out==NULL){
		cout <<"�ļ�δ�ҵ�!" <<endl; return false;
	}
	multimap<string,long> * map_All_Word = new multimap<string,long>; //�洢���еĵ���
	map<string,long>* m_map =new map<string,long>; string str_Input; //�洢��ͬ�ĵ���
	while(getline(f_in,str_Input)){ //����ÿһ��
		if (str_Input.length()==0)	continue;//������У�����
		nCountRow++; //������+1
#if _MyDebug
		cout <<"��ȡԭʼ���ݵ�" <<nCountRow <<"��!" <<endl;
#endif
		nCountWordIn1Sentence = 0; //ÿ���е�����Ŀ��ʼ��Ϊ0
		int nSpacePos = 0; //λ��
		nSpacePos = str_Input.find(" ");
		while(nSpacePos>=0){
			while (nSpacePos == 0 ){
				str_Input = str_Input.erase(0,1);nSpacePos=str_Input.find(" ");//ɾ�����׵������ַ�
			}
			string subString = str_Input.substr(0,nSpacePos); //ȡ�����еĵ���
			str_Input.erase(0,nSpacePos+1);
			if (subString==" "||subString=="")
			{
			nSpacePos = str_Input.find(' '); continue ; //�ո�����
			}
			char c = subString.data()[0];
			if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
				//�������Ӣ��
				if(m_map->find(subString)==m_map->end())
					m_nCountEn++;
			}else{
				//�������͵Ķ���ΪӢ��
				if(m_map->find(subString)==m_map->end())
				m_nCountZh++;
			}
			nCountWordIn1Sentence++; //����һ�����ʣ�������+1
			multimap<string,long>::iterator myIter = map_All_Word->find(subString);  //�����Ƿ��Ѿ���¼���������
			if (myIter != map_All_Word->end())
			{ //�ҵ����ڵ�
				f_out <<myIter->second <<" "; //���ӳ�����������
				map_All_Word->insert(make_pair(subString,myIter->second));  //�ѵ�ǰ����ӳ���ٴβ���map��
#if _MYDEBUG
				cout <<myIter->second <<" ";
#endif
			}else{ //û�м�¼������ʣ���������µ�ӳ��
				f_out << nCountItem <<" ";
#if _MYDEBUG
				cout <<nCountItem <<"	";
#endif
				map_All_Word->insert(make_pair(subString,nCountItem));
				m_map->insert(make_pair(subString,nCountItem)); nCountItem++;   	//�ܵ��ʼ���+1
			}
			nSpacePos = str_Input.find(' ');
		}
		f_out <<endl; f_out << nCountWordIn1Sentence << endl; //����
	}
	/*
	f_out <<nCountRow <<" " << map_All_Word.size() <<" " << min_sup <<" " <<m_map->size() <<endl; //�����һ��
	*/
	//��ֵ
	m_minSup = min_sup; m_nCountRows = nCountRow; m_nCountItem = map_All_Word->size();
	m_nCountDifItem = m_map->size(); m_Index = new string[m_nCountDifItem];
	for (map<string,long>::iterator iter = m_map->begin();
		iter!=m_map->end();iter++)
		m_Index[iter->second]=iter->first; //��ʼ��
	delete m_map;
	delete map_All_Word;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��ȡ���ݵ��ڴ��У����ҳ�ʼ��pDatabaseָ��ָ�����ݿ�
// ���ݿ�ĸ�ʽΪ��
// ��i��Ϊ��ת����ľ���
// ��i+1��Ϊ����һ�о���һ���ж��ٸ�����
// pDatabase�����ݴ洢��ʽΪ��
// ÿ�У����еĵ����� ����ÿ������
// ��ʼm_pWordProject��������ʽΪ��
// ��i�е����ݴ洢����i���г��ֵ�����
//////////////////////////////////////////////////////////////////////////
bool DataShared::LoadData()
{
	if(ChuliShuju(m_minSup)==false){
		//��ȡ����
		return false;
	}
	ifstream in(tempFile);
	if (in==NULL)
	{
		cout <<"�ļ�δ�ҵ�!" <<endl;
		return false;
	}
	string str_firstInput;
	string str_secondInput;
	int pos;
	//��ʼ����������
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
	//�������ݶ�ȡ
	long nCountWord = 0;  //�洢ÿһ���еĵ�����Ŀ
	long nRow = 0; //�洢��ǰ�ڼ���
	while(getline(in,str_firstInput)){ //str_firstInput�д��һ��  str_secondInput�д������ܵ�����Ŀ
		getline(in,str_secondInput); nCountWord = a2long(str_secondInput.data());
		m_pDatabase[nRow] = new long[nCountWord+1]; //��ʼ��
		long * pWriter = m_pDatabase[nRow]; //ָ��pDatabase�еĵ�n������
		*pWriter = nCountWord; //д�뱾���ܵ���
		pWriter++;
		while(1){
			pos = str_firstInput.find(' ');
			if(pos == -1) break; //��ǰ���Ѿ���������
			str_secondInput = str_firstInput.substr(0,pos);  str_firstInput.erase(0,pos+1);
			*pWriter = a2long(str_secondInput.data());
			m_pWordCout[*pWriter]++;
			//�жϵ�ǰӳ�����Ӣ��������Ӧ��ӳ������
			string temp = m_Index[*pWriter];
			char c = temp.data()[0];
			if((c<='z'&&c>='a')||(c<='Z'&&c>='A')){
				//Ӣ��
				//���ж�set���Ƿ��¼��Ҳ�����Ƿ��һ�γ���
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
			//д������,ע�⣬һ�仰�г�������ͬͬ���ĵ��ʣ�ֻд��һ������
			vector<long> *pTemp = m_pWordProject[*pWriter];
			if (pTemp->size()==0){ //�����һ�β��룬��ôֱ�Ӳ���
				pTemp->push_back(nRow);
			}else{
				if (pTemp->back()!=nRow){ 	//����Ѿ���¼��ǰ�У���ô���ټ�¼
					pTemp->push_back(nRow);
				}
			}
			pWriter++;
		}
		//������һ��
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
