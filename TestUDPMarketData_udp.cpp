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

//�ڴ���sockaddrʱ��ʹ�õı�ʾ���ȵ����Ͳ�ͬ gyd 20020605 �� UNIX��ΪLINUX

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


	struct sockaddr_in servaddr; //IPv4�׽ӿڵ�ַ����
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //����UDP�׽���
	/* set reuse and non block for this socket */
	int son=1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&son, sizeof(son));
#if defined(HP_UX) || defined(AIX)
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&son, sizeof(son));
#endif

	memset(&servaddr, 0, sizeof(servaddr)); //��ַ�ṹ����
	servaddr.sin_family = AF_INET; //IPv4Э��
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//�ں�ָ����ַ
	servaddr.sin_port = htons(port); //�˿�

	//����Э���ַ,�󶨶˿�
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
	BYTE	Version;			/**< �汾��	1	�������޷���������Ŀǰ�汾Ϊ1*/
	BYTE	Chain;				/**< ������	1	ASCII���ַ���*/
	WORD	SequenceSeries;		/**< ��������	2	�������޷��Ŷ�������*/
	DWORD	TransactionId;		/**<��TID��	FTD��Ϣ��������	4	�������޷���������*/
	DWORD	SequenceNumber;		/**<��SeqNo��	���к�	4	�������޷���������*/
	WORD	FieldCount;			/**< ����������	2	�������޷��Ŷ�������*/
	WORD	FTDCContentLength;	/**< FTDC��Ϣ���ĳ���	2	�������޷��Ŷ����������ֽ�Ϊ��λ��*/
	DWORD	RequestId;			/**< ������(�ɷ���������ά����Ӧ���л����)  4 �������޷���������*/
};

struct TFieldHeader
{
	WORD FieldID;
	WORD Size;
};

#pragma pack(push)
#pragma pack(1)
///�������ʱ������
class CMarketDataUpdateTimeField
{
public:
	///��Լ����
	char InstrumentID[31];
	///����޸�ʱ��
	char UpdateTime[9];
	///����޸ĺ���
	int	UpdateMillisec;
	///ҵ������
	char ActionDay[9];
};

///�������ż�����
class CMarketDataBestPriceField
{
public:
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
};

class CMarketDataStaticField
{
public:
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///������
	double	ClosePrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///�����
	double	SettlementPrice;
	///����ʵ��
	double	CurrDelta;
};

class CMarketDataLastMatchField
{
public:
	///���¼�
	double	LastPrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ֲ���
	double	OpenInterest;
};

///�����������������
class CMarketDataBid23Field
{
public:
	///����۶�
	double	BidPrice2;
	///��������
	int	BidVolume2;
	///�������
	double	BidPrice3;
	///��������
	int	BidVolume3;
};


///������������������
class CMarketDataAsk23Field
{
public:
	///�����۶�
	double	AskPrice2;
	///��������
	int	AskVolume2;
	///��������
	double	AskPrice3;
	///��������
	int	AskVolume3;
};

///���������ġ�������
class CMarketDataBid45Field
{
public:
	///�������
	double	BidPrice4;
	///��������
	int	BidVolume4;
	///�������
	double	BidPrice5;
	///��������
	int	BidVolume5;
};

///���������ġ�������
class CMarketDataAsk45Field
{
public:
	///��������
	double	AskPrice4;
	///��������
	int	AskVolume4;
	///��������
	double	AskPrice5;
	///��������
	int	AskVolume5;
};
class CFTDMarketDataBaseField
{
public:
         //������
         char TradingDay[9];
         //�ϴν����
         double PreSettlementPrice;
         //������
         double PreClosePrice;
         //��ֲ���
         double PreOpenInterest;
         //����ʵ��
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

	///У��һ�°��Ƿ���ȷ
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
	///����Field
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
