class logger_ui
{
  public:
    void     begin(int led_pin);
    void     tick(void);
    void     new_frame(void);
    boolean  is_event_now(void);
    int      get_event_num(void);
    void     error(void);
    
  private:
    int      m_led_pin;
    int      m_state_changed_ms;
    int      m_on_dur_ms;
    int      m_off_dur_ms;
    boolean  m_repeat;
    boolean  m_logging;
    int      m_event;
    boolean  m_error;
};

