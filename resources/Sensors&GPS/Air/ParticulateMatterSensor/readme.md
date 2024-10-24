You will need to install the following libraries in Arduino for this to work: 
AdaFruit BusIO version 1.16.1, Adafruit PM25 AQI and its dependencies. When you get it running, it takes samples of the number of particles present. 
It then calculates equivalent particle diameter and the number of particles with different diameter per unit volume. 


The result in the terminal puts out something like this:

---------------------------------------
Particles > 0.3um / 0.1L air:0
Particles > 0.5um / 0.1L air:0
Particles > 1.0um / 0.1L air:0
Particles > 2.5um / 0.1L air:0
Particles > 5.0um / 0.1L air:0
Particles > 10 um / 0.1L air:0
---------------------------------------


That air rating is 0, meaning that there is very little, if any other particulate matter that the sensor can pick up! That number would increase if we took a match to introduce dust or smoke. I can’t comment on the accuracy of this entirely, but its still a good start.
