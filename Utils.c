//
// Created by 王乾凱 on 2021/5/13.
//

#include "Utils.h"
#include "QueryParser.h"

/**
     * 一维字节数组转 short 值(2 字节)
     */
short byteArrayToShort(byte* b) {
    return byteArrayToShort_offset(b, 0);
}

/**
     * 一维字节数组转 short 值(2 字节)
     */

short byteArrayToShort_offset(byte* b, int offset) {
    return (short) (((b[offset] & 0xff) << 8) | (b[offset + 1] & 0xff));
}

/**
     * 将 short 类型数据转为 byte[]
     */
byte* shortToByteArray(short i) {
    byte *result = (byte*) malloc(sizeof(byte)*2);
    //由高位到低位
    result[0] = (byte)((i >> 8) & 0xFF);
    result[1] = (byte)(i & 0xFF);
    return result;
}

/**
    * byte 转 int
    */
int byteToInt (byte b) {
    return (b & 0xff);
}

/**
     * 一维字节数组转 int 值(4 字节)
     */
int byteArrayToInt(byte* b) {
    return byteArrayToInt_offset(b, 0);
}
int byteArrayToInt_offset(byte* bytes, int offset) {
    int value= 0;
    //由高位到低位
    for (int i = 0; i < 4; i++) {
        int shift= (4 - 1 - i) * 8;
        value +=(bytes[i] & 0x000000FF) << shift;//往高位游
    }
    return value;
}

/**
     * 将 int 类型数据转为 byte[]
     */
byte* intToByteArray(int i) {
    byte *result = (byte*) malloc(sizeof(byte)*4);
    //由高位到低位
    result[0] = (byte)((i >> 24) & 0xFF);
    result[1] = (byte)((i >> 16) & 0xFF);
    result[2] = (byte)((i >> 8) & 0xFF);
    result[3] = (byte)(i & 0xFF);
    return result;
}

/**
    * 一维字节数组转 int 值(4 字节)
    */
char* byteToHexString (byte b) {
    int hexInt  = byteToInt(b);
    char* s = (char*) malloc(sizeof(char)*10);
    sprintf(s, "%X", hexInt);
    return s;
}

/**
     * byte[] 转化为16进制字符串
     */
char* byteArrayToHexString (byte* bytes) {
    char *sb = (char*) malloc(sizeof(char)*20);
    char *s = (char*) malloc(sizeof(char)*20);
    //这样s是被更新还是被覆盖？如果后续的字符串大于前面的，s是否会有前者冗余？
    int len = strlen(bytes);
    int j = 0;
    for(int i = 0; i < len; i++) {
        s = byteToHexString(bytes[i]);
        if(strlen(s) < 2) {
            sb[j] = '0';
            j++;
        }
        strcat(sb, s);
        j += strlen(s);
    }
    return sb;
}

/**
     * 一维字节数组转化为Ascii对应的字符串
     */
char* byteArrayToAscii(byte* bytes, int offset, int dataLen, int len) {
    if ((bytes == NULL) || (strlen(bytes) == 0) || (offset < 0) || (dataLen <= 0)
        || (offset + dataLen > len)) {
        return NULL;
    }
    byte *data = (byte*) malloc(sizeof(byte)*dataLen);
    for(int i = 0; i < dataLen; i++) {
        data[i] = bytes[i+offset];
    }
    return data;
}

/**
    * 从字节数组中提取出域名
    */
char* extractDomain(byte* bytes, int offset, int stopByte, int len) {
    char* stringBuffer = (char*) malloc(sizeof(char)*50);
    int subLen =  0;
    while (offset < dataLength && byteToInt(data[offset]) != stopByte) {
        subLen = byteToInt(data[(offset)++]);
        char *tem = byteArrayToAscii(bytes, offset, subLen, len);
        cStrcat(stringBuffer, tem, offset);
        offset += subLen;
        if(offset < dataLength && byteToInt(data[offset]) != stopByte) {
            cStrcat(stringBuffer, ".", offset);
        }
    }
    return stringBuffer;
}

/**
     * 域名转化为字节数组
     */
byte* domainToByteArray(char* domain) {
    byte *result = (byte*) malloc(sizeof(byte)* strlen((domain)) + 2);
    int offset = 0;
    char **subDomainArray = malloc(sizeof(char*)*10);
    for(int i = 0; i < 10; i++) {
        *subDomainArray = malloc((sizeof(char)*10));
    }
    int num = split(domain, "\\.", subDomainArray);
    for(int i = 0; i < num; i++) {
        int len = (int) strlen(subDomainArray[i]);
        result[offset++] = (byte) strlen(subDomainArray[i]);
        for(int j = 0; j < len; j++) {
            result[offset++] = (byte)subDomainArray[i][j];
        }
    }
    result[offset++] = 0x00;
    return result;
}

/**
     * IPv4点分十进制转换为一维字节数组
     */
byte* ipv4ToByteArray(char* ipv4) {
    byte *result = malloc(sizeof(byte)*4);
    char **ipv4SubArray = malloc(sizeof(char*)*10);
    for(int i = 0; i < 10; i++) {
        *ipv4SubArray = malloc(sizeof(char)*10);
    }
    int num = split(ipv4, "\\.", ipv4SubArray);
    if(num != 4) {
        printf("不是合法的ipv4地址\n");
        return NULL;
    }
    for(int i = 0; i < num; i++) {
        int nums = atoi(ipv4SubArray[i]);
        byte tmp = 0;
        if(nums > 127) {
            tmp = (byte)(nums - 256);
        }
        else {
            tmp = (byte)nums;
        }
        result[i] = tmp;
    }
    return result;
}

/**
 * 字符串分割
 */
int split(char *src,const char *separator,char **dest) {
    /*
        src 源字符串的首地址(buf的地址)
        separator 指定的分割字符
        dest 接收子字符串的数组
    */
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止
        return count;
    if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止
        return count;
    pNext = (char *)strtok(src,separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
    while(pNext != NULL) {
        *dest++ = pNext;
        count++;
        pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
    }
    return count;
}

void cStrcat(char* stringBuffe, byte *dat, offset) {
    int i = 0;
    for(i = 0; stringBuffe[i]; i++) {

    }
    for(int j = 0; dat[j]; j++) {
        stringBuffe[i+j] = dat[j];
    }
}