class  Cube {
private:
  int pinSize = 16;
  int layerSize = 4;
  byte pin[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,A0,A1};
  byte layer[4] = {A2,A3,A4,A5};
  byte *status;
  void initCube();
public:
  void offById(int);
  void offAll();
  void timer1Init();
  Cube(){ initCube(); }
};
void Cube::initCube(){
  for(int i=0;i<this->pinSize;i++){
    pinMode(this->pin[i],OUTPUT);
  }
  for(int i=0;i<this->layerSize;i++){
    pinMode(this->layer[i],OUTPUT);
  }
  this->status = new byte[pinSize * layerSize];
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
void Cube::timer1Init(){
  TCCR1A = 0x00;
  TCCR3A = 0x00;
}

void setup() {
  Cube cube;
}

void loop() {
}
