/*
   Data logger for TekPower TP4000ZC multimeter with Adafruit datalogger shield
  	 
   About DS1307 Square wave output:
     It requires an external pullup (max 5V)
     When enabled at 1Hz, the falling edge of SQW will indicate time change

*/

#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "PinChangeInt.h"
#include "button.h"
#include "ui.h"
#include "data_file.h"



RTC_DS1307 rtc;

#define USER_EVENT_BUTTON_PIN 6    // Button to write an "event" into the data file
#define START_STOP_BUTTON_PIN 7    // Button to start / stop logging
#define SQW_PIN               8    // SQW signal from RTC
#define LED_PIN               9    // LED to signal user
#define SD_CS_PIN            10    // SD card chip select



class button start_stop_button;
class button user_event_button;
class logger_ui ui;
class data_file my_data_file;




void setup()
{
    pinMode(SQW_PIN, INPUT_PULLUP);
    attachPinChangeInterrupt(SQW_PIN, sqw_isr, FALLING);

    start_stop_button.begin(START_STOP_BUTTON_PIN);
    user_event_button.begin(USER_EVENT_BUTTON_PIN);
    ui.begin(LED_PIN);

    Serial.begin(2400);  // Required by multimeter

    Wire.begin();        // for rtc
    rtc.begin();

    // EDN added this method to the RTC library: 
    rtc.enable1Hz();     // Turn on SQW at 1 hz, falling edge is new second
    
    if (rtc.isrunning())
        Serial.println("RTC ok");
    else
    {
        Serial.println("RTC failed");
        ui.error();
    }
    
    pinMode(SD_CS_PIN, OUTPUT);
    if (SD.begin(SD_CS_PIN))
        Serial.println("SD card ok");
    else
    {
        Serial.println("SD card failed or missing");
        ui.error();
    }
    
    read_config_file();
}



int dmm_raw_buffer[14];

void loop()
{
  user_event_button.tick();
  start_stop_button.tick();
  ui.tick();
 
  // When 14 (or more) bytes are available, there may
  // be a complete data frame from the DMM.  Try to get it:
  if (Serial.available() >= 14)
  {
    int i = 0;
    boolean bad_frame = false;

    for (i=0; i<14; i++)
    {
      dmm_raw_buffer[i] = Serial.read();
      if ((dmm_raw_buffer[i] & 0xf0) != ((i+1)<<4))
      {
        // Oops, this is not the byte we were expecting.  Throw away
        // what we've collected so far, and maybe it will be better
        // next time:
        bad_frame = true;
        break;
      }
    }
    
    if (!bad_frame)
    {
      my_data_file.new_dmm_data(&dmm_raw_buffer[0]);
    }
  }  
}



void read_config_file(void)
{
  File configFile = SD.open("/config.txt", FILE_READ);
  
  if (configFile)
  {
    // Note that the text content of the file is ignored.
    // We just look for integers in a specific order:
    long min_ms = configFile.parseInt();

    Serial.print("Config file found, min_interval_ms = ");
    Serial.println(min_ms);    
    my_data_file.set_min_interval(min_ms);
    configFile.close();
  }
}


