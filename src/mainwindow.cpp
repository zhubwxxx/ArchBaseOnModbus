#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "modbustable.h"
#include "modbusstack.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ModbusTable table = ModbusTable::Instance();

    foreach (ModbusUnit *unit, table.getModbusAddrVec()) {
        qDebug()<< "type:" << unit->getType() << " addr:" << unit->getAddr() << " data:" << unit->getData();
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    ModbusStack *stack = ModbusStack::Instance();
    MbsWritePack pack;
    pack.regAddr = 1;
    pack.regCnt = 4;

    for (int i = 0; i < pack.regCnt; i++) {
        pack.data[i] = i + 1;
    }

    stack->writeMbsPack(pack);
}
