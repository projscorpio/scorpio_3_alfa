#!/bin/bash
#Video streaming reveiver script
netcat -l -p 6000 | mplayer -fps 60 -cache 2048 -
