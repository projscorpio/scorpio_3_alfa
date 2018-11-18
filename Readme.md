# scorpio 3 alpha

## Requirements:
* gipio library
* joystick
* linux system

## Use:
* Connect rpi to network
* run 
  ```
    make -C rpi_rover/
    sudo ./rpi_code/drive_rover
  ```
* change ip address to real in pc_code (line 21)
  ```
    vim pc_code/controll_rover.cc
    make -C pc_code/
  ```
* connect joystick and run
  ```
    ./pc_code/controll_rover
  ```
