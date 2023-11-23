#include "mpu6050.h"



// 初始化MPU6050
bool MPU6050_Init(void)
{
	bool Res;
	uint8_t RxDev;
	
	/* 复位MPU6050 */
	Res = MPU6050_Tx_Data(MPU6050_DEV_ADDR, MPU6050_PWR_MGMT1_REG, 1, PTU8(MPU6050_RESET_CMD));
	Delay_Ms(100);
	Res = MPU6050_Tx_Data(MPU6050_DEV_ADDR, MPU6050_PWR_MGMT1_REG, 1, PTU8(MPU6050_RESET_CMD));
	
	/* 设置角速度传感器和加速度传感器的满量程范围 */
	MPU6050_SetGyroFsr(3);					// 陀螺仪传感器,±2000dps
	MPU6050_SetAccelFsr(0);					// 加速度传感器,±2g
	MPU6050_SetRate(50);					// 设置采样率50Hz
	
	/* 设置其他参数 */
	MPU6050_Rx_Data(MPU6050_DEV_ADDR, MPU6050_DEVICE_ID_REG, 1, &RxDev);

	Res = (bool)(RxDev == MPU6050_DEV_ADDR);
	
	mpu_dmp_init();	// MPU6050 DMP初始化
	
	return Res;
}


//设置MPU6050陀螺仪传感器满量程范围
//Fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
bool MPU6050_SetGyroFsr(uint8_t Fsr)
{
	//设置陀螺仪满量程范围 
	return MPU6050_Tx_Data(MPU6050_DEV_ADDR, MPU6050_GYRO_CFG_REG, 1, PTU8(Fsr<<3)); 
}


//设置MPU6050加速度传感器满量程范围
//Fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
bool MPU6050_SetAccelFsr(uint8_t Fsr)
{
	//设置加速度传感器满量程范围  
	return MPU6050_Tx_Data(MPU6050_DEV_ADDR, MPU6050_ACCEL_CFG_REG, 1, PTU8(Fsr<<3));
}


//设置MPU6050的数字低通滤波器
//Lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
bool MPU6050_SetLPF(uint16_t Lpf)
{
	uint8_t Data;
	
	if (Lpf>=188) 	  Data = 1;
	else if (Lpf>=98) Data = 2;
	else if (Lpf>=42) Data = 3;
	else if (Lpf>=20) Data = 4;
	else if (Lpf>=10) Data = 5;
	else              Data = 6;
	
	return MPU6050_Tx_Data(MPU6050_DEV_ADDR, MPU6050_CFG_REG, 1, &Data);//设置数字低通滤波器  
}


//设置MPU6050的采样率(假定Fs=1KHz)
//Rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
bool MPU6050_SetRate(uint16_t Rate)
{
	bool Res;
	uint8_t Data;
	
	if (Rate>1000) Rate = 1000;
	if (Rate<4) Rate = 4;
	
	Data = 1000/Rate - 1;
	Res = MPU6050_Tx_Data(MPU6050_DEV_ADDR, MPU6050_SAMPLE_RATE_REG, 1, &Data);	//设置数字低通滤波器
 	Res = MPU6050_SetLPF(Rate/2);	//自动设置LPF为采样率的一半
	
	return Res;
}


//得到温度值
//返回值:温度值(扩大了100倍)
short MPU6050_GetTemperature(void)
{
	bool Res;
	short Raw;
	float Temp;
	
    uint8_t RxBuff[2]; 
	
	Res = MPU6050_Rx_Data(MPU6050_DEV_ADDR, MPU6050_TEMP_OUTH_REG, 2, RxBuff); 
	
	if (Res == false) return 0;
	
    Raw = ((uint16_t)RxBuff[0]<<8) | RxBuff[1];  
    Temp = (36.53 + ((double)Raw)/340);	// 根据公式计算温度
    
	return (Temp*100);
}


//得到陀螺仪值(原始值)
//Gx,Gx,Gx:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU6050_GetGyroscope(short *Gx,short *Gy,short *Gz)
{
    uint8_t RxBuff[6], Res;  
	
	Res = MPU6050_Rx_Data(MPU6050_DEV_ADDR, MPU6050_GYRO_XOUTH_REG, 6, RxBuff);
	
	if(Res == true)
	{
		*Gx=((uint16_t)RxBuff[0]<<8)|RxBuff[1];  
		*Gy=((uint16_t)RxBuff[2]<<8)|RxBuff[3];  
		*Gz=((uint16_t)RxBuff[4]<<8)|RxBuff[5];
	} 
	
    return Res;;
}


//得到加速度值(原始值)
//Ax,Ax,Ax:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU6050_GetAccelerometer(short *Ax,short *Ay,short *Az)
{
    uint8_t RxBuff[6], Res;  
	
	Res = MPU6050_Rx_Data(MPU6050_DEV_ADDR, MPU6050_ACCEL_XOUTH_REG, 6, RxBuff);
	
	if(Res == true)
	{
		*Ax=((uint16_t)RxBuff[0]<<8)|RxBuff[1];  
		*Ay=((uint16_t)RxBuff[2]<<8)|RxBuff[3];  
		*Az=((uint16_t)RxBuff[4]<<8)|RxBuff[5];
	} 	
    
	return Res;
}


// 发送数据给MPU6050
bool MPU6050_Tx_Data(uint8_t DevAddr, uint16_t RegAddr, 
		uint8_t TxLen, uint8_t* Buff) 
{
	return (bool)(I2C_WriteRegister(DevAddr, RegAddr, Buff, TxLen));
}


// 接收来自MPU6050的数据
bool MPU6050_Rx_Data(uint8_t DevAddr, uint16_t RegAddr, 
		uint8_t RxLen, uint8_t* Buff) 
{
	return (bool)(I2C_WriteRegister(DevAddr, RegAddr, Buff, RxLen));
}
