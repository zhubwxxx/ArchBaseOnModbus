#ifndef COMM_H
#define COMM_H


#include <qtextcodec.h>
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <error.h>
//#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <algorithm>

#include <qdebug.h>

//modbus
typedef struct {
    uint16_t regAddr;       //寄存器地址
    uint16_t regCnt;        //寄存器个数
    uint16_t data[128];     //数据
} MbsWritePack;





class Comm
{
public:
    Comm();


};


void setUTF8Code();

#endif // COMM_H
