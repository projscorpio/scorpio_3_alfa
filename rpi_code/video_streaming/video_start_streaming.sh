#!/bin/bash 
#Camera sending script

#raspivid -t 0 -w 1920 -h 1080 -o - | nc 192.168.1.111 6000
raspivid -t 0 -w 1920 -h 1080 -o - | netcat -l -u 6000 
