# TP4000ZC-arduino-logger
Log data from TekPower TP4000ZC multimeter to SD card with arduino uno &amp; adafruit datalogger shield

0. This is meant to run on an arduino uno using the adafruit datalogger shield (sd card + rtc)
1. You will need the RTC lib (DS1307) and SD card lib.
2. I modified the RTC lib (DS1307) to include a method called enable1Hz, which turns on the SQW output at 1 hz.  I will include that source (see below).  Add this to your RTC library if it doesn't already have something similar.
3. There's a button to start & stop logging
4. There's another button to add "event" markers to your data
5. I'm not an electical engineer, so forgive my sad serial circuit, using overpriced FETs, or build your own
6. Make sure the DMM is not sending data if you want to send data from your pc (ie, download code).  You can do either, but not both at the same time.
7. Be sure to stop logging before removing the SD card (push start/stop button)
9. LED solid = ready to log
10. LED blinking fast = error (need to reset)
11. LED flashing occasionally = logging a data record
12. An optional config file is now supported.  If config.txt exists at the root of the SD card, the code will read it.
It ignores text and reads the first long integer as the min time between samples.  If the file is not present, a min time
of 0 seconds is assumed.



void   RTC_DS1307::enable1Hz(void)

{

    WIRE.beginTransmission(DS1307_ADDRESS);

    WIRE.write(7);

    WIRE.write(0x10);

    WIRE.endTransmission();

}

