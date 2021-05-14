//
// Created by 王乾凱 on 2021/5/12.
//

#include "DNSRR.h"

byte* RRToByteArray(struct DNSRR dnsrr) {
    byte *data = (byte*)malloc(sizeof(byte)*(dnsrr.rdlength + 12));
    int offset = 0;
    byte *byte_2;
    byte *byte_4;
    byte_2 = shortToByteArray(dnsrr.aname);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsrr.atype);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsrr.aclass);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_4 = intToByteArray(dnsrr.ttl);
    for (int i=0; i<4; i++) {
        data[offset++] = byte_4[i];
    }
    byte_2 = shortToByteArray(dnsrr.rdlength);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    if (dnsrr.rdlength == 4) {
        byte_4 = ipv4ToByteArray(dnsrr.rdata);
        for (int i=0; i<4; i++) {
            data[offset++] = byte_4[i];
        }
    }
    return data;

}