/************
 * 
 * Copyright 2019 by Fernando Trias. All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ************/

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
