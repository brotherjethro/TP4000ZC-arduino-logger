class averager
{
  public:
    averager(void)
    {
      clear();
    }
    
    char *   get_as_str(void);
    float    get(void);
    void     clear(void);
    void     add(String &value_str);
    
  private:
    float  m_total;
    long   m_count;
    char   m_str[11];     // "-1.234E+06" 
};


