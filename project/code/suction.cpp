#include "suction.h"

struct pwm_info pwm_1_info;
struct pwm_info pwm_2_info;


 /**
  * @brief  负压电调初始化
  * @param  None
  * @note 目前频率50HZ，周期为20ms，最大值为10000

        // 计算无刷电调转速   （1ms - 2ms）/20ms * 10000（10000是PWM的满占空比时候的值）
        // 无刷电调转速 0%   为 500
        // 无刷电调转速 20%  为 600
        // 无刷电调转速 40%  为 700
        // 无刷电调转速 60%  为 800
        // 无刷电调转速 80%  为 900
        // 无刷电调转速 100% 为 1000
  * @retval None
  */
 void suction_init(uint16 duty_1,uint16 duty_2)
 {
    // 获取PWM设备信息
    pwm_get_dev_info(Suction_PWM_1, &pwm_1_info);
    pwm_get_dev_info(Suction_PWM_2, &pwm_2_info);

    pwm_set_duty(Suction_PWM_1, duty_1);
    pwm_set_duty(Suction_PWM_2, duty_2);
 }

