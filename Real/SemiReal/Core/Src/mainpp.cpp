/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include <mainpp.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <array>
#include "BMP180.h"

using std::array;


TIM_HandleTypeDef hhtim1;
TIM_HandleTypeDef hhtim2;

float velx=0;
float velz=0;
float vely=0;

int zero = 75;
int velrigth = 90;
int velleft = 68;

/* VARIABLES-----------------*/

float Temperature = 0;
float Pressure = 0;
float Altitude = 0;

char Temperature1[10];
char Pressure1[10];
char Altitude1[10];


/**
 * FUNCIONES MOTOR ****************************
 */



void cmd_vel_cb( const geometry_msgs::Twist& twist){
	velx = twist.linear.x;
	vely = twist.linear.y;
	velz = twist.linear.z;
}

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher temp_pub("temperature", &str_msg);
ros::Publisher press_pub("pressure", &str_msg);
ros::Publisher alt_pub("altitude", &str_msg);

ros::Subscriber<geometry_msgs::Twist> sub_mov("cmd_vel", cmd_vel_cb );


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	nh.getHardware()->reset_rbuf();
}



void publishMsg(char pub,char str[]){

	str_msg.data = str;
	if(pub=='t') temp_pub.publish(&str_msg);
	else if(pub=='p') press_pub.publish(&str_msg);
	else if(pub=='a') alt_pub.publish(&str_msg);
	nh.spinOnce();
}


void start(TIM_HandleTypeDef contador1,TIM_HandleTypeDef contador2){
	HAL_TIM_PWM_Start(&contador1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&contador1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&contador1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&contador1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&contador2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&contador2, TIM_CHANNEL_2);
}

void tur_init(TIM_HandleTypeDef contador1,TIM_HandleTypeDef contador2, int cal){
	__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, cal);
	__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, cal);
	__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, cal);
	__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, cal);
	__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, cal);
	__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, cal);
	HAL_Delay(7000);
}

void controler(int x, int y, int z,TIM_HandleTypeDef contador1,TIM_HandleTypeDef contador2, int cal, int velpos, int velneg){
	if (x == 0 && y == 0 && z == 0){

		//publishMsg('t',"0");
		__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, cal); //abajo derecha trasero
		__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, cal); //abajo izquierda alfrente
		__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, cal); //abajo izquierdo trasero
		__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, cal); //abajo derecha alfrente
		__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, cal); //arriba derecho
		__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, cal); //ariba izquierdox
	}
	else{
		if(y > 0 ){

			// publishMsg('t',"+y");
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, velpos); //abajo derecha trasero
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, velneg); //abajo izquierda alfrente
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, velneg); //abajo izquierdo trasero
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, velneg); //abajo derecha alfrente
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, cal); //arriba derecho
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, cal); //ariba izquierdox
		}
		else if(y < 0){

			//publishMsg('t',"-y");
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, velneg); //abajo derecha trasero
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, velpos); //abajo izquierda alfrente
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, velpos); //abajo izquierdo trasero
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, velpos); //abajo derecha alfrente
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, cal); //arriba derecho
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, cal); //ariba izquierdo
		}
		else if(x > 0  ){

			//publishMsg('t',"x+");
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, velneg); //abajo derecha trasero
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, velneg); //abajo izquierda alfrente
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, velneg); //abajo izquierdo trasero
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, velpos); //abajo derecha alfrente
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, cal); //arriba derecho
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, cal); //ariba izquierdo
		}
		else if(x < 0){

			//publishMsg('t',"-x");
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, velpos); //abajo derecha trasero
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, velpos); //abajo izquierda alfrente
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, velpos); //abajo izquierdo trasero
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, velneg); //abajo derecha alfrente
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, cal); //arriba derecho
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, cal); //ariba izquierdo
		}
		else if(z > 0 ){

			//publishMsg('t',"+z");
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, cal); //abajo derecha trasero
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, cal); //abajo izquierda alfrente
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, cal); //abajo izquierdo trasero
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, cal); //abajo derecha alfrente
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, velpos); //arriba derecho
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, velneg); //ariba izquierdo
		}
		else if(z < 0){

			//publishMsg('t',"-z");
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_1, cal); //abajo derecha trasero
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_2, cal); //abajo izquierda alfrente
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_4, cal); //abajo izquierdo trasero
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_1, cal); //abajo derecha alfrente
			__HAL_TIM_SET_COMPARE(&contador2, TIM_CHANNEL_2, velneg); //arriba derecho
			__HAL_TIM_SET_COMPARE(&contador1, TIM_CHANNEL_3, velpos); //ariba izquierdo
		}
	}
}





void setup(TIM_HandleTypeDef phtim1, TIM_HandleTypeDef phtim2)
{
	hhtim1 = phtim1;
	hhtim2 = phtim2;
	start(hhtim1,hhtim2);
	tur_init(hhtim1,hhtim2,zero);

	nh.initNode();
	nh.advertise(temp_pub);
	nh.advertise(press_pub);
	nh.advertise(alt_pub);
	nh.subscribe(sub_mov);

	BMP180_Start();
}

void loop()
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);


	//Temperature = BMP180_GetTemp();
	//sprintf(Temperature1, "%.2f", Temperature);
	//Pressure = BMP180_GetPress (0);
	//sprintf(Pressure1, "%.2f", Pressure);
	//Altitude = BMP180_GetAlt(0);
	//sprintf(Altitude1, "%.2f", Altitude);

	//publishMsg('t',"The temperature is: ");
	//publishMsg('t',Temperature1);
	//publishMsg('p',"The pressure is: ");
	//publishMsg('p',Pressure1);
	//publishMsg('a',"The altitude is: ");
	//publishMsg('a',Altitude1);

	nh.spinOnce();

	controler(velx, vely, velz, hhtim1,hhtim2,zero,velrigth,velleft);
	HAL_Delay(1);
}
