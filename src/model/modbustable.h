#ifndef MODBUSTABLE_H
#define MODBUSTABLE_H

#include "modbusunit.h"
#include "Comm/singleton.h"

#include <vector>

using namespace std;

class ModbusTable
{
public:
    ModbusTable();

    static ModbusTable& Instance()
    {
        return Singleton<ModbusTable>::instance();
    }

    vector<ModbusUnit*> getModbusAddrVec();
    void registerAddr(ModbusUnit **p, uint16_t type, uint16_t addr);

    //1区
    ModbusUnit *addr1x_1;
    ModbusUnit *addr1x_2;
    ModbusUnit *addr1x_3;
    ModbusUnit *addr1x_4;
    ModbusUnit *addr1x_6;
    ModbusUnit *addr1x_9;
    ModbusUnit *addr1x_11;
    ModbusUnit *addr1x_12;

    //3区
    ModbusUnit *addr3x_31;
    ModbusUnit *addr3x_32;
    ModbusUnit *addr3x_33;
    ModbusUnit *addr3x_34;
    ModbusUnit *addr3x_36;
    ModbusUnit *addr3x_39;
    ModbusUnit *addr3x_11;
    ModbusUnit *addr3x_22;

    //4区
    ModbusUnit *addr4x_21;
    ModbusUnit *addr4x_22;
    ModbusUnit *addr4x_23;
    ModbusUnit *addr4x_24;
    ModbusUnit *addr4x_26;
    ModbusUnit *addr4x_29;
    ModbusUnit *addr4x_11;
    ModbusUnit *addr4x_12;


private:
    vector<ModbusUnit*> m_modbusAddrVec;

};

#endif // MODBUSTABLE_H
