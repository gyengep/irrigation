#!/bin/sh

APP_DIR=/usr/share/irrigation
APP_FILE=$APP_DIR/irrigation
LOG_DIR=/var/log/irrigation
LOG_FILE=$LOG_DIR/irrigation.log
CFG_FILE=$APP_DIR/irrigation.xml
SERVICE=/etc/init.d/irrigation_service
WEB_ROOT_DIR=$APP_DIR/web_root

	
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
	# RESOURCES
	#################################
	mkdir -p $WEB_ROOT_DIR
	chmod 777 $WEB_ROOT_DIR
	chown pi:pi $WEB_ROOT_DIR
	
	# /usr/share/irrigation/web_root/*
	cp -f ./web_root/* $WEB_ROOT_DIR
	chmod 444 $WEB_ROOT_DIR/*
	chown pi:pi $WEB_ROOT_DIR/*
	
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
	cp ./scripts/irrigation_service $SERVICE
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


install_other() {
	echo "install_other"

	#################################
	# APPLICATION
	#################################
	mkdir -p $APP_DIR
	
	# /usr/share/irrigation/irrigation.xml
	cp --no-clobber ./data/irrigation.xml $CFG_FILE
	chmod 644 $CFG_FILE
	
	#################################
	# RESOURCES
	#################################
	mkdir -p $WEB_ROOT_DIR
	
	# /usr/share/irrigation/web_root/*
	cp -f ./web_root/* $WEB_ROOT_DIR
	chmod 444 $WEB_ROOT_DIR/*
	
	#################################
	# LOGGING
	#################################
	mkdir -p $LOG_DIR
	touch $LOG_FILE
	chmod 644 $LOG_FILE
}

uninstall_other() {
	echo "uninstall_other"
}


RPI=$(cat /proc/cpuinfo | grep Hardware | grep BCM)
if [ -n "$RPI" ]; then
	if [ "$1" = "--uninstall" ]; then
		uninstall_rpi
	else
		install_rpi
	fi
else
	if [ "$1" = "--uninstall" ]; then
		uninstall_other
	else
		install_other
	fi
fi

exit 0