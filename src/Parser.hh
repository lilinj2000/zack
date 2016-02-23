#ifndef ZACK_PARSER_HH
#define ZACK_PARSER_HH

#include "FtdcDef.hh"

namespace zack
{

class Parser
{
 public:
  Parser(char *buf, int len);

  ~Parser();

  std::string instru();

  std::string updateTime();

  int updateMillisec();
  
  void output();

 protected:
  
  void checkHeader();

  void parseField();


 private:

  char* buf_;
  int len_;
  
  TFTDCHeader header_;

  CMarketDataUpdateTimeField update_time_field_;
  bool update_time_field_exist_;
  
  CMarketDataBestPriceField  best_price_field_;
  bool best_price_field_exist_;
  
  CMarketDataStaticField     static_field_;
  bool static_field_exist_;
  
  CMarketDataLastMatchField  last_match_field_;
  bool last_match_field_exist_;
  
  CFTDMarketDataBaseField    base_field_;
  bool base_field_exist_;
  
  CMarketDataAsk23Field ask23_field_;
  bool ask23_field_exist_;
  
  CMarketDataAsk45Field ask45_field_;
  bool ask45_field_exist_;
  
  CMarketDataBid23Field bid23_field_;
  bool bid23_field_exist_;
  
  CMarketDataBid45Field bid45_field_;
  bool bid45_field_exist_;
  
};

};

#endif
