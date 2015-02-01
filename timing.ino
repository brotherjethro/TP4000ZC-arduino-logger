
volatile uint32_t this_second_started_at_ms;

void sqw_isr(void)
{
    // The RTC SQW pin just went low, which means
    // a new second has started.
    this_second_started_at_ms = millis();
}

int ms_since_second_started(void)
{
    int delta = millis() - this_second_started_at_ms;
  
    // In case the millis are running a bit faster than the RTC:
    if (delta > 999)
        delta = 999;
      
    return delta;
}


