#ifndef MBSRTU_H
#define MBSRTU_H

#include "comm/comm.h"

#include <modbus/modbus.h>
#include <modbus/modbus-rtu.h>


#define RS485_RTL_DELAY_US  0UL
#define SELECT_TIMEOUT_US   500000UL


typedef struct {
    uint8_t slaveAddr;    /*从机地址*/
    uint8_t function;     /*功能码*/
    uint16_t regAddr;     /*寄存器地址*/
    uint16_t regCnt;      /*寄存器数量*/
    /*For Modbus TCP IP*/
    uint16_t tid;         /*mbs-tcp 事务处理标识符*/
    int      readCnt;     /*读数据返回个数*/
    int      writCnt;     /*用于非标功能码*/
    union {
        uint8_t  reg8[128];
        uint16_t reg16[128];   /*数据区*/
    }data;
}MbsReqAck;

typedef struct
{
    modbus_t    *mb;
    uint8_t     *query; //主站的请求数组指针
    modbus_mapping_t *mapping;
    int header_length;  //主站功能码所在数组下标
    int rc;
}modbus_server;
typedef union {
    modbus_t      *mb;
    modbus_server mb_server;    //服务端
}RtuBus;

class MbsRtu
{
public:
    MbsRtu();

public:
    static void open_debug(modbus_t* mb, bool open = true);

    static int read_input_bits_regs(RtuBus& dev, uint8_t slaveAddr,
                                uint16_t regAddr, uint16_t regCnt, uint8_t *data);
    /* 0x03读多个连续的保持寄存器*/
    static int  read_hold_regs(RtuBus& dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t regCnt, uint16_t *data);
    static int  read_hold_regs(RtuBus& dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t *data);
    /* 0x04读多个连续的输入寄存器*/
    static int  read_input_regs(RtuBus& dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t regCnt, uint16_t *data);
    /* 0x06写单个保持寄存器*/
    static int  write_single_hold_reg(RtuBus& dev, uint8_t slaveAddr, uint16_t regAddr, int value);
    /* 0x10写多个保持寄存器*/
    static int  write_multi_hold_regs(RtuBus& dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t regCnt, const uint16_t *data);

    /* tcp-Client*/
    static bool modbus_tcpclient_init(RtuBus& bus, char* dev_name,
                                      const char *ip, int port);
    static int  mbstcp_send_raw_request(RtuBus& dev, uint8_t *raw_req, int raw_req_length,
                                        uint8_t *ack, uint16_t tid);
    static bool mbs_reconnect(RtuBus& bus);

    static void printf_data(uint8_t* data, int cnt);
private:
    /* crc校验*/
    static uint16_t ModbusCRC(uint8_t *data, uint16_t len);

};

#endif // MBSRTU_H
