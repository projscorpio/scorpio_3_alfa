#!/bin/bash 
#Camera sending script

raspivid -t 0 -w 1920 -h 1080 -o - | netcat -l -p 6000  -q 1
