#include "comm.h"



Comm::Comm()
{

}


void setUTF8Code()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}
