# hm-11-scan

Scan for nearby Bluetooth LE peripherals using HM-11 and Arduino.
This works on any ble device that advertises itself: beacons, trackers, wearables.

# Software

The code puts the HM-11 into master mode, and then scans every minute for nearby devices.
Software returns device name and id.

# Hardware

Connect the HM-11 pins to a 3.3v Arduino, or to a regular 5v Arduino through a logic level shifter.

<pre>
    _______
   |       |          HM-11  -->  Arduino
 1 :       : 16       9           3.3V
   :       :          12          GND
   :       :          4 (RX)      3 (TX)
 8 :_______: 9        2 (TX)      2 (RX)
     HM-11
</pre>