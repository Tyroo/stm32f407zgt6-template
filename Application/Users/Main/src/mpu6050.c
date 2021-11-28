#include "mpu6050.h"


// ��ʼ��MPU6050
bool MPU6050_Init(void)
{
	bool Res;
	
	/* ��λMPU6050 */
	Res = MPU6050_Tx_Data(PTU8(MPU6050_RESET_CMD), MPU6050_PWR_MGMT1_REG, 1);
	Delay_Ms(100);
	Res = MPU6050_Tx_Data(PTU8(MPU6050_RESET_CMD), MPU6050_PWR_MGMT1_REG, 1);
	
	/* ���ý��ٶȴ������ͼ��ٶȴ������������̷�Χ */
	// pass
	
	/* ������������ */
	//pass
	
	return Res;
}


//����MPU6050�����Ǵ����������̷�Χ
//Fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
bool MPU6050_SetGyroFsr(uint8_t Fsr)
{
	//���������������̷�Χ 
	return MPU6050_Tx_Data(PTU8(Fsr<<3), MPU6050_GYRO_CFG_REG, 1); 
}


//����MPU6050���ٶȴ����������̷�Χ
//Fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
bool MPU6050_SetAccelFsr(uint8_t Fsr)
{
	//���ü��ٶȴ����������̷�Χ  
	return MPU6050_Tx_Data(PTU8(Fsr<<3), MPU6050_ACCEL_CFG_REG, 1);
}


//����MPU6050�����ֵ�ͨ�˲���
//Lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
bool MPU6050_SetLPF(uint16_t Lpf)
{
	uint8_t Data;
	
	if (Lpf>=188) 	  Data = 1;
	else if (Lpf>=98) Data = 2;
	else if (Lpf>=42) Data = 3;
	else if (Lpf>=20) Data = 4;
	else if (Lpf>=10) Data = 5;
	else              Data = 6;
	
	return MPU6050_Tx_Data(&Data, MPU6050_CFG_REG, 1);//�������ֵ�ͨ�˲���  
}


//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//Rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
bool MPU6050_SetRate(uint16_t Rate)
{
	bool Res;
	uint8_t Data;
	
	if (Rate>1000) Rate = 1000;
	if (Rate<4) Rate = 4;
	
	Data = 1000/Rate - 1;
	Res = MPU6050_Tx_Data(&Data, MPU6050_SAMPLE_RATE_REG, 1);	//�������ֵ�ͨ�˲���
 	Res = MPU6050_SetLPF(Rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
	
	return Res;
}


//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU6050_GetTemperature(void)
{
	bool Res;
	short Raw;
	float Temp;
	
    uint8_t RxBuff[2]; 
	
	Res = MPU6050_Rx_Data(RxBuff, MPU6050_TEMP_OUTH_REG, 2); 
	
	if (Res == False) return 0;
	
    Raw = ((uint16_t)RxBuff[0]<<8) | RxBuff[1];  
    Temp = (36.53 + ((double)Raw)/340);	// ���ݹ�ʽ�����¶�
    
	return (Temp*100);
}


//�õ�������ֵ(ԭʼֵ)
//Gx,Gx,Gx:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU6050_GetGyroscope(short *Gx,short *Gy,short *Gz)
{
    uint8_t RxBuff[6], Res;  
	
	Res = MPU6050_Rx_Data(RxBuff, MPU6050_GYRO_XOUTH_REG, 6);
	
	if(Res == True)
	{
		*Gx=((uint16_t)RxBuff[0]<<8)|RxBuff[1];  
		*Gy=((uint16_t)RxBuff[2]<<8)|RxBuff[3];  
		*Gz=((uint16_t)RxBuff[4]<<8)|RxBuff[5];
	} 
	
    return Res;;
}


//�õ����ٶ�ֵ(ԭʼֵ)
//Ax,Ax,Ax:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU6050_GetAccelerometer(short *Ax,short *Ay,short *Az)
{
    uint8_t RxBuff[6], Res;  
	
	Res = MPU6050_Rx_Data(RxBuff, MPU6050_ACCEL_XOUTH_REG, 6);
	
	if(Res == True)
	{
		*Ax=((uint16_t)RxBuff[0]<<8)|RxBuff[1];  
		*Ay=((uint16_t)RxBuff[2]<<8)|RxBuff[3];  
		*Az=((uint16_t)RxBuff[4]<<8)|RxBuff[5];
	} 	
    
	return Res;
}


// �������ݸ�MPU6050
bool MPU6050_Tx_Data(uint8_t* Data,
		uint16_t Addr, uint8_t TxLen) 
{
	bool 	Res;	// Ӧ����Ӧ
	uint8_t Index;	// ������������

	Res = True, Index = 0;
	
	IIC_Start();					// ����IIC��ʼ�ź�
	Res = IIC_Send_Byte((MPU6050_DEV_ADDR<<1) | MPU6050_WRITE_CMD);// ���ͼĴ�����ַ��λ
	Res = IIC_Send_Byte(Addr); 		// ���ͼĴ�����ַ
	
//	/* �������ݴ�������, �������һ���ֽں�����Ӧ�� */
//	IIC_Start();					// ����IIC��ʼ�ź�
	Res = IIC_Send_Byte((MPU6050_DEV_ADDR<<1) | MPU6050_READ_CMD); // ���Ͷ�����
	
	if (!Res) 
	{ 
		IIC_Stop();
		return False;
	}
	
	while((Index < TxLen) && (Res == True)) 
	{
		Res = IIC_Send_Byte(*(Data+Index));	// ��ȡ��һ���ֽ�						
		Index++;
	}
	
	IIC_Stop();					// ֹͣ����
	
	return (bool)Res;
}


// ��������MPU6050������
bool MPU6050_Rx_Data(uint8_t* Buff, 
		uint16_t Addr, uint8_t RxLen) 
{
	bool 	Res;	// Ӧ����Ӧ
	uint8_t Index;	// ������������

	Res = True, Index = 0;
	
	IIC_Start();					// ����IIC��ʼ�ź�
	Res = IIC_Send_Byte((MPU6050_DEV_ADDR<<1) | MPU6050_WRITE_CMD);// ���ͼĴ�����ַ��λ
	Res = IIC_Send_Byte(Addr); 		// ���ͼĴ�����ַ
	
//	/* �������ݴ�������, �������һ���ֽں�����Ӧ�� */
//	IIC_Start();						// ����IIC��ʼ�ź�
	Res = IIC_Send_Byte((MPU6050_DEV_ADDR<<1) | MPU6050_READ_CMD); // ���Ͷ�����
	
	if (!Res) 
	{ 
		IIC_Stop();
		return False;
	}
	
	while((Index < RxLen) && (Res == True)) 
	{
		if (Index != 1)
			*(Buff+Index) = IIC_Read_Byte(1);	// ����Ӧ���ź�
		else
			*(Buff+Index) = IIC_Read_Byte(0);	// ���ͷ�Ӧ���ź�							
		Index++;
	}
	
	IIC_Stop();									// ֹͣ����
	
	return (bool)Res;
}
