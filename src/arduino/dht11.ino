
#define TIMEOUT UINT8_MAX 
const  uint8_t  DPin = 5;
uint8_t _bit, _port, _maxcycles;
uint8_t cycles[82];

uint8_t count;
uint8_t portState;
unsigned long start;


void initializeDHT() {
//  Serial.println("start");
  delay(1100);
  pinMode(DPin, OUTPUT);
  digitalWrite(DPin, LOW);
  delay(20);
  pinMode(DPin, INPUT_PULLUP);
  delayMicroseconds(40);
}


uint8_t expectPulse(bool level) {
    count = 0;
    portState = level ? _bit :0;
    start = micros();
    while ( (PIND & _bit) == portState) {
    if (count++ >= _maxcycles) {
      return TIMEOUT; // Exceeded timeout, fail.
    }
  }
  return micros() - start;
}

bool checkResponse() {
  if (expectPulse(LOW) == TIMEOUT) {
    return 0;
  }
  if (expectPulse(HIGH) == TIMEOUT) {
    return 0;
  }
  return 1;
}

void getData() {
    uint8_t result[4]; 
    for (int i = 0; i < 80; i += 2) {
    cycles[i] = expectPulse(LOW);
    cycles[i + 1] = expectPulse(HIGH);
    }
    
   for (int i = 0; i < 40; ++i) {
    uint8_t lowCycles = cycles[2 * i];
    uint8_t highCycles = cycles[2 * i + 1];
    result[i/8] <<=1;
    if (highCycles > lowCycles) {
      result[i/8] |=1;
    }
//    Serial.print(i);
//    Serial.print(" --" );
//    Serial.print(lowCycles);
//    Serial.print(":");
//    Serial.println(highCycles);
    
}
   if (result[4] == result[0] + result[1] + result[2] + result[3]) {
    Serial.print("Humidity: ");
    Serial.println(result[0]);
    Serial.print("Temperature: ");
    Serial.print(result[2]);
    Serial.print(".");
    Serial.println(result[3]);
   }
   else {
    Serial.println("CRC failed");
   }
  
    
}
void setup() {
   _bit = digitalPinToBitMask(DPin); //00100000
  _port = digitalPinToPort(DPin);    //00000100
  _maxcycles =microsecondsToClockCycles(1000);
  Serial.begin(9600);
//  Serial.println(_bit, BIN);
//  Serial.println(_port, BIN);
//  Serial.println(_maxcycles);
  
  

}

void loop() {
  initializeDHT();
  if (checkResponse()){
    getData();
  }

}
