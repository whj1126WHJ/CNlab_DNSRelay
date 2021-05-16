//
// Created by 王乾凱 on 2021/5/13.
//

#include "DNSQuestion.h"

byte* questionToByteArray(struct DNSQuestion dnsQuestion) {
    byte *data = (byte*)malloc(sizeof(byte)*(strlen(dnsQuestion.qname) + 2 + 4));
    int offset = 0;
    byte *domainByteArray = domainToByteArray(dnsQuestion.qname);
    for(int i = 0; i < strlen(domainByteArray); i++) {
        data[offset++] = domainByteArray[i];
    }
    byte *byte_2;
    byte_2 = shortToByteArray(dnsQuestion.qtype);
    for(int i = 0; i < 2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsQuestion.qclass);
    for(int i = 0; i < 2; i ++) {
        data[offset++] = byte_2[i];
    }
    questionLength = offset;
    return data;
}