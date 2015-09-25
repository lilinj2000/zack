#include "Parser.hh"
#include "ZackLog.hh"

#include <cstdio>
#include <cstdlib>
#include <string>

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

#define GET_LAST_SOCK_ERROR() errno

#include <memory>

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    ZACK_ERROR <<argv[0] <<" [port]";
    exit(1);
  }
  
  int port = atoi(argv[1]);

  struct sockaddr_in servaddr; //IPv4套接口地址定义
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //建立UDP套接字
  /* set reuse and non block for this socket */
  int son=1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&son, sizeof(son));

  memset(&servaddr, 0, sizeof(servaddr)); //地址结构清零
  servaddr.sin_family = AF_INET; //IPv4协议
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//内核指定地址
  servaddr.sin_port = htons(port); //端口

  //分配协议地址,绑定端口
  if (bind(sockfd, (sockaddr *) &servaddr, sizeof(servaddr)) != 0)
  {
    ZACK_ERROR <<"bind port fail.";
    exit(1);
  }

  for (;;)
  {
    int on = 1;
    if (ioctl(sockfd, FIONBIO, (char *)&on) < 0)
    {
      
      if (GET_LAST_SOCK_ERROR() == 4)
        continue;
      
      ZACK_ERROR <<"Can not set FIONBIO";
    }
    else
      break;
  }

  int rcvbufsize = 1*1024*1024;
  socklen_t paramlen = sizeof(rcvbufsize);
  int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char *)&rcvbufsize, paramlen);
  if(ret != 0)
  {
    ZACK_ERROR <<"Can not setsockopt revbuf";
  }

  int on = 1;
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
        ZACK_ERROR <<"Error in recvFrom,ErrNo: " <<nErrno;
        return -1;
      }
    }

    std::auto_ptr<zack::Parser> parser( new zack::Parser(buffer, nCount) );
  }

  return 0;

}
