#!/bin/bash
#Video streaming reveiver script

srv_port="192.168.1.101"
#netcat -l -p 6000 | mplayer -fps 60 -cache 2048 -
netcat -u $srv_port 6000 | mplayer -fps 60 -cache 2048 -
