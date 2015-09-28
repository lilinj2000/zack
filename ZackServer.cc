#include "ZackServer.hh"
#include "ZackLog.hh"
#include "Parser.hh"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#include "boost/tokenizer.hpp"

namespace zack
{

ZackServer::ZackServer(int argc, char* argv[]):
    sock_fd_(-1)
{
  ZACK_TRACE <<"ZackServer::ZackServer()";

  config_.reset( new ZackConfig(argc, argv) );

  boost::tokenizer<> tok( config_->zackOptions()->instrus_filter );
  for( boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end(); ++beg )
  {
    ZACK_DEBUG <<"instrus filter: " <<*beg;
    instrus_filter_.insert(*beg);
  }

  md_file_.reset( new MData(config_->zackOptions()->md_file) );

  go();

}

ZackServer::~ZackServer()
{
  ZACK_TRACE <<"ZackServer::~ZackServer()";
}

void ZackServer::go()
{
  ZACK_TRACE <<"ZackServer::go()";

  setSocket();

  do
  {
    char buf[4096];

    sockaddr_in from_addr;
    socklen_t addrlen = sizeof(from_addr);
    int len = recvfrom( sock_fd_, buf, sizeof(buf), 0,
                        (sockaddr*)&from_addr, &addrlen );
    if( len==0 )
      continue;

    if( len==-1 )
    {
      int error = errno;
      if( error==0 || error==251 || error==EWOULDBLOCK )
        continue;

      outputError();

      throw std::runtime_error("recv error.");
    }

    boost::posix_time::ptime time_stamp = boost::posix_time::microsec_clock::local_time();
    

    std::auto_ptr<Parser> parser( new Parser(buf, len) );

    if( instrus_filter_.count( parser->instru() )>0 )
    {
      parser->output();

      md_file_->outMData( parser->updateTime(), parser->updateMillisec(),
                          time_stamp);
      
    }
    
  }while(true);
  
}

void ZackServer::setSocket()
{
  ZACK_TRACE <<"ZackServer::setSocket()";

  sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if( sock_fd_<0 )
  {
    outputError();
    
    throw std::runtime_error("create socket fialed.");
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0x0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(config_->zackOptions()->udp_port);

  int ret = bind(sock_fd_, (sockaddr *)&servaddr, sizeof(servaddr));
  if( ret!=0 )
  {
    outputError();

    throw std::runtime_error("bind error.");
  }

  int on = 1;
  ret = ioctl(sock_fd_, FIONBIO, (char *)&on);
  if( ret<0 )
  {
    outputError();

    throw std::runtime_error("ioctl failed.");
  }

  int rcvbuf_size = 1*1024*1024;
  socklen_t param_len = sizeof(rcvbuf_size);
  ret = setsockopt(sock_fd_, SOL_SOCKET, SO_RCVBUF, (const char*)&rcvbuf_size, param_len);
  if( ret<0 )
  {
    outputError();

    throw std::runtime_error("setsockoopt SO_RCVBUF failed.");
  }

  on = 1;
  ret = setsockopt(sock_fd_, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
  if( ret<0 )
  {
    outputError();
    throw std::runtime_error("setsockoopt SO_BROADCAST failed.");
  }
  
}

void ZackServer::outputError()
{
  ZACK_TRACE <<"ZackServer::outputError()";

  int error = errno;
  
  ZACK_ERROR <<strerror( error );
}

};
