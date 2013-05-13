/*
 * 读取文件，将空格分离的数据替换成数字
 * Zaks Wang
 * 2013-5-10
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hashmap.h"
#include "getFileName.h"
#if defined(DEBUG)
#include "leak_detector_c.h"
#endif


#define BUFLEN (1024)
#define KEY_MAX_LENGTH (1024)
#define INFILEPATH "/home/nlp/data/"
#define OUTFILEPATH "/home/nlp/data_out/"

static unsigned long currentCount = 0;
typedef struct data_struct_s{
  char key_string[KEY_MAX_LENGTH];
  unsigned long number;
}data_struct_t;
int freeVale(any_t item ,any_t data){
  free(data);
  return MAP_OK;
}
/*
 * 读取文件，过滤重复行
 * 格式为三行一组
 */
void doFilter(map_t map,const char *file){
  char temp[512]={0};
  sprintf(temp,"%s%s",INFILEPATH,file); //输入
  FILE *fileIn = fopen(temp,"r");
  sprintf(temp,"%s%s",OUTFILEPATH,file); //输出
  FILE *fileOut = fopen(temp,"w");
  if(fileIn==NULL||temp==NULL){
    fprintf(stderr,"Open file error%s\n",file);
    return ;
  }
  char buf[BUFLEN]={0}; //初始化0
  char buf1[BUFLEN]={0};
  char buf2[BUFLEN]={0};
  int i=0;
  int error;
  data_struct_t* value; //pair数据
  while(fgets(buf,BUFLEN,fileIn)){
    //读取每一行
    fgets(buf1,BUFLEN,fileIn);
    assert(buf1[0]!='\n'); //不应该出现空行
    fgets(buf2,BUFLEN,fileIn);
    assert(buf2[0]!='\n'); //同上
    i=0;
    while(i<BUFLEN&&buf[i]!='\n'&&buf[i]!='\0'){
      i++;
    }
    if(i!=0){
      //不为空行
      value=(data_struct_t*)malloc(sizeof(data_struct_t));
      memset(value->key_string,0,KEY_MAX_LENGTH);
      strncpy(value->key_string,buf,i);
      value->number=1;
      error=hashmap_put(map,value->key_string,value);
      if(error==MAP_USED){
        //已经存在
        //不输出
      }else{
        assert(error==MAP_OK);
        fprintf(fileOut,"%s",buf);
        fprintf(fileOut,"%s",buf1);
        fprintf(fileOut,"%s",buf2);
      }
    }
    memset(buf,0,BUFLEN),memset(buf1,0,BUFLEN),memset(buf2,0,BUFLEN);
  }
  fclose(fileIn),fclose(fileOut);
}
/*
 * 读取文件，替换单词为数字
 */
void doWork(map_t map,const char *file){
  char temp[512]={0};
  sprintf(temp,"%s%s",INFILEPATH,file); //输入
  FILE *fileIn = fopen(temp,"r");
  sprintf(temp,"%s%s",OUTFILEPATH,file); //输出
  FILE *fileOut = fopen(temp,"w");
  if(fileIn==NULL||temp==NULL){
    fprintf(stderr,"Open file error%s\n",file);
    return ;
  }
  char buf[BUFLEN]={0}; //初始化0
  int i=0,j=0;
  int error;
  data_struct_t* value; //pair数据
  while(fgets(buf,BUFLEN,fileIn)){
    //读取每一行
    i=0;j=0;
    while(i<BUFLEN&&buf[i]!='\n'&&buf[i]!='\0'){
      while(buf[i]==' '){
        i++,j++; //跳过头部空格
      }
      while(buf[i]!=' '&&buf[i]!='\n'&&i<BUFLEN&&buf[i]!='\0') i++;
      assert(i!=j);
      //i为空格位置
      value=(data_struct_t *)malloc(sizeof(data_struct_t));
      memset(value->key_string,0,KEY_MAX_LENGTH);
      strncpy(value->key_string,(buf+j),i-j);
      value->number = currentCount;
      error=hashmap_put(map,value->key_string,value);
      if(error==MAP_USED){
        //已经存在
        data_struct_t * temp=NULL;
        error=hashmap_get(map,value->key_string,(void **)(&temp));
        assert(error==MAP_OK);
        free(value);
        fprintf(fileOut,"%d\t",temp->number);
      }else{
        assert(error==MAP_OK);
        fprintf(fileOut,"%d\t",value->number);
        currentCount++; //下一个
      }
      j=++i;//跳过空格
    }
    memset(buf,0,BUFLEN);
    if(i!=0){
      fputc('\n',fileOut);
    }
  }
  fclose(fileIn),fclose(fileOut);
}

int main(char * argv,int argc)
{
#if defined(DEBUG)
  atexit(report_mem_leak);
#endif
  _fileNameNode *pFiles = getFileNameByDir(INFILEPATH);
  _fileNameNode *pCur = pFiles;
  map_t mymap;
  mymap = hashmap_new(2000000ul);
  while(pCur){
    /*
     * 对每个文件操作
     */
    //doFilter(mymap,pCur->fileName);
        doWork(mymap,pCur->fileName);
    pCur=pCur->next;
  }
  hashmap_iterate(mymap,freeVale,(void *)NULL);
  hashmap_free(mymap);
  freefileNameList(pFiles);
  pFiles = pCur = NULL;
  return 0;
}
