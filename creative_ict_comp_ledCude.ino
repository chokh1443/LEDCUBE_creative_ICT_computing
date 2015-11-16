class  Cube {
private:
  int pinSize = 16;
  int layerSize = 4;
  byte pin[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,A0,A1};
  byte layer[4] = {A2,A3,A4,A5};
  byte *status;
public:
  void offById(int);
  //void offByLayer(int);
  void offAll();
  void onById(int);
  //void onByLayer(int);
  void onAll();
  void timer0Init();
  void init();
  int getSizeOfCube(){ return (this->pinSize * this->layerSize); };
  int getSizeOfLayer(){ return this->layerSize; };
  void showByLayer(int);
};
void Cube::init(){
  for(int i=0;i<this->pinSize;i++){
    pinMode(this->pin[i],OUTPUT);
  }
  for(int i=0;i<this->layerSize;i++){
    pinMode(this->layer[i],OUTPUT);
  }
  this->status = new byte[this->pinSize * this->layerSize];
  this->offAll();
}
void Cube::offById(int id){
  this->status[id] = 0;
}
void Cube::offAll(){
  for(int i=0;i< (this->pinSize * this->layerSize); i++){
    this->offById(i);
  }
}
void Cube::onById(int id){
  this->status[id] = 1;
}
void Cube::onAll(){
  for(int i=0;i< (this->pinSize * this->layerSize); i++){
    this->onById(i);
  }
}
void Cube::timer0Init(){
  //240Hz
  cli();
  //CTC mode, 분주비 1024
  //datasheet pg106
  TCCR0A = 0x02;// (1<<WGM01)
  TCCR0B = 0x05;// (1<<CS01) | (1<<CS00)
  TCNT0 = 0x00;
  OCR0A = 0x40;//64
  TIMSK0 = 0x02;// (1<<OCIE0A);
  sei();
}
void Cube::showByLayer(int num){
  for(int i=0;i<this->pinSize;i++){
    if(this->status[(num*16+i)] == 1) {
      
      digitalWrite(this->pin[i],HIGH);  
    }
    else {  
      digitalWrite(this->pin[i],LOW);
    }
  }
  for(int i=0;i<this->layerSize;i++){
    if(i == num){
      digitalWrite(this->layer[i],HIGH);
    }
    else {
      digitalWrite(this->layer[i],LOW);
    }
  }
}

Cube cube;
int ms4 = 0;
void setup() {
  cube.init();
  cube.timer0Init();
}

void loop() {
  if(ms4 < 120){
    cube.onAll();
  }else{
    cube.offAll();
  }
}

ISR(TIMER0_COMPA_vect){
  //Interrupt Subrutine
  static int counter = 0;
  ms4 = ++ms4%250;
  counter = ++counter%cube.getSizeOfLayer();
  cube.showByLayer(counter);
  
}

