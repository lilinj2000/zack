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

  std::auto_ptr<ZackConfig> config_;

  int sock_fd_;

  std::set<std::string> instrus_filter_;

  std::auto_ptr<air::MDataFile> md_file_;
};

};

#endif
