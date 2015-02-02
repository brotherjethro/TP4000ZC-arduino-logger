void logger_ui::begin(int led_pin)
{
  m_led_pin = led_pin;
  pinMode(m_led_pin, OUTPUT);
  digitalWrite(m_led_pin, 1);
  m_state_changed_ms = millis();
  m_on_dur_ms = 100;
  m_off_dur_ms = 0;
  m_repeat = true;
  m_logging = false;
}

void logger_ui::tick(void)
{
  long delta_ms = millis() - m_state_changed_ms;
  
  if (digitalRead(m_led_pin))
  {
    if (delta_ms >= m_on_dur_ms)
    {
      digitalWrite(m_led_pin, 0);
      m_state_changed_ms = millis();
    }
  }
  else
  {
    if ((delta_ms >= m_off_dur_ms) && m_repeat)
    {
      digitalWrite(m_led_pin, 1);
      m_state_changed_ms = millis();
    }
  }
 
  if (m_error)
    return;
    
  // now look at button:
  if (start_stop_button.is_new_push())
  {
    start_stop_button.clear_new_push();
    user_event_button.clear_new_push();  // clean up any old ones
    
    m_logging = !m_logging;
    if (m_logging)
    {
      my_data_file.start();
      digitalWrite(m_led_pin, 0);
      m_state_changed_ms = millis();
      m_on_dur_ms = 0;
      m_off_dur_ms = 0;
      m_repeat = false;
      m_event = 0;
    }
    else
    {
      my_data_file.stop();
      digitalWrite(m_led_pin, 1);
      m_state_changed_ms = millis();
      m_on_dur_ms = 100;
      m_off_dur_ms = 0;
      m_repeat = true;
    }
  }
}


void logger_ui::new_frame(void)
{
  if (m_logging && !m_error)
  {
    digitalWrite(m_led_pin, 1);
    m_state_changed_ms = millis();
    m_on_dur_ms = 5;
    m_off_dur_ms = 0;
    m_repeat = false;
  }
}

boolean logger_ui::is_event_now(void)
{
  if (user_event_button.is_new_push())
  {
    m_event++;
    user_event_button.clear_new_push();
    return true;
  }
  else
    return user_event_button.is_pushed();
}

int logger_ui::get_event_num(void)
{
  return m_event;
}

void logger_ui::error(void)
{
  m_error = true;
  m_on_dur_ms = 20;
  m_off_dur_ms = 80;
  m_repeat = true;
  digitalWrite(m_led_pin, 1);
  m_state_changed_ms = millis();
}

