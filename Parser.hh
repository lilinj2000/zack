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
  
 protected:
  
  void checkHeader();

  void parseField();

 private:

  char* buf_;
  int len_;
  
  TFTDCHeader header_;
  
  CMarketDataUpdateTimeField update_time_field_;
  CMarketDataBestPriceField  best_price_field_;
  CMarketDataStaticField     static_field_;
  CMarketDataLastMatchField  last_match_field_;
  CFTDMarketDataBaseField    base_field_;
  CMarketDataAsk23Field ask23_field_;
  CMarketDataAsk45Field ask45_field_;
  CMarketDataBid23Field bid23_field_;
  CMarketDataBid45Field bid45_field_;

  
};

};

#endif
