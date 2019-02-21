#include "modbusunit.h"


ModbusUnit::ModbusUnit(uint16_t type, uint16_t addr)
{
    m_type = type;
    m_addr = addr;
    m_data = 0;
}


uint16_t ModbusUnit::getType()
{
    return m_type;
}

uint16_t ModbusUnit::getAddr()
{
    return m_addr;
}

uint16_t ModbusUnit::getData()
{
    return m_data;
}
