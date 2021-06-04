# Edge Impulse - OpenMV Image Classification Example

import sensor, image, time, os, tf

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.

net = "trained.tflite"
labels = [line.rstrip('\n') for line in open("labels.txt")]

clock = time.clock()
while(True):
   clock.tick()

   img = sensor.snapshot()

   # default settings just do one detection... change them to search the image...
   for obj in tf.classify(net, img, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
      for i in range(len(obj.output())):
         print("%s = %f" % (labels[i], obj.output()[i]))
   img.draw_rectangle(obj.rect())
   img.draw_string(obj.x()+3, obj.y()-1, labels[obj.output().index(max(obj.output()))], mono_space = False)
   print("This is : ",labels[obj.output().index(max(obj.output()))])