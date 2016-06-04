/*================================
作者:颜廷涛
时间:2016/6/2
内容:利用内存映射拷贝文件
=================================*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <errno.h>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
	int fd = open("app1.cpp",O_RDWR);
    if (fd < 0){
    	perror("open:");
    	exit(-1);
    }
	int len = lseek(fd,0,SEEK_END);
	void* addr = mmap(NULL,len,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	if (addr == NULL){
		perror("mmap:");
		exit(-1);
	}
    close(fd);
    
    fd = open("app11.app",O_CREAT|O_EXCL|O_RDWR,0777);
    if (fd < 0){
    	if (errno == EEXIST){
    		fd = open("app11.app",O_RDWR);
    		if (fd < 0){
    			perror("open:");
    			exit(-1);
    		}
    	}else{
    		perror("open:");
    		exit(-1);
    	}
    }
    int length = lseek(fd,len,SEEK_SET);
    write(fd," ",1);
    void* addr2 = mmap(NULL,length,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    if (addr2 == NULL){
    	perror("mmap:");
    	exit(-1);
    }
    close(fd);
    memcpy(addr2,addr,len);
    munmap(addr,len);
    munmap(addr2,length);
	return 0;
}