// TestUDPMarketData.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WINDOWS
#include "winsock2.h"
typedef unsigned int uint32_t;
#else
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#endif

#ifdef WIN32
#define GET_LAST_SOCK_ERROR() h_errno
typedef int socklen_t;
#define EWOULDBLOCK	WSAEWOULDBLOCK
#else
#define GET_LAST_SOCK_ERROR() errno
typedef unsigned int   DWORD;
typedef unsigned short WORD;
typedef unsigned int   UNINT32;
typedef unsigned char  BYTE;
#define ioctlsocket ioctl
#endif

bool ParseMarketData(char* buffer,int nCount);

//在传送sockaddr时，使用的表示长度的类型不同 gyd 20020605 将 UNIX变为LINUX

#if defined(LINUX)||defined(AIX)// || defined(HP_UX)
#define SOCKADDRLEN	socklen_t
//#define SOCKADDRLEN	unsigned
#else
#define SOCKADDRLEN	int
#endif

int main(int argc, char* argv[])
{
	/// Create udp client
	if(argc < 2)
	{
		printf("TestUDPMarketData [port]\n");
		exit(1);
	}
	int port = atoi(argv[1]);

#ifdef WIN32
	WSADATA initData;
	initData.wVersion=0;
	initData.wHighVersion=2;
	strcpy(initData.szDescription,"");
	strcpy(initData.szSystemStatus,"");
	initData.iMaxSockets=100;
	initData.lpVendorInfo=0;
	initData.lpVendorInfo="";
	WSAStartup(2,&initData);
	if (WSAStartup(2,&initData)!=0)
		printf("Can not init socket");
#endif


	struct sockaddr_in servaddr; //IPv4套接口地址定义
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //建立UDP套接字
	/* set reuse and non block for this socket */
	int son=1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&son, sizeof(son));
#if defined(HP_UX) || defined(AIX)
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&son, sizeof(son));
#endif

	memset(&servaddr, 0, sizeof(servaddr)); //地址结构清零
	servaddr.sin_family = AF_INET; //IPv4协议
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//内核指定地址
	servaddr.sin_port = htons(port); //端口

	//分配协议地址,绑定端口
	if (bind(sockfd, (sockaddr *) &servaddr, sizeof(servaddr)) != 0)
	{
		printf("bind port fail\n");
		exit(1);
	}

	for (;;)
	{
#ifdef LINUX
		int on=1;
		if (ioctlsocket(sockfd, FIONBIO, (char *)&on) < 0){
#endif
#ifdef WIN32
		unsigned long on_windows=1;
		if (ioctlsocket(sockfd, FIONBIO, &on_windows) < 0){
#endif
#ifdef VOS
		int off=0;
		if (ioctlsocket(sockfd, FIONBIO, (char *)&off) <0){
#endif
		
			if (GET_LAST_SOCK_ERROR() == 4)
				continue;
			printf("Can not set FIONBIO\n");
		}
		else
			break;
	}

	int rcvbufsize = 1*1024*1024;
	SOCKADDRLEN paramlen = sizeof(rcvbufsize);
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char *)&rcvbufsize, paramlen);
	if(ret != 0)
	{
		printf("Can not setsockopt revbuf\n");
	}

#ifdef WIN32
	char on = 1;
#else
	int on = 1;
#endif
	ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	if(ret != 0)
	{
		printf("Can not setsockopt\n");
	}

	int number = 4000;
	char buffer[4096];

	while(1)
	{
		sockaddr_in fromAddr;
		int nFromLen = sizeof(sockaddr_in);
		int nCount = recvfrom(sockfd, buffer, 4000, 0, (sockaddr *)&fromAddr,(socklen_t *) &nFromLen);
		if(nCount == 0)
			continue;
		if (nCount == -1)
		{
			int nErrno = GET_LAST_SOCK_ERROR();
			if (nErrno == 0 || nErrno == 251 || nErrno == EWOULDBLOCK)	/*251 for PARisk */	//20060224 IA64 add 0
				continue;
			else
			{
				printf("Error in recvFrom,ErrNo[%d]\n",nErrno);
				return -1;
			}
		}

		ParseMarketData(buffer,nCount);
	}

	return 0;
}



struct	TFTDCHeader
{
	BYTE	Version;			/**< 版本号	1	二进制无符号整数。目前版本为1*/
	BYTE	Chain;				/**< 报文链	1	ASCII码字符。*/
	WORD	SequenceSeries;		/**< 序列类别号	2	二进制无符号短整数。*/
	DWORD	TransactionId;		/**<（TID）	FTD信息正文类型	4	二进制无符号整数。*/
	DWORD	SequenceNumber;		/**<（SeqNo）	序列号	4	二进制无符号整数。*/
	WORD	FieldCount;			/**< 数据域数量	2	二进制无符号短整数。*/
	WORD	FTDCContentLength;	/**< FTDC信息正文长度	2	二进制无符号短整数。以字节为单位。*/
	DWORD	RequestId;			/**< 请求编号(由发送请求者维护，应答中会带回)  4 二进制无符号整数。*/
};

struct TFieldHeader
{
	WORD FieldID;
	WORD Size;
};

#pragma pack(push)
#pragma pack(1)
///行情更新时间属性
class CMarketDataUpdateTimeField
{
public:
	///合约代码
	char InstrumentID[31];
	///最后修改时间
	char UpdateTime[9];
	///最后修改毫秒
	int	UpdateMillisec;
	///业务日期
	char ActionDay[9];
};

///行情最优价属性
class CMarketDataBestPriceField
{
public:
	///申买价一
	double	BidPrice1;
	///申买量一
	int	BidVolume1;
	///申卖价一
	double	AskPrice1;
	///申卖量一
	int	AskVolume1;
};

class CMarketDataStaticField
{
public:
	///今开盘
	double	OpenPrice;
	///最高价
	double	HighestPrice;
	///最低价
	double	LowestPrice;
	///今收盘
	double	ClosePrice;
	///涨停板价
	double	UpperLimitPrice;
	///跌停板价
	double	LowerLimitPrice;
	///今结算
	double	SettlementPrice;
	///今虚实度
	double	CurrDelta;
};

class CMarketDataLastMatchField
{
public:
	///最新价
	double	LastPrice;
	///数量
	int	Volume;
	///成交金额
	double	Turnover;
	///持仓量
	double	OpenInterest;
};

///行情申买二、三属性
class CMarketDataBid23Field
{
public:
	///申买价二
	double	BidPrice2;
	///申买量二
	int	BidVolume2;
	///申买价三
	double	BidPrice3;
	///申买量三
	int	BidVolume3;
};


///行情申卖二、三属性
class CMarketDataAsk23Field
{
public:
	///申卖价二
	double	AskPrice2;
	///申卖量二
	int	AskVolume2;
	///申卖价三
	double	AskPrice3;
	///申卖量三
	int	AskVolume3;
};

///行情申买四、五属性
class CMarketDataBid45Field
{
public:
	///申买价四
	double	BidPrice4;
	///申买量四
	int	BidVolume4;
	///申买价五
	double	BidPrice5;
	///申买量五
	int	BidVolume5;
};

///行情申卖四、五属性
class CMarketDataAsk45Field
{
public:
	///申卖价四
	double	AskPrice4;
	///申卖量四
	int	AskVolume4;
	///申卖价五
	double	AskPrice5;
	///申卖量五
	int	AskVolume5;
};
class CFTDMarketDataBaseField
{
public:
         //交易日
         char TradingDay[9];
         //上次结算价
         double PreSettlementPrice;
         //昨收盘
         double PreClosePrice;
         //昨持仓量
         double PreOpenInterest;
         //昨虚实度
         double PreDelta;
};


#pragma pack(pop) 

bool ParseMarketData(char* buffer,int nCount)
{
	printf("Receive Data,Len[%d]\n",nCount);
	
	if(nCount < sizeof(TFTDCHeader))
	{
		printf("Invalid Package!\n");
		return false;
	}
	TFTDCHeader header;
	memcpy(&header,buffer,sizeof(header));

	///校验一下包是否正确
	if(header.Chain != 'L' && header.Chain != 'C')
	{
		printf("Invalid Package!,Invalid Header[%c]\n",header.Chain);
		return false;
	}

	if(header.FTDCContentLength != (nCount - sizeof(TFTDCHeader) ))
	{
		printf("Invalid Package Length!\n");
		printf("Content Len[%d],Expected Len[%d]\n",nCount - sizeof(TFTDCHeader));
		return false;
	}

	if(header.TransactionId != 0x0000F103)
	{
		printf("Not a Market Package!\n");
		return false;
	}

	char buf[4096];
	
	CMarketDataUpdateTimeField updateTimeField;
	CMarketDataBestPriceField  bestPriceField;
	CMarketDataStaticField	   staticField;
	CMarketDataLastMatchField  lastMatchField;

	CFTDMarketDataBaseField    databaseField;
     
	CMarketDataAsk23Field ask23Field;
	CMarketDataAsk45Field ask45Field;
	CMarketDataBid23Field bid23Field;
	CMarketDataBid45Field bid45Field;

	TFieldHeader fieldHeader;
	///遍历Field
	char* pz = buffer+sizeof(TFTDCHeader);
	for(int i=0;i<header.FieldCount;i++)
	{
		memcpy(&fieldHeader,pz,sizeof(TFieldHeader));
		pz += sizeof(TFieldHeader);
		switch(fieldHeader.FieldID)
		{
		case 0x2439:
			{
				memcpy(&updateTimeField,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
				printf("Receive Data\n");
				printf("	Instrument[%s]\n",updateTimeField.InstrumentID);
				printf("	Time[%s]\n",updateTimeField.UpdateTime);
				printf("	ms[%d]\n",updateTimeField.UpdateMillisec);
			}
			break;
		case 0x2434:
			{
				memcpy(&bestPriceField,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
				printf("best price\n");
                                printf("         BidPrice1[%f]\n",bestPriceField.BidPrice1);
                                printf("        BidVolume1[%d]\n",bestPriceField.BidVolume1);
                                printf("        AskPrice1[%f]\n",bestPriceField.AskPrice1);
                                printf("        AskVolume1[%d]\n",bestPriceField.AskVolume1);			
			}
			break;
		case 0x2432:
			{
				memcpy(&staticField,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
				printf("staticfield\n");
                                printf("         OpenPrice[%f]\n",staticField.OpenPrice);
                                printf("         HighestPrice[%f]\n",staticField.HighestPrice);
                                printf("         LowestPrice[%f]\n",staticField.LowestPrice);
                                printf("         ClosePrice[%f]\n",staticField.ClosePrice);
                                printf("         UpperLimitPrice[%f]\n",staticField.UpperLimitPrice);
                                printf("         LowerLimitPrice[%f]\n",staticField.LowerLimitPrice);
                                printf("         SettlementPrice[%f]\n",staticField.SettlementPrice);
                                printf("         CurrDelta[%f]\n",staticField.CurrDelta);
			}
			break;
		case 0x2433:
			{
				memcpy(&lastMatchField,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
				printf("lastMatchField\n");
                                printf("         LastPrice[%f]\n",lastMatchField.LastPrice);
                                printf("         Volume[%d]\n",lastMatchField.Volume);
                                printf("         Turnover[%f]\n",lastMatchField.Turnover);
                                printf("         OpenInterest[%f]\n",lastMatchField.OpenInterest);
			}
			break;
		case 0x2435:
			{
				memcpy(&bid23Field,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
			}
			break;
		case 0x2436:
			{
				memcpy(&ask23Field,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
			}
			break;
		case 0x2437:
			{
				memcpy(&bid45Field,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
			}
			break;
		case 0x2438:
			{
				memcpy(&ask45Field,pz,fieldHeader.Size);
				pz +=  fieldHeader.Size;
			}
			break;
		case 0x2431:
                        {

                                memcpy(&databaseField,pz,fieldHeader.Size);
                                pz +=  fieldHeader.Size;
                                printf("Receive Data\n");
                                printf("        Tradingday[%s]\n",databaseField.TradingDay);
                        }
                        break;
		} 
	}

	return true;
}
