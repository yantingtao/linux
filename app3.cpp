/*====================================================
作者:颜廷涛
时间:2016/6/4
内容:文件数据用随机数生成后映射到内存,进行排序
======================================================*/
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <cstdlib>
#include <string>
using namespace std;

int fastsort(int pstr[],int left,int right)
{
   	int tmp = pstr[left];
	while (left < right){
        while (left < right && tmp < pstr[right]){
        	right--;
        }
        if (right > left){
        	pstr[left] = pstr[right];
        	left++;
        }
       while (left < right && tmp > pstr[left]){
       	left++;
       }
       if (right > left){
       	pstr[right] = pstr[left];
       	right--;
       }
	}
	pstr[left] = tmp;
	return left;
}

void sort(int pstr[],int left,int right)
{
  if (left < right){
  	int midnum = fastsort(pstr,left,right);
  	   sort(pstr,0,midnum-1);
  	   sort(pstr,midnum+1,right);
  }
}
int main(int argc, char const *argv[])
{
	int rand_num;
	int i = 0;
	char buf[1024];
	bzero(buf,sizeof(buf));
	int fd = open("app3.dat",O_CREAT|O_EXCL|O_RDWR,0777);
	if (fd < 0){
		if (errno == EEXIST){
         fd = open("app3.dat",O_RDWR);
         if (fd < 0){
         	perror("open:");
         	exit(-1);
         }
		}else{
			perror("open:");
			exit(-1);
		}
	}
	for (i = 0;i < 10;i++){//产生随机数并写入文件
		rand_num = rand()%10000;
		sprintf(buf,"%d\t",rand_num);
		write(fd,buf,strlen(buf));
		bzero(buf,sizeof(buf));
	}
    int len  =  lseek(fd,0,SEEK_END);
    void* addr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (addr == NULL){
    	perror("mmap:");
    	exit(-1);
    }
    close(fd);
    
    int count = len;
    int pstr[10];
    i = 0;
    int j = 0;
    bzero(buf,sizeof(buf));
    while (count--){
        if (*(char*)addr == '\t'){
          pstr[j] = atoi(buf);
          i = 0;
          j++;
          // cout<<"数字:"<<buf<<" ";
          bzero(buf,sizeof(buf));
          addr = (char *)addr + 1;
        }
        buf[i] = *(char*)addr;
        addr = (char *)addr + 1;
        i++;
    }
   sort(pstr,0,9);
   i = 0;
   for (i = 0; i < 10; i++){
   	cout<<pstr[i]<<endl;
   }
	return 0;
}