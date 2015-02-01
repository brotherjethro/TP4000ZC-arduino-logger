
void button::begin(int pin)
{
  m_pin = pin;
  pinMode(m_pin, INPUT_PULLUP);
  m_last_down_ms = 0;
  m_last_up_ms = 0;
  m_wait_for_up = false;
  m_pushed = false;
  m_new_push = false;
  m_new_push_at_ms = 0;
}

void button::tick(void)
{
  if (!digitalRead(m_pin))
  {
    m_last_down_ms = millis();
    if ((millis() - m_last_up_ms) >= 100)
    {
      // Button has been pushed long enough:
      m_pushed = true;
      
      if (!m_wait_for_up)
      {
        m_wait_for_up = true;
        m_new_push = true;
        m_new_push_at_ms = millis();
      }
    }
  }
  else
  {
    m_last_up_ms = millis();  // button up
    if ((millis() - m_last_down_ms) >= 333)
    {
      // Button has been up for long enough:
      m_pushed = false;
      m_wait_for_up = false;
    }
    
    // New push expires after one second if nobody clears it:
    if (m_new_push && ((millis() - m_new_push_at_ms) > 1000))
      m_new_push = false;
  }
}

boolean button::is_pushed(void)
{
  return m_pushed;
}

boolean button::is_new_push(void)
{
  return m_new_push;
}

void button::clear_new_push(void)
{
  m_new_push = false;
}


