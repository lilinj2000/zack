#ifndef ZACK_SERVER_HH
#define ZACK_SERVER_HH

#include <memory>

#include "ZackConfig.hh"
#include "air/MDataFile.hh"

namespace zack
{

class ZackServer
{
 public:
  ZackServer(int argc, char *argv[]);

  ~ZackServer();

 protected:

  void go();

  void setSocket();
  
  void outputError();

  air::SpeedMData* toSpeedMData(const std::string& instru,
                                const std::string& update_time,
                                int update_millisec);

  
 private:

  std::unique_ptr<ZackConfig> config_;

  int sock_fd_;

  std::unique_ptr<air::MDataFile> speed_file_;
};

};

#endif
