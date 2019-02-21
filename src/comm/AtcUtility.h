#ifndef _ATC_UTILITY_H
#define _ATC_UTILITY_H

#include "AtcAfx.h"

#include <iostream>
#include <vector>
#include <QTime>
#include <unistd.h>
#ifdef Q_OS_WIN

#include <WinSock2.h>
#include <time.h>
#endif

#ifdef Q_OS_LINUX
#include <sys/times.h>
#endif


#ifdef Q_OS_WIN
#define CPU_CLK_TCK CLOCKS_PER_SEC
#endif

#ifdef Q_OS_LINUX
#define CPU_CLK_TCK sysconf(_SC_CLK_TCK)
#endif

using namespace std;

namespace AtcUtility
{

UInt8 hex2bcd8(UInt8 hex);
UInt8 bcd2hex8(UInt8 bcd);

// 获得小端值
UInt16 getLittleValue(UInt16 bigValue);
UInt16 getBigValue(UInt16 littleValue);
UInt8 GetBitValue8(UInt8 val,UInt8 pos);
string trim(const string& str);
int split(const string& str, vector<string>& ret_, string sep = ",");

string replace(const string& str, const string& src, const string& dest);

template< typename CharT >
inline void TrimLeft(std::basic_string<CharT> &str)
{
	str.erase(0, str.find_first_not_of(' '));
}

template< typename CharT >
inline void TrimRight(std::basic_string<CharT> &str)
{
	str.erase(str.find_last_not_of(' ') + 1);
}

template< typename CharT >
inline void Trim(std::basic_string<CharT> &str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
}

template<typename CharT>
inline void Erase(std::basic_string<CharT> &str, const CharT &charactor)
{
	str.erase(remove_if(str.begin(), str.end(),
		bind2nd(std::equal_to<CharT>(), charactor)), str.end());
}

template<typename CharT>
inline void Replace(std::basic_string<CharT> &str, const std::basic_string<CharT> &strObj, const std::basic_string<CharT> &strDest)
{
	while(true)
	{
		CharT pos = str.find(strObj);

		if( pos != std::basic_string<CharT>::npos )
			str.replace(pos, strObj.size(), strDest);
		else
			break;
	}

}


string UTF8ToGBK(const string& strUTF8);
vector<string> split(string str, string separator) ;
double transDouble(string str) ;
// 将16进制数转字符串
string byteToHexStr(char* str,UInt16 len);
string VectorToHexStr(vector<UInt8>str);
// 32位高低转换
UInt32 htonl32(UInt32 A);	

// 16位高低转换
UInt16 htonl16(UInt16 A);

//string转为小写
inline void str2lower(string &str); 
//string转为大写
inline void str2upper(string &str);

void bcd2str(const unsigned char* BCD,size_t bcdLen,char* str,size_t strLen);
void bcd2str(const unsigned char* BCD,size_t bcdLen,string &str);
int str2bcd(const char* str,size_t strLen,unsigned char* BCD,size_t bcdLen);
//判断字符串是否是有字母和数字组成，含其它字符则返回false
bool isLetterOrNumber(const string& str);

/*******************************************************************************
  * @函数名称	UpdateCRC16
  * @函数说明   更新输入数据的ＣＲＣ校验
  * @输入参数   crcIn
                byte
  * @输出参数   无
  * @返回参数   ＣＲＣ校验值
*******************************************************************************/
UInt16 UpdateCRC16(UInt16 crcIn, UInt8 byte);
/*******************************************************************************
  * @函数名称	UpdateCRC16
  * @函数说明   更新输入数据的ＣＲＣ校验
  * @输入参数   data ：数据
                size ：长度
  * @输出参数   无
  * @返回参数   ＣＲＣ校验值
*******************************************************************************/
UInt16 Cal_CRC16(const UInt8* data, UInt32 size);

int encryptKey(const UInt8 key[],const int keyLen,UInt8* encryptKey,int& encryptKeyLen);
int decryptKey(const UInt8 encryptKey[],const int encryptKeyLen ,UInt8* key,int& keyLen);
//转换字节顺序
void swapArrayOrder(UInt8* buf,int len);

void MSleep(int msec);
void msleep(UInt32 microseconds);

// 是否超时
clock_t GetCurrentClock(void);
bool IsTimeout(QDateTime lastTime,UInt32 timeout);
bool IsTimeout(clock_t starttime,UInt32 ms);

}

#endif
