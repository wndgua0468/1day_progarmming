/////////////////////////////////////////////////////고정값//////////////////////////////////////////////////////////////

#include <Servo.h>
#include <MsTimer2.h>
#include <NewPing.h>

#define steering_offset 15
#define MAX_DISTANCE    300

#define Kp 3.5
#define Kd 7.4
#define Ki 0.1

#define steer_pin       2
#define LED             53

#define motor_ENA 4
#define motor_ENB 10
#define motor_EN  5

#define Front_sonar_pin 8
#define Back_sonar_pin  9
#define Right_sonar_pin 10
#define Left_sonar_pin  11

#define R_ir_pin        A0
#define L_ir_pin        A1
#define CDS_pin         A5

Servo motor;
Servo steering;

NewPing F_sonar(Front_sonar_pin, Front_sonar_pin, MAX_DISTANCE);
NewPing B_sonar(Back_sonar_pin, Back_sonar_pin, MAX_DISTANCE);
NewPing R_sonar(Right_sonar_pin, Right_sonar_pin, MAX_DISTANCE);
NewPing L_sonar(Left_sonar_pin, Left_sonar_pin, MAX_DISTANCE);

/////////////////////////////////////////////////////변수//////////////////////////////////////////////////////////////

long timer     = 0;
long save_time = 0;
long save_old  = 0;

int stop_flag       = 0;
int mode_ob         = 0;
int save_front_data = 0;
int flag            = 0;

float temp      = 0;
float error_s   = 0;
float error_d   = 0;
float error_old =0;

struct {
    int speed_value = 0;
    int steer_value = 0;
}actuator;

struct {
    float Front_sonar_value = 0;
    float Back_sonar_value = 0;
    float Right_sonar_value = 0;
    float Left_sonar_value = 0;
}sonar;

struct {
    float R_V = 0;
    float L_V = 0;
    float R_V_old = 0;
    float L_V_old = 0;
}ir_sensor;

/////////////////////////////////////////////////////init 함수//////////////////////////////////////////////////////////////
void initsensor(){
  pinMode(CDS_pin,INPUT);
  pinMode(R_ir_pin,INPUT);
  pinMode(L_ir_pin,INPUT);
  
  pinMode(42,INPUT);
  pinMode(43,INPUT);
  pinMode(44,INPUT);
  pinMode(45,INPUT);
  pinMode(46,INPUT);
  }

void initactuator(){
  pinMode(LED,OUTPUT);
  steering.attach(steer_pin); //조향 서보
  pinMode(motor_ENA ,OUTPUT);
  pinMode(motor_ENB ,OUTPUT);
  pinMode(motor_EN  ,OUTPUT);
  
  }

void initcontrolcycle(int timer){
  MsTimer2::set(timer, Control_Callback); //제어 타이머 설정
  MsTimer2::start();                      //시작
  }

void initSerial(){
  Serial.begin(115200);     //보드레이트 설정
  }

void inittimer(){
  timer = millis();         //현재 시간 저장
  }

/////////////////////////////////////////////////////제어 함수//////////////////////////////////////////////////////////////

void actuator_status_Serial(int speed,int angle){ //엑추레이터 상태 표시
 // Serial.print("Speed status : ");
 // if     (speed == 1)  Serial.print("MAX_Front  ");
 // else if(speed == -1) Serial.print("MAX_back   ");
 // else                 Serial.print("Clear      ");

 // Serial.print("Steer status : ");
//  if     (speed == 1)  Serial.println("MAX_right  ");
 // else if(speed == -1) Serial.println("MAX_left   ");
//  else                 Serial.println("Clear      ");
  }

void Control_Callback(){                            //구동 제어 함수
  actuator_status_Serial(motor_control(actuator.speed_value),steering_control(actuator.steer_value));
  }

/////////////////////////////////////////////////////구동 함수(actuator)//////////////////////////////////////////////////////////////

int motor_control(int speed){    // 구동 모터 제어 함수
  if     (speed >= 210)     speed = 210;
  else if(speed <= -210)    speed = -210;

  if(speed >= 0){
    digitalWrite(motor_ENA,HIGH);
    digitalWrite(motor_ENB,LOW);
    analogWrite(motor_EN,speed);
  }
  else{
    digitalWrite(motor_ENB,HIGH);
    digitalWrite(motor_ENA,LOW);
    analogWrite(motor_EN,-speed);
    }

  if     (speed >= 255)    return 1;
  else if(speed <= -255)   return -1;
  else                     return 0;
  }

int steering_control(int angle){  // 조향 제어 함수
  
  if     (angle+90+steering_offset >= 180) angle = 90 - steering_offset;
  else if(angle+90+steering_offset <= 0)   angle = -90 - steering_offset;
  
  steering.write(angle+90+steering_offset);

  if     (angle+90+steering_offset >= 180) return 1;
  else if(angle+90+steering_offset <= 0)   return -1;
  else                                     return 0;
  }

/////////////////////////////////////////////////////구동 함수(sensor)//////////////////////////////////////////////////////////////

void F_sonar_val(){  //초음파 센서 정면
  temp = F_sonar.ping_cm()*10;
  if(temp == 0){
    temp = MAX_DISTANCE * 10;
    }
  if(temp <= MAX_DISTANCE * 10)  sonar.Front_sonar_value = temp;
  else F_sonar_val();
 }

void B_sonar_val(){  //초음파 센서 후면
  temp = B_sonar.ping_cm()*10;
  if(temp == 0){
    temp = MAX_DISTANCE * 10;
    }
  if(temp <= MAX_DISTANCE * 10)  sonar.Back_sonar_value = temp;
  else B_sonar_val();
 }

void R_sonar_val(){  //초음파 센서 우측
  temp = R_sonar.ping_cm()*10;
  if(temp == 0){
    temp = MAX_DISTANCE * 10;
    }
  if(temp <= MAX_DISTANCE * 10)  sonar.Right_sonar_value = temp;
  else R_sonar_val();
 }

void L_sonar_val(){  //초음파 센서 좌측
  temp = L_sonar.ping_cm()*10;
  if(temp == 0){
    temp = MAX_DISTANCE * 10;
    }
  if(temp <= MAX_DISTANCE * 10)  sonar.Left_sonar_value = temp;
  else L_sonar_val();
 }

float ir_val(){      //적외선 센서(좌우측 동시에)
  ir_sensor.R_V = 13*pow(analogRead(R_ir_pin)*0.0048828125,-1);
  ir_sensor.L_V = 13*pow(analogRead(L_ir_pin)*0.0048828125,-1);
  
  if(ir_sensor.R_V > 30) ir_sensor.R_V = ir_sensor.R_V_old;
  if(ir_sensor.L_V > 30) ir_sensor.L_V = ir_sensor.L_V_old;
  
  ir_sensor.R_V_old = ir_sensor.R_V;
  ir_sensor.L_V_old = ir_sensor.L_V;

  }

void head_light(){      //CDS를 이용한 LED작동 포함
  int CDS_V = analogRead(CDS_pin);
  
   if (CDS_V > 500) {
    digitalWrite(LED, HIGH); 
  } else {
    digitalWrite(LED, LOW);
  }
}

void stop_mode(){       //적외선(라인센서)을 이용한 stop_flag 생성
  int sum = 0;
  for(int i=0; i<5; i++){
    if(digitalRead(42+i) == HIGH) sum++;
    }
  if(sum >= 3){
    stop_flag = 1;
    }
  else{
    stop_flag = 0;
    }
  }

/////////////////////////////////////////////////////misson//////////////////////////////////////////////////////////////

void PID_angle(float error){  //조향 제어용 PID함수 Kp,Kd,Ki값 조정 필요
  float output=0;
  error_s += error;
  error_d = error - error_old;
  error_s = (error_s >=  10) ?  10 : error_s;
  error_s = (error_s <= -10) ? -10 : error_s;
  
  output = Kp*error + Kd*error_d + Ki*error_s;
    
  if(fabs(error) <= 1){
    error_s = 0;
    }
  else{
    actuator.steer_value = output*2;
    }
  error_old = error;
  }

void obstacle_avoidance(){       //장애물 회피 코드
  
  if(mode_ob == 0){
    if(ir_sensor.L_V >= 4 && ir_sensor.L_V <= 5)    actuator.steer_value = 0;
    else if(ir_sensor.L_V < 4)                      actuator.steer_value = 30;
    else                                            PID_angle(4 - ir_sensor.L_V);

    if(sonar.Front_sonar_value <= 250 && millis() - timer >= 700){
      while(1){
        F_sonar_val();
        actuator.steer_value = 60;
        if(sonar.Front_sonar_value > 340) break;
      }
      mode_ob = 1;
      timer = millis();
    }
  }
  if(mode_ob == 1){
    if(ir_sensor.R_V >= 6 && ir_sensor.R_V <= 7)   actuator.steer_value = 0;
    else if(ir_sensor.R_V < 6)                     actuator.steer_value = -30;
    else                                           PID_angle(ir_sensor.R_V - 5);

    if(sonar.Front_sonar_value <= 250 && millis()-timer >= 700){
      while(1){
        F_sonar_val();
        actuator.steer_value = -60;
        if(sonar.Front_sonar_value > 340) break;
      }
      mode_ob = 0;
      timer = millis();
    }
  }
 }

void parking_T_Right(){     //T자 주차 우측
  
  if(sonar.Right_sonar_value <= 450 && flag == 1){
    actuator.speed_value = 50;
    actuator.steer_value = 0;
    flag = 0;
    actuator.steer_value = 0;
    save_front_data = sonar.Front_sonar_value;
    if(save_front_data != 3000) save_old = save_front_data; 
    if(save_front_data == 3000) save_front_data = save_old;
    }
    
  else{
    if(flag != 1){
    while(1){
      F_sonar_val();
      B_sonar_val();
      L_sonar_val();
      R_sonar_val();
      actuator.speed_value = 50;
      actuator.steer_value = 0;
      if(sonar.Front_sonar_value <= save_front_data - 285) break;
    }
    
    save_time = millis();
    actuator.steer_value = -30;
    actuator.speed_value = -60;
    delay(500);
    
    while(1){
      F_sonar_val();
      B_sonar_val();
      L_sonar_val();
      R_sonar_val();
      actuator.steer_value = -30;
      actuator.speed_value = -60;
      delay(500);
      if(sonar.Left_sonar_value + sonar.Right_sonar_value <=350 && sonar.Left_sonar_value <= 120 && millis() - save_time >= 1000){
        break;
      }
    }
    actuator.steer_value = -30;
    actuator.speed_value = -60;
    delay(50);
    while(1){
      F_sonar_val();
      B_sonar_val();
      L_sonar_val();
      R_sonar_val();
      actuator.steer_value = -((sonar.Right_sonar_value - sonar.Left_sonar_value)*0.5);
      if(sonar.Back_sonar_value <= 200){
        actuator.steer_value = 0;
        actuator.speed_value = 0;
        break;
      }
      else{
        actuator.speed_value = -60;
        }
      }
    actuator.steer_value = 0;
    actuator.speed_value = 0;
  }
  else{
    actuator.speed_value = 50;
    actuator.steer_value = 0;
  }
  //flag = 1;
  }
 }
void parking_T_Left(){     //T자 주차 좌측
  
  if(sonar.Left_sonar_value <= 450 && flag == 0){
    actuator.speed_value = 50;
    flag = 0;
    actuator.steer_value = 0;
    save_front_data = sonar.Front_sonar_value;
    if(save_front_data != 3000) save_old = save_front_data; 
    if(save_front_data == 3000) save_front_data = save_old;
    }
    
  else{
    if(flag != 1){
    while(1){
      F_sonar_val();
      B_sonar_val();
      L_sonar_val();
      R_sonar_val();
      actuator.speed_value = 50;
      actuator.steer_value = 0;
      if(sonar.Front_sonar_value <= save_front_data - 285) break;
    }
    
    save_time = millis();
    actuator.steer_value = 30;
    actuator.speed_value = -60;
    delay(500);
    
    while(1){
      F_sonar_val();
      B_sonar_val();
      L_sonar_val();
      R_sonar_val();
      actuator.steer_value = 30;
      actuator.speed_value = -60;
      delay(500);
      if(sonar.Left_sonar_value + sonar.Right_sonar_value <=350 && sonar.Right_sonar_value <= 120 && millis() - save_time >= 1000){
        break;
      }
    }
    actuator.steer_value = 30;
    actuator.speed_value = -60;
    delay(50);
    while(1){
      F_sonar_val();
      B_sonar_val();
      L_sonar_val();
      R_sonar_val();
      actuator.steer_value = -((sonar.Right_sonar_value - sonar.Left_sonar_value)*0.5);
      if(sonar.Back_sonar_value <= 200){
        actuator.steer_value = 0;
        actuator.speed_value = 0;
        break;
      }
      else{
        actuator.speed_value = -60;
        }
      }
    actuator.steer_value = 0;
    actuator.speed_value = 0;
  }
  flag = 1;
  }
 }

void line_stop(){  //적외선(라인센서)를 이용해 모터의 구동 상태를 결정
  if(stop_flag == 1){
    actuator.speed_value = 60;
    }
  else{
    actuator.speed_value = 0;
    }
  }

void maze_move(){            //미로구간 제어용 함수
  int angle_ir = ir_sensor.R_V-ir_sensor.L_V;
  
  if(angle_ir <= 0.3 && angle_ir >= -0.3){
    actuator.steer_value = 0;
    }
  else{
    if(stop_flag == 1){
      actuator.steer_value = 0;
      }
    else{
      PID_angle(angle_ir);
    }
   }
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  initactuator();      //엑추레이터 설정
  initsensor();        //센서 핀모드
  initSerial();        //시리얼 보드레이트 설정
  initcontrolcycle(5); //제어 주기 설정
  inittimer();         //타이머 현재 시간 저장
  flag = 1;
}

void loop() {
  F_sonar_val(); //센서 초음파 정면
  B_sonar_val(); //센서 초음파 후면
  R_sonar_val(); //센서 초음파 우측
  L_sonar_val(); //센서 초음파 좌측
  ir_val();      //센서 ir센서
  head_light();  //센서 및 LED CDS
  //stop_mode();   //센서 라인센서
  //stop_flag = 0;
  //obstacle_avoidance();
  parking_T_Right();

//  Serial.print("Right IR Sensor Value: ");
//  Serial.print(ir_sensor.R_V);
//  Serial.print("  Left IR Sensor Value: ");
//  Serial.println(ir_sensor.L_V); 

  //Serial.print("Front Sonar Value: ");
  //Serial.println(sonar.Front_sonar_value);
//  Serial.print("Back Sonar Value: ");
//  Serial.println(sonar.Back_sonar_value);
//  Serial.print("Right Sonar Value: ");
//  Serial.println(sonar.Right_sonar_value);
//  Serial.print("Left Sonar Value: ");
//  Serial.println(sonar.Left_sonar_value);

  // maze_move();
 
  //actuator.speed_value = -150;
  //actuator.steer_value = 0;
}
