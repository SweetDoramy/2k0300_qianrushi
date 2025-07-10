#include"encoder.h"

EncoderStructTypeDef Encoderstruct;

/**
  * @brief  编码器定时器回调函数
  * @param  None
  * @note   放入定时器中断当中
  *         从左往右看，车轮逆时针，编码器顺时针，车子正跑，我这里认为是电机正转
  *         而两个编码器对称排布，所以跑起来应该是一正一负，这里用的又是同一个库函数，就是读值
  * @retval None
  */
void pit_callback_encoder(void)
{
    Encoderstruct.encoder_left  = encoder_get_count(ENCODER_1);           // PWM_F / 64
    Encoderstruct.encoder_right = encoder_get_count(ENCODER_2);

    // printf("Encoderstruct.encoder_left = %d.\r\n",  Encoderstruct.encoder_left);
    // printf("Encoderstruct.encoder_right = %d.\r\n",  Encoderstruct.encoder_right);
}


/**
  * @brief  编码器初始化
  * @param  None
  * @note   创建一个定时器1ms周期，回调函数为pit_callback,每10ms采集一次数据
  * @retval None
  */
 void encoder_init(void)
 {
    pit_ms_init(1, pit_callback_encoder);
 }


