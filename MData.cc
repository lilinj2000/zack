#include "MData.hh"

namespace zack
{

MData::MData(const std::string& file)
{
  file_.open( file.data() );
  
  if( !file_.good() )
  {
    throw std::runtime_error("open the data file " + file + " failed.");
  }
}

MData::~MData()
{
  file_.close();
}

void MData::outMData(const std::string& update_time, int update_millisec,
                     const boost::posix_time::ptime& time_stamp)
{

  namespace bg=boost::gregorian;
  namespace bp=boost::posix_time;
  
  bg::date today( bg::day_clock::local_day() );
  bp::ptime start_time(today, bp::duration_from_string(update_time));
  start_time = start_time + bp::milliseconds(update_millisec);

  bp::time_duration delay = time_stamp - start_time;
  
  file_<<update_time <<"." <<std::setfill('0') <<std::setw(3)
       <<update_millisec <<"\t"
       <<start_time <<"\t"
       <<time_stamp <<"\t"
       <<delay.total_microseconds() <<std::endl;
}


};
