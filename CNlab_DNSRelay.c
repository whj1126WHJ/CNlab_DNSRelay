// CNlab_DNSRelay.cpp: 定义应用程序的入口点。
//

#include "CNlab_DNSRelay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static mapNode* map;
int main(int argc, char** argv)
{
    printf("hello CN_lab");
    char fileName[1024];
    map = (mapNode*)malloc(sizeof(mapNode));
    if(argc < 1){
        printf("请输入映射表文件\n");
    }else{
        strcpy(fileName, argv[0]);
    }
	return 0;
}
