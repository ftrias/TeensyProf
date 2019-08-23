TeensyProf
=====================

Limited implementation of gprof profiler for Teensy 4 platform from PJRC.

Only the histogram is implemented. Call heirarchies are not available because
the compiler included by Teensyduino does not support profiling
instrumentation.

This implementation guide is for Linux/Mac. A Windows version is possible.

Requires 
--------------

1. Add to platforms.txt

recipe.hooks.postbuild.4.pattern="cp" "{build.path}/{build.project_name}.elf" "/tmp/build.elf"

2. Build of gprof for ARM. Instructions for this are beyond the scope of this
text. Look up how to build your own cross-compiler. Normally, this is part of
a build, but Teensyduino does not include it.

Overview
-------------

The profiler samples the current instruction every 1 millisecond. That is
at each millisecond it looks to see what function is currently running and
keeps a counter of how many times this happens. Over time, this gives
an approximate value of how much time each function consumes.

On desktops, gprof will also keep track of which function calls which, but
that is not supported on Teensy 4.

After a while, a file "gmon.out" is written out with the function counters.
This file is then cross-referenced by gprof with the original executable to 
generate a table of execution times.

Quick Instructions
--------------

1. Modify platforms.txt as above
2. Build gprof
3. Open up Arduino and TeensyProf example
4. Select Tools/Optimize/Debug
5. Select a USB that includes Serial
6. Compile and upload
7. Run `readfile.py --serial /dev/cu.usb*` [substitute actual usb serial]

`readfile.py` will open the serial port, print out anything it receives
and process the `gmon.out` data. It will write out the `gmon.out` file and then
run `gprof` showing the outout.

Todo
--------------

For now, using the "-pg" option of gcc
won't work because the Teensyduino package does not
include libc_p.a (the profiling version of the C library libc.a). Without that,
the link stage will fail. This limits what can be done. If libc_p.a is
included, then a complete implementation of gprof is possible.

Add to boards.txt
teensy40.menu.opt.pgstd=Profile
teensy40.menu.opt.pgstd.build.flags.optimize=-pg -Og
teensy40.menu.opt.pgstd.build.flags.ldspecs=

References
---------------

For a more complete solution see: 
https://mcuoneclipse.com/2015/08/23/tutorial-using-gnu-profiling-gprof-with-arm-cortex-m/

For an interesting overview of gprof:
http://wwwcdf.pd.infn.it/localdoc/gprof.pdf

