

float averager::get(void)
{
  if (m_count)
    return m_total / (float)m_count;
  else
    return 0.0;
}


void averager::clear(void)
{
  m_total = 0.0;
  m_count = 0;
}
  


void averager::add(String &value_str)
{
  m_total += atof(value_str.c_str());
  m_count++;
}


char * averager::get_as_str(void)
{
  float   f = get();
  int     exp = 0;
  boolean err = false;
  boolean negative = false;
  
  if (f < 0.0)
  {
    negative = true;
    f = 0.0 - f;
  }
  
  while ((((int)f) > 9999) && !err)
  {
    f /= 10.0;
    exp++;
    if (exp > 15)
      err = true;
  }
  while ((((int)f) < 1000) && !err)
  {
    f *= 10.0;
    exp--;
    if (exp < -15)
      err = true;
  }
  
  exp += 3;
  
  if (!err)
  {
    int i = (int) f;
    int n = 0;
    
    if (negative) m_str[n++] = '-';
    m_str[n++] = '0' + (i / 1000);    
    m_str[n++] = '.';
    m_str[n++] = '0' + ((i % 1000) / 100);    
    m_str[n++] = '0' + ((i % 100) / 10);    
    m_str[n++] = '0' + (i % 10);
    m_str[n++] = 'e';
    if (exp < 0)
    {
      m_str[n++] = '-';
      exp = 0 - exp;
    }
    else
    {
      m_str[n++] = '+';
    }
    if (exp / 10)
      m_str[n++] = '0' + (exp / 10);
    m_str[n++] = '0' + (exp % 10);
    m_str[n++] = 0;
  }
  else
  {
    m_str[0] = '0';
    m_str[1] = 0;
  }
  
  return m_str;
}


