Disable eth0:
sudo ifconfig eth0 down

Disable WIFI Power Save on Raspbian Jessie
- Use iw wlan0 get power_save to see if the wifi power save feature is enabled or not
- If you want to permantently disable it add this to the /etc/network/interfaces after the iface wlan0 line wireless-power off
- Reboot and if you run iw wlan0 get power_save you should see that is off.

Enable One-Wire interface
One-wire is a single-wire communication bus typically used to connect sensors to the Pi.
The Raspberry Pi supports one-wire on any GPIO pin, but the default is GPIO 4.
To enable the one-wire interface you need to add the following line to /boot/config.txt, before rebooting your Pi:
  dtoverlay=w1-gpio
or
  dtoverlay=w1-gpio,gpiopin=x
