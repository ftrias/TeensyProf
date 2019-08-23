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
