#include"oscilloscope.h"
#include"img_tcp.h"

static uint8 tail[4] = {0x00, 0x00, 0x80, 0x7f};

/**
  * @brief  示波器发送数据
  * @param  None
  * @note  解析上位机发送过来的参数，解析后数据会存放在seekfree_assistant_oscilloscope_data数组中，可以通过在线调试的方式查看数据
           例程为了方便因此写在了主循环，实际使用中推荐放到周期中断等位置，需要确保函数能够及时的被调用，调用周期不超过20ms
           放在定时器中断中
  * @retval None
  */
 static void pit_callback_oscilloscope_tcp_client(void)
 {
     seekfree_assistant_data_analysis();
 }



 /**
  * @brief  示波器初始化
  * @param  None
  * @note   TCP server,逐飞助手本机IP
  * @retval None
  */
void oscilloscope_tcp_client_init(void)
{
    if(tcp_client_init(SERVER_IP, PORT) == 0)
    {
        printf("tcp_client ok\r\n");
    }
    else
    {
        printf("tcp_client error\r\n");
        return;
    }

    // 逐飞助手初始化 设置回调函数
    seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);
    // 定时器
    pit_ms_init(20, pit_callback_oscilloscope_tcp_client);
}


/**
  * @brief  发送justfloat给vofa 2个通道
  * @param  None
  * @note   None
  * @retval None
  */
 void vofa_send_justfloat_2ch(float data1, float data2)
 {
     float temp[2] = {data1, data2};
     tcp_client_send_data((uint8*)temp, sizeof(float) * 2);
     tcp_client_send_data(tail, 4);
 }


/**
  * @brief  发送justfloat给vofa 3个通道
  * @param  None
  * @note   None
  * @retval None
  */
 void vofa_send_justfloat_3ch(float data1, float data2, float data3)
 {
     float temp[3] = {data1, data2, data3};
     tcp_client_send_data((uint8*)temp, sizeof(float) * 3);
     tcp_client_send_data(tail, 4);
 }


 /**
  * @brief  发送justfloat给vofa 8个通道
  * @param  None
  * @note   None
  * @retval None
  */
 void vofa_send_justfloat_8ch(float data1, float data2,float data3, float data4,float data5, float data6,float data7, float data8)
 {
     float temp[8] = {data1, data2, data3, data4, data5, data6, data7, data8};
     tcp_client_send_data((uint8*)temp, sizeof(float) * 8);
     tcp_client_send_data(tail, 4);
 }