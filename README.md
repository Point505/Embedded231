# Embedded231
  
    임베디드 Total 소스코드


# 1. 프로젝트의 목적 및 용도 
  
  
	아이디어 배경

		연구실 밖을 잠깐 나간동안 침입을 감지하는 시스템의 필요성

	아이디어 내용

		초음파 센서로 문과의 거리를 측정	
		거리 차이가 발생시 시각을 기록하고 열림 또는 닫힘을 출력

	아이디어 기대효과 

		앞으로 문으로 출입하는 사람이 있는지 몇 번 열리고 닫혔는지 
		시각은 언제인지를 확인할 수 있게 됨


 # 2. 프로젝트를 시작하는 방법
 
   
    작동은 gcc 로 컴파일한후 sudo 명령어로 실행 시키면 된다
 
    각 모듈에 연결된 GPIO는 total.c 안에 #define 되어 있음으로 참고 하면 된다.
 
    단순한게 초음파 센서와 시계 모듈을 사용하였기 때문에

    만들기가 어렵지 않아 I2C GPIO 와 같이 연결하는 방법만 숙지하고 있으면 된다.











# 3. 동작


     조건문을 통하여 변수 frc[0]로 값을 현재 상태를 확인한다.
     20 보다 크면 OPEN 을 6보다 작으면 CLOSE를 출력한다.

   	if((int)frc[0] >=20 && Check != 1)
        {
		ClockOPEN();//Clock 함수 호출 후 시간 확
		Check = 1;
						
	}

		
		
	else if((int)frc[0]< 6 && Check != 0)
	{
		ClockCLOSE();
		Check = 0;
		
        }
        
        //-------------------------------------------------------------- 시간 최초 초기
        
        I2C를 사용하였으며 최초로 값을 초기화 시켜주는 함수이다.
	0을 입력하면 Wtite에 입력한 값으로 초기화 된다.
        
        if(atoi(argv[1])==0){
        
        
              wiringPiI2CWriteReg8(i2c_fd,0x06,0x22);
              wiringPiI2CWriteReg8(i2c_fd,0x05,0x12);
              wiringPiI2CWriteReg8(i2c_fd,0x04,0x08);
              wiringPiI2CWriteReg8(i2c_fd,0x03,0x04);


              wiringPiI2CWriteReg8(i2c_fd, 0x02,0x17);// hour
              wiringPiI2CWriteReg8(i2c_fd, 0x01,0x05);// min  
              wiringPiI2CWriteReg8(i2c_fd, 0x00,0x00); 
        }
		
        //-------------------------------------------------------------------- 
	
	// 초음파 센서로 부터 값을 읽어들인다.
	
       int PinNum()

	{
	    long start, stop; // 센서 변수 
	    float distance; // 센서 변수수
	
	    pinMode( GPIO_TRIGGER, OUTPUT );// GPIO_TRIGGER = 15 define
	    pinMode( GPIO_ECHO, INPUT );// GPIO_ECHO = 18 define
    
  
	    digitalWrite( GPIO_TRIGGER, LOW );
	    digitalWrite( GPIO_TRIGGER, HIGH);
	    delayMicroseconds( 10 );

	    digitalWrite( GPIO_TRIGGER, LOW);

	    wait_state(LOW);

	    start = micros();

	    wait_state(HIGH);

	    stop = micros();

	    distance = (float)(stop - start) / 58.8235;

	
	 delay(90);
	
	
	return distance;
	}
	
	//------------------------------------------------------------------- 멀티 프로세스 및 IPC
	void Distance()// 센서 동작 함수
	{
	
		......
	
		pid_t pid;

		pipe(FIFO);
		pid = fork(); // 분기 지점

		while ( 1 )
    		{
		
            		if(pid == 0) // 자식 프로세스
      			{
		 		fwc[0]=PinNum();
				write(FIFO[1],fwc,sizeof(char)); // FIFO 통신으로 초음파 값을 저장한다.
	 		}
     
     
	
			else // 부모 프로세스
    			{  
				 
				read(FIFO[0],frc,sizeof(char));// 자식 프로세스로 부터 받아온 값
  
 			      	..........
  			}
  		delay(100); 
		}
	}
	
	
	
# 모듈과 라즈베리 파이간 연결
![1](https://user-images.githubusercontent.com/104303815/208619925-06f75e4d-4883-48ee-9ac8-7424cf0ea712.PNG)


# 기능 시연 영상



동작 후 OFF 상태로 전환


https://user-images.githubusercontent.com/104303815/208547538-8d2ed67a-f44e-4ed3-9c2c-1ab7738f7d89.mp4


OFF 상태인 경우


https://user-images.githubusercontent.com/104303815/208547553-478bd961-ec82-4c38-8f7d-7763a13689ae.mp4


ON 상태로 전환 후


https://user-images.githubusercontent.com/104303815/208547726-14f0359c-3e6f-48d2-898b-4fb498cc1610.mp4


