#include "macro.h"
#include <cstdio>

#define RAMSIZE (128*1024*1024)

static paddr_t ram[RAMSIZE / sizeof(paddr_t)];
static long img_size  = 0;

void * get_img_start() {
    return &ram[0];
}

long get_img_size() {
    return img_size;
}


void init_ram(const char * img) {
    assert(img);

    FILE *fp = fopen(img, "rb");

    if (!fp) {
        printf("can not open '%s'\n", img);
        assert(0);
    }

    fseek(fp, 0, SEEK_END);
    img_size = ftell(fp);

    int ret = fread(ram, img_size, 1, fp);
    assert(ret == 1);
    fclose(fp);    
}

 
extern "C" void ram_helper(
    paddr_t rIdx,
    paddr_t *rdata,
    paddr_t wIdx,
    paddr_t wdata,
    paddr_t wmask,
    uint8_t wen
) {
    *rdata = ram[rIdx];
    if (wen) {
        ram[wIdx] = (wdata & wmask) | (ram[wIdx] & ~wmask);
    }
}








