#ifndef LOG_H
#define LOG_H

#include "singleton.h"
#include <qdir.h>
#include <QDate>
#include <qtextstream.h>

#include <qdebug.h>

const QString print_level[] = {
    "DEBUG    ",
    "INFO     ",
    "WARNING  ",
    "ERROR    ",
    "NONE     "
};
const QString print_level_temp[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "NONE"
};

class Log
{

public:
    enum PRINT_LEVEL {
        DEBUG_LOG,
        INFO_LOG,
        WARNING_LOG,
        ERROR_LOG,
        NONE_LOG
    };
    Log();
    int log_level;
    static Log& instance()
    {
        return Singleton<Log>::instance();
    }

    void console(QString msg, PRINT_LEVEL level = DEBUG_LOG);
    void DeleteOldLog();
    void SaveStr2File(QString& Str);

private:
    QString m_logPath;
    QDir m_dir;
};


#endif // LOG_H
