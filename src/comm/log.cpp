#include "log.h"


Log::Log()
    : log_level(DEBUG_LOG)
{
#if 0
    m_logPath = QCoreApplication::applicationDirPath()+"/Log/";
    if (!m_dir.exists(m_logPath))
        m_dir.mkdir(m_logPath);
    m_dir.setPath(m_logPath);
    qInstallMsgHandler(LogSaveRegister);

#endif
}

void Log::console(QString msg, PRINT_LEVEL level)
{
    static QString sComp;

    /*防止多次保存相同错误信息*/
    if (sComp != msg){
        sComp = msg;
    } else {
//        Debug << msg;
        return;
    }

    if (log_level <= level) {
        QDateTime dateTime = QDateTime::currentDateTime();
        QString date = dateTime.toString("yyyy-MM-dd hh:mm:ss ");
        QString strType = print_level[level];
        QString str = date+strType;

        str.append(msg);
        qDebug() << str;
    }
}

void Log::DeleteOldLog()
{
    m_dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileList = m_dir.entryInfoList();

    //@1: 至少保留30天日志
    if (fileList.count() <= 30)
        return;

    //@2: 超过30天创建的日志删除
    QDate nowDate = QDate::currentDate();
    foreach (QFileInfo fileInfo, fileList) {
        QDate fileDate = fileInfo.lastModified().date().addDays(30);
        if (fileDate < nowDate)
        {
            //+30day后仍然小于当前时间
            QFile file(fileInfo.filePath());
            file.setPermissions(QFile::WriteOwner);
            qDebug() << fileInfo.fileName() << ":删除老的LOG记录";
            file.remove();
            return;
        }
    }
}

void Log::SaveStr2File(QString& Str)
{
    QString filePath = m_logPath+
            QDate::currentDate().toString("yyyy-MM-dd") + ".txt";

    QFile outFile(filePath);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << Str << endl;
}

