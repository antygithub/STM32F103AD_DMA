#include "ad2ma.h"
#include "adc.h"
#include "usart.h"

/******************************采集一次AD******************************/
//使用端口PA2
uint16_t Collect_OnceAD(void)
{
    uint16_t AD_Value;
	/*##-1- Start the conversion process #######################################*/ 
      
	HAL_ADC_Start(&hadc1);		//启动ADC转换
 
      /*##-2- Wait for the end of conversion #####################################*/ 
       /*  Before starting a new conversion, you need to check the current state of
                  the peripheral; if it’s busy you need to wait for the end of current
                  conversion before starting a new one.
                  For simplicity reasons, this example is just waiting till the end of the
                  conversion, but application may perform other tasks while conversion
                  operation is ongoing. */
      HAL_ADC_PollForConversion(&hadc1, 50);		//等待转换完成 超时时间50ms
 
      /* Check if the continous conversion of regular channel is finished */ 
      if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
      {
          /*##-3- Get the converted value of regular channel  ######################*/
          AD_Value = HAL_ADC_GetValue(&hadc1);
//		  printf("AD_VAL %d\r\n", AD_Value);
		  return AD_Value;
//          printf("MCU Temperature : %.1f¡æ\r\n",((AD_Value*3300/4096-760)/2.5+25));
      }
//      HAL_Delay(1000);
}

/***************************** AD均值 *****************************/
uint16_t AVG_8(void)
{
	uint8_t i;
	uint16_t val = 0;
	
	for(i = 0; i < 8; i++)
	{
		val += Collect_OnceAD();
	}
	return (val >>= 3);
}

/***************************** 过采样 *****************************/
uint32_t Oversampling(void)
{
	uint16_t i;
	uint32_t VAL_SUM = 0;
	
	for(i = 0; i < 256; i++)
	{
//		VAL_SUM += (uint32_t)AVG_8();
		VAL_SUM += ADC_Value[i];
	}
	VAL_SUM >>= 4;
//	printf("VAL_SUM\t%d\r\n", VAL_SUM);
	return VAL_SUM;	
}

/***************************** AD转电流 *****************************/
//3.3V <-> 65535
//x10000uA
float AD2mA(void)
{
	uint8_t i;
	uint32_t COF = 34, temp = 0;
	float mA, COE = 3.5053;
	
//	for(i = 0; i < 32; i++)
//	{
//		temp += Oversampling();
//	}
//	temp >>= 5;
	temp = Oversampling();
	if(temp < A1)
	{
		mA = 0.0f;
	}
	else if(temp == A1)
	{
		mA = 4.0f;
	}
	else if((temp > A1)&&(temp < A2))
	{
		mA = 4.0f + K1 * (temp - A1) / 1000000l; 
	}
	else if(temp == A2)
	{
		mA = 12.0f;
	}
	else if(temp > A2)
	{
		mA = 12.0f + K1 * (temp - A2) / 1000000l;
	}
//	printf("%d\r\n", Oversampling());
//	mA = COE * Oversampling() / 10000;
	return mA;
}

/***************************** AD转电压 *****************************/
//x100 uV
uint32_t AD2mV(void)
{
	uint32_t mV, COF = 5035;
	
	mV = COF * (uint32_t)Oversampling();
	return mV;
}

/***************************** AD转PPM *****************************/
//x10^5PPM
uint32_t AD2PPM(void)
{
	uint32_t COF = 59586;
	
	return (COF * Oversampling());
}

