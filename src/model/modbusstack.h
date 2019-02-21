#ifndef MODBUSSTACK_H
#define MODBUSSTACK_H

#include "Comm/worker.h"
#include "modbustable.h"
#include "MbsRtu.h"

#include <qobject.h>
#include <qmap.h>
#include <qlist.h>

#include <stdio.h>
#include <qglobal.h>
#include <queue>
#include <vector>
#include <set>

using namespace std;

#define MODBUS_ZYKZQ_ADDR   1

class ModbusStack
{
private:
    ModbusStack(); //构造函数
public:
    static ModbusStack *Instance();//获取单实例
    int writeMbsPack(const MbsWritePack pack);

private:
    void Init();
    //从地址集合中得到连续（非连续）地址下标集合
    void getContinuousMarkFromAddrVec(vector<ModbusUnit*> &addrVec,
                                      QMap<int, int> &continuousMap,
                                      QList<int> &noncontinuousList);
    static bool upAlgorithm(ModbusUnit *a, ModbusUnit *b);

    //读数据线程
    void readThread();
    //读取连续地址容器
    void readContinuousAddrs(vector<ModbusUnit*> &addrVec, QMap<int, int> &continuousMap);
    //读取非连续地址容器
    void readNoncontinuousAddrs(vector<ModbusUnit*> &addrVec, QList<int> &noncontinuousList);
    int getReadAddrCntMax(int regType);
    int readMbsAddrs(RtuBus &dev, int regType, int regAddr, int regCnt, void *data);
    void updateMbsData(vector<ModbusUnit *> &addrVec, int regType, int index, int regCnt, void *data);

    //写数据线程
    void writeThread();
    bool getOneMsgFromWriteQue(MbsWritePack &pack);
    int writeMbsAddrs(RtuBus &dev, int regType, int regAddr, int regCnt, void *data);

private:
    static ModbusStack *m_instance;
    RtuBus m_rtuBus;

    //读数据线程
    Worker<ModbusStack> m_readThread;

    vector<ModbusUnit*> m_1xAddrVec;    //modbus1区地址集合
    QMap<int, int> m_1xContinuousMarkMap; //modbus1区连续下标集合
    QList<int> m_1xNoncontinuousMarkList; //modbus1区非连续下标集合

    vector<ModbusUnit*> m_3xAddrVec;    //modbus3区地址集合
    QMap<int, int> m_3xContinuousMarkMap; //modbus3区连续下标集合
    QList<int> m_3xNoncontinuousMarkList; //modbus3区非连续下标集合

    vector<ModbusUnit*> m_4xAddrVec;    //modbus4区地址集合
    QMap<int, int> m_4xContinuousMarkMap; //modbus4区连续下标集合
    QList<int> m_4xNoncontinuousMarkList; //modbus4区非连续下标集合

    //写数据线程
    Worker<ModbusStack> m_writeThread;
    queue<MbsWritePack> m_writeQueue;
    QMutex m_writeQueueMutex;

};

#endif // MODBUSSTACK_H
