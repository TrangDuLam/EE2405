import pyb, sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
clock = time.clock()

f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)

def degrees(radians):
    return (180 * radians) / math.pi

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

while(True):
   clock.tick()
   img = sensor.snapshot()
   for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
      img.draw_rectangle(tag.rect(), color = (255, 0, 0))
      img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
      # The conversion is nearly 6.2cm to 1 -> translation

      x = tag.x_translation()
      y = tag.y_translation()
      z = tag.z_translation()

      dis = math.sqrt(x * x + y * y + z * z)
      angle = degrees(tag.y_rotation())

      if angle < 10 or angle > 350 :
        print("Distance %f, Is Perpendicular\r\n" %dis)
      elif angle > 10 and angle < 90 :
        print("Distance %f, Is Left\r\n" %dis)
      elif angle > 270 and angle < 350 :
        print("Distance %f, Is Right\r\n" %dis)




      #print_args = (dis, degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation()))
      # Translation units are unknown. Rotation units are in degrees.
      #uart.write(("Distance: %f, Rx %f, Ry %f, Rz %f" % print_args).encode())
   #uart.write(("FPS %f\r\n" % clock.fps()).encode())
