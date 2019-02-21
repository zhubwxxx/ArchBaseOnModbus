#include "modbustable.h"

ModbusTable::ModbusTable()
{
    registerAddr(&addr1x_1, 1, 1);
    registerAddr(&addr1x_2, 1, 2);
    registerAddr(&addr1x_3, 1, 3);
    registerAddr(&addr1x_4, 1, 4);
    registerAddr(&addr1x_6, 1, 6);
    registerAddr(&addr1x_9, 1, 9);
    registerAddr(&addr1x_11, 1, 11);
    registerAddr(&addr1x_12, 1, 12);

    registerAddr(&addr3x_31, 3, 31);
    registerAddr(&addr3x_32, 3, 32);
    registerAddr(&addr3x_33, 3, 33);
    registerAddr(&addr3x_34, 3, 34);
    registerAddr(&addr3x_36, 3, 36);
    registerAddr(&addr3x_39, 3, 39);
    registerAddr(&addr3x_11, 3, 11);
    registerAddr(&addr3x_22, 3, 22);

    registerAddr(&addr4x_21, 4, 21);
    registerAddr(&addr4x_22, 4, 22);
    registerAddr(&addr4x_23, 4, 23);
    registerAddr(&addr4x_24, 4, 24);
    registerAddr(&addr4x_26, 4, 26);
    registerAddr(&addr4x_29, 4, 29);
    registerAddr(&addr4x_11, 4, 11);
    registerAddr(&addr4x_12, 4, 12);
}

vector<ModbusUnit*> ModbusTable::getModbusAddrVec()
{
    return m_modbusAddrVec;
}

void ModbusTable::registerAddr(ModbusUnit **p, uint16_t type, uint16_t addr)
{
    *p = new ModbusUnit(type, addr);
    m_modbusAddrVec.push_back(*p);
}
