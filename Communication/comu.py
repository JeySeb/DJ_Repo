#!/usr/bin/env python3

import rospy

from pynput import keyboard as kb
from std_msgs.msg import String
from geometry_msgs.msg import Twist

class Control:

    def __init__(self) -> None:
        #Publisher
        self.cmdPublisher = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
        rate = rospy.Rate(10)
        print('Ingrese el valor de la velocidad: ')
        self.vel_linear = float(input())
        print('Robot listo!')
    def pressKey(self, key):
        """
        Callback for kb.LIstener: It is called when a key is pressed and depending on the key that is pressed
        a Twist message is published in the cmd_vel topic.
        """
        twistMessage = Twist()
        if key == kb.KeyCode.from_char('d'):
            twistMessage.linear.x = self.vel_linear
        elif key == kb.KeyCode.from_char('a'):
            twistMessage.angular.x = -self.vel_linear
        elif key == kb.KeyCode.from_char('w'):
            twistMessage.linear.y = - self.vel_linear
        elif key == kb.KeyCode.from_char('s'):
            twistMessage.angular.y = - self.vel_linear
        elif key == kb.KeyCode.from_char('z'):
            twistMessage.angular.z = - self.vel_linear
        elif key == kb.KeyCode.from_char('x'):
            twistMessage.angular.z = - self.vel_linear
        self.cmdPublisher.publish(twistMessage)
    
    def releaseKey(self, key):
        """
        Callback for kb.Listener: It is called when a key is released and a Twist message with all the parameters
        in 0 is published.
        """
        twistMessage = Twist()
        self.cmdPublisher.publish(twistMessage)

if __name__ == '__main__':
    try:
        rospy.init_node('comu', anonymous=True)
        control = Control()
        with kb.Listener(control.pressKey, control.releaseKey) as escuchador:
            escuchador.join()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
