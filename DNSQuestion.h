//
// Created by 王乾凱 on 2021/5/13.
//

#ifndef PROJECT_DNSQUESTION_H
#define PROJECT_DNSQUESTION_H

#endif //PROJECT_DNSQUESTION_H


#include "Utils.h"

struct DNSQuestion {
    /**
	 * Question 查询字段
		0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                     ...                       |
	  |                    QNAME                      |
	  |                     ...                       |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    QTYPE                      |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    QCLASS                     |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 */

    /* QNAME 8bit为单位表示的查询名(广泛的说就是：域名) */
    char qname[1024];

    /* QTYPE（2字节） */
    short qtype;

    /* QCLASS（2字节） */
    short qclass;
};

byte* questionToByteArray(struct DNSQuestion);