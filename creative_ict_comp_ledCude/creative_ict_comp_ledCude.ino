/*
 * 2015-12-11 Cho Kwanghee ( https://github.com/chokh1443 )
 * Term Project of Creative ICT Compution class at CAU, South Korea
 * program to controll 4*4*4 LED cube using Arduino Uno
 */
#define pinSize 16
#define layerSize 4

byte pin[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,A0,A1};
byte layer[4] = {A2,A3,A4,A5};
byte shape[pinSize * layerSize];

int ms4counter = 0;//1초마다 초기화됨
int timeElapse = 0;//1초마다 1씩 올라감

void offById(int);           //turn off single led indexed by id
void onById(int);            //turn on single
void offAll();               //turn off all led
void onAll();                //turn on all led
    
void offByLayer(int);      //turn off single height layer indexed by layer id    
void onByLayer(int);       //turn on single height layer indexed by layer id
void offByRowLayer(int);      //turn off single row layer indexed by layer id    
void onByRowLayer(int);       //turn on single row layer indexed by layer id
void offByColLayer(int);      //turn off single col layer indexed by layer id    
void onByColLayer(int);       //turn on single col layer indexed by layer id
   
void offByLine(int);      //turn off single height line indexed by layer id    
void onByLine(int);       //turn on single height line indexed by layer id
void offByRowLine(int);      //turn off single row line indexed by layer id    
void onByRowLine(int);       //turn on single row line indexed by layer id
void offByColLine(int);      //turn off single col line indexed by layer id    
void onByColLine(int);       //turn on single col line indexed by layer id    
    
void timer2Init();           //timer0 interrupt initialize with frequency of 240hz
void cubeInit();                 //initialize arduino uno pin setting
void showByLayer(int);       //display led of single layer that should be called at timer0 interrupt

void cubeInit() {
  // 큐브의 핀의 I/O 설정하는 함수
  for (int i = 0; i < pinSize; i++) {
    pinMode(pin[i], OUTPUT);
  }
  for (int i = 0; i < layerSize; i++) {
    pinMode(layer[i], OUTPUT);
  }
  offAll();
}
void offById(int id) {
  //큐브의 LED 하나를 off 시키는 함수
  shape[id] = 0;
}
void offAll() {
  // 큐브의 모든 LED를 전부 off 시키는 함수
  for (int i = 0; i < (pinSize * layerSize); i++) {
    offById(i);
  }
}
void onById(int id) {
  // 큐브의 LED 하나를 on 시키는 함수
   shape[id] = 1;
}
void onAll() {
  //큐브의 LED를 전부 on 시키는 함수
  for (int i = 0; i < (pinSize * layerSize); i++) {
    onById(i);
  }
}
void onByLayer(int num) {
  // 큐브의 한 층을 전부 on 시키는 함수
 for (int i = 0; i < (pinSize); i++) {
    onById(num*pinSize + i);
  }
}
void offByLayer(int num) {
  // 큐브의 한 층을 전부 off 시키는 함수
  for (int i = 0; i < (pinSize); i++) {
    offById(num*pinSize + i);
  }
}
void onByRowLayer(int num) {
  // 큐브의 한 Row을 전부 on 시키는 함수
  for (int i = 0; i < (pinSize); i++) {
    onById(layerSize*i+num);
  }
}
void offByRowLayer(int num) {
  // 큐브의 한 Row을 전부 off 시키는 함수
  for (int i = 0; i < (pinSize); i++) {
    offById(layerSize*i+num);
  }
}
void onByColLayer(int num) {
  // 큐브의 한 Col을 전부 on 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    for (int j = 0; j < (layerSize); j++) {
      onById(layerSize*num + j*pinSize + i);
    }
  }
}
void offByColLayer(int num) {
  //큐브의 한 Col을 전부 off 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    for (int j = 0; j < (layerSize); j++) {
      offById(layerSize*num + j*pinSize + i);
    }
  }
}
void onByLine(int num) {
  //큐브의 한 줄을 전부 on 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    onById(num+ i*16);
  }
}

void offByLine(int num) {
  //큐브의 한 줄을 전부 off 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    offById(num+ i*16);
  }
}
void onByRowLine(int num) {
  //큐브의 한 줄(row방향)을 전부 on 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    onById(num/4*16+num%4+ i*4);
  }
}
void offByRowLine(int num) {
  //큐브의 한 줄(row방향)을 전부 off 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    offById(num/4*16+num%4+ i*4);
  }
}
void onByColLine(int num) {
  //큐브의 한 줄(col방향)을 전부 on 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    onById(num*4 + i);
  }
}
void offByColLine(int num) {
  //큐브의 한 줄(col방향)을 전부 off 시키는 함수
  for (int i = 0; i < (layerSize); i++) {
    offById(num*4 + i);
  }
}

void timer2Init() {
  /* TIMER2 인터럽트 설정
   * interrupt freq : 240hz
   * CTC mode
   * clkio 1024
   */
  cli();
  TCCR2A = 0x02;// (1<<WGM21)
  TCCR2B = 0x07;// (1<<CS22) | (1<<CS21) | (1<<CS20)
  TCNT2 = 0x00;
  OCR2A = 0x40;//64
  TIMSK2 = 0x02;// (1<<OCIE2A);
  sei();
}
void showByLayer(int num) {
  //한 층씩 보여 보여주는 함수
  for (int i = 0; i < pinSize; i++) {
    if (shape[(num * 16 + i)] == 1) {
      digitalWrite(pin[i], HIGH);
    }
    else {
      digitalWrite(pin[i], LOW);
    }
  }
  for (int i = 0; i < layerSize; i++) {
    if (i == num) {
      digitalWrite(layer[i], HIGH);
    }
    else {
      digitalWrite(layer[i], LOW);
    }
  }
}



void setup() {
  cubeInit();//핀모드
  timer2Init();//타이머시작
}

void loop() {
  if(timeElapse%32 == 0 && ms4counter == 0){
    offAll();
  }
  if(timeElapse <32){//LED가 아래서부터 하나씩 켜짐
    onById(timeElapse%32*2 + ((ms4counter<240)?0:1));
  }
  else if(timeElapse < 32*2){//LED가 위에서부터 하나씩 켜짐
    onById(63 - timeElapse%32*2 - ((ms4counter<240)?0:1));
  }
  else if(timeElapse < 32*2+8){//레이어 한층씩 켜짐 위로 shift
    onByLayer(((timeElapse%32)+0)%4);
    offByLayer(((timeElapse%32)+1)%4);
    offByLayer(((timeElapse%32)+2)%4);
    offByLayer(((timeElapse%32)+3)%4);
  }
  else if(timeElapse < 32*2+16){//레이어 한층씩 켜짐 아래로 shift
    onByLayer(3-((timeElapse%32)+0)%4);
    offByLayer(3-((timeElapse%32)+1)%4);
    offByLayer(3-((timeElapse%32)+2)%4);
    offByLayer(3-((timeElapse%32)+3)%4);
  }
  else if(timeElapse < 32*2+24){//레이어 한층씩 켜짐 앞으로 shift
    onByRowLayer(((timeElapse%32)+0)%4);
    offByRowLayer(((timeElapse%32)+1)%4);
    offByRowLayer(((timeElapse%32)+2)%4);
    offByRowLayer(((timeElapse%32)+3)%4);
  }
  else if(timeElapse < 32*3){//레이어 한층씩 켜짐 뒤으로 shift
    onByRowLayer(3-((timeElapse%32)+0)%4);
    offByRowLayer(3-((timeElapse%32)+1)%4);
    offByRowLayer(3-((timeElapse%32)+2)%4);
    offByRowLayer(3-((timeElapse%32)+3)%4);
  }
  else if(timeElapse < 32*3+8){//레이어 한층씩 켜짐 왼쪽으로 shift
    onByColLayer(((timeElapse%32)+0)%4);
    offByColLayer(((timeElapse%32)+1)%4);
    offByColLayer(((timeElapse%32)+2)%4);
    offByColLayer(((timeElapse%32)+3)%4);
  }
  else if(timeElapse < 32*3+16){//레이어 한층씩 켜짐 오른쪽으로 shift
    onByColLayer(3-((timeElapse%32)+0)%4);
    offByColLayer(3-((timeElapse%32)+1)%4);
    offByColLayer(3-((timeElapse%32)+2)%4);
    offByColLayer(3-((timeElapse%32)+3)%4);
  }
  else if(timeElapse < 32*4){//높이 방향으로 한 줄 켜짐
    onByLine(((timeElapse%32)+0)%16);
    for(int i=1;i<16;i++){
      offByLine(((timeElapse%32)+i)%16);
    }
  }
  else if(timeElapse < 32*4+16){//row 방향으로 한 줄 켜짐
    onByRowLine(((timeElapse%32)+0)%16);
    for(int i=1;i<16;i++){
      offByRowLine(((timeElapse%32)+i)%16);
    }
  }
  else if(timeElapse < 32*5){//col 방향으로 한 줄 켜짐
    onByColLine(((timeElapse%32)+0)%16);
    for(int i=1;i<16;i++){
      offByColLine(((timeElapse%32)+i)%16);
    }
  }  
  else{
    onAll();
    delay(500);
    offAll();
    delay(500);
  }
}

ISR(TIMER2_COMPA_vect) {
  /* 타이머 인터럽트 서비스 루틴
   *  한 층씩 출력해준다.
   */
  static int counter = 0;
  int timerSpeed = 240;//240;//240으로 하면 약 1초마다 cube->timerElapse가 1씩 올라감
  if((ms4counter = ++ms4counter % timerSpeed) == 0) {
    timeElapse++;
  }
  counter = ++counter % layerSize;
  showByLayer(counter);
}

