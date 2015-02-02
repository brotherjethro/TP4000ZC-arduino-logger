
void data_file::start(void)
{
  if (m_started)
    return;
    
  DateTime now = rtc.now();

  m_started = true;
  m_base_unixtime = now.unixtime();
  m_header_written = false;

  String s = "/";

  s += now.year();
  if (now.month() < 10)
      s += '0';
  s += now.month();
  if (now.day() < 10)
      s += '0';
  s += now.day();
  
  s.toCharArray(m_dirname, sizeof(m_dirname));
  
  s += '/';
  if (now.hour() < 10)
      s += '0';
  s += now.hour();
  if (now.minute() < 10)
      s += '0';
  s += now.minute();
  if (now.second() < 10)
      s += '0';
  s += now.second();
  s += ".CSV";

  s.toCharArray(m_fullpath, sizeof(m_fullpath));
  
  Serial.print("Full pathname is: ");
  Serial.println(s);
}



void data_file::make_timestamp(String &s)
{
    long offset_ms = ms_since_second_started();
    
    DateTime now = rtc.now();

    s += now.year();
    s += "-";

    if (now.month() < 10)
        s += '0';
    s += now.month();
    s += "-";

    if (now.day() < 10)
        s += '0';
    s += now.day();
    s += " ";

    if (now.hour() < 10)
        s += '0';
    s += now.hour();
    s += ":";

    if (now.minute() < 10)
        s += '0';
    s += now.minute();
    s += ":";

    if (now.second() < 10)
        s += '0';
    s += now.second();
    s += ".";
    if (offset_ms < 100)
        s += "0";
    if (offset_ms < 10)
        s += "0";
    s += offset_ms;

    s += ", ";
    s += now.unixtime() - m_base_unixtime;
    s += ".";
    if (offset_ms < 100)
        s += "0";
    if (offset_ms < 10)
        s += "0";
    s += offset_ms;
}



void data_file::new_dmm_data(int * pdata)
{
  String value_string;
  String units_string;
  String notes_string;
  String timestamp_string;
  char c;
  
  
  make_timestamp(timestamp_string);
  
  if ((pdata[1] & 0x08) > 0)
    value_string += '-';
  
  value_string += char_from_7_seg(((pdata[1] & 7) << 4) | (pdata[2] & 0xf));

  if ((pdata[3] & 0x08) > 0)
    value_string += '.';

  value_string += char_from_7_seg(((pdata[3] & 7) << 4) | (pdata[4] & 0xf));

  if ((pdata[5] & 0x08) > 0)
    value_string += '.';

  value_string += char_from_7_seg(((pdata[5] & 7) << 4) | (pdata[6] & 0xf));

  if ((pdata[7] & 0x08) > 0)
    value_string += '.';

  value_string += char_from_7_seg(((pdata[7] & 7) << 4) | (pdata[8] & 0xf));

  if ((pdata[9] & 0x08) > 0)
    value_string += "e-6";

  if ((pdata[9] & 0x04) > 0)
    value_string += "e-9";

  if ((pdata[9] & 0x02) > 0)
    value_string += "e+3";

  if ((pdata[9] & 0x01) > 0)
    units_string += "Diode ";
    
  if ((pdata[10] & 0x08) > 0)
    value_string += "e-3";

  if ((pdata[10] & 0x04) > 0)
    units_string += "% Duty";
  if ((pdata[10] & 0x02) > 0)
    value_string += "e+6";

  if ((pdata[10] & 0x01) > 0)
    units_string += "Continuity ";
    
  if ((pdata[11] & 0x08) > 0)
    units_string += "Farads";
  if ((pdata[11] & 0x04) > 0)
    units_string += "Ohms";
  if ((pdata[11] & 0x02) > 0)
    notes_string += "Relative ";
  if ((pdata[11] & 0x01) > 0)
    notes_string += "Hold ";
    
  if ((pdata[12] & 0x08) > 0)
    units_string += "Amps";
  if ((pdata[12] & 0x04) > 0)
    units_string += "Volts";
  if ((pdata[12] & 0x02) > 0)
    units_string += "Hz";
  if ((pdata[12] & 0x01) > 0)
    notes_string += "Low Battery ";
    
  if ((pdata[13] & 0x04) > 0)
    units_string += "Degrees C";

  if ((pdata[0] & 0x08) > 0)
    units_string += " AC";
  if ((pdata[0] & 0x04) > 0)
    units_string += " DC";
  if ((pdata[0] & 0x02) > 0)
    notes_string += "Auto ";

  if (is_started())
  {
    boolean early = ((millis() - m_last_record_written_ms) < m_min_interval_ms);

    m_averager.add(value_string);
    
    if (   m_header_written
        && early
        && !ui.is_event_now()
       )
    {
      //----------------------------------
      // Skip this record.  Nothing to do.
      //----------------------------------
      //Serial.print("skipped: ");
      //Serial.print((millis() - m_last_record_written_ms));
      //Serial.print(", min_ms: ");
      //Serial.println(m_min_interval_ms);
    }
    else
    {
      //-------------------------
      // Log this record:
      //-------------------------
      SD.mkdir(m_dirname);

      File dataFile = SD.open(m_fullpath, FILE_WRITE);
    
      if (dataFile)
      {
        ui.new_frame();
        
        // Don't update the time if this record is early.  We want to keep the
        // same timing even if the user presses the event button (but we do want
        // to log data near the time of their event...
        if (!early)
          m_last_record_written_ms = millis();

        if (!m_header_written)
        {
          m_header_written = true;
          dataFile.println("Date, Seconds, Event, Value, Units, Notes, Avg");
        }
        
        dataFile.print(timestamp_string);
        dataFile.print(", ");
        if (ui.is_event_now())
          dataFile.print(ui.get_event_num());
        dataFile.print(", ");
        dataFile.print(value_string);
        dataFile.print(", ");
        dataFile.print(units_string);
        dataFile.print(", ");
        dataFile.print(notes_string);
        dataFile.print(", ");
        dataFile.println(m_averager.get_as_str());
        dataFile.close();

        m_averager.clear();
        Serial.println(value_string);
      }  
      else
      {
        Serial.println("file error");
        ui.error();
        stop();
      }
    }
  }
}




char char_from_7_seg(int i)
{
  switch(i)
  {
    case 0x7d: return '0';
    case 0x05: return '1';
    case 0x5b: return '2';
    case 0x1f: return '3';
    case 0x27: return '4';
    case 0x3e: return '5';
    case 0x7e: return '6';
    case 0x15: return '7';
    case 0x7f: return '8';
    case 0x3f: return '9';
    case 0x68: return 'L';
    case 0x00: return ' ';
    default:   return '?';
  }
}

