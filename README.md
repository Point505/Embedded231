# Embedded231
  
    임베디드 Total 소스코드


# 1. 프로젝트의 목적 및 용도 
  
  
    이 프로젝트의 목적은 라즈베리 파이와 모듈을 사용하여 일정거리 이상 또는 이하로 변화가 일어나면 그 변화가 발생하는
    
    시간을 기록하여 출입을 확인하기 위함이다.






 # 2. 프로젝트를 시작하는 방법
 
    이 프로젝트는 리눅스 환경에서 작성되었기 때문에 윈도우에서는 작동하지 않을 수 있다 그럼으로 버츄얼 박스를 설치하거나
   
    라즈베리 파이를 따로 구입하여 실행시키는 것을 권장한다.
 
    만약 리눅스 환경이라면 gedit나 geany 같은 편집기를 이용하여 코드를 수정 시킬 수 있으며
   
    작동은 gcc 로 컴파일한후 sudo 명령어로 실행 시키면 된다
 
    각 모듈에 연결된 GPIO는 total.c 안에 #define 되어 있음으로 참고 하면 된다.
 
    단순한게 초음파 센서와 시계 모듈을 사용하였기 때문에

    만들기가 어렵지 않아 I@C GPIO 와 같이 연결하는 방법만 숙지하고 있으면 된다.











# 3. 동작


     조건문을 통하여 변수 distance로 값을 확인한다. distance 변수는 초음파 센서가 측정한 값이다.
  
       
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
        
        
        //-------------------------------------------------------------- 시간 최초 초기
        
        I2C를 사용하였으며 최초로 값을 초기화 시켜주는 함수이다.
        
        if(atoi(argv[1])==0){
        
        
              wiringPiI2CWriteReg8(i2c_fd,0x06,0x22);
              wiringPiI2CWriteReg8(i2c_fd,0x05,0x12);
              wiringPiI2CWriteReg8(i2c_fd,0x04,0x08);
              wiringPiI2CWriteReg8(i2c_fd,0x03,0x04);


              wiringPiI2CWriteReg8(i2c_fd, 0x02,0x17);// hour
              wiringPiI2CWriteReg8(i2c_fd, 0x01,0x05);// min  
              wiringPiI2CWriteReg8(i2c_fd, 0x00,0x00); 
        }
		
        //-------------------------------------------------------------------- 버튼 인식
         
        GPIO를 통하여 버튼을 인식한다.
        
        Int PinNum()
        {
            delay(90);
            i = digitalRead(GPIO_INPUT);

             return i;
      	}


# 모듈과 라즈베리 파이간 연결
![image](https://user-images.githubusercontent.com/104303815/208537651-6b177d9c-c81b-4372-8a94-970f4eaf507b.png)

# 기능 시연 영상

동작 후 버튼을 누르기 전까지

OFF 상태인 경우

ON 상태로 전환 후

