
class button
{
  public:
    void begin(int pin);
    void tick(void);
    boolean is_pushed(void);
    boolean is_new_push(void);
    void clear_new_push(void);

  private:
    int     m_pin;
    long    m_last_down_ms;
    long    m_last_up_ms;
    boolean m_wait_for_up;
    boolean m_pushed;
    boolean m_new_push;
    long    m_new_push_at_ms;
};


