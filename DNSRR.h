//
// Created by 王乾凱 on 2021/5/12.
//

#ifndef PROJECT_DNSRR_H
#define PROJECT_DNSRR_H

struct DNSRR{
    /**
	 * Answer/Authority/Additional
	   0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |					   ... 						  |
	  |                    NAME                       |
	  |                    ...                        |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    TYPE                       |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    CLASS                      |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    TTL                        |
      |                                               |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    RDLENGTH                   |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	  |                    ...                        |
	  |                    RDATA                      |
	  |                    ...                        |
	  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	 */

    /* NAME (2字节 采用消息压缩) */
    short aname;

    /* TYPE（2字节） */
    short atype;

    /* CLASS（2字节） */
    short aclass;

    /* TTL（4字节） */
    int ttl;

    /* RDLENGTH（2字节） */
    short rdlength;

    /* RDATA IPv4为4字节*/
    char rdata[1024];
};
#endif //PROJECT_DNSRR_H
