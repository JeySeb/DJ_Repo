/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include <mainpp.h>
#include <ros.h>
#include <std_msgs/String.h>

#include "BMP180.h"
#include "stdio.h"



ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher temp_pub("temperature", &str_msg);
ros::Publisher press_pub("pressure", &str_msg);
ros::Publisher alt_pub("altitude", &str_msg);

char hello[] = "Hello world!";

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
}

/* VARIABLES-----------------*/

float Temperature = 0;
float Pressure = 0;
float Altitude = 0;

char Temperature1[10];
char Pressure1[10];
char Altitude1[10];


void setup(void)
{


  nh.initNode();
  nh.advertise(temp_pub);
  nh.advertise(press_pub);
  nh.advertise(alt_pub);

  BMP180_Start();
}


void publishMsg(char pub,char str[]){

	  str_msg.data = str;
	  if(pub=='t') temp_pub.publish(&str_msg);
	  else if(pub=='p') press_pub.publish(&str_msg);
	  else if(pub=='a') alt_pub.publish(&str_msg);
	  nh.spinOnce();
}

void loop(void)
{
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);


  Temperature = BMP180_GetTemp();
  sprintf(Temperature1, "%.2f", Temperature);

  Pressure = BMP180_GetPress (0);
  sprintf(Pressure1, "%.2f", Pressure);

  Altitude = BMP180_GetAlt(0);
  sprintf(Altitude1, "%.2f", Altitude);



  //publishMsg('t',"The temperature is: ");
  publishMsg('t',Temperature1);

  //publishMsg('p',"The pressure is: ");
  publishMsg('p',Pressure1);

  //publishMsg('a',"The altitude is: ");
  publishMsg('a',Altitude1);



  HAL_Delay(1000);
}

