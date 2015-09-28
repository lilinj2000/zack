#ifndef MDATA_HH
#define MDATA_HH

#include <string>
#include <fstream>

#include "boost/date_time.hpp"

namespace zack
{

class MData
{
 public:
  MData(const std::string& file);

  virtual ~MData();

  void outMData(const std::string& update_time, int update_millisec,
                const boost::posix_time::ptime& time_stamp);


 private:
  
  std::ofstream  file_;
};

};

#endif
