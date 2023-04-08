#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <iomanip>
#include <fstream>
#include <functional>
#include <inttypes.h>
#include "emu.h"

VerilatedContext* contextp;




int main(int argc, const char ** argv) {
    contextp = new VerilatedContext;
    
    auto emu = Emulator(argc, argv);
}