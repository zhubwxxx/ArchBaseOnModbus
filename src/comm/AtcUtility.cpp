#include "AtcUtility.h"

#include <stdio.h>

#include <QTime>
#include <QCoreApplication>

namespace AtcUtility
{
    UInt8 hex2bcd8(UInt8 hex)
    {
        UInt8 ret = 0,retTemp = 0;
        ret = hex%10;
        retTemp = (hex/10)<<4;
        return ret + retTemp;
    }

    UInt8 bcd2hex8(UInt8 bcd)
    {
        return ((bcd&0xf) + ((bcd&0xf0)>>4)*10);
    }


    // 获得小端值
    UInt16 getLittleValue(UInt16 bigValue)
    {
        UInt16 littleValue = 0;
        littleValue = (bigValue&0x00ff)<<8|(bigValue&0xff00)>>8;
        return littleValue;
    }

    //=========================================
    // 获得大端值
    UInt16 getBigValue(UInt16 littleValue)
    {
        UInt16 bigValue = 0;
        bigValue = (littleValue&0x00ff)<<8|(littleValue&0xff00)>>8;
        return bigValue;
    }

	string trim(const string& str)
	{
		string::size_type pos = str.find_first_not_of(' ');
		if (pos == string::npos)
		{
			return str;
		}
		string::size_type pos2 = str.find_last_not_of(' ');
		if (pos2 != string::npos)
		{
			return str.substr(pos, pos2 - pos + 1);
		}
		return str.substr(pos);
	}

	int split(const string& str, vector<string>& ret_, string sep/* = ","*/)
	{
		if (str.empty())
		{
			return 0;
		}

		string tmp;
		string::size_type pos_begin = str.find_first_not_of(sep);
		string::size_type comma_pos = 0;

		while (pos_begin != string::npos)
		{
			comma_pos = str.find(sep, pos_begin);
			if (comma_pos != string::npos)
			{
				tmp = str.substr(pos_begin, comma_pos - pos_begin);
				pos_begin = comma_pos + sep.length();
			}
			else
			{
				tmp = str.substr(pos_begin);
				pos_begin = comma_pos;
			}

			if (!tmp.empty())
			{
				ret_.push_back(tmp);
				tmp.clear();
			}
		}
		return 0;
	}

	string replace(const string& str, const string& src, const string& dest)
	{
		string ret;

		string::size_type pos_begin = 0;
		string::size_type pos       = str.find(src);
		while (pos != string::npos)
		{
			cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
			ret.append(str.data() + pos_begin, pos - pos_begin);
			ret += dest;
			pos_begin = pos + 1;
			pos       = str.find(src, pos_begin);
		}
		if (pos_begin < str.length())
		{
			ret.append(str.begin() + pos_begin, str.end());
		}
		return ret;
	}

	string UTF8ToGBK(const string& strUTF8)
	{

	}

	vector<string> split(string str, string separator)  
	{  
		vector<string> result;  
		int cutAt;  
		while( (cutAt = str.find_first_of(separator)) != str.npos )  
		{  
			if(cutAt > 0)  
			{  
				result.push_back(str.substr(0, cutAt));  
			}else{  
				result.push_back("");  
			}  
			str = str.substr(cutAt + 1);  
		}  
		if(str.length() > 0)  
		{  
			result.push_back(str);  
		}else{  
			result.push_back("");  
		}  
		return result;  
	}  

    UInt8 GetBitValue8(UInt8 val,UInt8 pos)
    {
        val = val>>pos;
        val = val&0x01;
        return val;
    }

	// 将16进制数转字符串
	string byteToHexStr(char* str,UInt16 len)
	{
		string hexstr="";
		for (int i=0;i<len;i++)
		{
			char hex1;
			char hex2;
			int value=(UInt8)*(str+i); //直接将unsigned char赋值给整型的值，系统会正动强制转换
			int v1=value/16;
			int v2=value % 16;

			//将商转成字母
			if (v1>=0&&v1<=9)
				hex1=(char)(48+v1);
			else
				hex1=(char)(55+v1);

			//将余数转成字母
			if (v2>=0&&v2<=9)
				hex2=(char)(48+v2);
			else
				hex2=(char)(55+v2);

			//将字母连接成串
			hexstr += hex1;
			hexstr += hex2;
		}
		return hexstr;

	}


    // 将16进制数转字符串
    string VectorToHexStr(std::vector<UInt8>str)
    {
        string hexstr="";
        for (int i=0;i<str.size();i++)
        {
            char hex1;
            char hex2;
            int value=(UInt8)(str[i]); //直接将unsigned char赋值给整型的值，系统会正动强制转换
            int v1=value/16;
            int v2=value % 16;

            //将商转成字母
            if (v1>=0&&v1<=9)
                hex1=(char)(48+v1);
            else
                hex1=(char)(55+v1);

            //将余数转成字母
            if (v2>=0&&v2<=9)
                hex2=(char)(48+v2);
            else
                hex2=(char)(55+v2);

            //将字母连接成串
            hexstr += hex1;
            hexstr += hex2;
            hexstr += " ";
        }
        return hexstr;

    }


	// 32位高低转换
	UInt32 htonl32(UInt32 A) 
	{
		UInt32 ret = 0;
		ret |= (A&0xFF000000) >> 24; 
		ret |= (A&0x00FF0000) >> 8;
		ret |= (A&0x0000FF00) << 8; 
		ret |= (A&0x000000FF) << 24; 
		return ret;
	}	



	// 16位高低转换
	UInt16 htonl16(UInt16 A) 
	{
		UInt16 ret = 0;
		ret |= (A&0xFF00) >> 8;
		ret |= (A&0x00FF) << 8; 
		return ret;
	}

	//string转为小写
	inline void str2lower(string &str)  
	{  
		transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);  
	} 
	//string转为大写
	inline void str2upper(string &str)  
	{  
		transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);  
	} 


	void bcd2str(const unsigned char* BCD,size_t bcdLen,char* str,size_t strLen)
	{
		char ch[16];
		size_t i;
		str[0] = '\0';
		for (i=0;i<bcdLen;i++)
		{
            snprintf(ch,sizeof(ch),"%02x",BCD[i]);
            strcat(str,ch);
		}
	}

	void bcd2str(const unsigned char* BCD,size_t bcdLen,string &str)
	{
		str = "";
		char ch[16];
        for (int n=0;n<bcdLen;n++)
		{
            snprintf(ch,sizeof(ch),"%02x",BCD[n]);
			str += ch;
		}
	}

	int str2bcd(const char* str,size_t strLen,unsigned char* BCD,size_t bcdLen)
	{
		size_t i;
		if (strLen%2==1)
		{
			return -1;
		}
		if (bcdLen!=strLen/2)
		{
			return -1;
		}
		for (i=0;i<strLen;i+=2)
		{
            std::string s = string(str+i,2);
            QString qs = s.c_str();
            BCD[i/2] = hex2bcd8(qs.toInt());
		}

		return 0;
	}

	//判断字符串是否是有字母和数字组成，含其它字符则返回false
	bool isLetterOrNumber(const string& str)
	{
		for (int i=0;i<str.size();i++)
		{
			char ch = str.data()[i];
			if (ch>='0' && ch<='9')
			{
				continue;
			}

			if (ch>='a' && ch<='z')
			{
				continue;
			}

			if (ch>='A' && ch<='Z')
			{
				continue;
			}

			return false;
		}

		return true;
	}


	
/*******************************************************************************
  * @函数名称	UpdateCRC16
  * @函数说明   更新输入数据的ＣＲＣ校验
  * @输入参数   crcIn
                byte
  * @输出参数   无
  * @返回参数   ＣＲＣ校验值
*******************************************************************************/
UInt16 UpdateCRC16(UInt16 crcIn, UInt8 byte)
{
    UInt32 crc = crcIn;
    UInt32 in = byte|0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if (in&0x100)
            ++crc;
        if (crc&0x10000)
            crc ^= 0x1021;
    }
    while (!(in&0x10000));
    return crc&0xffffu;
}

/*******************************************************************************
  * @函数名称	UpdateCRC16
  * @函数说明   更新输入数据的ＣＲＣ校验
  * @输入参数   data ：数据
                size ：长度
  * @输出参数   无
  * @返回参数   ＣＲＣ校验值
*******************************************************************************/
UInt16 Cal_CRC16(const UInt8* data, UInt32 size)
{
    UInt32 crc = 0;
    const UInt8* dataEnd = data+size;
    while (data<dataEnd)
        crc = UpdateCRC16(crc,*data++);

    crc = UpdateCRC16(crc,0);
    crc = UpdateCRC16(crc,0);
    return crc&0xffffu;
}
//////////////////////////////////////////////以下是加密和解密/////////////////////////////////////////////
const UInt8 secret[]=
{
	0x51,0x27,0x45,0xC4,0x86,0x85,0xD2,0x2C,0x4D,0xA8,
	0xCB,0xD2,0x09,0xB0,0x08,0xE2,0xE1,0x0F,0x3D,0x12,
	0xAC,0xAB,0x0B,0xC5,0x98,0x4D,0x24,0x01,0xDE,0xAF,
	0x15,0xE8,0x8B,0x00,0x85,0xAA,0x0B,0xF7,0x7E,0xEC,
	0x48,0xB2,0x22,0x0A,0x02,0x9C,0xAC,0x26,0x3B,0x0B,
	0x7F,0x2C,0x25,0x0F,0xD5,0x2A,0xE0,0x86,0x3C,0xAE,
	0x59,0xAF,0x66,0x84,0xA4,0x8C,0xC1,0x45,0x44,0xBF,
	0x13,0xAE,0x1F,0x15,0xF9,0x39,0xA4,0x05,0x64,0x0B,
	0xC1,0x9A,0x7B,0xF6,0xC9,0x8D,0x78,0xB4,0x15,0xA0,
	0xC0,0xC0,0x33,0xB3,0xA3,0xD7,0x94,0x63,0x86,0xFC,
	0x63,0x70,0xDF,0x16,0x6F,0x43,0x50,0x6F,0xF5,0xDC,
	0xEB,0x6E,0x2D,0x80,0x9E,0x66,0xFB,0xC6,0x22,0x2A,
	0x2B,0x0D,0x29,0xB7,0x5E,0x80,0xDD,0x0C,0x7E,0xEA,
	0x74,0x1C,0xFC,0xFB,0x45,0x65,0x49,0x48,0x1F,0x19,
	0x00,0x4E,0xF0,0xD1,0xD9,0x63,0x56,0xB8,0x3A,0xA2,
	0xDA,0xF0,0xE1,0x9A,0xE3,0x30,0x78,0x08,0xB1,0x04,
	0xFB,0x96,0x9F,0x5C,0x84,0xD2,0xF6,0x21,0x0C,0x2D,
	0x3A,0x3E,0x16,0x63,0x17,0xF8,0x7A,0xF0,0x13,0x74,
	0x3B,0x96,0xA7,0x94,0xFD,0x87,0x84,0xCC,0xAA,0xEB,
	0x0F,0x98,0x16,0x09,0x49,0x1E,0xEA,0x8A,0x12,0x98,
	0xA5,0x9E,0xFC,0xA3,0xA0,0xE8,0xB6,0x29,0x00,0x48,
	0x74,0x2C,0x8F,0x77,0xA5,0x8B,0x8E,0x04,0x9C,0x09,
	0x3C,0x9C,0x65,0x0E,0x67,0x13,0x50,0x9B,0x44,0xF0,
	0x09,0x2D,0xFA,0x90,0x5B,0x67,0x79,0x92,0xBE,0x71,
	0x6D,0x3B,0xA2,0xFC,0x85,0x06,0xD7,0x89,0xDA,0x18
};

int decryptKey(const UInt8 encryptKey[],const int encryptKeyLen ,UInt8* key,int& keyLen)
{
	keyLen = encryptKeyLen-1;
	memcpy(key,encryptKey,keyLen);//取出除随机码索引的所以密文

	int index = encryptKey[encryptKeyLen-1];//取出随机码索引
	//对加密的报文进行解密
	for (int i=0;i<encryptKeyLen-1;i++)
	{
		key[i] -= secret[index % sizeof(secret)];
		index++;
	}

	UInt16 crc16 = AtcUtility::Cal_CRC16(key,keyLen-2);
	if ( ( ((crc16>>8)&0xFF) == key[keyLen-1] ) && ( (crc16&0xFF) == key[keyLen-2] ) )
	{
		key[keyLen-2]=0;
		key[keyLen-1]=0;
		keyLen -= 2;
		return 0;
	}

	
	memset(key,0,keyLen);
	keyLen = 0;

	return -1;
}

void swapArrayOrder(UInt8* buf,int len)
{
    for(int i=0;i<len/2;i++)
    {
        UInt8 tmp=0;
        tmp = buf[i];
        buf[i] = buf[len-1-i];
        buf[len-1-i] = tmp;
    }
}

void MSleep(int msec)
{
    QDateTime dieTime = QDateTime::currentDateTime().addMSecs(msec);
    while(QDateTime::currentDateTime()<dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

void msleep(UInt32 microseconds)
{

    #ifdef Q_OS_WIN
        MSleep(microseconds);
    #else
        struct timespec ts;
        ts.tv_sec = (microseconds/1000);
        ts.tv_nsec = (microseconds % 1000) * 1000 * 1000;
        while(nanosleep(&ts, NULL) == -1)
        {
            ;
        }
    #endif

}
// -----------------------------------------------------------------------------
// 函数描述：是否超时
// 输入参数: lastTime - 上一次更新时间 timeout-超时时间
// 输出参数: 无
// 返回值：t-超时  f-未超时
// -----------------------------------------------------------------------------
bool IsTimeout(QDateTime lastTime,UInt32 timeout)
{
    UInt32 retMs = 0;
    QDateTime time_current = QDateTime::currentDateTime();
    retMs = lastTime.msecsTo(time_current);

    if(retMs > timeout) {
        return true;
    } else {
        return false;
    }
}

clock_t GetCurrentClock(void)
{
    clock_t now;

#ifdef Q_OS_WIN

#endif

#ifdef Q_OS_LINUX
    struct tms  tmsnow;
    now = times(&tmsnow);
#else
    now = clock();
#endif

    return now;
}

bool IsTimeout(clock_t starttime,UInt32 ms)
{
    clock_t now = GetCurrentClock();

    double ts = (double)(now-starttime)/(double)CPU_CLK_TCK;

    if((ts*1000) > ms)
    {
        return true;
    }
    else
    {
        return false;
    }
}

}
