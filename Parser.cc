#include "Parser.hh"
#include "FtdcDef.hh"
#include "ZackLog.hh"

#include <stdexcept>

namespace zack
{

Parser::Parser(char* buf, int len):
    buf_(buf), len_(len)
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
          memcpy( &update_time_field_, pz, field_header.Size );

        break;
        
      case 0x2434:
          memcpy(&best_price_field_, pz, field_header.Size);

        break;
        
      case 0x2432:
          memcpy(&static_field_, pz, field_header.Size);

        break;
        
      case 0x2433:
          memcpy(&last_match_field_, pz, field_header.Size);

        break;
        
      case 0x2435:
          memcpy(&bid23_field_, pz, field_header.Size);

        break;
        
      case 0x2436:
          memcpy(&ask23_field_, pz, field_header.Size);

        break;
        
      case 0x2437:
          memcpy(&bid45_field_, pz, field_header.Size);

        break;
        
      case 0x2438:
          memcpy(&ask45_field_, pz, field_header.Size);

        break;
        
      case 0x2431:
          memcpy(&base_field_, pz, field_header.Size);
        break;
    }
    pz +=  field_header.Size;
    
  }
  
}

};
