//
// Created by 王乾凱 on 2021/5/12.
//

#ifndef PROJECT_DNSHEADER_H
#define PROJECT_DNSHEADER_H

#endif //PROJECT_DNSHEADER_H

#include "Utils.h"
/*
	 * DNS Header
	    0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                      ID                       |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |QR|  opcode   |AA|TC|RD|RA|   Z    |   RCODE   |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    QDCOUNT                    |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    ANCOUNT                    |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    NSCOUNT                    |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    ARCOUNT                    |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

struct DNSHeader {
    /* 会话标识（2字节）*/
    short transID;

    /* Flags（2字节）*/
    short flags;

    /* QDCOUNT（2字节）*/
    short qdcount;

    /* ANCOUNT（2字节）*/
    short ancount;

    /* NSCOUNT（2字节）*/
    short nscount;

    /* ARCOUNT（2字节）*/
    short arcount;
};

byte* headerToByteArray(struct DNSHeader);