#include "commonFunc.h"
vector<string> getInfo(const char *src,const char *tag)
{
  vector<string> infoVec;
  char *p = (char*)src;
  char *pstr = strstr(p,tag);
  while(pstr)
  {
    infoVec.push_back(string(p,pstr-p));
    p = pstr+strlen(tag);
    pstr = strstr(p,tag);
  }
  if(0 != strlen(p))
  {
    infoVec.push_back(p);
  }
  return infoVec;
}

