//
// Created by rols on 6/12/16.
//

#ifndef SS_LOADERCONF_H
#define SS_LOADERCONF_H


static const size_t MEMORY_SIZE = 1024 * 1024;
static const size_t STACK_START = 128 * 1024;
static const size_t STACK_SIZE = 8 * 1024;

static const size_t IVT_SIZE = 1024;
static const size_t IVT_START = 256 * 1024;

static const size_t KEYBOARD_POS = 0x1000;
static const size_t KEYBOARD_STATUS_POS = 0x1010;
static const u_int32_t KEYBOARD_STATUS_MASK = 1L << 9;
static const size_t OUTPUT_POS = 0x2000;

static const size_t IRET_CODE = 0x230ee;

static const size_t CODE_START = 0x40; //16 * 4


#endif //SS_LOADERCONF_H
