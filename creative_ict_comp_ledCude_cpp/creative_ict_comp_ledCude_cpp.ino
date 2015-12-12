/*
 * 2015-11-17 Cho Kwanghee ( https://github.com/chokh1443 )
 * Term Project of Creative ICT Compution class at CAU, South Korea
 * program to controll 4*4*4 LED cube using Arduino Uno
 */

class  Cube {//class to controll LED CUBE
  private:
    int pinSize;//led size of each layer
    int layerSize;//height of layer
    byte *pin;//pin map array ex: { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,A0,A1};
    byte *layer;//layer pin map array ex: {A2,A3,A4,A5};
    byte *shape;//storage of output shape
  public:
    int ms4counter = 0;//increase when timer interrupt, reset every 1 sec
    int timeElapse = 0;//increase when ms4counter is reset
    Cube(int cubeSize, byte* pins, byte* layers);//contructer 
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
    
    void timer2Init();           //timer2 interrupt initialize with frequency of 240hz
    void init();                 //initialize arduino uno pin setting
    void showByLayer(int);       //display led of single layer that should be called at timer0 interrupt

    int getSizeOfCube() {
      return (this->pinSize * this->layerSize);
    };
    int getSizeOfLayer() {
      return this->layerSize;
    };

};
Cube::Cube(int cubeSize, byte* pin, byte* layer) {
  /* Cube class constructor
   * cubeSize는 LED 큐브의 한 변의 크기이다.
   * pin는 LED 큐브의 각 줄에 연결된 아두이노의 핀 번호이다.
   * layer는 LED 큐브의 각 층에 연결된 아두이노의 핀 번호이다.
   * 
   * 예시 : 4*4*4 큐브
   * byte pin[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1};
   * byte layer[4] = {A2, A3, A4, A5};
   * Cube cube4(4, pin, layer);
   */
  this->layerSize = cubeSize;
  this->pinSize = cubeSize * cubeSize;
  this->pin = new byte[this->pinSize];
  this->layer = new byte[this->layerSize];
  this->shape = new byte[this->pinSize * this->layerSize];
  for(int i=0; i<this->pinSize; i++) {
    this->pin[i] = pin[i];
    this->layer[i] = layer[i];
  }
}
void Cube::init() {
  /* 큐브의 핀의 I/O 설정하는 메소드
   * 아두이노의 setup함수에서 호출되어야 한다.
   */
  for (int i = 0; i < this->pinSize; i++) {
    pinMode(this->pin[i], OUTPUT);
  }
  for (int i = 0; i < this->layerSize; i++) {
    pinMode(this->layer[i], OUTPUT);
  }
  this->offAll();
}
void Cube::offById(int id) {
  /* 큐브의 LED 하나를 off 시키는 메소드
   */
  this->shape[id] = 0;
}
void Cube::offAll() {
  /* 큐브의 모든 LED를 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->pinSize * this->layerSize); i++) {
    this->offById(i);
  }
}
void Cube::onById(int id) {
  /* 큐브의 LED 하나를 on 시키는 메소드
   */
  this->shape[id] = 1;
}
void Cube::onAll() {
  /* 큐브의 LED를 전부 on 시키는 메소드
   */
  for (int i = 0; i < (this->pinSize * this->layerSize); i++) {
    this->onById(i);
  }
}
void Cube::onByLayer(int num) {
  /* 큐브의 한 층을 전부 on 시키는 메소드
   */
  for (int i = 0; i < (this->pinSize); i++) {
    this->onById(num*this->pinSize + i);
  }
}
void Cube::offByLayer(int num) {
  /* 큐브의 한 층을 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->pinSize); i++) {
    this->offById(num*this->pinSize + i);
  }
}
void Cube::onByRowLayer(int num) {
  /* 큐브의 한 Row을 전부 on 시키는 메소드
   */
  for (int i = 0; i < (this->pinSize); i++) {
    this->onById(this->layerSize*i+num);
  }
}
void Cube::offByRowLayer(int num) {
  /* 큐브의 한 Row을 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->pinSize); i++) {
    this->offById(this->layerSize*i+num);
  }
}
void Cube::onByColLayer(int num) {
  /* 큐브의 한 Col을 전부 on 시키는 메소드
   */
  for (int i = 0; i < (this->layerSize); i++) {
    for (int j = 0; j < (this->layerSize); j++) {
      this->onById(this->layerSize*num + j*this->pinSize + i);
    }
  }
}
void Cube::offByColLayer(int num) {
  /* 큐브의 한 Col을 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->layerSize); i++) {
    for (int j = 0; j < (this->layerSize); j++) {
      this->offById(this->layerSize*num + j*this->pinSize + i);
    }
  }
}
void Cube::onByLine(int num) {
  /* 큐브의 한 줄을 전부 on 시키는 메소드
   */
  for (int i = 0; i < (this->layerSize); i++) {
    this->onById(num+ i*16);
  }
}

void Cube::offByLine(int num) {
  /* 큐브의 한 줄을 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->layerSize); i++) {
    this->offById(num+ i*16);
  }
}
void Cube::onByRowLine(int num) {
  /* 큐브의 한 줄(row방향)을 전부 on 시키는 메소드
  */
  for (int i = 0; i < (this->layerSize); i++) {
    this->onById(num/4*16+num%4+ i*4);
  }
}
void Cube::offByRowLine(int num) {
  /* 큐브의 한 줄(row방향)을 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->layerSize); i++) {
    this->offById(num/4*16+num%4+ i*4);
  }
}
void Cube::onByColLine(int num) {
  /* 큐브의 한 줄(col방향)을 전부 on 시키는 메소드
  */
  for (int i = 0; i < (this->layerSize); i++) {
    this->onById(num*4 + i);
  }
}
void Cube::offByColLine(int num) {
  /* 큐브의 한 줄(col방향)을 전부 off 시키는 메소드
   */
  for (int i = 0; i < (this->layerSize); i++) {
    this->offById(num*4 + i);
  }
}

void Cube::timer2Init() {
  /* TIMER2 인터럽트 설정
   * interrupt freq : 240hz
   * CTC mode
   * clkio 1024
   */
  cli();
  TCCR2A = 0x02;// (1<<WGM21)
  TCCR2B = 0x07;// (1<<CS22) | (1<<CS21) | (1<<CS20)
  TCNT2 = 0x00;
  OCR2A = 0x40;//64.i1  
  TIMSK2 = 0x02;// (1<<OCIE2A);
  sei();
}
void Cube::showByLayer(int num) {
  /* 한 층씩 보여 보여주는 메소드
   * num 에 해당하는 층만 보여준다
   */
  for (int i = 0; i < this->pinSize; i++) {
    if (this->shape[(num * 16 + i)] == 1) {

      digitalWrite(this->pin[i], HIGH);
    }
    else {
      digitalWrite(this->pin[i], LOW);
    }
  }
  for (int i = 0; i < this->layerSize; i++) {
    if (i == num) {
      digitalWrite(this->layer[i], HIGH);
    }
    else {
      digitalWrite(this->layer[i], LOW);
    }
  }
}

//핀배열
byte pin[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1};
byte layer[4] = {A2, A3, A4, A5};

Cube cube4(4, pin, layer);//객체생성
void setup() {
  cube4.init();//핀모드
  cube4.timer2Init();//타이머 인터럽트 시작
}

void loop() {
  if(cube4.timeElapse%32 == 0 && cube4.ms4counter == 0){
    cube4.offAll();
  }
  if(cube4.timeElapse <32){//LED가 아래서부터 하나씩 켜짐
    cube4.onById(cube4.timeElapse%32*2 + ((cube4.ms4counter<120)?0:1));
  }
  else if(cube4.timeElapse < 32*2){//LED가 위에서부터 하나씩 켜짐
    cube4.onById(63 - cube4.timeElapse%32*2 - ((cube4.ms4counter<120)?0:1));
  }
  else if(cube4.timeElapse < 32*2+8){//레이어 한층씩 켜짐 위로 shift
    cube4.onByLayer(((cube4.timeElapse%32)+0)%4);
    cube4.offByLayer(((cube4.timeElapse%32)+1)%4);
    cube4.offByLayer(((cube4.timeElapse%32)+2)%4);
    cube4.offByLayer(((cube4.timeElapse%32)+3)%4);
  }
  else if(cube4.timeElapse < 32*2+16){//레이어 한층씩 켜짐 아래로 shift
    cube4.onByLayer(3-((cube4.timeElapse%32)+0)%4);
    cube4.offByLayer(3-((cube4.timeElapse%32)+1)%4);
    cube4.offByLayer(3-((cube4.timeElapse%32)+2)%4);
    cube4.offByLayer(3-((cube4.timeElapse%32)+3)%4);
  }
  else if(cube4.timeElapse < 32*2+24){//레이어 한층씩 켜짐 앞으로 shift
    cube4.onByRowLayer(((cube4.timeElapse%32)+0)%4);
    cube4.offByRowLayer(((cube4.timeElapse%32)+1)%4);
    cube4.offByRowLayer(((cube4.timeElapse%32)+2)%4);
    cube4.offByRowLayer(((cube4.timeElapse%32)+3)%4);
  }
  else if(cube4.timeElapse < 32*3){//레이어 한층씩 켜짐 뒤으로 shift
    cube4.onByRowLayer(3-((cube4.timeElapse%32)+0)%4);
    cube4.offByRowLayer(3-((cube4.timeElapse%32)+1)%4);
    cube4.offByRowLayer(3-((cube4.timeElapse%32)+2)%4);
    cube4.offByRowLayer(3-((cube4.timeElapse%32)+3)%4);
  }
  else if(cube4.timeElapse < 32*3+8){//레이어 한층씩 켜짐 왼쪽으로 shift
    cube4.onByColLayer(((cube4.timeElapse%32)+0)%4);
    cube4.offByColLayer(((cube4.timeElapse%32)+1)%4);
    cube4.offByColLayer(((cube4.timeElapse%32)+2)%4);
    cube4.offByColLayer(((cube4.timeElapse%32)+3)%4);
  }
  else if(cube4.timeElapse < 32*3+16){//레이어 한층씩 켜짐 오른쪽으로 shift
    cube4.onByColLayer(3-((cube4.timeElapse%32)+0)%4);
    cube4.offByColLayer(3-((cube4.timeElapse%32)+1)%4);
    cube4.offByColLayer(3-((cube4.timeElapse%32)+2)%4);
    cube4.offByColLayer(3-((cube4.timeElapse%32)+3)%4);
  }
  else if(cube4.timeElapse < 32*4){//높이 방향으로 한 줄 켜짐
    cube4.onByLine(((cube4.timeElapse%32)+0)%16);
    for(int i=1;i<16;i++){
      cube4.offByLine(((cube4.timeElapse%32)+i)%16);
    }
  }
  else if(cube4.timeElapse < 32*4+16){//row 방향으로 한 줄 켜짐
    cube4.onByRowLine(((cube4.timeElapse%32)+0)%16);
    for(int i=1;i<16;i++){
      cube4.offByRowLine(((cube4.timeElapse%32)+i)%16);
    }
  }
  else if(cube4.timeElapse < 32*5){//col 방향으로 한 줄 켜짐
    cube4.onByColLine(((cube4.timeElapse%32)+0)%16);
    for(int i=1;i<16;i++){
      cube4.offByColLine(((cube4.timeElapse%32)+i)%16);
    }
  }
  else{
    cube4.onAll();
    delay(500);
    cube4.offAll();
    delay(500);
  }
}

ISR(TIMER2_COMPA_vect) {
  /* 타이머 인터럽트 서비스 루틴
   *  한 층씩 출력해준다.
   */
  static int counter = 0;
  int timerSpeed = 240;//240;//240으로 하면 약 1초마다 cube->timerElapse가 1씩 올라감
  if((cube4.ms4counter = ++cube4.ms4counter % timerSpeed) == 0) {
    cube4.timeElapse++;
  }
  counter = ++counter % cube4.getSizeOfLayer();
  cube4.showByLayer(counter);
}

