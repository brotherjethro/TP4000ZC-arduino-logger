#include "averager.h"

class data_file
{
  public:
    data_file(void)
    {
      m_started = false;
      m_base_unixtime = 0;
      m_header_written = false;
      m_last_record_written_ms = 0;
      m_min_interval_ms = 0;
    };
   
    void start(void);
   
    void stop(void)
    {
      m_started = false;
    };
   
    boolean is_started(void)
    {
      return m_started;
    };
   
    void new_dmm_data(int * pdata);
    
    void set_min_interval(long ms)
    {
      m_min_interval_ms = ms;
    };
    
  private:
    boolean   m_started;
    char      m_dirname[10];             // "/yyyymmdd"
    char      m_fullpath[21];            // "/yyyymmdd/hhmmss.csv"
    uint32_t  m_base_unixtime;           // unixtime when file started
    boolean   m_header_written;
    long      m_last_record_written_ms;
    long      m_min_interval_ms;
    
    class averager m_averager;    
    
    void make_timestamp(String &s);
};
