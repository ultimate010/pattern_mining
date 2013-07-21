#include <fstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;
string PATH = "run.conf";

void loadConf(string path,map<string,string> & confMap);

int main(){
  map<string,string> confMap;
  loadConf(PATH,confMap);
  return 0;
}

void loadConf(string path,map<string,string> & confMap){
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
