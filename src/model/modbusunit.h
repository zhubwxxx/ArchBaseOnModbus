#ifndef MODBUSUNIT_H
#define MODBUSUNIT_H

#include "Comm/comm.h"

class ModbusUnit
{
public:
    ModbusUnit(uint16_t type, uint16_t addr);

    //getters
    uint16_t getType();
    uint16_t getAddr();
    uint16_t getData();
    //setters
    void setType(uint16_t type) { m_type = type; }
    void setAddr(uint16_t addr) { m_addr = addr; }
    void setData(uint16_t data) { m_data = data; }


private:
    uint16_t m_type;//类型
    uint16_t m_addr;//地址
    uint16_t m_data;//数据
};

#endif // MODBUSUNIT_H
