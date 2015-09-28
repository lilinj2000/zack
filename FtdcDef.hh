#ifndef ZACK_FTDC_DEF_HH
#define ZACK_FTDC_DEF_HH

#include <string>
#include <ostream>

namespace zack
{

typedef unsigned int   DWORD;
typedef unsigned short WORD;
typedef unsigned int   UNINT32;
typedef unsigned char  BYTE;

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, TFTDCHeader const& aFTDCHeader)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"TFTDCHeader\": {" <<std::endl;
  os <<"        \"Version\": \"" <<aFTDCHeader.Version  <<"\"," <<std::endl;
  os <<"        \"Chain\": \"" <<aFTDCHeader.Chain  <<"\"," <<std::endl;
  os <<"        \"SequenceSeries\": \"" <<aFTDCHeader.SequenceSeries  <<"\"," <<std::endl;
  os <<"        \"TransactionId\": \"" <<aFTDCHeader.TransactionId  <<"\"," <<std::endl;
  os <<"        \"SequenceNumber\": \"" <<aFTDCHeader.SequenceNumber  <<"\"," <<std::endl;
  os <<"        \"FieldCount\": \"" <<aFTDCHeader.FieldCount  <<"\"," <<std::endl;
  os <<"        \"FTDCContentLength\": \"" <<aFTDCHeader.FTDCContentLength  <<"\"," <<std::endl;
  os <<"        \"RequestId\": \"" <<aFTDCHeader.RequestId  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

struct TFieldHeader
{
  WORD FieldID;
  WORD Size;
};

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, TFieldHeader const& aFieldHeader)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"TFieldHeader\": {" <<std::endl;
  os <<"        \"FieldID\": \"" <<aFieldHeader.FieldID  <<"\"," <<std::endl;
  os <<"        \"Size\": \"" <<aFieldHeader.Size  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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
template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataUpdateTimeField const& aMarketDataUpdateTimeField)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataUpdateTimeField\": {" <<std::endl;
  os <<"        \"InstrumentID\": \"" <<aMarketDataUpdateTimeField.InstrumentID  <<"\"," <<std::endl;
  os <<"        \"UpdateTime\": \"" <<aMarketDataUpdateTimeField.UpdateTime  <<"\"," <<std::endl;
  os <<"        \"UpdateMillisec\": \"" <<aMarketDataUpdateTimeField.UpdateMillisec  <<"\"," <<std::endl;
  os <<"        \"ActionDay\": \"" <<aMarketDataUpdateTimeField.ActionDay  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataBestPriceField const& aMarketDataBestPriceField)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataBestPriceField\": {" <<std::endl;
  os <<"        \"BidPrice1\": \"" <<aMarketDataBestPriceField.BidPrice1  <<"\"," <<std::endl;
  os <<"        \"BidVolume1\": \"" <<aMarketDataBestPriceField.BidVolume1  <<"\"," <<std::endl;
  os <<"        \"AskPrice1\": \"" <<aMarketDataBestPriceField.AskPrice1  <<"\"," <<std::endl;
  os <<"        \"AskVolume1\": \"" <<aMarketDataBestPriceField.AskVolume1  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataStaticField const& aMarketDataStaticField)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataStaticField\": {" <<std::endl;
  os <<"        \"OpenPrice\": \"" <<aMarketDataStaticField.OpenPrice  <<"\"," <<std::endl;
  os <<"        \"HighestPrice\": \"" <<aMarketDataStaticField.HighestPrice  <<"\"," <<std::endl;
  os <<"        \"LowestPrice\": \"" <<aMarketDataStaticField.LowestPrice  <<"\"," <<std::endl;
  os <<"        \"ClosePrice\": \"" <<aMarketDataStaticField.ClosePrice  <<"\"," <<std::endl;
  os <<"        \"UpperLimitPrice\": \"" <<aMarketDataStaticField.UpperLimitPrice  <<"\"," <<std::endl;
  os <<"        \"LowerLimitPrice\": \"" <<aMarketDataStaticField.LowerLimitPrice  <<"\"," <<std::endl;
  os <<"        \"SettlementPrice\": \"" <<aMarketDataStaticField.SettlementPrice  <<"\"," <<std::endl;
  os <<"        \"CurrDelta\": \"" <<aMarketDataStaticField.CurrDelta  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataLastMatchField const& aMarketDataLastMatchField)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataLastMatchField\": {" <<std::endl;
  os <<"        \"LastPrice\": \"" <<aMarketDataLastMatchField.LastPrice  <<"\"," <<std::endl;
  os <<"        \"Volume\": \"" <<aMarketDataLastMatchField.Volume  <<"\"," <<std::endl;
  os <<"        \"Turnover\": \"" <<aMarketDataLastMatchField.Turnover  <<"\"," <<std::endl;
  os <<"        \"OpenInterest\": \"" <<aMarketDataLastMatchField.OpenInterest  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataBid23Field const& aMarketDataBid23Field)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataBid23Field\": {" <<std::endl;
  os <<"        \"BidPrice2\": \"" <<aMarketDataBid23Field.BidPrice2  <<"\"," <<std::endl;
  os <<"        \"BidVolume2\": \"" <<aMarketDataBid23Field.BidVolume2  <<"\"," <<std::endl;
  os <<"        \"BidPrice3\": \"" <<aMarketDataBid23Field.BidPrice3  <<"\"," <<std::endl;
  os <<"        \"BidVolume3\": \"" <<aMarketDataBid23Field.BidVolume3  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}


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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataAsk23Field const& aMarketDataAsk23Field)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataAsk23Field\": {" <<std::endl;
  os <<"        \"AskPrice2\": \"" <<aMarketDataAsk23Field.AskPrice2  <<"\"," <<std::endl;
  os <<"        \"AskVolume2\": \"" <<aMarketDataAsk23Field.AskVolume2  <<"\"," <<std::endl;
  os <<"        \"AskPrice3\": \"" <<aMarketDataAsk23Field.AskPrice3  <<"\"," <<std::endl;
  os <<"        \"AskVolume3\": \"" <<aMarketDataAsk23Field.AskVolume3  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataBid45Field const& aMarketDataBid45Field)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataBid45Field\": {" <<std::endl;
  os <<"        \"BidPrice4\": \"" <<aMarketDataBid45Field.BidPrice4  <<"\"," <<std::endl;
  os <<"        \"BidVolume4\": \"" <<aMarketDataBid45Field.BidVolume4  <<"\"," <<std::endl;
  os <<"        \"BidPrice5\": \"" <<aMarketDataBid45Field.BidPrice5  <<"\"," <<std::endl;
  os <<"        \"BidVolume5\": \"" <<aMarketDataBid45Field.BidVolume5  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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

template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CMarketDataAsk45Field const& aMarketDataAsk45Field)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CMarketDataAsk45Field\": {" <<std::endl;
  os <<"        \"AskPrice4\": \"" <<aMarketDataAsk45Field.AskPrice4  <<"\"," <<std::endl;
  os <<"        \"AskVolume4\": \"" <<aMarketDataAsk45Field.AskVolume4  <<"\"," <<std::endl;
  os <<"        \"AskPrice5\": \"" <<aMarketDataAsk45Field.AskPrice5  <<"\"," <<std::endl;
  os <<"        \"AskVolume5\": \"" <<aMarketDataAsk45Field.AskVolume5  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}

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
template< typename CharT, typename TraitsT >
std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, CFTDMarketDataBaseField const& aMarketDataBaseField)
{
  os <<std::endl;
  os <<"{" <<std::endl;
  os <<"    \"CFTDMarketDataBaseField\": {" <<std::endl;
  os <<"        \"TradingDay\": \"" <<aMarketDataBaseField.TradingDay  <<"\"," <<std::endl;
  os <<"        \"PreSettlementPrice\": \"" <<aMarketDataBaseField.PreSettlementPrice  <<"\"," <<std::endl;
  os <<"        \"PreClosePrice\": \"" <<aMarketDataBaseField.PreClosePrice  <<"\"," <<std::endl;
  os <<"        \"PreOpenInterest\": \"" <<aMarketDataBaseField.PreOpenInterest  <<"\"," <<std::endl;
  os <<"        \"PreDelta\": \"" <<aMarketDataBaseField.PreDelta  <<std::endl;
  os <<"    }" <<std::endl;
  os <<"}" <<std::endl;

}



#pragma pack(pop)

};

#endif
