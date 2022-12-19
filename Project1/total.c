//---------------------------- Sensor 헤더 추
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/resource.h>
#include <wiringPi.h>

//------------------------------------I2C 사용 헤더 추가
#include <wiringPiI2C.h>
#include <sys/ioctl.h>
 
#define SLAVE_ADDR_01 0x68 //I2C 슬레이브 주소
 
 
#define GPIO_TRIGGER  15 // 센서 GPIO
#define GPIO_ECHO  18// 센서 GPIO

#define GPIO_INPUT 20 //  버튼 입력 확인
#define GPIO_OUTPUT 21// LED 출력

// 거리 센서에서 값이 넘어 오면 거리 확인 후 시간 출력
static const char* I2C_DEV = "/dev/i2c-1"; // I2C 주소값
void ClockOPEN();
void ClockCLOSE();
void Distance();
void Light_Control();



int y,mo,d,h,m,s,i; // 시계 최초 초기화 및 이격 발생시 시간을 출력하기 위한 함
char wd[7][12]={"월","화","수","목","금","토","일"};//0x03 날짜 배열
int i2c_fd,Check = 10,System_State=0,i,PCheck;//
char fwc[1],frc[1];





int FIFO[2];












unsigned long get_usec_tick()// 어디서 사용되는 함수
{
    struct timespec ts;
    unsigned theTick = 0U;
    clock_gettime( CLOCK_REALTIME, &ts );
    theTick  = ts.tv_nsec / 1000;
    theTick += ts.tv_sec * 1000000;
    return theTick;
}


int wait_state(int state)// 초음파 센서 상태  LOW HIGH
{
    while ( digitalRead( GPIO_ECHO ) == state );
    return 0;
}

int PinNum()
{
	 delay(90);
	i = digitalRead(GPIO_INPUT);
	
	return i;
	}
    


    
    
void Light_Control() // 161번쨰 줄 버튼 인식
						// 버튼 입력받는 것을 병렬 처리 
{
	
		
		if(digitalRead(GPIO_OUTPUT) == 1)
		{
			
			
					while(1)
					{
						
						digitalWrite(GPIO_OUTPUT,LOW);
						if(System_State == 0)
						{
								printf("\n");
								printf("---------------------------\n");
								printf("         System Off\n");
								printf("---------------------------\n");
								printf("\n");
								
								
								System_State = 1;
						}
						delay(80);
						  read(FIFO[0],frc,sizeof(char));
						if(frc[0] == 0 && digitalRead(GPIO_OUTPUT)== 0)
						{		
								printf("\n");
								printf("---------------------------\n");
								printf("         System On\n");
								printf("---------------------------\n");
								printf("\n");
								System_State=0;
								digitalWrite(GPIO_OUTPUT,HIGH);
								break;
						}
									
						
						
					}
					if(Check == 1 || Check == 0)
							Check = 10 ;
				
						
					Distance();
		}
	
	
	
}    
    
    
void Distance()// 센서 동작 함수
{
	
	long start, stop; // 센서 변수 
	int loop = 0, count; // 센서 변수
	float f; // 센서 변수
	float distance; // 센서 변수수
	
	
	
	// 1 = Write 0 = Read = 리턴값 읽어 올 
	pid_t pid;
	
	pipe(FIFO);
	pid = fork();
	

	pinMode( GPIO_TRIGGER, OUTPUT );// GPIO_TRIGGER = 15 define
    pinMode( GPIO_ECHO, INPUT );// GPIO_ECHO = 18 define
    
  
    digitalWrite( GPIO_TRIGGER, LOW );
    
   // delay(500);
    
    while ( 1 )
    {
	//	printf("distance While함수 진입\n");
        // Send 10us pulse to trigger
        //GPIO로 읽어오는 값을 멀티 프로세서로 사용
        digitalWrite( GPIO_TRIGGER, HIGH);
        delayMicroseconds( 10 );
        
        digitalWrite( GPIO_TRIGGER, LOW);
        
        wait_state(LOW);
        
        start = micros();
        
        wait_state(HIGH);
        
        stop = micros();
        
        distance = (float)(stop - start) / 58.8235;
      //센서는 계속 작동해야 하고 일정값 이상이 잡히면 Clock 함수 호출 
      //----------------------------------------------------------------------------------- 멀티프로세스
      
      
     
      if(pid == 0)
      {
		  
		
		  delay(50);
		  fwc[0]=PinNum();
		   
	//printf("%d\n",fwc[0]);
		  write(FIFO[1],fwc,sizeof(char));
		}
	
	else
    {  
       
		//FIFO 통신 // 저장된 값을 F에 저장 // 여기서 값이 계속 넘어 오니까 그런 
		

      read(FIFO[0],frc,sizeof(char));
    
       if(frc[0] == 0) //  받아온 값으로 확인 
				{	Light_Control();}	
		
        
        
       
        if((int)distance >=10 && Check != 1)
        {
				ClockOPEN();//Clock 함수 호출 후 시간 확
				Check = 1;
		}

		
		
		else if((int)distance < 4 && Check != 0)
		{
				ClockCLOSE();
				Check = 0;
		}
		
     
        
        if(loop++ == 100)break;//그냥 100번 수행되고 종료 되는 듯
     
	//printf( "Distance : %d cm\n", (int)distance );

    }
  delay(50); 
	}
}
	
	
//력--------------------------------------------------------------------------------------------------------------	  시간 출	
	
void ClockOPEN()// 센서 거리 확인 후 시간 출인
{
	y = wiringPiI2CReadReg8 (i2c_fd, 0x06);
	mo = wiringPiI2CReadReg8 (i2c_fd, 0x05);
	d = wiringPiI2CReadReg8 (i2c_fd, 0x04);
	i = wiringPiI2CReadReg8 (i2c_fd, 0x03);
	h = wiringPiI2CReadReg8 (i2c_fd, 0x02);
	m = wiringPiI2CReadReg8 (i2c_fd, 0x01);
	s = wiringPiI2CReadReg8 (i2c_fd, 0x00);

	printf("ClockOPEN: ");
	printf("%x년 %x월 %x일 %s요일  %x:%x:%x\n",y,mo,d,wd[3],h,m,s);
	printf("\n");
  
  
	
	
		
}
	
	
	void ClockCLOSE()// 센서 거리 확인 후 시간 출인
{
	y = wiringPiI2CReadReg8 (i2c_fd, 0x06);
	mo = wiringPiI2CReadReg8 (i2c_fd, 0x05);
	d = wiringPiI2CReadReg8 (i2c_fd, 0x04);
	i = wiringPiI2CReadReg8 (i2c_fd, 0x03);
	h = wiringPiI2CReadReg8 (i2c_fd, 0x02);
	m = wiringPiI2CReadReg8 (i2c_fd, 0x01);
	s = wiringPiI2CReadReg8 (i2c_fd, 0x00);
 
	printf("ClockCLOSE: ");
	printf("%x년 %x월 %x일 %s요일  %x:%x:%x\n",y,mo,d,wd[3],h,m,s);
	printf("\n");
  
  

		
}
//-------------------------------------------------------------------------------------------------------------------------	
int main(int argc,char*argv[])
{
	
	
	
	if(wiringPiSetup() < 0 )
	{printf("wiringPiSetup() is failed\n");
	return 1;}
	
	
	if ( wiringPiSetupGpio() == -1 )
        exit( 1 );
	
	
	i2c_fd = wiringPiI2CSetupInterface (I2C_DEV, SLAVE_ADDR_01); //i2C 연결 시도 
	
	
	//i2c_fd = wiringPiI2CSetupInterface (I2C_DEV, SLAVE_ADDR_01); //i2C 연결 시도 
	printf("\n\n");
	
	//-------------------------------------------------------------- 시간 최초 초기
	if(atoi(argv[1])==0){
	wiringPiI2CWriteReg8(i2c_fd,0x06,0x22);
	wiringPiI2CWriteReg8(i2c_fd,0x05,0x12);
	wiringPiI2CWriteReg8(i2c_fd,0x04,0x08);
	wiringPiI2CWriteReg8(i2c_fd,0x03,0x04);
	
	
	wiringPiI2CWriteReg8(i2c_fd, 0x02,0x17);// hour
	wiringPiI2CWriteReg8(i2c_fd, 0x01,0x05);// min  
	wiringPiI2CWriteReg8(i2c_fd, 0x00,0x00); 
	}
	
	//-----------------------------------------------------------------
	
	
	
	
	
	pinMode(GPIO_INPUT,INPUT);
	pinMode(GPIO_OUTPUT,OUTPUT);
	
	
	digitalWrite(GPIO_OUTPUT,HIGH);
	
	
	
	Distance();//거리 확인을 위한 센서 함수 호출 리턴값 없
	
	
	printf("distance 해제\n");
	return 0;
	
	}
//스위치는 신호가 올때까지 대기 상태
