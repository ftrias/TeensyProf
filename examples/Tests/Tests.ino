#include "TeensyProf.h"

int stall_3() {
  volatile int x;
  for(int i=0; i<3000; i++) {
    for(int j=0; j<10000; j++) {
      x = 0;
    }
  }
  return x;
}
int stall_2() {
  volatile int x;
  for(int i=0; i<2000; i++) {
    for(int j=0; j<10000; j++) {
      x = 0;
    }
  }
  return x;
}
int stall_1() {
  volatile int x;
  for(int i=0; i<1000; i++) {
    for(int j=0; j<10000; j++) {
      x = 0;
    }
  }
  return x;
}

void setup() {
  Serial.begin(115200);
  if (! TeensyProf_begin()) {
    Serial.println("fail");
  }
}

long start = millis();

void loop() {
  if (start) Serial.println("loop");
  stall_1();
  stall_2();
  stall_3();
  if (start && millis() - start > 10000) {
    TeensyProf_end();
    start = 0;
  }
}
