#include "Parser.hh"
#include "FtdcDef.hh"
#include "ZackLog.hh"

#include <stdexcept>

namespace zack
{

Parser::Parser(char* buf, int len):
    buf_(buf), len_(len),
    update_time_field_exist_(false),
    best_price_field_exist_(false),
    static_field_exist_(false),
    last_match_field_exist_(false),
    base_field_exist_(false),
    ask23_field_exist_(false),
    ask45_field_exist_(false),
    bid23_field_exist_(false),
    bid45_field_exist_(false)
{
  ZACK_TRACE <<"Parser::Parser()";

  ZACK_DEBUG <<"len_: " <<len_;

  checkHeader();

  parseField();
  
}

Parser::~Parser()
{
  ZACK_TRACE <<"Parser::~Parser()";
}

void Parser::checkHeader()
{
  ZACK_TRACE <<"Parser::checkHeader";

  std::stringstream err_stream;

  try
  {
    if( len_<sizeof(TFTDCHeader) )
    {
      err_stream << "Invalid Package!";
      
      throw;
    }

    memcpy(&header_, buf_, sizeof(header_));

    if(header_.Chain != 'L' && header_.Chain != 'C')
    {
      err_stream << "Invalid Package! Invalid Header chain - "
                 <<header_.Chain;
      throw;
    }

    if(header_.FTDCContentLength != (len_ - sizeof(TFTDCHeader) ))
    {
      err_stream <<"Invalid Package Length!";
      err_stream <<"Content Len: " <<header_.FTDCContentLength
                 <<" Expected Len: " <<len_ - sizeof(TFTDCHeader);
      throw;
    }

    if(header_.TransactionId != 0x0000F103)
    {
      err_stream <<"Not a Market Package!";
      throw;
    }
  }
  catch( ... )
  {
    throw std::runtime_error( err_stream.str() );
  }
}

void Parser::parseField()
{
  ZACK_TRACE <<"Parser::parseField()";
  
  TFieldHeader field_header;
  
  char* pz = buf_ + sizeof(TFTDCHeader);
  for(int i=0; i<header_.FieldCount; i++)
  {
    memcpy(&field_header, pz, sizeof(TFieldHeader));
    pz += sizeof(TFieldHeader);
    
    switch(field_header.FieldID)
    {
      case 0x2439:
        update_time_field_exist_ = true;
        memcpy( &update_time_field_, pz, field_header.Size );

        break;
        
      case 0x2434:
        best_price_field_exist_ = true;
        memcpy(&best_price_field_, pz, field_header.Size);

        break;
        
      case 0x2432:
        static_field_exist_ = true;
        memcpy(&static_field_, pz, field_header.Size);

        break;
        
      case 0x2433:
        last_match_field_exist_ = true;
        memcpy(&last_match_field_, pz, field_header.Size);

        break;
        
      case 0x2435:
        bid23_field_exist_ = true;
        memcpy(&bid23_field_, pz, field_header.Size);

        break;
        
      case 0x2436:
        ask23_field_exist_ = true;
        memcpy(&ask23_field_, pz, field_header.Size);

        break;
        
      case 0x2437:
        bid45_field_exist_ = true;
        memcpy(&bid45_field_, pz, field_header.Size);

        break;
        
      case 0x2438:
        ask45_field_exist_ = true;
        memcpy(&ask45_field_, pz, field_header.Size);

        break;
        
      case 0x2431:
        base_field_exist_ = true;
        memcpy(&base_field_, pz, field_header.Size);
        break;
    }
    pz +=  field_header.Size;
    
  }
  
}

std::string Parser::instru()
{
  if( update_time_field_exist_ )
    return update_time_field_.InstrumentID;

  return "";
}

std::string Parser::updateTime()
{
  if( update_time_field_exist_ )
    return update_time_field_.UpdateTime;

  return "";
}

int Parser::updateMillisec()
{
  if( update_time_field_exist_ )
    return update_time_field_.UpdateMillisec;

  return 0;
}

void Parser::output()
{
  ZACK_TRACE <<"Parser::output()";

  ZACK_INFO <<header_;
  
  if( update_time_field_exist_ )
    ZACK_INFO <<update_time_field_;

  if( best_price_field_exist_ )
    ZACK_INFO <<best_price_field_;

  if( static_field_exist_ )
    ZACK_INFO <<static_field_;

  if( last_match_field_exist_ )
    ZACK_INFO <<last_match_field_;

  if( base_field_exist_ )
    ZACK_INFO <<base_field_;

  if( bid23_field_exist_ )
    ZACK_INFO <<bid23_field_;

  if( ask23_field_exist_ )
    ZACK_INFO <<ask23_field_;

  if( bid45_field_exist_ )
    ZACK_INFO <<bid45_field_;

  if( ask45_field_exist_ )
    ZACK_INFO <<ask45_field_;
}

};
