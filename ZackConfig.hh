#ifndef ZACK_CONFIG_HH
#define ZACK_CONFIG_HH

#include <string>
#include <memory>
#include "soil/Config.hh"

namespace zack
{

namespace po = boost::program_options;

class ZackOptions : public soil::Options
{
 public:

  ZackOptions();
  
  virtual ~ZackOptions();

  virtual po::options_description* configOptions();

  int udp_port;
  
  std::string instru;
  
  std::string log_cfg;

 private:
  boost::program_options::options_description config_options_;
};

class ZackConfig
{
 public:
  
  ZackConfig(int argc=0, char* argv[]=NULL);
  ~ZackConfig();

  ZackOptions* zackOptions()
  {
    return zack_options_.get();
  }


 private:
  std::auto_ptr<ZackOptions> zack_options_;
  
};

}  


#endif 
