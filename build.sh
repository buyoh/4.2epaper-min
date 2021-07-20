#!/bin/bash

gcc -o eptest -lwiringPi -lm -DDEBUG -DRPI -DUSE_WIRINGPI_LIB -O3 DEV_Config.c EPD_4in2.c main.c
