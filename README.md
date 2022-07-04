Disable eth0:
sudo ifconfig eth0 down

Disable WIFI Power Save on Raspbian Jessie
- Use iw wlan0 get power_save to see if the wifi power save feature is enabled or not
- If you want to permantently disable it add this to the /etc/network/interfaces after the iface wlan0 line wireless-power off
- Reboot and if you run iw wlan0 get power_save you should see that is off.
