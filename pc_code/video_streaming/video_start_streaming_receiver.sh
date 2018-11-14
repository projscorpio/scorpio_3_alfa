#!/bin/bash
#Video streaming reveiver script

srv_port="192.168.1.101"
netcat  $srv_port  6000 -w 1| mplayer -fps 60 -cache 2048 -
