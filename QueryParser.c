//
// Created by 王乾凱 2019211504 on 2021/5/13.
//
#ifndef	_CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "QueryParser.h"
#include "Utils.h"
#include "DNSHeader.h"
#include "DNSQuestion.h"
#include "DNSRR.h"
#include "CNlab_DNSRelay.h"

#include <stdio.h>

#ifdef _WIN32 /* for Windows Visual Studio */

#include <winsock.h>
#include <io.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>

#else /* for Linux & mac */


#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>


#endif


static int sock;
int len = 0;
int sockfd;
struct sockaddr_in servaddr = {};

char* clientIp[32];

void socket_recv(){
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    /* 填充struct sockaddr_in */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(53);
//    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    //TODO: if(sockfd < 0)
//    int res = bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

//    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
//        //TODO: ABORT
//        printf("bind_failed\n");
//    }
    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        //TODO: ABORT
        printf("bind_failed\n");
    }
//    listen(sockfd, SOMAXCONN);

//    sock = accept(sockfd, 0, 0); //TODO: 监听客户端ip及端口
//    if(sock < 0){} //TODO: ABORT  failed
    socklen_t addrLength = sizeof(addrLength);
    int length = recvfrom(sockfd, data, sizeof(data), 0,
                          (struct sockaddr *)&servaddr, &addrLength);
    if(length < 0) {} //TODO: ABORT
    else dataLength = length;


    //test:
    printf("IP:%s\n", (char *)inet_ntoa(servaddr.sin_addr));
    printf("Port:%d\n", htons(servaddr.sin_port));
}
void send_socket(byte* response_data, int length){
    sendto(sockfd, response_data, length, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
}
void run(){
    socket_recv();
    byte buff_2[2];
    int offset = 0;

    struct DNSHeader dnsHeader = {};
    struct DNSQuestion dnsQuestion = {};
    int flag = 0;
    for(int i = 1024-1; i >= 0; i--) {
        if(data[i])
            flag = 1;
        if(flag)
            len++;
    }

    //处理DNS协议头
    for (int i = 0; i < 2; i++) {
        buff_2[i] = data[i + offset];
    }
    offset += 2;
    dnsHeader.transID = byteArrayToShort(buff_2);

    for (int i = 0; i < 2; i++) {
        buff_2[i] = data[i + offset];
    }
    offset += 2;
    dnsHeader.flags = byteArrayToShort(buff_2);

    for (int i = 0; i < 2; i++) {
        buff_2[i] = data[i + offset];
    }
    offset += 2;
    dnsHeader.qdcount = byteArrayToShort(buff_2);

    for (int i = 0; i < 2; i++) {
        buff_2[i] = data[i + offset];
    }
    offset += 2;
    dnsHeader.ancount = byteArrayToShort(buff_2);

    for (int i = 0; i < 2; i++) {
        buff_2[i] = data[i + offset];
    }
    offset += 2;
    dnsHeader.nscount = byteArrayToShort(buff_2);

    for (int i = 0; i < 2; i++) {
        buff_2[i] = data[i + offset];
    }
    offset += 2;
    dnsHeader.arcount = byteArrayToShort(buff_2);

    // 获取查询的域名
    if (dnsHeader.qdcount > 0) { // qdcount通常为1
        char* domainName = extractDomain(data, offset, 0x00, len);
        cStrcat(dnsQuestion.qname, domainName, 0);
        offset += strlen(domainName) + 2;
        for (int j = 0; j < 2; j++) {
            buff_2[j] = data[j + offset];
        }
        offset += 2;
        dnsQuestion.qtype= byteArrayToShort(buff_2);

        for (int j = 0; j < 2; j++) {
            buff_2[j] = data[j + offset];
        }
        offset += 2;
        dnsQuestion.qclass = byteArrayToShort(buff_2);
    }
    else {
        //TODO: System.out.println(Thread.currentThread().getName() + " DNS数据长度不匹配，Malformed Packet");
    }

    // 查询本地域名-IP映射
    char* ip = getIpByDomin(dnsQuestion.qname);
    printf("本地查找结果 domain: %s QTYPE: %d ip: %s\n", dnsQuestion.qname, dnsQuestion.qtype, ip);
    if(strcmp(ip, "") != 0 && dnsQuestion.qtype == 1){
        //header
        short flags = 0;
        if (strcmp(ip, "0.0.0.0") != 0) { // rcode为0（没有差错）
            flags = (short) 0x8580;
        } else { // rcode为3（名字差错），只从一个授权名字服务器上返回，它表示在查询中指定的域名不存在
            flags = (short) 0x8583;
        }
        struct DNSHeader dnsHeaderResponse = {dnsHeader.transID, flags, dnsHeader.qdcount, (short)1, (short) 1, (short) 0};
        byte* dnsHeaderByteArray = headerToByteArray(dnsHeaderResponse);

        //question
        byte* dnsQuestionByteArray = questionToByteArray(dnsQuestion);

        //answer
        struct DNSRR anDNSRR = {(short) 0xc00c, dnsQuestion.qtype, dnsQuestion.qclass, 3600*24, (short) 4};
        anDNSRR.rdata = (char*)malloc(sizeof(char) * strlen(ip));
        strcpy(anDNSRR.rdata, ip);
        byte* anDNSRRByteArray = RRToByteArray(anDNSRR);
        int anDNSRRlength = rrLength;
        // Authoritative nameservers 只是模拟了包格式，nameserver实际指向了查询的域名
        struct DNSRR nsDNSRR = {(short) 0xc00c, (short) 6, dnsQuestion.qclass, 3600*24, (short) 0};
        nsDNSRR.rdata = (char*)malloc(sizeof(char) * 0);
        strcpy(nsDNSRR.rdata, "");
        byte* nsDNSRRByteArray = RRToByteArray(nsDNSRR);
        int nsDNSRRlength = rrLength;

        byte* response_data = (byte*)malloc(sizeof(byte) * (headerlength + questionLength + anDNSRRlength + nsDNSRRlength));
        int responseOffset = 0;
        for (int i = 0; i < headerlength; i++) {
            response_data[responseOffset++] = dnsHeaderByteArray[i];
        }
        for (int i = 0; i < questionLength; i++) {
            response_data[responseOffset++] = dnsQuestionByteArray[i];
        }
        if (strcmp(ip, "0.0.0.0") != 0) {
            for (int i = 0; i < anDNSRRlength; i++) {
                response_data[responseOffset++] = anDNSRRByteArray[i];
            }
        }
        for (int i = 0; i < nsDNSRRlength; i++) {
            response_data[responseOffset++] = nsDNSRRByteArray[i];
        }
        send_socket(response_data, responseOffset);
        printf("获得socket，响应 %s : %s\n", dnsQuestion.qname, ip);
    }
    else{ //TODO: 本地未检索到，请求因特网DNS服务器
    }
}
