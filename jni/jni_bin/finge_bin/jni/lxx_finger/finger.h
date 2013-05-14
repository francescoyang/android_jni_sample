#ifndef  __FINGER_H
#define  __FINGER_H


/**********************错误返回码***********************/

#define ZAZ_OK                0x00	    //表示指令执行OK
#define ZAZ_COMM_ERR          0x01		//表示数据包接受错误
#define ZAZ_NO_FINGER         0x02		//表示无指纹输入
#define ZAZ_GET_IMG_ERR       0x03		//表示录入指纹图像失败
#define ZAZ_FP_TOO_DRY        0x04		
#define ZAZ_FP_TOO_WET        0x05
#define ZAZ_FP_DISORDER       0x06
#define ZAZ_LITTLE_FEATURE    0x07	   //表示特征点太少而生不成特征
#define ZAZ_NOT_MATCH         0x08	   //表示指纹不匹配
#define ZAZ_NOT_SEARCHED      0x09	   //表示没搜索到指纹
#define ZAZ_MERGE_ERR         0x0a	   //表示特征合并失败
#define ZAZ_ADDRESS_OVER      0x0b	   //表示访问指纹库时地址序号超出指纹库范围
#define ZAZ_READ_ERR          0x0c	   //表示指纹库读模版出错或无效
#define ZAZ_UP_TEMP_ERR       0x0d	   //表示上传特征失败
#define ZAZ_RECV_ERR          0x0e	   //表示特征不能接受后续数据包
#define ZAZ_UP_IMG_ERR        0x0f	   //表示上传图像失败
#define ZAZ_DEL_TEMP_ERR      0x10	   //表示删除模版失败
#define ZAZ_CLEAR_TEMP_ERR    0x11	   //表示清空指纹库失败
#define ZAZ_SLEEP_ERR         0x12
#define ZAZ_INVALID_PASSWORD  0x13	   //表示口令不正确
#define ZAZ_RESET_ERR         0x14	   //表示复位失败
#define ZAZ_INVALID_IMAGE     0x15	   
#define ZAZ_HANGOVER_UNREMOVE 0x17
#define ZAZ_FLASH_ERR         0x18
#define ZAZ_NODEFINE_ERR      0x19


/**********************缓冲区*************************/
#define CHAR_BUFFER_1          0x01
#define CHAR_BUFFER_2          0x02
#define CHAR_BUFFER_3          0x03
#define CHAR_BUFFER_4		   0x04
#define MODEL_BUFFER           0x05

/************************系统基本参数序号*****************/
int SysPara_num[3]={0x04,0x05,0x06};

int ZAZ_Read_SysPara[6]={0x01,0x00,0x03,0x3f,0x00,0x43}; //读系统参数

/**************************应答包************************/
int UART_RECEVICE_BUFFER[24]={0};			  //应答包
int Subsequent_RECEVICE_BUFFER[2048];				  //后续包	  
int Subsequent_RECEVICE_DATA[1024]={0};
int Subsequent_RECEVICE_LENGTH[24]={0};
int Subsequent_RECEVICE_SUM[10]={0};

int BUFFER_NUM;

/*********************ZAZ-404-B通信协议定义*********************/
int ZAZ_Pack_Head[6]     = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};      //协议包头
// system command
int ZAZ_Templete_Num[6]  ={0x01,0x00,0x03,0x4D,0x00,0x51 };      //获得模版总数															 

// Fingerprint processing 
int ZAZ_Set_Syspara[8]   ={0x01,0x00,0x05,0x3e,0x05,0x02,0x00,0x4b};	 //设置系统基本参数安全等级为2
int ZAZ_Get_Img[6]       = {0x01,0x00,0x03,0x31,0x00,0x35};       //录指纹图像
int ZAZ_Up_Image[6]      = {0x01,0x00,0x03,0x3a,0x00,0x3e};      //上传图像
int ZAZ_Down_Image[6]    = {0x01,0x00,0x03,0x3b,0x00,0x3f};      //下载图像
int ZAZ_Img_To_Buffer[7] = {0x01,0x00,0x04,0x32,0x01,0x00,0x38}; //将图像放入到CHAR_BUFFER
int ZAZ_Reg_Model[6]     = {0x01,0x00,0x03,0x35,0x00,0x39};        //将CHAR_BUFFER_1、2、3、4合成特征模版
int ZAZ_Up_Buffer[7]    = {0x01,0x00,0x04,0x38,0x01,0x00,0x3e};  //将CHAR_BUFFER上传给上位机
int ZAZ_Down_To_Buffer[7] = {0x01,0x00,0x04,0x39,0x01,0x00,0x3f};	//上位机下载特征文件到CHAR_BUFFER
int ZAZ_Match[6]       = {0x01,0x00,0x03,0x33,0x00,0x37};        //精确比对两枚指纹特征
int ZAZ_Search[11]     ={0x01,0x00,0x08,0x34,0x01,0x00,0x00,0x03,0xA1,0x00,0xe2}; //搜索指纹搜索范围0 - 929
int ZAZ_Delet_All_Model[6]={0x01,0x00,0x03,0x3d,0x00,0x41};//删除指纹模块里所有的模版
int ZAZ_Save_Finger[9]={0x01,0x00,0x06,0x36,0x01,0x00,0x0B,0x00,0x49};//将BUFFER1中的特征码存放到指定的位置
int ZAZ_Load_Char[9]  ={0x01,0x00,0x06,0x37,0x01,0x00,0x0A,0x00,0x49};	 //将 flash 数据库中指定 ID 号的指纹模板读入到模板缓冲区
int ZAZ_Delete_Model[10]={0x01,0x00,0x07,0x3C,0x00,0x00,0x00,0x01,0x00,0x45}; //删除指定的模版


#endif

										   