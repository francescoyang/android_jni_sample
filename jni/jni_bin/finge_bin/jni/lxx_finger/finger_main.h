 /*****************函数实现定义******************/
  
int FINGERPRINT_Recevice_Data(unsigned int ucLength); //接收反馈数据缓冲
int * ZAZ_ReadSysPara(void);     						//读模块系统基本参数
int ZAZ_GetImag(void);								//录入指纹图像
int ZAZ_GenChar(int BufferID);        // 图像生成特征存于BufferID中
int ZAZ_RegModel(void);     		 //将BUFFER1 跟 BUFFER2 中的特征码合并成指纹模版
int ZAZ_UpChar(int BufferID);			//将特征缓冲区 CharBuffer1、2、3、4中的特征文件上传给上位机
int ZAZ_DownChar(int BufferID);			//上位机下载特征文件到模块的一个特征缓冲区中
int ZAZ_Empty(void); 	  //删除指纹模块里的所有指纹模版
int ZAZ_DeletChar(int uiID_temp,int N_temp );	//删除指纹模块里的指定指纹模版
int * ZAZ_TempleteNum(void);					 //获得指纹模板数量
int * ZAZ_Two_Match(void);						 //精确比对两枚指纹的特征
int * ZAZ_Search_Finger(int BufferID,int StartPage,int PageNum); //搜索指纹库
int ZAZ_StoreChar( int BuffID,int Finger_Num ); //存储指纹模版
int ZAZ_Loadchar(int BuffID,int Finger_Num);  // 读出模版 ：将 flash 数据库中指定 ID 号的指纹模板读入到模板缓冲区

int ZAZ_add_new_user(int BufferID,int Finger_Num);   //指纹添加新用户 存在在指纹库中
int ZAZ_addnewuser_To_Pc(int BufferID,int Finger_Num);	//指纹添加新用户 存在在上位机中
int ZAZ_flashlib_to_pc(int BufferID,int Finger_Num);   //从 flash 指纹库中读取一个指定的模板上传
int ZAZ_Search_Comparison(int BufferID,int StartPage,int PageNum);	//指纹模块内比对
int ZAZ_Match_Comparison(int BufferID1,int BufferID2);								//上位机下载模块到指纹模块内比对（1:1）

int ZAZ_FingerLIB_Search_Comparison(int BufferID,int StartPage,int PageNum); //上位机下载模块到指纹库内比对（1:N）

