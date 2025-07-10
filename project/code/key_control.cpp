#include"code_headfiles.h"

static uint32               scanner_period = 0;             // 按键的扫描周期
static uint32               key_press_time[KEY_NUMBER];     // 按键信号持续时长
static key_state_enum       key_state[KEY_NUMBER];          // 按键状态


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键状态扫描
// 参数说明     void
// 返回参数     void
// 使用示例     key_scanner();
// 备注信息     这个函数放在主循环或者 PIT 中断中
//-------------------------------------------------------------------------------------------------------------------
void key_scanner (void)
{
    uint8 i = 0;
    for(i = 0; KEY_NUMBER > i; i ++)             //在几个按键中循环检查
    {
        if(KEY_RELEASE_LEVEL != gpio_get_level(key_list[i]))                   // 按键按下    
        {
            key_press_time[i] ++;
            /*if(KEY_LONG_PRESS_PERIOD / scanner_period <= key_press_time[i])
            {
                key_state[i] = KEY_LONG_PRESS;
            }*/
        }
        else                                                                    // 按键释放
        {
            if(KEY_LONG_PRESS_PERIOD / scanner_period <= key_press_time[i]){
                key_state[i] = KEY_LONG_PRESS;
            }
            else if(KEY_LONG_PRESS_PERIOD / scanner_period >= key_press_time[i] && (KEY_MAX_SHOCK_PERIOD / scanner_period <= key_press_time[i]))
            {
                key_state[i] = KEY_SHORT_PRESS;
            }
            else
            {
                key_state[i] = KEY_RELEASE;
            }
            key_press_time[i] = 0;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取按键状态
// 参数说明     key_n           按键索引
// 返回参数     key_state_enum  按键状态
// 使用示例     key_get_state(KEY_P13);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
key_state_enum key_get_state (key_index_enum key_n)
{
    return key_state[key_n];
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清除指定按键状态
// 参数说明     key_n           按键索引
// 返回参数     void            无
// 使用示例     key_clear_state(KEY_P13);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void key_clear_state (key_index_enum key_n)
{
    key_state[key_n] = KEY_RELEASE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清除所有按键状态
// 参数说明     void            无
// 返回参数     void            无
// 使用示例     key_clear_all_state();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void key_clear_all_state (void)
{
    uint8 loop_temp = 0; 
    for(loop_temp = 0; KEY_NUMBER > loop_temp; loop_temp ++)
    {
        key_state[loop_temp] = KEY_RELEASE;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键初始化
// 参数说明     period          按键扫描周期 以毫秒为单位
// 返回参数     void
// 使用示例     key_init(10);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void key_init (uint32 period)
{
    key_clear_all_state();
    scanner_period = period;
    // pit_ms_init(period, key_scanner);
    // pit_ms_init(period, key_callback);
}

/**
  * @brief  按键轮询
  * @param  None
  * @note   放在定时器中断中,与UI界面联系在一起
  * @retval None
  */
 void key_callback(void)
 {
    //目前是KEY_P13，之后换成右按键
    if(key_get_state(KEY_P13) == KEY_SHORT_PRESS)       //到时候迁移到右键按键上面，短按页面右滑或者参数增大
    {

        beep_run();

        ui_shift(1);              //页面右滑
        ui_param_set(1);          //参数增大

        key_clear_state(KEY_P13);
    }
    else if(key_get_state(KEY_P13) == KEY_LONG_PRESS)
    {

        /*写操作*/

        key_clear_state(KEY_P13);
    }

    //目前是KEY_P14，之后换成左键
    if(key_get_state(KEY_P14) == KEY_SHORT_PRESS)       //到时候迁移到左键按键上面，短按页面左滑或者参数减小
    {

        beep_run();

        ui_shift(0);              //页面左滑
        ui_param_set(0);          //参数减小

        key_clear_state(KEY_P14);
    }
    else if(key_get_state(KEY_P14) == KEY_LONG_PRESS)
    {

        /*写操作*/

        key_clear_state(KEY_P14);
    }

    //目前是KEY_P15，之后换成下键
    if(key_get_state(KEY_P15) == KEY_SHORT_PRESS)        //到时候迁移到下键按键上面，短按下滑
    {      

        beep_run();
        ui_param_set_dir(0);
    //    ui_param_set_toggle(1);

        key_clear_state(KEY_P15);
    }
    else if(key_get_state(KEY_P15) == KEY_LONG_PRESS)
    {

        /*写操作*/

        key_clear_state(KEY_P15);
    }

    //目前是KEY_P16，之后换成上键
    if(key_get_state(KEY_P16) == KEY_SHORT_PRESS)       //到时候迁移到上键按键上面，短按上滑
    {

        beep_run();
        ui_param_set_dir(1);

        key_clear_state(KEY_P16);
    }
    else if(key_get_state(KEY_P16) == KEY_LONG_PRESS)
    {

        /*写操作*/

        key_clear_state(KEY_P16);
    }

    //拨码1
    if(gpio_get_level(SWITCH_0) == KEY_RELEASE_LEVEL)       //拨码处于左边
    {
        key_clear_state(SWITCH_1_ctrol);
    }
    else                                                   //拨码处于右边
    {
        key_clear_state(SWITCH_1_ctrol);
    }


    //拨码2,用来换调参标志位状态
    if(gpio_get_level(SWITCH_1) == KEY_RELEASE_LEVEL)      
    {

      //  beep_run();

        ui_param_set_toggle(1);     //调参界面

        ImuStruct.integral_yaw_flag = 1; 

        key_clear_state(SWITCH_1_ctrol);
    }
    else
    {

        ui_param_set_toggle(0);     //滑动界面

        ImuStruct.integral_yaw_flag = 0; 

        key_clear_state(SWITCH_1_ctrol);
    }
 }