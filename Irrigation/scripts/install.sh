#!/bin/sh

APP_DIR=/usr/share/irrigation
APP_FILE=$APP_DIR/irrigation
LOG_DIR=/var/log/irrigation
LOG_FILE=$LOG_DIR/irrigation.log
CFG_FILE=$APP_DIR/irrigation.xml
SERVICE=/etc/init.d/irrigation_service

	
install_rpi() {
	echo "install_rpi"
	
	#################################
	# APPLICATION
	#################################
	mkdir -p $APP_DIR
	chmod 777 $APP_DIR
	chown pi:pi $APP_DIR
	
	# /usr/share/irrigation/irrigation
	cp ./bin/irrigation $APP_FILE
	chmod 544 $APP_FILE
	chown pi:pi $APP_FILE

	# /usr/share/irrigation/irrigation.xml
	cp --no-clobber ./data/irrigation.xml $CFG_FILE
	chmod 644 $CFG_FILE
	chown pi:pi $CFG_FILE
	
	#################################
	# LOGGING
	#################################
	mkdir -p $LOG_DIR
	chmod 777 $LOG_DIR
	chown pi:pi $LOG_DIR

	touch $LOG_FILE
	chmod 644 $LOG_FILE
	chown pi:pi $LOG_FILE
	
	#################################
	# SERVICE
	#################################	
	cp ./scripts/irrigation_service_rpi $SERVICE
	chmod 755 $SERVICE
	update-rc.d irrigation_service defaults	
	service irrigation_service start
}

uninstall_rpi() {
	echo "uninstall_rpi"

	service irrigation_service stop
	update-rc.d irrigation_service remove
	rm -f $SERVICE
	rm -f $APP_FILE
}


install_cygwin() {
	echo "install_cygwin"

	#################################
	# APPLICATION
	#################################
	mkdir -p $APP_DIR
	
	# /usr/share/irrigation/irrigation.xml
	cp --no-clobber ./data/irrigation.xml $CFG_FILE
	chmod 644 $CFG_FILE
	
	#################################
	# LOGGING
	#################################
	mkdir -p $LOG_DIR
	touch $LOG_FILE
	chmod 644 $LOG_FILE
}

uninstall_cygwin() {
	echo "uninstall_cygwin"
}


MACHINE_TYPE=$(uname -s)
case $MACHINE_TYPE in
    "Linux raspberrypi"*)     
		if [ "$1" = "--uninstall" ]; then
			uninstall_rpi
		else
			install_rpi
		fi
		;;
    "CYGWIN"*)    			
		if [ "$1" = "--uninstall" ]; then
			uninstall_cygwin
		else
			install_cygwin
		fi
		;;
    *)          			
		echo "Unknown machine type: $MACHINE_TYPE"
esac

