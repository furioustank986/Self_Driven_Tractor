      #include <SoftwareSerial.h>   //블루투스 통신하기 위한 외부의 소스를 참조하는 역할  (블루투스 관련된 라이브러리, 함수를 사용할 수 있음. 이게 무슨 말이냐면 이미 누군가가 작성해놓은 프로그램을 토대로 필요한 기능을 불러와서 프로그램을 작성하겠다는 의미다. 아두이노는 오픈된 개발환경이다보니 다양한 사람들이 프로그램을 개발하고 공개해 놓았다. 우리는 그것을 가져와서 사용한다는 말이다.)
      #include "LedControlMS.h"  
  

     
      
      #define TX 2  // 블루투스 통신에서 메시지를 받기 위한 아두이노 10PIN으로 설정한다. 이것은 신호를 받는데 사용한다.
      #define RX 3   //블루투스 통신에서 메시지를 주기 위한 아섹스를 무선으로 발송하기위해서 사용하는 부분이다.설정한다. 

      
      #define OUTPUT_UP 9
      #define OUTPUT_LEFT 10
      #define OUTPUT_RIGHT 11  
      #define OUTPUT_DOWN 12
      
       
  
      #define  DIN A1 
      #define  CS  A2
      #define  CLK A3

      #define PIEZO 4 //피에조 부저
     
  
      
  

      int long_speed  = 40;  // 모터 동작 시간 타임을 조절할 수 있다. 1000로 설정하면 1초이다.현재  40이기 때문에 1초를 100으로 나눠서 4 정도의 시간을 정했다는 의미다. 이러한 시간을 정하는 이유는 모터가 물리적 기계이기 때문에 정해줘야한다. 기계들은 구체적으로 알려주야 일을 한다. 그래서 무엇이든지 꼼꼼하게 빈틈없이 철저하게 확실히 알려줘야한다.(강조 확실히 함)
      
      
      
      
      
      /* 블루투스 통신을 사용하기 위해서 문자,문자열 선언 */
      /*글자를 어떻게 송수신 받을 것인지 정하는 부분이다. */
      
      
      char inBTChar;          //blueTooth 통신에서 한 글자씩 받기위한 char형 변수로 선언
      String inputBTString ;  //up,down,left,right... 등등 블루투스에 보낸  명령어 받기 위한 문자열 저장 공간
      char inSLChar;
      String inputSLString ;  //serial(컴퓨터) 통신을 하기 위해 문자열 저장 공간
      
      
      SoftwareSerial BTSerial(TX, RX); //bluetooth 클래스를 아두이노 9핀,10핀에서 송수신 정의
      
      
      /*  void setup()
          setup 아두이노 시작할 때, 단 한번 밖에 실행되지 않으면,
          주로 변수 선언 및 초기화, 장비 동작 유무 테스트하는데 많이 사용된다.
      */
      int rccar_off = 0;
  
       #define NBR_MTX 1 // 8X8 LED Matrix를 몇개 사용할 지를 정한다. 현재 하나만 셋팅.
       LedControl lc=LedControl(DIN,CLK,CS, NBR_MTX); //8X8 LED Matrix 클래스를 세개의 핀으로 연결한다. 


     /*** 
     *  
     *  address 0x20, 0x27, 0x3F
     *  SDA --> A4 , SCL --->A5   
     ****/
    

          byte right[] =   //right
           {
               B00011000,   
               B00111100,
               B01111110,
               B11111111,         
               B00111100,
               B00111100,
               B00111100,
               B00111101
            };
            byte up[] =   //up
           {
               B00011000,   
               B00111000,
               B01111000,
               B11111111,
               B11111111,
               B01111111,
               B00111000,
               B00011001
            };
            byte down[] =   //비행기 이모티콘 모양
           {
               B00011000,   
               B00011100,
               B00011110,
               B11111111,
               B11111111,
               B11111110,
               B00011100,
               B00011001
            };
            byte left[] =   //비행기 이모티콘 모양
           {
               B00011100,   
               B00011100,
               B00011100,
               B00011100,
               B11111111,
               B01111110,
               B00111100,
               B00011000
            };
  
            byte all[] =   //stop 
           {
               B00111100,   
               B01111110,   
               B01111110,   
               B11111111,   
               B11111111,   
               B01111110,   
               B01111110,   
               B00111100   
              
            };
  
       byte e1[] =  //비행기 이모티콘 모양
      {
         B01000010,  
         B01111110,
         B01111110,
         B11011011,
         B01111110,
         B00100100,
         B01111110,
         B11011001
      };
      
  
         /*
         오른쪽 방향으로 동작하기 위해서 왼쪽 모터가 동작해야 한다.
         오른쪽 모터 -,- 로
         왼쪽 모터 +,- 로 셋팅
      */
      void Right() {
       

        digitalWrite(OUTPUT_RIGHT, HIGH);  // 왼쪽 모터
        printByte(up);
      
        delay(1000);
        digitalWrite(OUTPUT_RIGHT, LOW);  // 왼쪽 모터
  
        printByte(right);
      
        delay(long_speed);
      
      }
      
      /*
          왼쪽 방향으로 동작하기 위해서 오른쪽 모터가 동작해야 한다.
          오른쪽 모터 +,- 로
          왼쪽 모터 -,- 로 셋팅
      */
      void Left() {


        digitalWrite(OUTPUT_LEFT, HIGH);  // 왼쪽 모터
        printByte(up);
      
        delay(1000);
        digitalWrite(OUTPUT_LEFT, LOW);  // 왼쪽 모터

         printByte(left);
      
      }
      
      /*
          전진으로 동작
          오른쪽 모터 +,- 로
          왼쪽 모터 +,- 로 셋팅
      */
      void Up() {
      
        //Motor one

        digitalWrite(OUTPUT_UP, HIGH);  // 왼쪽 모터
        printByte(up);
      
        delay(1000);
        digitalWrite(OUTPUT_UP, LOW);  // 왼쪽 모터


        printByte(up);
      
      }
      
      
      /*
          후진으로 동작
          오른쪽 모터 -,+ 로
          왼쪽 모터 -,+ 로
      */
      
      void Down() {
        //Motor_one


       

        digitalWrite(OUTPUT_DOWN, HIGH);  // 왼쪽 모터
        printByte(up);
      
        delay(1000);
        digitalWrite(OUTPUT_DOWN, LOW);  // 왼쪽 모터

        printByte(down);
      
 
      }
      
      
      /*  정지 동작
          왼쪽 모터   - , -
          오른쪽 모터  -, -
          다 동작을 멈춤
      */
      
      void Stop() {
      
        digitalWrite(OUTPUT_UP, LOW);  // 왼쪽 모터
        digitalWrite(OUTPUT_LEFT, LOW);
  
        printByte(all);
        tone(PIEZO,1000,200);
      
        delay(10);
      
      }
      
  
      
      void setup() {
      
        Serial.begin(9600);
        BTSerial.begin(9600);  //블루투스 통신 속도 9600byte 1초당
        inputBTString = "";    //블루투스 통신하기 위한 문자열 비우기 (초기화 작업) 초기화를 해야지 그곳에 나중에 문자를 넣을 수 있다. 처음에는 블루투스 통신 칩도 마음을 비워놔야 한다. //블루투스 통신하기 위한 문자열 비우기 (초기화 작업) 초기화를 해야지 그곳에 나중에 문자를 넣을 수 있다. 처음에는 블루투스 통신 칩도 마음을 비워놔야 한다.
   
     
        pinMode(OUTPUT_DOWN, OUTPUT);     //왼쪽 모터 출력값으로 선언하여 전압 5V이내의 신호를 보낼 수 있습니다.
        pinMode(OUTPUT_RIGHT, OUTPUT);    

        pinMode(OUTPUT_UP,OUTPUT); 
        pinMode(OUTPUT_LEFT,OUTPUT); 

        
        pinMode(PIEZO,OUTPUT);
  
        // 8X8 LED Matrix 클래스에  일일히 하나씩 LED 위치 등록하여 초기화 
         for (int i=0; i< NBR_MTX; i++){
          lc.shutdown(i,false);
          lc.setIntensity(i,8);
          lc.clearDisplay(i);
          
          delay(200);
        }


 
          printByte(all);
  
  
    
  
        tone(PIEZO,1000,100);
        digitalWrite(5,HIGH);
        digitalWrite(9,HIGH); 
  
  
  
        tone(PIEZO,1000,100);
        
    }
      
     
      /*   void loop()
          아두이노에서 무한 반복하는 함수로써 동작하는 부분에서 많이 사용됨.
           센서값 가져오기, 모터 동작, 블루투스 값 등을 계속 가져오기 위해서 반복적으로 실행한다. 사실 무한반복이다. 전기가 끊어질때까지 계속 일을 시키겠다는 명령어다.
      */
  
      bool light_onoff = true;
      
      void loop() {
      
        while (BTSerial.available() || Serial.available()) {   //블루투스 통신이 연결되었을 경우에만 무한 루프로 실행한다.  무한이 일을 하면서 자료들을 처리하겠다는 의도다.
          inBTChar = (char)BTSerial.read();  //블루투스 통신에서 한글자씩 가져오면 inBTChar 문자 변수로 저장한다.
          inSLChar = (char)Serial.read();
      
          /*
             1. inBTChar 문자 변수가 '\n' , '\r', '_'  저장될 경우, 비교하여 아래를 실행합니다.
             2. inBTChar 한 글자씩 저장된 문자열 inputBTString 값에서 "stop", "up" 값을 비교
             3. 값이 참일 경우에 "BUZZER", "stop","up","down","left" 부분에서의 해당되는 모터,서보 모터 동작 (전진,후진,좌,우)
             4. 값이 거짓일 경우에 inputBTString 값을 ""빈 문자로 저장한다.
             5. 무한 루프 동작한다.
      
             6. inBTChar 문자 변수가 '\n', '\r', '_' 아닐 경우에
             7. inBTChar(앱에서 블루투스를 통해 보낸 메시지)문자 변수를 inputBTString 문자열에 하나씩 저장한다.
             8. 다시 무한 루프 동작한다.
          */
      
          if (inBTChar == '\n' || inBTChar == '\r' || inBTChar == '_' || inSLChar == '_' || inSLChar == '\r' || inSLChar == '\n') {
  
            if (inputBTString.equals("STOP") || inputSLString.equals("STOP")) { //앱에서 "stop" 메시지를 보냈을 경우에
              //inputBTString 문자열값과 비교하여 같을 경우에 모터 동작을 멈춘다.
      
  
              Stop();                              // 모터 동작 멈춤
              tone(PIEZO,1000,200);
              BTSerial.print("OK");                //실행이 성공적이었다면 앱에 "OK"확인 메시지 보냄.
              Serial.print("\nSTOP");
              tone(PIEZO,1000,100);
              
              inputBTString = ""; 
              inputSLString =""; 
      
            } else if (inputBTString.equals("LEFT") || inputSLString.equals("LEFT")) { //앱에서 "left" 메시지를 보냈을 경우에
              //inputBTString 문자열값과 비교하여 같을 경우에
              // 오른쪽 모터만 정방향으로 동작시킨다.
      
              Left();           //오른쪽 모터만 동작하는 함수 호출한다. 그래서 오른쪽 모터만 움직인다.
              BTSerial.print("OK");           //실행이 성공적이었다면 앱에 "OK"확인 메시지 보냄.
              Serial.println("LEFT ");
              tone(PIEZO,1000,100);
              
              inputBTString = ""; 
              inputSLString = ""; 
      
            } else if (inputBTString.equals("RIGHT") || inputSLString.equals("RIGHT")) { //앱에서 "right" 메시지를 보냈을 경우에
              //inputBTString 문자열값과 비교하여 같을 경우에
      
      
              Right();                          //왼쪽 모터만 동작하는 함수 호출한다.왼쪽 모터만 움직인다.
              Serial.println("RIGHT ");
              BTSerial.print("OK");            //실행이 성공적이었다면 앱에 "OK"확인 메시지 보냄.
              tone(PIEZO,1000,100);
  
              inputBTString = ""; 
              inputSLString = ""; 
      
            } else if (inputBTString.equals("UP") || inputSLString.equals("UP") ) {   //앱에서 "up" 메시지를 보냈을 경우에
              //inputBTString 문자열값과 비교하여 같을 경우에
              // 정면 방향,모터로 동작
      
              Up();   //up 함수 호출
              BTSerial.println("OK");   //실행이 성공적이었다면 앱에 "OK"확인 메시지 보냄.
              Serial.print("UP ");

              tone(PIEZO,1000,100);
              inputBTString = ""; 
              inputSLString = ""; 
      
      
            } else if (inputBTString.equals("DOWN") || inputSLString.equals("DOWN") ) { //앱에서 "down" 메시지를 보냈을 경우에
              //inputBTString 문자열값과 비교하여 같을 경우에
              // 후진 방향, 모터로 동작
      
              Down(); //down 함후 호출
              BTSerial.print("OK");  //실행이 성공적이었다면 앱에 "OK"확인 메시지 보냄.
              Serial.println("DOWN ");

              tone(PIEZO,1000,100);
              inputBTString = ""; 
              inputSLString = ""; 
      
            }
            
            
            else {
                 tone(PIEZO,000,100);
                     
             
               Serial.println("ELSE ");
               tone(PIEZO,1000,100);

              
              inputBTString = "";   //블루루스 송수신 문자열이 up/down/left/right/leftup/rightup 아닐경우에
              inputSLString = "";   //블루루스 송수신 문자열이 up/down/left/right/leftup/rightup 아닐경우에
              //블루투스 송수진 문자열 빈 공간으로 초기화
            }
          }
          else  {
            
            inputBTString += inBTChar;   // 블루투스 송수신 문자열에 앱에서 보낸 메시지 문자 하나씩 저장
            inputSLString += inSLChar;
          delay(30);
          }
      
      } //while   

      delay(50); 
  

             
        
    } //loop
       
      
    void printByte(byte character []){
                int i = 0;
                for(i=0;i<8;i++){
                  lc.setColumn(0,7-i,character[i]);
                }
     }
 
