// CNlab_DNSRelay.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once


// TODO: 在此处引用程序需要的其他标头。


typedef struct node{
    char* ip;
    char* domin;
    struct node* next;
}mapNode;

char* getIpByDomin(char* qname);