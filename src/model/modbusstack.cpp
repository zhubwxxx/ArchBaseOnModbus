#include "modbusstack.h"

#include <qsettings.h>

ModbusStack* ModbusStack::m_instance = 0;
ModbusStack::ModbusStack():
   m_readThread(this, &ModbusStack::readThread),
   m_writeThread(this, &ModbusStack::writeThread)
{

    Init();

}

ModbusStack *ModbusStack::Instance()
{
    static QMutex mutex;

    if (!m_instance) {
        QMutexLocker locker(&mutex);
        if (!m_instance) {
            m_instance = new ModbusStack;
        }
    }
    return m_instance;
}


void ModbusStack::Init()
{

    ModbusTable table = ModbusTable::Instance();
    foreach (ModbusUnit *unit, table.getModbusAddrVec()) {
        if (unit->getType() == 1) {
            m_1xAddrVec.push_back(unit);
        } else if (unit->getType() == 3) {
            m_3xAddrVec.push_back(unit);
        } else if (unit->getType() == 4) {
            m_4xAddrVec.push_back(unit);
        } else {
            Log::instance().console("未知的Modbus Addr类型", Log::ERROR_LOG);
        }
    }
    //标记相应的地址集合
    getContinuousMarkFromAddrVec(m_1xAddrVec, m_1xContinuousMarkMap, m_1xNoncontinuousMarkList);
    getContinuousMarkFromAddrVec(m_3xAddrVec, m_3xContinuousMarkMap, m_3xNoncontinuousMarkList);
    getContinuousMarkFromAddrVec(m_4xAddrVec, m_4xContinuousMarkMap, m_4xNoncontinuousMarkList);

    //初始化modbus-tcp连接
    QSettings *ini = new QSettings("networkCfg.ini", QSettings::IniFormat);
    ini->beginGroup("Network");
    QString ip = ini->value("ip").toString();
    int port = ini->value("port").toInt();
    qDebug()<< "ip: " << ip << " port: " << port;
    if (MbsRtu::modbus_tcpclient_init(m_rtuBus, "ZJJK",ip.toUtf8().data(), port)) {
            m_readThread.start();
            m_writeThread.start();
    } else {
        Log::instance().console("init modbus-tcp failed", Log::ERROR_LOG);
    }

}



void ModbusStack::getContinuousMarkFromAddrVec(vector<ModbusUnit *> &addrVec,
                                               QMap<int, int> &continuousMap,
                                               QList<int> &noncontinuousList)
{
    int i;
    int j;
    QMap<int, int>::const_iterator iter;
    vector<int> indexVec;

    if (addrVec.size() == 1) {
        noncontinuousList.push_back(0);
        return;
    }

    //对地址集合排序
    std::sort(addrVec.begin(), addrVec.end(), upAlgorithm);
    printf("%dX all addr:", addrVec[0]->getType());
    foreach (ModbusUnit *unit, addrVec) {
        printf("<%d>", unit->getAddr());
    }
    printf("\n");


    //查找连续&非连续地址下标
    for (i = 0, j = 0; i < addrVec.size(), j < addrVec.size(); ) {
        indexVec.clear();
        for (j = i + 1; j < addrVec.size(); ) {
            if ( (addrVec[i]->getAddr()+1) == (addrVec[j]->getAddr())) {
                indexVec.push_back(i);
                i++;
                j++;
                if ( (j >= addrVec.size()) || (i >= addrVec.size())) {
                    continuousMap.insert(indexVec[0], j-1);
                }
            } else {
                noncontinuousList.push_back(i);
                if (indexVec.size() > 0) {
                    continuousMap.insert(indexVec[0], j-1);
                    i++;
                    break;
                } else {
                    i++;
                    j++;
                }
                if (j >= addrVec.size()) {
                    noncontinuousList.push_back(j-1);
                }

            }
        }
    }



    //删除非连续下标集合中重复的值
    for (i=0; i<noncontinuousList.size(); i++) {
        for (j=i+1; j<noncontinuousList.size(); j++) {
            if (noncontinuousList.at(i) == noncontinuousList.at(j)) {
                noncontinuousList.removeAt(i);
                j--;
            }
        }
    }
    //删除 连续下标集合 和 非连续下标集合 重复的值
    iter = continuousMap.begin();
    while (iter != continuousMap.constEnd()) {
        for (i = 0; i < noncontinuousList.size(); i++) {
            if ( (noncontinuousList.at(i) < iter.key()) ||
                 (noncontinuousList.at(i) > iter.value()) ) {
                continue;
            } else {
                noncontinuousList.removeAt(i);
                i--;
            }
        }
        iter++;
    }

    //对非连续地址下标集合排序
    std::sort(noncontinuousList.begin(), noncontinuousList.end());

    //打印调试输出
    printf("%dX Non-contimuous addr:\n",addrVec[0]->getType());
    foreach (int index, noncontinuousList) {
        printf("<%d>", addrVec.at(index)->getAddr());
    }
    printf("\n");

    printf("%dX Contimuous addr:\n", addrVec[0]->getType());
    iter = continuousMap.begin();
    while (iter != continuousMap.end()) {
        printf("head=%d, end=%d ", iter.key(), iter.value());
        for (i = iter.key(); i <= iter.value(); i++) {
            printf("[%d]", addrVec.at(i)->getAddr());
        }
        printf("\n");
        iter++;
    }
    printf("\n");
    fflush(stdout);
}

bool ModbusStack::upAlgorithm(ModbusUnit *a, ModbusUnit *b)
{
    return (a->getAddr() < b->getAddr());
}



void ModbusStack::readThread()
{
    while ( !m_readThread.isStopped() ) {
        readContinuousAddrs(m_1xAddrVec, m_1xContinuousMarkMap);
        readNoncontinuousAddrs(m_1xAddrVec, m_1xNoncontinuousMarkList);

        readContinuousAddrs(m_3xAddrVec, m_3xContinuousMarkMap);
        readNoncontinuousAddrs(m_3xAddrVec, m_3xNoncontinuousMarkList);

        readContinuousAddrs(m_4xAddrVec, m_4xContinuousMarkMap);
        readNoncontinuousAddrs(m_4xAddrVec, m_4xNoncontinuousMarkList);

        m_readThread.msleep(5000);
    }
}


void ModbusStack::readContinuousAddrs(vector<ModbusUnit*> &addrVec, QMap<int, int> &continuousMap)
{
    uint8_t  data[MODBUS_MAX_READ_BITS];
    ModbusUnit *unitS;
    ModbusUnit *unitE;
    int addrS;
    int addrE;
    int addrCnt;
    int addrCntMax;
    int regType;
    int i;
    int index;

    QMap<int, int>::const_iterator iter = continuousMap.constBegin();
    while (iter != continuousMap.end()) {
        unitS = addrVec.at(iter.key());
        unitE = addrVec.at(iter.value());
        addrS = unitS->getAddr();
        addrE = unitE->getAddr();
        addrCnt = addrE - addrS + 1;
        regType = unitS->getType();
        addrCntMax = getReadAddrCntMax(regType);

        if (addrCnt < 2) {
            Log::instance().console("读取连续寄存器个数小于2， 不符合要求");
            continue;
        }

        if (addrCnt <= addrCntMax) {
            if (readMbsAddrs(m_rtuBus, regType, addrS, addrCnt, data) > 0) {
                updateMbsData(addrVec, regType, iter.key(), addrCnt, data);
            } else {
                Log::instance().console("读取连续数据错误");
            }
        } else {
            for (i = 0; i < (addrCnt/addrCntMax); i++) {
                index = i * addrCntMax + iter.key();
                if (readMbsAddrs(m_rtuBus, regType, addrVec.at(index)->getAddr(), addrCntMax, data) > 0) {
                    updateMbsData(addrVec, regType, index, addrCntMax, data);
                } else {
                    Log::instance().console("读取连续数据错误（分段读取）");
                }
            }
        }

        iter++;
    }


}

int ModbusStack::getReadAddrCntMax(int regType)
{
    int ret = 128;

    //??????

    return ret;
}

int ModbusStack::readMbsAddrs(RtuBus &dev, int regType, int regAddr, int regCnt, void *data)
{
    int ret = -1;

    int regCntMax = getReadAddrCntMax(regType);

    if (regCnt < 1 || regCnt > regCntMax) {
        Log::instance().console("读取连续寄存器个数不符合要求");
    } else {
        switch (regType) {
        case 1:
            ret = MbsRtu::read_input_bits_regs(dev, MODBUS_ZYKZQ_ADDR, regAddr, regCnt, (uint8_t*)data);
            break;
        case 3:
            ret = MbsRtu::read_hold_regs(dev, MODBUS_ZYKZQ_ADDR, regAddr, regCnt, (uint16_t*)data);
            break;
        case 4:
            ret = MbsRtu::read_input_regs(dev, MODBUS_ZYKZQ_ADDR, regAddr, regCnt, (uint16_t*)data);
            break;
        default:
            break;
        }
    }

    return ret;
}

void ModbusStack::updateMbsData(vector<ModbusUnit *> &addrVec, int regType, int index, int regCnt, void *data)
{
    if (index + regCnt > addrVec.size()) {
        qDebug("数组越界");
        return;
    }

    switch (regType) {
    case 1:
        for (int i = 0; i < regCnt; i++) {
            addrVec[index+i]->setData(((uint8_t*)data)[i]);
        }
        break;
    case 3:
    case 4:
        for (int i = 0; i < regCnt; i++) {
            addrVec[index+i]->setData(((uint16_t*)data)[i]);
        }
        break;
    default:
        break;
    }

}

void ModbusStack::readNoncontinuousAddrs(vector<ModbusUnit *> &addrVec, QList<int> &noncontinuousList)
{
    uint8_t  data[2];
    int regType;
    int regAddr;

    foreach (int index, noncontinuousList) {
        regType = addrVec.at(index)->getType();
        regAddr = addrVec.at(index)->getAddr();
        if (readMbsAddrs(m_rtuBus, regType, regAddr, 1, data) > 0) {
            updateMbsData(addrVec, regType, index, 1, data);
        } else {
            Log::instance().console("读取非连续数据错误");
        }
    }
}


void ModbusStack::writeThread()
{
    MbsWritePack pack;

    while ( !m_writeThread.isStopped() ) {

        if (!m_writeQueue.empty()) {
            if (!getOneMsgFromWriteQue(pack)) { //如果取出消息失败
                continue;
            }
            //发送modbus数据
            writeMbsAddrs(m_rtuBus, 4, pack.regAddr, pack.regCnt, pack.data);

        } else {
            m_writeThread.msleep(100);
        }

    }
}

bool ModbusStack::getOneMsgFromWriteQue(MbsWritePack &pack)
{
    QMutexLocker lock(&m_writeQueueMutex);
    bool ret = false;

    if (!m_writeQueue.empty()) {//如果发送队列为空，则等待一段时间
        /*从队列获取一包数据*/
        pack = m_writeQueue.front();//返回队头消息
        m_writeQueue.pop();//删除队头消息
        ret = true;
    }

    return ret;
}

int ModbusStack::writeMbsAddrs(RtuBus &dev, int regType, int regAddr, int regCnt, void *data)
{
    int ret = -1;

    if (regType == 4) {
        if (regCnt == 1) {
            ret = MbsRtu::write_single_hold_reg(dev, MODBUS_ZYKZQ_ADDR, regAddr, ((uint16_t*)data)[0]);
        } else if (regCnt >= 2){
            ret = MbsRtu::write_multi_hold_regs(dev, MODBUS_ZYKZQ_ADDR, regAddr, regCnt, (uint16_t*)data);
        } else {
            Log::instance().console("寄存器个数小于等于0", Log::ERROR_LOG);
        }
    } else {
        Log::instance().console("写非四区寄存器", Log::ERROR_LOG);
    }

    return ret;
}


int ModbusStack::writeMbsPack(const MbsWritePack pack)
{
    QMutexLocker locker(&m_writeQueueMutex);

    m_writeQueue.push(pack);

    return pack.regCnt;
}
