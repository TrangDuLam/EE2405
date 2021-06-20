# Final Markdown

## Introduction

    In the final project, I use the Xbee system to control the car. In addition, I will use the OpenMV to detect the lines and objects.

## Design flow

    In short, the main.cpp is based on the Xbee controlling system. The RPC commands are built in the bbcar_rpc.h. The input interface is built upon the Python script. I could just type the computer keyboard to steer the bbcar. 

## Function explanation

    1.  Line following 

        In this part, I will apply the open CV line detection program to realize it. The camera will detect the line in the screenshot in the IDE. Once the program detects the line in the screenshot. The serial output in the IDE will output the information of the line, including the location of the two end points and the length of the line. As attaining the information, I could steer the car upon the information. 

    2.  Circling an object

        In this part, the command is written in the car_control.py file. The command is ust to press "O" on the keyboard. The car would start turning around.

    3. Calibrating the car

        In this part, I will apply the OpenCV file to realize it. The OpenCV program will detect the AprilTag and return the angel upon the AprilTag. The serial screen in the IDE will return the status of the bbcar to determine whether the car is calibrated or not.

    4. Following another line 

        In this part, I will steer the bbcar depend on the screenshot of the IDE. I could steer the car given the information returned form the IDE serial.
