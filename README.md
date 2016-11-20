# OpenLedstrip
Controlling ledstrips over HTTP/TCPIP, ethernet/wifi using Arduino or ESP8266

[Check the video on YouTube!](https://www.youtube.com/watch?v=2L2MmDTe5es)

![Preview gif](https://github.com/Paul-Ver/OpenLedstrip/blob/master/Images/WifiHttpLedstrip.gif)

Currently includes:
- Hardware schematic + board files (Fritzing)
- Nano + Nano Ethernet software (Compact ethernet version)
- ESP8266 (WiFi version)
- HTTP message handling
- TCP message handling

TODO:
- Animations
  - Fade (in/out, fade colorwheel, random colors) (fadespeed,delta)
  - Flicker (on/off, colorwheel, random colors) (ontime,offtime,delta)
- Bring Nano Ethernet version up to date. (Or delete it)
- something funky, UDP broadcast synchronisation of led strips!?
- Make the message handling more async/non-blocking
- Order/check the hardware schematics
- Add support for digital led strips?
(Though I don't know what specific things to implement)
