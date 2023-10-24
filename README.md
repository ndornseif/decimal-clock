# decimal-clock

A clock that displays the current time in a decimal format.  

## Overview
This code implements a decimal clock that gets its time from an NTP server.  
It is written for the LDM8E display controller and uses the LDM8E-Library.  
See the GitHub repos for the [hardware](https://github.com/ndornseif/LDM8E-LEDDisplayController) and [library](https://github.com/ndornseif/LDM8E-Library) for more info.  

![Decimal clock showing 7882 (18:55:00.480)](Images/clock_running.png)  
The clock shows "7882" which is 18:55:00.480 in 24-hour time.  

## Installation
The code should work on any LDM8E as long as the dimming mode and WiFi credentials are properly configured.  

## Time format  
The clock represents the time of day with a number between 0 and 9999, where each increment represents 8.64 seconds (one ten-thousandth of a day).  
See this table for some example times:    
| Decimal Time |   24h Time   |
|--------------|--------------|
|     0000     | 00:00:00.000 |
|     0001     | 00:00:08.640 |
|     1000     | 02:24:00.000 |
|     5000     | 12:00:00.000 |
|     9000     | 21:36:00.000 |
|     9999     | 23:59:51.260 |

## Dependencies

- [Arduino ESP32 WiFi](https://github.com/espressif/arduino-esp32/tree/master)  
- [LDM8E-Library](https://github.com/ndornseif/LDM8E-Library)  

## Other
Published under GPL-3.0 license.  