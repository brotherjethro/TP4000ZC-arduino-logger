# TP4000ZC-arduino-logger
Log data from TekPower TP4000ZC multimeter to SD card with arduino uno &amp; adafruit datalogger shield

0. This is meant to run on an arduino uno using the adafruit datalogger shield (sd card + rtc)
1. You will need the RTC lib and SD card lib.
2. I modified the RTC lib to include a method called enable1Hz, which turns on the SQW output at 1 hz.  I will include that source
3. There's a button to start & stop logging
4. There's another button to add "event" markers to your data
5. I'm not an electical engineer, so forgive my sad serial circuit, using overpriced FETs, or build your own
6. Make sure the DMM is not sending data if you want to send data from your pc (ie, download code).  You can do either, but not both at the same time.
7. Be sure to stop logging before removing the SD card (push start/stop button)
8. Hold the reset button while installing the SD card (or do it with power off)
9. LED solid = ready to log
10. LED blinking fast = error (need to reset)
11. LED flashing occasionally = logging a data record
