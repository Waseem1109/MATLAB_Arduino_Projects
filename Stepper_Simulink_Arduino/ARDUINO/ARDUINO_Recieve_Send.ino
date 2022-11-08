// Create a union to easily convert float to byte
typedef union {
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

// Create the variable you want to send
FLOATUNION_t myValue1;
FLOATUNION_t myValue2;
FLOATUNION_t myValue3;
FLOATUNION_t myValue4;
FLOATUNION_t myValue5;
int A, S, I, D, i, d, j, B, n = 0;
const int Dir1 = 9;
const int Step1 = 8;
const int Dir2 = 11;
const int Step2 = 10;

void setup() {
  // initialize serial, use the same boudrate in the Simulink Config block
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(Dir1, OUTPUT);
  pinMode(Step1, OUTPUT);
  pinMode(Dir2, OUTPUT);
  pinMode(Step2, OUTPUT);
  digitalWrite(13, LOW);

}
void loop() {
  A = getFloat(); // Give Steps or angle
  S = getFloat(); // Speed or delay
  I = getFloat(); // Iteration
  d = getFloat(); // direction
  B = getFloat(); // start button
  delay(100);
  digitalWrite(13, HIGH);
  if (A >= 0 && A <= 200 && S >= 500 && S <= 32000 && B == 1) {
    n = 0;
    WriteMATLAB();
    if (d == 1) {
      D = 0;
    }
    if (d == 2) {
      D = 1;
    }
    for (j = 0; j < I; j++) {
      StepperMotor(D, S, A);
      D = Reverse_direction(D);
      delay(500);
      StepperMotor(D, S, A);
      D = Reverse_direction(D);
      n = n + 1;
      WriteMATLAB();
      delay(500);
    }
    n = j;
    WriteMATLAB();
    delay(1);
  }

}


float getFloat() {
  int cont = 0;
  FLOATUNION_t f;
  while (cont < 4 ) {
    f.bytes[cont] = Serial.read() ;
    cont = cont + 1;
  }
  return f.number;
}
void StepperMotor(int Direction, int Delay, int steps) {
  for (i = 0; i < steps; i++) {
    digitalWrite(Dir1, Direction);
    digitalWrite(Dir2, Direction);
    digitalWrite(Step1, HIGH);
    digitalWrite(Step2, HIGH);
    delayMicroseconds(Delay);
    digitalWrite(Step1, LOW);
    digitalWrite(Step2, LOW);
    delayMicroseconds(Delay);
  }
}
int Reverse_direction(int D) {
  if (D == 0)
  {
    D = 1;
  }
  else if (D == 1)
  {
    D = 0;
  }
  return D;
}
void WriteMATLAB() {
  myValue1.number = A * 1.8;
  myValue2.number = 150000 / S;
  myValue3.number = I;
  myValue4.number = d;
  myValue5.number = n;
  Serial.write('A');
  for (int i = 0; i < 4; i++) {
    Serial.write(myValue1.bytes[i]);
  }
  for (int i = 0; i < 4; i++) {
    Serial.write(myValue2.bytes[i]);
  }
  for (int i = 0; i < 4; i++) {
    Serial.write(myValue3.bytes[i]);
  }
  for (int i = 0; i < 4; i++) {
    Serial.write(myValue4.bytes[i]);
  }
  for (int i = 0; i < 4; i++) {
    Serial.write(myValue5.bytes[i]);
  }
  Serial.print('\n');
}
