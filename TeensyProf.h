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

#ifndef GPROF_H_INCLUDED
#define GPROF_H_INCLUDED

/* Use one of the following #defines to specify the output for the
 * gmon.out file
 */

#ifndef TEENSYPROF_OUT
// Write to an SD card
// #define TEENSYPROF_OUT 1
// Write to Serial in a binary format (must have suitable reader)
#define TEENSYPROF_OUT 2
// Write to MIDI using a binary format
// #define TEENSYPROF_OUT 3
// Write to serial as an ascii hex file
// #define TEENSYPROF_OUT 4
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool TeensyProf_begin();
void TeensyProf_end();

int TeensyProf_open(const char *fn, int flags, int perm);
int TeensyProf_write(int fp, const void *data, int length);
int TeensyProf_close(int fp);
void TeensyProf_init_sd(int mosi, int sck, int cd);

#ifdef __cplusplus
}
#endif

#endif
