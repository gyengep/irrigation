- apt update
- apt upgrade
- apt install wiringpi
- apt install git-all
- apt install libmicrohttpd-dev (0.9.62)
- apt install libcurl4-openssl-dev  (7.64.0)
- apt install libssl-dev (1.1.1n)


SendInBlue key:
add the following line to end of the ~/.profile
export SENDINBLUE_SMTP_KEY=<key>


Enable NTP sync
- apt-get install ntp
- add to config file: /etc/ntp.conf
  server pool.ntp.org         # NTP pool project world-wide servers
  server time-a-g.nist.gov    # NIST internet time service


Disable eth0:
sudo ifconfig eth0 down

Disable WIFI Power Save on Raspbian Jessie

Old way - Not working:
- Use iw wlan0 get power_save to see if the wifi power save feature is enabled or not
- If you want to permantently disable it add this to the /etc/network/interfaces after the iface wlan0 line wireless-power off
- Reboot and if you run iw wlan0 get power_save you should see that is off.

New way:
- It's easier to add ONE line to /etc/rc.local
- /sbin/iwconfig wlan0 power off
- That's it, done.

Enable One-Wire interface
One-wire is a single-wire communication bus typically used to connect sensors to the Pi.
The Raspberry Pi supports one-wire on any GPIO pin, but the default is GPIO 4.
To enable the one-wire interface you need to add the following line to /boot/config.txt, before rebooting your Pi:
  - dtoverlay=w1-gpio
or
  - dtoverlay=w1-gpio,gpiopin=x
