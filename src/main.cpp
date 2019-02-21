#include "mainwindow.h"
#include <QApplication>

#include "modbusstack.h"
#include "Comm/comm.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setUTF8Code();

    ModbusStack *stack = ModbusStack::Instance();

    MainWindow w;
    w.show();

    return a.exec();
}
