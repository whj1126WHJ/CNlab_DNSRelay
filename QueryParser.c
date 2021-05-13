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
static byte data[1024];
static int dataLength;

static int sock;

int sockfd;
struct sockaddr_in servaddr;
void socket_recv(){
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /* 填充struct sockaddr_in */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    //TODO: if(sockfd < 0)

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        //TODO: ABORT
    }
    listen(sockfd, 5);

    sock = accept(sockfd, 0, 0); //TODO: 监听客户端ip及端口
    if(sock < 0){} //TODO: ABORT  failed

    ssize_t length = recv(sock, data, sizeof(data), 0);
    if(length < 0) {} //TODO: ABORT
    else dataLength = length;
}
void send_socket(byte* response_data){
    send(sock, response_data, 1, 0);
}
void run(){
    socket_recv();
    byte buff_2[2];
    int offset = 0;

    struct DNSHeader dnsHeader;
    struct DNSQuestion dnsQuestion;

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
        char* domainName = extractDomain(data, offset, 0x00);
        strcpy(dnsQuestion.qname, domainName);
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

    // TODO：查询本地域名-IP映射
    char* ip = getIpByDomin(dnsQuestion.qname);
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
        strcpy(anDNSRR.rdata, ip);
        byte* anDNSRRByteArray = RRToByteArray(anDNSRR);

        // Authoritative nameservers 只是模拟了包格式，nameserver实际指向了查询的域名
        struct DNSRR nsDNSRR = {(short) 0xc00c, (short) 6, dnsQuestion.qclass, 3600*24, (short) 0};
        strcpy(nsDNSRR.rdata, 0);
        byte* nsDNSRRByteArray = RRToByteArray(nsDNSRR);

        byte* response_data = (byte*)malloc(sizeof(byte) * (strlen(dnsHeaderByteArray) + strlen(dnsQuestionByteArray) + strlen(anDNSRRByteArray) + strlen(nsDNSRRByteArray)));
        int responseOffset = 0;
        for (int i = 0; i < strlen(dnsHeaderByteArray); i++) {
            response_data[responseOffset++] = dnsHeaderByteArray[i];
        }
        for (int i = 0; i < strlen(dnsQuestionByteArray); i++) {
            response_data[responseOffset++] = dnsQuestionByteArray[i];
        }
        if (strcmp(ip, "0.0.0.0") != 0) {
            for (int i = 0; i < strlen(anDNSRRByteArray); i++) {
                response_data[responseOffset++] = anDNSRRByteArray[i];
            }
        }
        for (int i = 0; i < strlen(nsDNSRRByteArray); i++) {
            response_data[responseOffset++] = nsDNSRRByteArray[i];
        }
    }
    else{ //TODO: 本地未检索到，请求因特网DNS服务器

    }
}