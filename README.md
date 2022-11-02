This is example code for using the AM210 humidity temperature sensor with a microcontroller.


It is tested with an ESP8266 on the arduino IDE.    It will require adjustment for other architectures.





Usage:

Connect the data line to D7, and the sensor to 3.3v power and gnd.

Run this code:

```C
  #include "AM2120.h"

  setupAM2120();

  float temp;
  float humidity;
  int err;
  if (err=readAM2120(&humidity, &temp)){ 
    Serial.print("Error: ");
    Serial.println("err");
  } 

  // variables temp and humidity are now filled and ready to go!
```




Note, you should only read the sensor a maximum of once every 2 seconds.

Code is very rough, but working.    PR's for tidyup, cross platform support, or arduino packaging are welcome.
