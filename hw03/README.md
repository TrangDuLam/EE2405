# Markdown HW03

In this homework, there are two RPC function to deal with requests. The first is Gesture UI function and the second is tilt angle detection function.

 1. The gesture UI will be built by the tensorflow package to do the gesture detection. Each gesture will represent different selection output. The output will be applied to the tilt angle detection.

 2. The tilt angel detection will apply the space vector calculation to realize this function. And the function will generate the extent of tilting given the result of gesture selection.

## Gesture UI RPC Function

This RPC function is mainly based on the structure of the code of lab 8.

I use the data collected in the lab 8 (all the gestures are listed below). Since there are three gesture selection, I have assigned the three gestures to represent three different angle; 30, 45, 60 respectively.

Once the gesture detected, the corresponding result will be recorded and exported to the next RPC process.


## Angle tilt detection RPC function

This RPC function is mainly based on the structure of the code of lab 10.

This function will compute the cosine value of the tilt angle by simple mathematical computation. Since the range of tilt angle is defined in the interval of 0 to 90 degree. In this interval, the cosine function is monotonic decreasing function. I could just compare the cosine value of the angle selecting from the gesture UI RPC.

Sine the acceleration vector as tilt angle = 0 degree is 988 (z direction). The z-vector as the board tilted could be calculated as z = g * cos(theta), where theta is the tilting angle. Thus, I could apply the theta value to judge the range of tilting angle. 

    * z > 855 => angle < 30 degree
    * 699 < z < 855 => 30 degree < angle < 45 degree
    * 494 < z < 699 => 45 degree < angle < 60 degree
    * z <494 => angel > 60 degree


After doing the simple comparison, it will output the given result on the uLCD.
