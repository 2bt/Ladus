#!/usr/bin/python3

from PIL import Image
import argparse, textwrap, os, sys


class BitWriter:
    def __init__(self):
        self.bytes   = []
        self.bit_pos = 0
    def __call__(self, value, nbits):
        assert value >= 0
        assert value < (1 << nbits)
        while nbits > 0:
            b = min(8 - self.bit_pos, nbits)
            nbits -= b
            v = (value & (((1 << b) - 1) << nbits)) >> nbits
            if self.bit_pos == 0: self.bytes.append(0)
            self.bytes[-1] |= v << self.bit_pos
            self.bit_pos += b
            self.bit_pos %= 8

class BitReader:
    def __init__(self, bytes):
        self.bytes = bytes
        self.bit_pos  = 0
        self.byte_pos = 0
    def __call__(self, nbits):
        value = 0
        while nbits > 0:
            if self.byte_pos == len(self.bytes): return -1
            b = min(8 - self.bit_pos, nbits)
            nbits -= b
            value <<= b
            value |= (self.bytes[self.byte_pos] >> self.bit_pos) & ((1 << b) - 1)
            self.bit_pos  += b
            self.byte_pos += self.bit_pos == 8
            self.bit_pos  %= 8
        return value


MIN_L   = 3
BITS_C  = 8
BITS_D  = 9
BITS_L1 = 4
BITS_L2 = 7

def encode(data):
    def max_len(i, d):
        l = 0
        while i + l < len(data) and data[i - d + l % d] == data[i + l]: l += 1
        return l

    write  = BitWriter()
    i      = 0

    while i < len(data):
        l = 0
        for dd in range(1, min(i + 1, 1 << BITS_D)):
            ll = max_len(i, dd)
            if ll > l:
                d = dd
                l = ll
                if l >= (1 << BITS_L2) + MIN_L: break

        if l < MIN_L:
            write(0, BITS_D)
            write(data[i], BITS_C)
            i += 1
        else:
            write(d, BITS_D)
            l = min(l, (1 << BITS_L2))
            i += l
            l -= MIN_L
            if l < (1 << BITS_L1):
                write(0, 1)
                write(l, BITS_L1)
            else:
                write(1, 1)
                write(l, BITS_L2)

    return write.bytes



def decode(code):
    read   = BitReader(code)
    data   = []
    while 1:
        d = read(BITS_D)
        if d == -1: break
        if d == 0: data.append(read(BITS_C))
        else:
            if read(1): l = read(BITS_L2)
            else:       l = read(BITS_L1)
            for _ in range(l + MIN_L): data.append(data[-d])
    return data


def main():
    parser = argparse.ArgumentParser(description="encode png")
    parser.add_argument("png_file", nargs="+")
    args = parser.parse_args()

    hpp = open("surfaces.hpp", "w+")
    hpp.write("""#pragma once

#include "surface.hpp"

namespace surfaces {
    void init();
""")
    cpp = open("surfaces.cpp", "w+")
    cpp.write("""#include "surfaces.hpp"

namespace {

""")


    total_size = 0
    for png in args.png_file:
        name = os.path.splitext(os.path.basename(png))[0];
        name = name.replace("-", "_")

        img = Image.open(png).convert("RGBA")
        colors = [(0, 0, 0, 0)]
        color_table = { (0, 0, 0, 0) : 0 }
        for i, (_, c) in enumerate(img.getcolors()):
            if c[3] == 0:
                color_table[c] = 0
            else:
                color_table[c] = len(colors)
                colors.append(c)

        W, H = img.size
        data = [color_table[img.getpixel((x, y))] for y in range(H) for x in range(W)]
        code = encode(data)
        assert data == decode(code)
        code = [W & 255, W >> 8, H & 255, H >> 8, len(colors)] + list(sum(colors, ())) + code

        print("%-20s%d" % (png, len(code)))
        total_size += len(code)


        cpp.write("const uint8_t %s_DATA[] = {\n" % name.upper())
        for l in textwrap.wrap(", ".join(map(str, code)), width = 90): cpp.write("    " + l + "\n")
        cpp.write("};\n")
    print(total_size)

    cpp.write("""
} // namespace

""")
    for png in args.png_file:
        name = os.path.splitext(os.path.basename(png))[0];
        name = name.replace("-", "_")
        hpp.write("    extern Surface %s;\n" % name)
        cpp.write("Surface surfaces::%s;\n" % name)
    hpp.write("}\n")
    cpp.write("""
void surfaces::init() {
""")

    for png in args.png_file:
        name = os.path.splitext(os.path.basename(png))[0];
        name = name.replace("-", "_")
        cpp.write("    %s.init(%s_DATA);\n" % (name, name.upper()))
    cpp.write("}\n")


    hpp.seek(0)
    print(hpp.read())
    cpp.seek(0)
    print(cpp.read())

main()
