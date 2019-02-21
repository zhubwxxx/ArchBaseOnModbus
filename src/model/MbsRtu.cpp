#include "MbsRtu.h"

MbsRtu::MbsRtu()
{

}

void MbsRtu::open_debug(modbus_t* mb, bool open)
{
    modbus_set_debug(mb, open);
}

bool MbsRtu::mbs_reconnect(RtuBus& dev)
{
    modbus_flush(dev.mb);

    if (modbus_connect(dev.mb) < 0)
        return false;
    else
        return true;
}
/**
 * @brief MbsRtu::mbstcp_send_raw_request mbstcp发送原始数据并读取
 * @param dev
 * @param raw_req
 * @param raw_req_length
 * @param ack
 * @param tid
 * @return 返回读到的字节数
 */
int MbsRtu::mbstcp_send_raw_request(RtuBus& dev, uint8_t *raw_req, int raw_req_length,
                                     uint8_t *ack, uint16_t tid)
{
    int req_length;

//    req_length = modbus_send_raw_request(dev.mb, raw_req, raw_req_length, tid);
    req_length = modbus_send_raw_request(dev.mb, raw_req, raw_req_length);

    if (req_length > 0)
    {
        req_length = modbus_receive_confirmation(dev.mb, ack);
    }
    else
    {
        req_length = -2;
    }
    return req_length;
}

bool MbsRtu::modbus_tcpclient_init(RtuBus& bus, char* dev_name,
                                   const char *ip, int port)
{
    QString qErr = QString::fromLatin1(dev_name) + ":";
    struct timeval t;

    bus.mb = modbus_new_tcp(ip, port);
    if (bus.mb == NULL) {
        qErr += QString::fromLatin1(modbus_strerror(errno));
        goto err2;
    }

    t.tv_sec  = 1;
    t.tv_usec = SELECT_TIMEOUT_US;

    modbus_set_response_timeout(bus.mb, t.tv_sec, t.tv_usec);

#if 1
    modbus_set_debug(bus.mb, 0);
#else
    modbus_set_debug(bus.mb, 1);
#endif

#if 0
    /* 打开后如果SOCKET通信失败将自动重连，但是通信将变为阻塞
     * RTU模式下不会阻塞
     */
    modbus_set_error_recovery(bus.mb, MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
#else
    modbus_set_error_recovery(bus.mb, MODBUS_ERROR_RECOVERY_PROTOCOL);
#endif
    if (modbus_connect(bus.mb) < 0) {
        //TCP允许重连
        qErr += "modbus_connect";
        Log::instance().console(qErr, Log::ERROR_LOG);
    }
    return true;
err1:
    modbus_free(bus.mb);
err2:
    Log::instance().console(qErr, Log::ERROR_LOG);
    return false;
}

int  MbsRtu::read_hold_regs(RtuBus& dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t *data)
{
    QString qErr;// = QString::fromLatin1(uartInfo->dev_name) + ": ";
    int regs = 0;

    if (modbus_set_slave(dev.mb, slaveAddr) != 0) {//set slave address
//        qErr += QString::fromLatin1(modbus_strerror(errno));
        regs = -1;
        goto err;
    }

    regs = modbus_read_registers(dev.mb, regAddr, 1, data);
    if (regs == -1){
        qErr += QString::fromLatin1(modbus_strerror(errno));
        goto err;
    }else if (regs) {
        usleep(RS485_RTL_DELAY_US); //for rtl exchange delay,minimal time
    }
    return regs;
err:
    Log::instance().console(qErr);
    return regs;
}

int MbsRtu::read_input_bits_regs(RtuBus& dev, uint8_t slaveAddr,
                                uint16_t regAddr, uint16_t regCnt, uint8_t *data)
{
    QString qErr;// = QString::fromLatin1(uartInfo->dev_name) + ": ";
    int regs = 0;


    if (modbus_set_slave(dev.mb, slaveAddr) != 0) {//set slave address
        qErr += "set slave address";
        regs = -1;
        goto err;
    }
    regs = modbus_read_input_bits(dev.mb, regAddr, regCnt, data);
    if (regs == -1){
        qErr = qErr + QString::fromLatin1(modbus_strerror(errno));
        goto err;
    }else if (regs) {
        usleep(RS485_RTL_DELAY_US); //for rtl exchange delay,minimal time
    }
    return regs;
err:
    Log::instance().console(qErr, Log::ERROR_LOG);
    return regs;
}

int MbsRtu::read_hold_regs(RtuBus &dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t regCnt, uint16_t *data)
{
    QString qErr;// = QString::fromLatin1(uartInfo->dev_name) + ": ";
    int regs = 0;

    if (modbus_set_slave(dev.mb, slaveAddr) != 0) {//set slave address
        qErr += QString::fromLatin1(modbus_strerror(errno));
        regs = -1;
        goto err;
    }
    regs = modbus_read_registers(dev.mb, regAddr, regCnt, data);
    if (regs == -1){
        qErr += QString::fromLatin1(modbus_strerror(errno));
        goto err;
    }else if (regs) {
        usleep(RS485_RTL_DELAY_US); //for rtl exchange delay,minimal time
    }
    return regs;
err:
    Log::instance().console(qErr, Log::ERROR_LOG);
    return regs;
}

int MbsRtu::read_input_regs(RtuBus &dev, uint8_t slaveAddr, uint16_t regAddr, uint16_t regCnt, uint16_t *data)
{
    QString qErr;// = QString::fromLatin1(uartInfo->dev_name) + ":";
    int regs = 0;

    if (modbus_set_slave(dev.mb, slaveAddr) != 0) {//set slave address
        qErr += QString::fromLatin1(modbus_strerror(errno));
        regs = -1;
        goto err;
    }
    regs = modbus_read_input_registers(dev.mb, regAddr, regCnt, data);
    if (regs == -1){
        qErr += QString::fromLatin1(modbus_strerror(errno));
        goto err;
    }else if (regs) {
        usleep(RS485_RTL_DELAY_US); //for rtl exchange delay,minimal time
    }

    return regs;
err:
    Log::instance().console(qErr, Log::ERROR_LOG);
    return regs;
}

int MbsRtu::write_single_hold_reg(RtuBus &dev, uint8_t slaveAddr, uint16_t regAddr, int value)
{
    QString qErr;// = QString::fromLatin1(uartInfo->dev_name) + ":";
    int regs = 0;

    if (modbus_set_slave(dev.mb, slaveAddr) != 0) {//set slave address
        qErr += QString::fromLatin1(modbus_strerror(errno));
        regs = -1;
        goto err;
    }
    regs = modbus_write_register(dev.mb, regAddr, value);
    if (regs == -1){
        qErr += QString::fromLatin1(modbus_strerror(errno));
        goto err;
    }else if (regs) {
        usleep(RS485_RTL_DELAY_US); //for rtl exchange delay,minimal time
    }

    return regs;
err:
    Log::instance().console(qErr, Log::ERROR_LOG);
    return regs;
}


int MbsRtu::write_multi_hold_regs(RtuBus &dev, uint8_t slaveAddr,
                                  uint16_t regAddr, uint16_t regCnt, const uint16_t *data)
{
    QString qErr;// = QString::fromLatin1(uartInfo->dev_name) + ":";
    int regs = 0;

    if (modbus_set_slave(dev.mb, slaveAddr) != 0) {//set slave address
        qErr += QString::fromLatin1(modbus_strerror(errno));
        regs = -1;
        goto err;
    }
    regs = modbus_write_registers(dev.mb, regAddr, regCnt, data);
    if (regs == -1){
        qDebug()<< __FUNCTION__;
        qErr = QString::fromLatin1(modbus_strerror(errno));
        goto err;
    }else if (regs) {
        usleep(RS485_RTL_DELAY_US); //for rtl exchange delay,minimal time
    }

    return regs;
err:
    Log::instance().console(qErr, Log::ERROR_LOG);
    return regs;
}

uint16_t MbsRtu::ModbusCRC(uint8_t *data, uint16_t len)
{
    uint16_t i, j;
    uint16_t crc=0xFFFF, flag=0;

    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            flag = crc & 0x0001;
            crc >>= 1;
            if (flag) {
                crc ^= 0xA001;
            }
        }
    }
    return crc;
}

void MbsRtu::printf_data(uint8_t* data, int cnt)
{
    printf("data:\n");
    for (int i = 0; i < cnt; i++){
        printf("%02x ", data[i]);
    }
    printf("\n");
}
