#ifndef _key_control_h
#define _key_control_h

#include "zf_common_typedef.h"
#include "zf_common_headfile.h"
#include "beep.h"
#include "ui.h"
#include "motor.h"
#include "imu.h"

#define KEY_0       "/dev/zf_driver_gpio_key_0"         //P13
#define KEY_1       "/dev/zf_driver_gpio_key_1"         //P14
#define KEY_2       "/dev/zf_driver_gpio_key_2"         //P15
#define KEY_3       "/dev/zf_driver_gpio_key_3"         //P16
#define SWITCH_0    "/dev/zf_driver_gpio_switch_0"      //拨码1
#define SWITCH_1    "/dev/zf_driver_gpio_switch_1"      //拨码2

static const char* key_list[] = {KEY_0, KEY_1, KEY_2, KEY_3, SWITCH_0, SWITCH_1};

#define KEY_RELEASE_LEVEL           (1        )      // 按键的默认状态 也就是按键释放状态的电平       
#define KEY_MAX_SHOCK_PERIOD        (10       )      // 按键消抖检测时长 单位毫秒 低于这个时长的信号会被认为是杂波抖动
#define KEY_LONG_PRESS_PERIOD       (1000     )      // 最小长按时长 单位毫秒 高于这个时长的信号会被认为是长按动作

typedef enum
{
    KEY_P13,
    KEY_P14,
    KEY_P15,
    KEY_P16,
    SWITCH_1_ctrol,
    SWITCH_2_ctrol,
    KEY_NUMBER,
}key_index_enum;                 // 按键索引 对应上方定义的按键引脚个数 默认定义六个按键

typedef enum
{
    KEY_RELEASE,          // 按键释放状态  0
    KEY_SHORT_PRESS,      // 按键短按状态  1
    KEY_LONG_PRESS,       // 按键长按状态  2
}key_state_enum;

void key_scanner (void);
key_state_enum key_get_state (key_index_enum key_n);
void key_clear_state (key_index_enum key_n);
void key_clear_all_state (void);
void key_init (uint32 period);
void key_callback(void);


#endif