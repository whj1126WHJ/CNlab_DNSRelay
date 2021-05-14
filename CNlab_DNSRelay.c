// CNlab_DNSRelay.cpp: 定义应用程序的入口点。
//

#include "CNlab_DNSRelay.h"
#include "QueryParser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static mapNode* map;

void readin(char* fileName){
    FILE *fp = fopen(fileName, "r");
    char ipWithDomin[1024];
    mapNode* root = map;
    int count = 0;
    while(!feof(fp)){
        fgets(ipWithDomin, 1000, fp);
        char *buff;
        buff = ipWithDomin;
        char *ip = strsep(&buff, " ");
        char *domin = strsep(&buff, "\r\n");
        root -> next = (mapNode*)malloc(sizeof(mapNode));
        root -> next -> ip = ip;
        root -> next -> domin = domin;
        root = root -> next;
        count++;
    }
    printf("已读入%d条数据\n", count);
}
int main(int argc, char** argv)
{
    printf("hello CN_lab");
    char fileName[1024];
    map = (mapNode*)malloc(sizeof(mapNode));
    if(argc < 1){
        printf("请输入映射表文件\n");
        gets(fileName);
    }else{
        strcpy(fileName, argv[1]);
    }
    readin(fileName);
    run();

	return 0;
}


char* getIpByDomin(char* qname){
    mapNode* root = map -> next;
    while(root){
        if(!strcmp(qname, root ->domin)){
            return root -> ip;
        }
        root = root -> next;
    }
    return NULL;
}
