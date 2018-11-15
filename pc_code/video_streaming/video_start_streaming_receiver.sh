#!/bin/bash
#Video streaming reveiver script

srv_port="pi3"
netcat  $srv_port  6000 -w 1| mplayer -fps 60 -cache 2048 -
