//
// Created by 王乾凱 on 2021/5/12.
//

#include "DNSHeader.h"

byte* headerToByteArray(struct DNSHeader dnsHeader) {
    byte *data = (byte*) malloc(sizeof(byte)*12);
    int offset = 0;
    byte *byte_2;
    byte_2 = shortToByteArray(dnsHeader.transID);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsHeader.flags);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsHeader.qdcount);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsHeader.ancount);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsHeader.nscount);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    byte_2 = shortToByteArray(dnsHeader.arcount);
    for (int i=0; i<2; i++) {
        data[offset++] = byte_2[i];
    }
    return data;
}