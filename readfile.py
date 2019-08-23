#!/usr/bin/env python3

gprof="arm-none-eabi-gprof"

import array
import binascii
import argparse
import os
import sys
import glob
import time

def call_gprof():
  os.system(gprof+" /tmp/build.elf")

def process_hex(filename, outfile):
    with open(filename) as inf:
        with open(outfile, "wb") as outf:
            for line in inf:
                outf.write(binascii.a2b_hex(line.strip()))

def process_msg(fxn, n, s):
  # print("process",fxn,n,s)
  global fp
  global ser
  if fxn == 1:  # open
    (fmode, fname) = s.decode('ascii').split(":", 2)
    fp = open(fname, fmode)
    print("open %s" % fname)
  elif fxn == 2:  # close
    fp.close()
    print("close")
    call_gprof()
  elif fxn == 3:  # read
    d = fp.read(n)
    os.write(ser, chr(l), 1)
    os.write(ser, d, l)
  elif fxn == 4:  # write
    fp.write(s)


def filehost():
  global ser
  fxn = os.read(ser, 1)  # function
  # print("@",fxn,"%")
  fxn = ord(fxn)
  n = os.read(ser, 1)  # length
  n = ord(n)
  s = os.read(ser, n)  # data
  process_msg(fxn, n, s)


def process_serial(file):
  global ser
  while True:
    if not os.path.exists(file):
      print("waiting for path:", file);
      while not os.path.exists(file):
        time.sleep(1)
      print("open serial %s" % file)
    ser = os.open(file, os.O_RDWR)
    while True:
      try:
        c = os.read(ser, 1)
      except OSError:
        break
      if len(c) > 0:
        if ord(c) == 1:  # control
          filehost()
        else:
          sys.stdout.write(c.decode('ascii'))
    print("close serial %s" % file)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Parse gmon.out file from Teensy.')
    parser.add_argument('--hex', help='Convert from ascii hex')
    parser.add_argument('--serial', help='Read serial device codes')

    args = parser.parse_args()
    if args.hex is not None:
        process_midi(args.hex)
    elif args.serial is not None:
        process_serial(args.serial)
    else:
        print("Nothing to do")
