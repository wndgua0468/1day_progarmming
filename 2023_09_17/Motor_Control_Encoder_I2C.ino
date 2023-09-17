#include <Wire.h>
#include "src/PinChangeInt.h"

#define SLAVE_ADDRESS 0x05  // 슬레이브 장치의 I2C 주소
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

int encoderValue = 0;  // 실제 엔코더 값을 저장하는 변수

void setup() {
  
    Wire.begin(SLAVE_ADDRESS);           // 슬레이브 모드로 I2C 통신 초기화
    Wire.onRequest(requestEvent);        // 데이터 요청 이벤트 핸들러 등록
    Wire.onReceive(receiveEvent);        // 데이터 수신 이벤트 핸들러 등록
    pinMode(ENCODER_PIN_A, INPUT_PULLUP); // 엔코더 핀 A를 입력으로 설정
    pinMode(ENCODER_PIN_B, INPUT_PULLUP); // 엔코더 핀 B를 입력으로 설정
    Interrupt();
    //attachInterrupt(ENCODER_PIN_A, handleEncoder, FALLING); // 엔코더 인터럽트 설정
    Serial.begin(115200);                 // 시리얼 통신 초기화
}

void loop() {
    // 엔코더 값을 측정 또는 계산하고, 이 값을 마스터로 전송
    // encoderValue = readEncoderValue();
    Serial.println(encoderValue);
  
    delay(100);  // 엔코더 값을 주기적으로 전송
}

// 마스터로부터 데이터 수신 시 호출되는 함수
void receiveEvent(int byteCount) {
  
  unsigned  char a[7];  // '#' +   '*'  //
  a[0] = Wire.read();
  a[1] = Wire.read();
  a[2] = Wire.read();
  a[3] = Wire.read();
  a[4] = Wire.read();
  a[5] = Wire.read();
  a[6] = Wire.read();
  
  /*
    if (Wire.available() >= 2) {
        int highByte = Wire.read();
        int lowByte = Wire.read();
        encoderValue = (highByte << 8) | lowByte;  
    }
    */

  Serial.println("Receive");
}
 

// 마스터로 데이터 요청 시 호출되는 함수
void requestEvent() {

  unsigned char s[8] = {0,};
  int temp;
  temp = 1*10;
  s[0]='#';
  s[1]= (temp&0xff00)>>8;
  s[2]= (temp&0x00ff); 
  s[3]= (encoderValue&0xff000000)>>24;    // encoder MSB 8bit
  s[4]= (encoderValue&0x00ff0000)>>16;
  s[5]= (encoderValue&0x0000ff00)>>8;
  s[6]= (encoderValue&0x000000ff);        // encoder LSB 8bit
  s[7]='*'; 

  Wire.write(s,8); // respond
  Serial.println("Send EncoderValue");
  
  /*
    // 엔코더 값을 마스터로 전송 (두 바이트로 분할하여 전송)
    Wire.write((encoderValue >> 8) & 0xFF); // 상위 바이트 전송
    //prinf("encoder_highByte = %d", encoderValue);
    Wire.write(encoderValue & 0xFF);        // 하위 바이트 전송
    //prinf("encoder_lowByte = %d", encoderValue);
   */
}

// 엔코더 값을 측정하고 저장하는 함수
void handleEncoder() {
    // 엔코더 값을 업데이트
    if (digitalRead(ENCODER_PIN_A)== digitalRead(ENCODER_PIN_B)) {
        encoderValue++;
//Serial.println("++");
    } else {
        encoderValue--;
         //Serial.println("--");
    }//
}

void Interrupt(){
  attachInterrupt(0, handleEncoder, FALLING);
   TCCR1B = TCCR1B & 0b11111000 | 1;                   // To prevent Motor Noise
}
