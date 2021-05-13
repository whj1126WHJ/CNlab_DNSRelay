//
// Created by 王乾凱 on 2021/5/13.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#endif //PROJECT_UTILS_H

typedef char byte;

short byteArrayToShort(byte* b);

short byteArrayToShort_offset(byte* b, int offset);

byte* shortToByteArray(short i);

int byteToInt (byte b);

int byteArrayToInt(byte* b);

int byteArrayToInt_offset(byte* bytes, int offset);

byte* intToByteArray(int i);

char* byteToHexString (byte b);

char* byteArrayToHexString (byte* bytes);

char* byteArrayToAscii(byte* bytes, int offset, int dataLen);

char* extractDomain(byte* bytes, int offset, int stopByte);

byte* domainToByteArray(char* domain);

byte* ipv4ToByteArray(char* ipv4);