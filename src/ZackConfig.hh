// Copyright (c) 2010
// All rights reserved.

#ifndef ZACK_CONFIG_HH
#define ZACK_CONFIG_HH

#include <string>
#include <memory>

#include "soil/Config.hh"

namespace zack {

namespace po = boost::program_options;

class ZackOptions : public soil::Options {
 public:
  ZackOptions();

  virtual ~ZackOptions();

  virtual po::options_description* configOptions();

  int udp_port;
  std::string instrus_filter;
  std::string speed_file;
  std::string log_cfg;

 private:
  boost::program_options::options_description config_options_;
};

class ZackConfig {
 public:
  explicit ZackConfig(int argc = 0, char* argv[] = nullptr);
  ~ZackConfig();

  ZackOptions* zackOptions() {
    return zack_options_.get();
  }

 private:
  std::unique_ptr<ZackOptions> zack_options_;
};

};  // namespace zack

#endif
