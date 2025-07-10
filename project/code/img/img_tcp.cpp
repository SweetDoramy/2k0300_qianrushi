#include "img_tcp.h"
int fps_counter = 0;
int fps_img = 0;
int timer_counter = 0;
int frequence_control = 0;
int frequence_control_counter = 0;

int fps_ui = 0;
int fps_while = 0;

static uint8 image_copy[IMG_HEIGHT][IMG_WIDTH];

/*------------------------------------WIFI 图传边线 不允许改动!--------------------------------------*/
uint16 leftLine[IMG_HEIGHT],rightLine[IMG_HEIGHT],midLine[IMG_HEIGHT];
/*------------------------------------WIFI 图传边线 不允许改动!--------------------------------------*/

/*------------------------------------tcp_init--------------------------------------*/
// 函数简介     启用逐飞 tcp-client，并进行连接和收发回调函数配置
// 参数说明    	ture 打开 false 关闭
// 返回参数     void, int*,int*,int*,int*,int*,int*
// 备注信息     需要放在循环外进行初始化
/*------------------------------------tcp_init--------------------------------------*/
void fps_pit_init(){
    pit_ms_init(1000, fps_callback);
}
void fps_callback(){
    fps_img = fps_counter;
    fps_counter = 0;
    frequence_control = frequence_control_counter;
    frequence_control_counter = 0 ;
    fps_ui = fps_while;
    fps_while = 0;
    cout << "fps:" + to_string(fps_img) << endl;
    cout << "fre:" + to_string(frequence_control) << endl;
    cout << "while:" + to_string(fps_ui) << endl;
}


/*------------------------------------tcp_init--------------------------------------*/
// 函数简介     启用逐飞 tcp-client，并进行连接和收发回调函数配置
// 参数说明    	ture 打开 false 关闭
// 返回参数     void, int*,int*,int*,int*,int*,int*
// 备注信息     需要放在循环外进行初始化
/*------------------------------------tcp_init--------------------------------------*/
void tcp_init(bool a){
    if(a){
        // 初始化TCP客户端,需要先打开TCP服务器,这才不会卡主。
        if(tcp_client_init(SERVER_IP, PORT) == 0){
            printf("tcp_client ok\r\n");
        }
        else{
            printf("tcp_client error\r\n");
            return;
        }
        // 逐飞助手初始化 设置回调函数
        seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);

        // for(uint16 i = 0; i < IMG_HEIGHT; i ++){
        //     leftLine[i] = i;
        //     midLine[i] = 160;
        //     rightLine[i] = 319 - i;
        // }

        seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], IMG_WIDTH, IMG_HEIGHT);
        //seekfree_assistant_camera_boundary_config(X_BOUNDARY, IMG_HEIGHT, img_data.leftline, img_data.cenline, img_data.rightline, NULL, NULL, NULL);
        //seekfree_assistant_camera_boundary_config(X_BOUNDARY, IMG_HEIGHT, leftLine, midLine, rightLine, NULL, NULL, NULL);
    }
}




/*------------------------------------send_img_to_tcp--------------------------------------*/
// 函数简介     向服务器端输出实时图像并标注准线
// 参数说明    	Mat  //必须是灰度图像指针
                    //必须是灰度图像指针
                    //必须是灰度图像指针
// 参数说明     linex1          存放边线1横坐标的地址  如果传递NULL参数则表示不发送边线1
// 参数说明     linex2          存放边线2横坐标的地址  如果传递NULL参数则表示不发送边线2
// 参数说明     linex3          存放边线3横坐标的地址  如果传递NULL参数则表示不发送边线3
// 参数说明     liney1          存放边线1纵坐标的地址  如果传递NULL参数则表示不发送边线1
// 参数说明     liney2          存放边线2纵坐标的地址  如果传递NULL参数则表示不发送边线2
// 参数说明     liney3          存放边线3纵坐标的地址  如果传递NULL参数则表示不发送边线3
// 返回参数     void
// 使用示例     send_img_to_tcp(img,NULL,NULL,NULL,NULL,NULL,NULL);
// 备注信息     需要放在循环外进行初始化
/*------------------------------------send_img_to_tcp-----------------*---------------------*/
void send_img_to_tcp(cv::Mat img){
    //uint8 image_copy[IMG_HEIGHT][IMG_WIDTH];
    uint8_t* pictures;
    pictures = reinterpret_cast<uint8_t *>(img.ptr(0));
    int32 i = 0,j = 0;
    //seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], IMG_WIDTH, IMG_HEIGHT);
    memcpy(image_copy[0], pictures, IMG_WIDTH * IMG_HEIGHT);
    //seekfree_assistant_camera_boundary_config(X_BOUNDARY, IMG_HEIGHT, img_data.leftline, img_data.cenline, img_data.rightline, NULL, NULL, NULL);
    seekfree_assistant_camera_send();
}
   



void ips200_show(cv::Mat img){
    // resize(img,img,Size(),1,0.75);
    uint8_t* pictures;
    pictures = reinterpret_cast<uint8_t *>(img.ptr(0));
    ips200_show_gray_image(0, 0, pictures, IMG_WIDTH , IMG_HEIGHT);
}