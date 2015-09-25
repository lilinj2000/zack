#include "ZackConfig.hh"
#include "ZackLog.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace zack
{

ZackOptions::ZackOptions():
    config_options_("FlashTCPConfigOptions")
{

  namespace po = boost::program_options;

  config_options_.add_options()
      ("zack.udp_port", po::value<int>(&udp_port), 
       "udp port")
      ("zack.instru", po::value<std::string>(&instru), 
       "instrument")

      ("zack.log_cfg", po::value<std::string>(&log_cfg), 
       "log config file")
      ;

  return;
  
}

ZackOptions::~ZackOptions()
{
}

po::options_description* ZackOptions::configOptions()
{
  return &config_options_;
}

ZackConfig::ZackConfig(int argc, char* argv[])
{
  zack_options_.reset(new ZackOptions());
      
  std::auto_ptr<soil::Config> config( soil::Config::create() );
  config->registerOptions( zack_options_.get() );

  config->configFile() = "zack.cfg";
  config->loadConfig(argc, argv);
  
  // init the log
  ZACK_LOG_INIT( zack_options_->log_cfg );
  
  return;
}

ZackConfig::~ZackConfig()
{
}

};  
