/*
 *                 <www.ruiva.com.cn>       
 *        
 *                S3C2410 串口通信实验
 *                       
 *                    2008.08.13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <termios.h>
#ifdef READ_ID
#endif

//#define IC_CARD_DEBUG 1
#define DATA        0x00
#define PSEL        0x01
#define FCONB       0x02
#define EGT         0x03
#define CRCSEL      0x04
#define RSTAT       0x05
#define SCNTL       0x06
#define INTCON      0x07
#define RSCH        0x08
#define RSCL        0x09    
#define CRCH        0x0a
#define CRCL        0x0b
#define TMRH        0x0c
#define TMRL        0x0d
#define BPOS        0x0e
#define SMOD        0x10
#define PWTH        0x11

#define FEND        0x01
#define CRCERR      0x02
#define TMROVER     0x04
#define DATOVER     0x08
#define FERR        0x10
#define PERR        0x20
#define CERR        0x40
   
#define TYPE_A      0x10
#define TYPE_B      0x00
#define ISO15693    0x20
#define ETK         0x30
#define MIFARE      0x50
#define SND_BAUD_106K   0x00
#define SND_BAUD_212K   0x04
#define SND_BAUD_424K   0x08
#define SND_BAUD_848K   0x0c
    
#define RCV_BAUD_106K   0x00
#define RCV_BAUD_212K   0x01
#define RCV_BAUD_424K   0x02
#define RCV_BAUD_848K   0x03
    
#define MBLOCK_SIZE   0x10
#define MWALLET_SIZE  0x04



#define STAT_CTRL     0x12
#define FM_CTRL       0x13
#define UART_STAT     0x14
#define CRYPTO1_CTRL  0x15
#define DATA0     0x16
#define DATA1     0x17
#define DATA2     0x18
#define DATA3     0x19
#define DATA4     0x1A
#define COL_STAT      0x1B
#define SND_CTRL      0x1C

unsigned char REQA[2] = {0x1,0x26};
unsigned char HLTA[2] = {0x1,0x52};
unsigned char ANTI[3] = { 0x2,0x93,0x20 }; 
unsigned char SELECT[3] = { 0x7,0x93,0x70 }; 
unsigned char AUTHA[2] = { 0x2,0x60 };
unsigned char m[5] ={0x52,0x26,0x93,0x20};
unsigned char comd[14][7] ={
          {0x01,0x26,0x00},{0x01,0x50,0x00},{0x02,0x93,0x20},
          {0x07,0x93,0x70},{0x02,0x60,0x00},{0x02,0x61,0x00},
          {0x02,0x30,0x00},{0x02,0xa0,0x00},{0x02,0xb0,0x00},
          {0x02,0xc2,0x00},{0x02,0xc0,0x00},{0x02,0xc1,0x00},
          {0x01,0x52,0x00},{0x00,0x00,0x00}
};

int fd, loops, status = 0;

enum { 
    REQA_CMD,
    HLTA_CMD,
    ANTI_CMD,
	SELECT_CMD,
    AUTHA_CMD,
	AUTHB_CMD,
	READ_CMD,
	WRITE_CMD,
	TRANSFER_CMD,
	RESTORE_CMD,
	DECREMENT_CMD,
	INCREMENT_CMD,
	WUPA_CMD
}; 

unsigned char temp[20];
unsigned char gUID[5];
unsigned char gKEYA[6];
unsigned char gKEYB[6];
//------------------ 串口配置 ------------------ 

int init_port(int fd,int baud_rate)
{
    struct termios option;

//------------------ 获得当前配置选项 ------------------
    tcgetattr( fd,&option );    

#if 1
//------------------ 设置为本地模式并使能接受 -----------------
    option.c_cflag |= (CLOCAL | CREAD);

//----------------- 设置数据位位数为 8 ------------------ 
    option.c_cflag &= ~CSIZE; 
    option.c_cflag |= CS8;

//------------------ 设置为无奇偶校验 ------------------       
    option.c_cflag &= ~PARENB;

//------------------ 设置停止位为1 -----------------
    option.c_cflag &= ~CSTOPB;

//------------------- 设置为无软件/硬件控制流 ------------------
    option.c_cflag &= ~(CRTSCTS);
 
    option.c_iflag &= ~(IXON | INLCR | ICRNL | IGNCR | IUCLC);

//------------------ 设置输出模式为非预处理 ------------------
    option.c_oflag &= ~OPOST;

//------------------ 设置输入为非格式化输入 ------------------
    option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);


//------------------ 所读最小字符数 ------------------
    option.c_cc[VMIN] = 1;

//------------------ 等待数据时间 -----------------
    option.c_cc[VTIME] = 10;

    option.c_cc[VINTR] = 0;  
    option.c_cc[VQUIT] = 0;  
    option.c_cc[VERASE] = 0;  
    option.c_cc[VKILL] = 0;  
    option.c_cc[VEOF] = 0;  
    option.c_cc[VTIME] = 10;  
    option.c_cc[VMIN] = 1;  
    option.c_cc[VSWTC] = 0;  
    option.c_cc[VSTART] = 0;  
    option.c_cc[VSTOP] = 0;  
    option.c_cc[VSUSP] = 0;  
    option.c_cc[VEOL] = 0;  
    option.c_cc[VREPRINT] = 0;  
    option.c_cc[VDISCARD] = 0;  
    option.c_cc[VWERASE] = 0;  
    option.c_cc[VLNEXT] = 0;  
    option.c_cc[VEOL2] = 0;  


#endif

//------------------ 设置波特率 ------------------  
    switch(baud_rate)
    {
        case 2400:
                   cfsetispeed(&option, B2400);
                   cfsetospeed(&option, B2400);
                   break;
        case 4800:
                   cfsetispeed(&option, B4800);
                   cfsetospeed(&option, B4800);
                   break;
        case 9600:
                   cfsetispeed(&option, B9600);
                   cfsetospeed(&option, B9600);
                   break;
        case 115200:
                   cfsetispeed(&option, B115200);
                   cfsetospeed(&option, B115200);
                   break;
        default:
                   cfsetispeed(&option, B9600);
                   cfsetospeed(&option, B9600);
                   break;
    }


    tcflush(fd, TCIOFLUSH);
//------------------ 立即保存以上配置 -----------------    
    if((tcsetattr (fd,TCSANOW,&option)) != 0)
    {
        return -1;
    }

    return 0;
}

//------------------ 打开端口 ------------------
int open_port(char * dev)
{
    int fd;
    int fcntl_ret;

//------------------ 以非控制台模式打开设备并忽略对端状态 -------------------
    fd = open(dev, O_RDWR | O_NOCTTY);
    if (-1 == fd)
        return -1;

//------------------ 恢复打开端口为阻塞模式 ------------------
    fcntl_ret = fcntl (fd, F_SETFL, 0);
    // fcntl_ret = fcntl (fd, F_GETFL, 0);
    if (fcntl_ret < 0)
        return -2;
    
    return fd;
}
void SPI_RecvBuff(unsigned char *buf,unsigned int num)
{
	int read_num=0;	
	read_num = read(fd, buf,num);
}
void SPI_SendBuff(unsigned char *buf,unsigned int num)
{
	int write_num = 0;
	write_num = write(fd, buf, num);
	
}	   


void THM_WriteReg(unsigned char address,unsigned char content)
{
	unsigned char temp_buff[2];
	// BIT7 =1	
	temp_buff[0] = address | 0x80;
	temp_buff[1] = content;
	// SPI 
	SPI_SendBuff(temp_buff,2);	
	usleep(12000);
}

unsigned char THM_ReadReg(unsigned char address)
{
	unsigned char temp_buff[1];
	unsigned char temp_buff0[1];

	temp_buff[0] = address & 0x7F;
	SPI_SendBuff(temp_buff,1);
	SPI_RecvBuff(temp_buff0,1);
	usleep(12000);
	return(temp_buff0[0]);
}
void THM_CloseRF()
{
    
	THM_WriteReg(SCNTL,0x00);
	return;
		
}
void THM_OpenRF()
{
	THM_WriteReg(SCNTL,0x01);
	return;
}
//send data frame mifare	 
void THM_MSendFrame(unsigned char *buffer,unsigned short num)
{
	unsigned char temp;	
	THM_WriteReg(SCNTL, 0x5);	                                //RAMPT_CLR =1,CLK_EN =1
	THM_WriteReg(SCNTL, 0x01);                                  // RAMPT_CLK=0;	
	
	temp = DATA | 0x80;			                             
	                        									//write mode 
	
	SPI_SendBuff(&temp,1);
	SPI_SendBuff(buffer,num);	                      
	usleep(12000);
	
	THM_WriteReg(SND_CTRL, 0x01);              
}		

//Waiting for receiving end, write the received data to the "buffer" ,its length is "len"
//Return of function is the receiving status
unsigned char THM_MWaitReadFrame(unsigned short *len, unsigned char *buffer)
{
	unsigned char temp,temp1;	
	*len =0;
	//waiting the end of receive
	while (1)	
	{  	     
	   temp = THM_ReadReg(UART_STAT);   
	   if (temp & 0xFF)
	        break;
        }	           
	//handling the receiving status
        if (temp & 0xEF) 
	{
	    THM_WriteReg(UART_STAT,0x00);
		return (temp & 0xEF);		
	}
	//read the data length
        *len =((unsigned short)(THM_ReadReg(RSCH)) <<8 ) + THM_ReadReg(RSCL);  		    
	//read the data
        if (*len != 0x00 )
        {
        
        	temp1 = DATA;
        	SPI_SendBuff( &temp1,1);
        	SPI_RecvBuff( buffer,*len);
		usleep(12000);
        
    	}  
	//clear the status
    	THM_WriteReg(UART_STAT,0x00);        
    	return (temp);
}

/////////////////////Program Body////////////////////////////////////////////////////////////

//Function: Change to THM3060 to Mifare Mode and Find& Select card
//Parameter: OUT b_uid, card's UID , 4 bytes
//Return value:   00    OK,Mifare card was found and Selected
//                01    Err, Not found
//                02    Err, Not 4 UIDs,May be other type
//                03    Err, ATQA Err
//                04    Err, Other  

unsigned char THM_MFindCard(unsigned char * b_uid)
{
    unsigned short iLen;
    unsigned char i,j;	
    unsigned char a;
    //Change to Mifare mode
    THM_WriteReg(PSEL,MIFARE|SND_BAUD_106K|RCV_BAUD_106K);
    //Disable CRC
    THM_WriteReg(CRCSEL,0x01);
    //Reset Cryppto1
    THM_WriteReg(FM_CTRL,0xc0);
    // don't care FDT, don't Encrypt  
    THM_WriteReg(STAT_CTRL,0x00);
    //Short Frame
    THM_WriteReg(FM_CTRL,0x40);
    //Send WUPA    
    THM_MSendFrame(&comd[WUPA_CMD][1],comd[WUPA_CMD][0]);  

    THM_MWaitReadFrame(&iLen, temp);
    if (iLen != 2)  //ATQA must equal 2
    {
        //Retry
        THM_MSendFrame(&comd[REQA_CMD][1],comd[REQA_CMD][0]);  
        THM_MWaitReadFrame(&iLen, temp);    
    }    
    if (iLen!= 2)  // ATQA must equal 2 
    {
		//Enable CRC
    	THM_WriteReg(CRCSEL,0xC1); 
        if (iLen == 0) return 0x01;   // no response
        return 0x3;                   
    }
    if ( (temp[0]& 0xC0)!= 0x00 )     
    {
		//Enable CRC
    	THM_WriteReg(CRCSEL,0xC1); 
        return 0x2;                   //not single UID 
    }    
    //Single UID CARD
    //ANTIcollision command,Bit Oriented Frame    
    THM_WriteReg(FM_CTRL,0x46);
    THM_MSendFrame(&comd[ANTI_CMD][1],comd[ANTI_CMD][0]); 
    THM_MWaitReadFrame(&iLen, temp);          
    if (iLen !=5) return 4;
    
    for (i =0; i< 5;i++)
    {
        gUID[i] = temp[i];
        *b_uid++ = temp[i];
    }    
    //SELECT Cmd,
    temp[0] = SELECT[1];
    temp[1] = SELECT[2];
    for (i =0; i< 5;i++)
    {
        temp[2+i]= gUID[i];        
    }
    //Enable CRC
    THM_WriteReg(CRCSEL,0xC1); 
	//Standard Frame                            
    THM_WriteReg(FM_CTRL,0x42);
    THM_MSendFrame(temp,0x07); 
    THM_MWaitReadFrame(&iLen, temp);  
    if (iLen!=3)
        return 4;
    else
        return 0x0;
    
} 

//Intialize KEY A and KEY B
void THM_MInitialKEY(unsigned char * keya, unsigned char * keyb) 
{
    unsigned char i;
    for (i =0; i<6;i++)
    {
        gKEYA[i] = *keya++;
        gKEYB[i] = *keyb++;
    }
    return;

}

// Mifare Authentication 
//Function:  Mifare Authentication 
//Parameter: IN bBlock  Card Block
//           IN bKeyNo  00 or 60 KEYA
//                      01 or 61 KEYB    
//Return value:   00    OK,Auth OK
//                01    Err, PARA wrong
//                02    Err, Get Rand
//                03    Auth Error
//                04    Other Error

unsigned char THM_MAuthentication(unsigned char bKeyNo,unsigned char bBlock)
{
    unsigned short iLen;
	unsigned char i;
	unsigned char tmp;
    unsigned char ms[4];
	  
    if ((bKeyNo == 0x00) || (bKeyNo == 0x60))    
    { // KEY A
        temp[0] = comd[AUTHA_CMD][1];
        temp[1] = bBlock;
        //Get Rand
        THM_MSendFrame(temp,comd[AUTHA_CMD][0]); 
    }    
    else if ((bKeyNo == 0x01) || (bKeyNo == 0x61))
    { // KEY B
        temp[0] = comd[AUTHB_CMD][1];
        temp[1] = bBlock;
        //Get Rand
        THM_MSendFrame(temp,comd[AUTHB_CMD][0]);     
    }
    else
	{
	    //Reset Crypto1
    	THM_WriteReg(FM_CTRL,0xc0); 
        return 0x1;
	}
    THM_MWaitReadFrame(&iLen, temp);  
    if (iLen !=4)
	{
	    //Reset Crypto1
    	THM_WriteReg(FM_CTRL,0xc0); 
        return 0x2;
	}
    //Enable Cyrpto1        
    THM_WriteReg(CRYPTO1_CTRL,0x08);        
    //WRITE Rand1
    THM_WriteReg(DATA1,temp[0]);
    THM_WriteReg(DATA2,temp[1]);
    THM_WriteReg(DATA3,temp[2]);
    THM_WriteReg(DATA4,temp[3]);
    //WRITE Key
    for ( i =0;i <6;i++)
    { 
        if (bKeyNo == 0x00 || bKeyNo == 0x60)
            THM_WriteReg(DATA0,gKEYA[i]);
        else
            THM_WriteReg(DATA0,gKEYB[i]);    
    }  
    //WRITE UID  
    for ( i =0;i <4;i++)
    {     
        THM_WriteReg(DATA0,gUID[i]);
    }    
    //
    //WRITE Rand2
    THM_WriteReg(DATA1,ms[0]);
    THM_WriteReg(DATA2,ms[1]);
    THM_WriteReg(DATA3,ms[2]);
    THM_WriteReg(DATA4,ms[3]);
    //Authentication
    THM_WriteReg(CRYPTO1_CTRL,0x0C);
    while (1)
    {
	tmp = THM_ReadReg(UART_STAT); 
	if (tmp & 0xff)
	break;
    }	
    if ((tmp & 0xef) ==0x00)
    {
        THM_WriteReg(CRYPTO1_CTRL,0x08);
 	// Data TRANSFER Encrypt
        THM_WriteReg(STAT_CTRL,0x01);
        return 0;
    }    
    else if (tmp & 0x80)
    {
	    //Reset Crypto1
        THM_WriteReg(FM_CTRL,0xc0); 
	    return 0x3;
    }
    else
    {
	    //Reset Crypto1
    	THM_WriteReg(FM_CTRL,0xc0); 
        return 0x4;
    }
    
}    


// Mifare Read
//Function:  Mifare Read
//Parameter: IN  bBlock  Card Block
//           OUT pdat    pointer of the Block data
//           OUT bLen    pointer of data length
//Return value:   00    OK;
//				  01    Err	

unsigned char THM_MRead (unsigned char bBlock,unsigned short * pbLen,unsigned char *pdat )
{
    
    
    temp[0] = comd[READ_CMD][1];
    temp[1] = bBlock;
    //Send Read Command    
    THM_MSendFrame(temp,comd[READ_CMD][0]);     
    THM_MWaitReadFrame(pbLen, pdat);  

    if (*pbLen != (MBLOCK_SIZE+2))	
	return 1;
    *pbLen= MBLOCK_SIZE;	 //remove 2 CRC bytes
    return 0;
}

// Mifare Write
//Function:  Mifare Write
//Parameter: IN  bBlock  Card Block
//           OUT pdat    pointer of the Block data
//Return value:   00    OK;
//                01    Write Command Response Error       

unsigned char THM_MWrite(unsigned char bBlock,unsigned char *pdat)
{
    unsigned short iLen;
    
    temp[0] = comd[WRITE_CMD][1];
    temp[1] = bBlock;
    
    //Write Command    
    THM_MSendFrame(temp,comd[WRITE_CMD][0]);         
    THM_MWaitReadFrame(&iLen, temp);
    if (iLen != 1 ||  temp[0]!= 0xA0 ) return 0x1;
    //Send Write Data
    THM_MSendFrame( pdat, MBLOCK_SIZE);             
    THM_MWaitReadFrame(&iLen, temp);   //Write Command no response
    return 0;
    
}    

// Mifare Transfer
//Function:  Mifare Transfer
//Parameter: IN  bBlock  Card Block
//           OUT pdat    pointer of the Block data
//           OUT bLen    pointer of data length
//Return value:   00    OK;
//                01    Err,response

unsigned char THM_MTransfer(unsigned char bBlock)
{
    unsigned short iLen;
    
    temp[0] = comd[TRANSFER_CMD][1];
    temp[1] = bBlock;
    
    //Transfer Command    
    THM_MSendFrame(temp,comd[TRANSFER_CMD][0]);         
    THM_MWaitReadFrame(&iLen, temp);
    if (iLen != 1 || temp[0] != 0xA0 ) 
	return 0x1;    
    return 0;    
}    

//Mifare Restore
//Function:  Mifare Resotre
//Parameter: IN  bBlock  Card Block
//Return value:   00    OK;
//                01    Err,Response

unsigned char THM_MRestore(unsigned char bBlock)
{
    unsigned short iLen;
    
    temp[0] = comd[RESTORE_CMD][1];
    temp[1] = bBlock;
    
    //Write Command    
    THM_MSendFrame(temp,comd[RESTORE_CMD][0]);         
    THM_MWaitReadFrame(&iLen, temp);
    
    if (iLen != 1 || temp[0] != 0xA0 ) 
	return 0x1;    
    //Send Data，Data have no Meaning
    THM_MSendFrame( temp, MWALLET_SIZE);//              
    THM_MWaitReadFrame(&iLen, temp);   //Restore Command no response
    return 0;
    
}    

//Mifare Increment
//Function:  Mifare Increment
//Parameter: IN  bBlock  Card Block
//           IN  pdat  increment value ( 4 bytes )
//Return value:   00    OK;
//                01    Err, reponse 

unsigned char THM_MIncrement(unsigned char bBlock,unsigned char *pbValue )
{
    unsigned short iLen;
    
    temp[0] = comd[INCREMENT_CMD][1];
    temp[1] = bBlock;
    
    //Increment Command    
    THM_MSendFrame(temp,comd[INCREMENT_CMD][0]);         
    THM_MWaitReadFrame(&iLen, temp);
    
    if (iLen != 1 || temp[0] != 0xA0 ) 
	return 0x1;    
    
    //Send Increment Data
    THM_MSendFrame( pbValue, MWALLET_SIZE);//              
    THM_MWaitReadFrame(&iLen,temp );   //no response
    return 0;
    
}    

//Mifare Decrement
//Function:  Mifare Decrement
//Parameter: IN  bBlock  Card Block
//           IN  pdat  Decrement value ( 4 bytes )
//Return value:   00    OK;
//                01    Err, reponse 
unsigned char THM_MDecrement(unsigned char bBlock,unsigned char *pbValue )
{
    unsigned short iLen;
    
    temp[0] = comd[DECREMENT_CMD][1];
    temp[1] = bBlock;
    
    //Decrement Command    
    THM_MSendFrame(temp,comd[DECREMENT_CMD][0]);         
    THM_MWaitReadFrame(&iLen, temp);
    
    if (iLen != 1 || temp[0] != 0xA0 ) 
	return 0x1;    
    
    //Send Decrement Data
    THM_MSendFrame( pbValue, MWALLET_SIZE); //              
    THM_MWaitReadFrame(&iLen, temp);     //no response
    return 0;
    
}    

// Mifare Write Value
//Function:  Mifare Write Value
//Parameter: IN  bBlock  Card Block
//           OUT pdat    pointer of the Block data
//Return value:   00    OK;
//                01    Write Command Response Error    

unsigned char THM_MWriteValue(unsigned char bBlock,unsigned char * pbValue,unsigned char bAddr)
{
    
    unsigned char i;
    //BYTE0~3 Value   BYTE4~7 !Value BYTE 8~11 Value BYTE 12 Addr BYTE 13 !Addr BYTE 14 Addr BYTE 15 !addr
    //Combine Value blcok
    for (i=0;i<4;i++)
    {                 
        temp[i+2] = pbValue[i];
        temp[i+6] = ~pbValue[i];
        temp[i+10]=pbValue[i];
    }    
    temp[14] = bAddr;
    temp[15] = ~bAddr;
    temp[16] = bAddr;
    temp[17] = ~bAddr;         
    return(THM_MWrite(bBlock,temp+2));    
}    

// Mifare Read Value
//Function:  Mifare Read Value
//Parameter: IN  bBlock  Card Block
//           OUT pbValue    pointer of the Block data
//           OUT pbAddr     pointer of Addr
//Return value:   00    OK;
//                01    Value format Error    
//                02    Addr format Error
//                03    Other Error

unsigned char THM_MReadValue(unsigned char bBlock,unsigned char * pbValue,unsigned char * pbAddr)
{
	unsigned short i;

	//Read bBlock data
	if ( THM_MRead (bBlock,&i,temp) != 0x00)	 	
	return 0x3;  //

	for (i =0;i < 4;i++)
	{
		
		if (temp[i]!= temp[i+8])
			return 0x1;       //Value format error
		if (temp[i]!=~temp[i+4])
			return 0x1;
		pbValue[i] = temp[i];
	}

	if (temp[12]!=temp[14]) return 0x2; //Addr format err
	if (temp[13]!=temp[15])	return 0x2;
	if (temp[12]!=~temp[13])return 0x2;

	*pbAddr = temp[12];
	return 0x00;  //Success
}
// Mifare Write Sector Trailer
//Function:  Mifare Write Sector Trailer
//Parameter: IN  bBlock  Card Block, Must be Block 3 in the Sector
//           IN  pbKEYA   
//           IN  pbKEYB
//           IN  pbAccessBits              
//           OUT pdat   pointer of the Block data
//Return value:   00    OK;
//                01    Err,Write Command Response Error    
//                02    Err,Block No Error
//                03    Err,Access bits Error
//bAccessBits
//BYTE 6
// !C2_3 !C2_2 !C2_1 !C2_0 !C1_3 !C1_2 !C1_1 !C1_0
//BYTE 7
// C1_3 C1_2 C1_1 C1_0 !C3_3 !C3_2 !C3_1 !C3_0
//BYTE 8
// C3_3 C3_2 C3_1 C3_0 C2_3 C2_2 C2_1 C2_0
//BYTE 9,not used

unsigned char THM_MWriteSectorTrailer(unsigned char bBlock,unsigned char* pbKEYA, unsigned char * pbKEYB, unsigned char * pbAccessBits)
{
    unsigned char i;
    
    // Block No. must be highest block of the sector
    if ((bBlock & 0x3)!= 0x3) return 0x2;
    // BYTE6 Low with BYTE7 High
    if ((~pbAccessBits[0] & 0xF)!=((pbAccessBits[1] & 0xF0) >> 4))  return 0x3;
    // BYTE6 High with BYTE8 Low
    if (((~pbAccessBits[0] & 0xF0) >> 4) !=(pbAccessBits[2] & 0xF)) return 0x3;
    //BYTE7 Low with BYTE8 High
    if ((~pbAccessBits[1] & 0xF)!=((pbAccessBits[2] & 0xF0) >> 4))  return 0x3;  
    
    //Combine SectorTrailer block
    for (i=0;i<6;i++)
    {
        temp[i+2] =  *pbKEYA++;
        temp[i+12] = *pbKEYB++;
    }         
    
    temp[8] =  pbAccessBits[0]; 
    temp[9] =  pbAccessBits[1];
    temp[10] = pbAccessBits[2];
    temp[11] = pbAccessBits[3];
    
    return(THM_MWrite(bBlock,temp+2)); 
}      

//Mifare HLTA
//Function:  Mifare Decrement
//Parameter: NONE
//           
//Return value:  NONE

void THM_MHLTA(void)
{
    unsigned short iLen;
	//Send HLTA command

    THM_MSendFrame(&comd[HLTA_CMD][1],comd[HLTA_CMD][0]); 
    THM_MWaitReadFrame(&iLen, temp);        
	return;  
}    


//read id  start function
//send data frame	 
void THM_SendFrame(unsigned char *buffer,unsigned short num)
{
	unsigned char temp;	
	THM_WriteReg(SCNTL, 0x5);	                                //RAMPT_CLR =1,CLK_EN =1
	THM_WriteReg(SCNTL, 0x01);                                  // RAMPT_CLK=0;	
	
	temp = DATA | 0x80;			                          	
	                                                            //write mode
	                        
	
	SPI_SendBuff(&temp,1);
	SPI_SendBuff(buffer,num);	                              
	                                                            //write data
	
	usleep(12000);	
	THM_WriteReg(SCNTL, 0x03);                                  //SEND =1 
	                                                            //SEND =1,start sending
}

//Waiting for receiving end, write the received data to the "buffer" ,its length is "len"
//Return of function is the receiving status
unsigned char THM_WaitReadFrame(unsigned short *len, unsigned char *buffer)
{
	unsigned char temp,temp1;	
	*len = 0;
	//waiting for the end of receive
	while (1)	
	{  	    
	   temp = THM_ReadReg(RSTAT);   
	   if (temp & 0x80)
	        break;
    }	           
	// handling the  receiving status
    if (temp & CERR )   
        temp = CERR;               
		                            //BitPos  
    else if (temp & PERR)       
        temp = PERR;                
		                            //Frame Error
    else if (temp & FERR)       
        temp = FERR;
    else if (temp & DATOVER)       
        temp = DATOVER;            
		                           //Data Overflow
    else if (temp & TMROVER)
        temp = TMROVER;             
		                            //Timeout
    else if (temp & CRCERR)   
        temp = CRCERR;              
		                            //CRC Error
    else 
        temp = FEND;                
		                            //Frame correct 
	//read the data length
    *len =((unsigned short)(THM_ReadReg(RSCH)) <<8 ) + THM_ReadReg(RSCL);  		    
	//read the data
    if (*len != 0x00 )
    {
        
        temp1 = DATA;
        SPI_SendBuff( &temp1,1);
        SPI_RecvBuff( buffer,*len);
    	usleep(12000);    
    }      
	//clear the status
    THM_WriteReg(RSTAT,0x00);        
    return (temp);
}
//return  :   if temp = FEND  read b_uid (12 char)
unsigned char THM_FindCard(unsigned char * b_uid)
{
	unsigned char  temp=0;
	unsigned char id_num;
	unsigned char id_buff[3]={0};
	
	THM_WriteReg(PSEL,TYPE_B|SND_BAUD_106K|RCV_BAUD_106K);
	id_num = 3;
	id_buff[0] = 0x05;
	id_buff[1] = 0x00;
	id_buff[2] = 0x00;
	THM_OpenRF();   //Open RF
	THM_SendFrame(id_buff,id_num); // Send to Card  read id:  f 05 00 00
	temp == (THM_WaitReadFrame(&id_num,b_uid));  //Receive from CARD
        THM_WriteReg(PSEL,MIFARE|SND_BAUD_106K|RCV_BAUD_106K);
	return temp;

}
//read id card end function 


int main(int argc, char *argv[])
{
	struct termios option;
	int write_num,read_num, chksum = 0, ic_len = 0,i;
	unsigned char buff[64] = {0};
	unsigned char id_buff[13] = {0};
	unsigned char rd_id[5]={0},Authentication_flag = 0;
	unsigned char key[12]={0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned int temp=0;
	unsigned short len=0;
	unsigned char pdat[20]={0};
	unsigned char w_dat[20]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x00};
	fd = open_port(argv[1]);
	if (fd > 0)
        	printf("Serial port %s is opened\n", argv[1]);
	else {
        	fprintf(stderr, "Failed to open %s with err %d\n", argv[1], fd);
	return -1;
        }

#if 1
	if (init_port(fd, 115200) < 0)
        	fprintf(stderr, "Failed to initialize the port %s\n", argv[1]);
	else
        	fprintf(stdout, "Port %s initialized\n", argv[1]);
#endif

    /* Writing and reading loop */
	loops = 50;
	chksum = 0x00;
	char lll=0;
	buff[0] = 0x01;
	buff[1] = 0x02;
	buff[2] = 0x03;
//	unsigned char q[]
    	while (1) {
	loops--;
	if(loops ==0)
		break;
	printf("start ic card \n");
	if(!(lll=THM_MFindCard(rd_id)))
	{
		
		printf("the card id is:");
		for(i=0;i < 4;i++)
		{
			printf(" %x",rd_id[i]);
		}	
		printf("\n");
		printf("init key\n");
                THM_MInitialKEY(&key[0],&key[6]);
		usleep(5000);
		printf("Authentication\n");
		if(!(temp=THM_MAuthentication(0x00,38)))
		{
			Authentication_flag = 1;
		}
		printf("temp %d\n",temp);
	}
	if(Authentication_flag == 1)
	{
		if(!THM_MRead (38,&len,&pdat))    //read block 8
		{
			printf("read card data is: ");
			i = 0;
			while(i != len)
				printf("%x ",pdat[i++]);
			printf("\n");
		}

		if(!THM_MWrite(38,&w_dat))       //write date to block 8
		{
			printf("write card data is: ");
			i = 0;
			while(i != len)
				printf("%x ",w_dat[i++]);
			printf("\n");
		}
		usleep(5000);
		
		if(!THM_MRead (38,&len,&pdat))    //read block 8
		{
			printf("read card data is: ");
			i = 0;
			while(i != len)
				printf("%x ",pdat[i++]);
			printf("\n");
		}
		usleep(5000);
	}
#ifdef READ_ID
	usleep(10000);
	if(FEND == (THM_FindCard(id_buff)))  //find id card
	{
		printf("the id is: " );
		for(i =0;i< 12;i++)
		{
			printf(" %x ",id_buff[i]);
		}
		printf("\n");
	}
#endif
	usleep(10000);
	}
	printf("##########end#########\n");
    return 0;
}
