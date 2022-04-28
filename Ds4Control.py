#!/usr/bin/env python3

import rospkg
import rospy

from sensor_msgs.msg import Joy
from std_msgs.msg import Float64
from geometry_msgs.msg import Twist



class Ds4Control:

    buttons=[]
    axes = []

    #Constructor de la clase
    def __init__(self):
        rospy.Subscriber('/joy', Joy, self.callback_buttonSelected)
        self.cmdPublisher = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
        rospy.spin() 


    def callback_buttonSelected(self, msg):
        twistMessage = Twist()
        self.buttons = msg.buttons
        self.axes = msg.axes


        if self.axes[7]>0:
            twistMessage.linear.y = 1.0
        elif self.axes[7]<0:
            twistMessage.linear.y = -1.0
        elif self.axes[6]<0:
            twistMessage.linear.x = 1.0
        elif self.axes[6]>0:
            twistMessage.linear.x = -1.0
        elif self.buttons[4]>0:
            twistMessage.linear.z = -1.0
        elif self.buttons[5]>0:
            twistMessage.linear.z = 1.0
        else:
            twistMessage.linear.x = 0
            twistMessage.linear.y = 0
            twistMessage.linear.z = 0
        self.cmdPublisher.publish(twistMessage)
        #print(self.buttons)
        #print(f"Ejes: {self.axes}")
        


# Main del programa, lanza el despliegue de la interfaz en el hilo principal.
if __name__ == '__main__':
    rospy.init_node('ds4control', anonymous=True)
    try:   
            sd = Ds4Control() 
    except rospy.ROSInterruptException:
            pass
    