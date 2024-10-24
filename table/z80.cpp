/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2024 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "table/z80.h"

struct _table_instr_z80 table_instr_z80[] =
{
  { "adc",  Z80_ADC  },
  { "add",  Z80_ADD  },
  { "and",  Z80_AND  },
  { "bit",  Z80_BIT  },
  { "call", Z80_CALL },
  { "ccf",  Z80_CCF  },
  { "cp",   Z80_CP   },
  { "cpd",  Z80_CPD  },
  { "cpdr", Z80_CPDR },
  { "cpi",  Z80_CPI  },
  { "cpir", Z80_CPIR },
  { "cpl",  Z80_CPL  },
  { "daa",  Z80_DAA  },
  { "dec",  Z80_DEC  },
  { "di",   Z80_DI   },
  { "djnz", Z80_DJNZ },
  { "ei",   Z80_EI   },
  { "ex",   Z80_EX   },
  { "exx",  Z80_EXX  },
  { "halt", Z80_HALT },
  { "im",   Z80_IM   },
  { "in",   Z80_IN   },
  { "inc",  Z80_INC  },
  { "ind",  Z80_IND  },
  { "indr", Z80_INDR },
  { "ini",  Z80_INI  },
  { "inir", Z80_INIR },
  { "jp",   Z80_JP   },
  { "jr",   Z80_JR   },
  { "ld",   Z80_LD   },
  { "ldd",  Z80_LDD  },
  { "lddr", Z80_LDDR },
  { "ldi",  Z80_LDI  },
  { "ldir", Z80_LDIR },
  { "neg",  Z80_NEG  },
  { "nop",  Z80_NOP  },
  { "or",   Z80_OR   },
  { "otdr", Z80_OTDR },
  { "otir", Z80_OTIR },
  { "out",  Z80_OUT  },
  { "outd", Z80_OUTD },
  { "outi", Z80_OUTI },
  { "pop",  Z80_POP  },
  { "push", Z80_PUSH },
  { "res",  Z80_RES  },
  { "ret",  Z80_RET  },
  { "reti", Z80_RETI },
  { "retn", Z80_RETN },
  { "rl",   Z80_RL   },
  { "rla",  Z80_RLA  },
  { "rlc",  Z80_RLC  },
  { "rlca", Z80_RLCA },
  { "rld",  Z80_RLD  },
  { "rr",   Z80_RR   },
  { "rra",  Z80_RRA  },
  { "rrc",  Z80_RRC  },
  { "rrca", Z80_RRCA },
  { "rrd",  Z80_RRD  },
  { "rst",  Z80_RST  },
  { "sbc",  Z80_SBC  },
  { "scf",  Z80_SCF  },
  { "set",  Z80_SET  },
  { "sla",  Z80_SLA  },
  { "sll",  Z80_SLL  },
  { "sra",  Z80_SRA  },
  { "srl",  Z80_SRL  },
  { "stop", Z80_STOP },
  { "sub",  Z80_SUB  },
  { "xor",  Z80_XOR  },
  { NULL,   0        }
};

// Note: Some items below *must* be ordered with the more-specific masked
//       item listed before the less-specific masked item to be correctly used.
//       This primarily involves the INDEX_HL vs REG8 type items.
//       Rules for any opcode:
//           OP_A_INDEX_HL       - must be before OP_A_REG8
//           OP_INDEX_HL         - must be before OP_REG8 and OP_A_REG_IHALF
//           OP_INDEX_HL         - must be before OP_REG8_V2
//           OP_REG8_INDEX_HL    - must be before OP_REG8_REG8
//           OP_INDEX_HL_REG8    - must be before OP_REG8_REG8
//           OP_INDEX_HL_NUMBER8 - must be before OP_REG8_NUMBER8
//           OP_BIT_INDEX_HL     - must be before OP_BIT_REG8

struct _table_z80 table_z80[] =
{
  { Z80_ADC,  OP_A_INDEX_HL,          0x8e,   0xff,   0x00,  7,  7 },
  { Z80_ADC,  OP_A_REG8,              0x88,   0xf8,   0x00,  4,  4 },
  { Z80_ADC,  OP_A_REG_IHALF,         0xdd8c, 0xdffe, 0x00,  8,  8 },
  { Z80_ADC,  OP_A_NUMBER8,           0xce,   0xff,   0x00,  7,  7 },
  { Z80_ADC,  OP_A_INDEX,             0xdd8e, 0xdfff, 0x00, 19, 19 },
  { Z80_ADC,  OP_HL_REG16_2,          0xed4a, 0xffcf, 0x00, 15, 15 },
  { Z80_ADD,  OP_A_INDEX_HL,          0x86,   0xff,   0x00,  7,  7 },
  { Z80_ADD,  OP_A_REG8,              0x80,   0xf8,   0x00,  4,  4 },
  { Z80_ADD,  OP_A_REG_IHALF,         0xdd84, 0xdffe, 0x00,  8,  8 },
  { Z80_ADD,  OP_A_NUMBER8,           0xc6,   0xff,   0x00,  7,  7 },
  { Z80_ADD,  OP_A_INDEX,             0xdd86, 0xdfff, 0x00, 19, 19 },
  { Z80_ADD,  OP_HL_REG16_1,          0x09,   0xcf,   0x00, 11, 11 },
  { Z80_ADD,  OP_XY_REG16,            0xdd09, 0xdfcf, 0x00, 15, 15 },
  { Z80_AND,  OP_INDEX_HL,            0xa6,   0xff,   0x00,  7,  7 },
  { Z80_AND,  OP_REG8,                0xa0,   0xf8,   0x00,  4,  4 },
  { Z80_AND,  OP_REG_IHALF,           0xdda4, 0xdffe, 0x00,  8,  8 },
  { Z80_AND,  OP_NUMBER8,             0xe6,   0xff,   0x00,  7,  7 },
  { Z80_AND,  OP_INDEX,               0xdda6, 0xdfff, 0x00, 19, 19 },
  { Z80_CALL, OP_ADDRESS,             0xcd,   0xff,   0x00, 17, 17 },
  { Z80_CALL, OP_COND_ADDRESS,        0xc4,   0xc7,   0x00, 10, 17 },
  { Z80_CCF,  OP_NONE,                0x3f,   0xff,   0x00,  4,  4 },
  { Z80_CP,   OP_INDEX_HL,            0xbe,   0xff,   0x00,  7,  7 },
  { Z80_CP,   OP_REG8,                0xb8,   0xf8,   0x00,  4,  4 },
  { Z80_CP,   OP_REG_IHALF,           0xddbc, 0xdffe, 0x00,  8,  8 },
  { Z80_CP,   OP_NUMBER8,             0xfe,   0xff,   0x00,  7,  7 },
  { Z80_CP,   OP_INDEX,               0xddbe, 0xdfff, 0x00, 19, 19 },
  { Z80_CPD,  OP_NONE16,              0xeda9, 0xffff, 0x00, 16, 16 },
  { Z80_CPDR, OP_NONE16,              0xedb9, 0xffff, 0x00, 16, 21 },
  { Z80_CPI,  OP_NONE16,              0xeda1, 0xffff, 0x00, 16, 16 },
  { Z80_CPIR, OP_NONE16,              0xedb1, 0xffff, 0x00, 16, 21 },
  { Z80_CPL,  OP_NONE,                0x2f,   0xff,   0x00,  4,  4 },
  { Z80_DAA,  OP_NONE,                0x27,   0xff,   0x00,  4,  4 },
  { Z80_DEC,  OP_INDEX_HL,            0x35,   0xff,   0x00, 11, 11 },
  { Z80_DEC,  OP_REG8_V2,             0x05,   0xc7,   0x00,  4,  4 },
  { Z80_DEC,  OP_REG_IHALF_V2,        0xdd25, 0xdff7, 0x00,  8,  8 },
  { Z80_DEC,  OP_INDEX,               0xdd35, 0xdfff, 0x00, 23, 23 },
  { Z80_DEC,  OP_REG16,               0x0b,   0xcf,   0x00,  6,  6 },
  { Z80_DEC,  OP_XY,                  0xdd2b, 0xdfff, 0x00, 10, 10 },
  { Z80_DI,   OP_NONE,                0xf3,   0xff,   0x00,  4,  4 },
  { Z80_DJNZ, OP_OFFSET8,             0x10,   0xff,   0x00,  8, 13 },
  { Z80_EI,   OP_NONE,                0xfb,   0xff,   0x00,  4,  4 },
  { Z80_EX,   OP_INDEX_SP_HL,         0xe3,   0xff,   0x00, 19, 19 },
  { Z80_EX,   OP_INDEX_SP_XY,         0xdde3, 0xdfff, 0x00, 23, 23 },
  { Z80_EX,   OP_AF_AF_TICK,          0x08,   0xff,   0x00,  4,  4 },
  { Z80_EX,   OP_DE_HL,               0xeb,   0xff,   0x00,  4,  4 },
  { Z80_EXX,  OP_NONE,                0xd9,   0xff,   0x00,  4,  4 },
  { Z80_HALT, OP_NONE,                0x76,   0xff,   0x00,  4,  4 },
  { Z80_IM,   OP_IM_NUM,              0xed46, 0xffc7, 0x00,  8,  8 },
  { Z80_IN,   OP_A_INDEX_N,           0xdb,   0xff,   0x00, 11, 11 },
  { Z80_IN,   OP_REG8_INDEX_C,        0xed40, 0xffc7, 0x00, 12, 12 },
  { Z80_IN,   OP_F_INDEX_C,           0xed70, 0xffff, 0x00, 12, 12 },
  { Z80_INC,  OP_INDEX_HL,            0x34,   0xff,   0x00, 11, 11 },
  { Z80_INC,  OP_REG8_V2,             0x04,   0xc7,   0x00,  4,  4 },
  { Z80_INC,  OP_REG_IHALF_V2,        0xdd24, 0xdff7, 0x00,  8,  8 },
  { Z80_INC,  OP_INDEX,               0xdd34, 0xdfff, 0x00, 23, 23 },
  { Z80_INC,  OP_REG16,               0x03,   0xcf,   0x00,  6,  6 },
  { Z80_INC,  OP_XY,                  0xdd23, 0xdfff, 0x00, 10, 10 },
  { Z80_IND,  OP_NONE16,              0xedaa, 0xffff, 0x00, 16, 16 },
  { Z80_INDR, OP_NONE16,              0xedba, 0xffff, 0x00, 16, 21 },
  { Z80_INI,  OP_NONE16,              0xeda2, 0xffff, 0x00, 16, 16 },
  { Z80_INIR, OP_NONE16,              0xedb2, 0xffff, 0x00, 16, 21 },
  { Z80_JP,   OP_ADDRESS,             0xc3,   0xff,   0x00, 10, 10 },
  { Z80_JP,   OP_INDEX_HL,            0xe9,   0xff,   0x00,  4,  4 },
  { Z80_JP,   OP_INDEX_XY,            0xdde9, 0xdfff, 0x00,  8,  8 },
  { Z80_JP,   OP_COND_ADDRESS,        0xc2,   0xc7,   0x00, 10, 10 },
  { Z80_JR,   OP_OFFSET8,             0x18,   0xff,   0x00, 12, 12 },
  { Z80_JR,   OP_JR_COND_ADDRESS,     0x20,   0xe7,   0x00,  7, 12 },
  { Z80_LD,   OP_REG8_INDEX_HL,       0x46,   0xc7,   0x00,  7,  7 },
  { Z80_LD,   OP_INDEX_HL_REG8,       0x70,   0xf8,   0x00,  7,  7 },
  { Z80_LD,   OP_REG8_REG8,           0x40,   0xc0,   0x00,  4,  4 },
//  "ld", 0xdd44, 0xdfc6, OP_REG8_REG_IHALF, Z80_LD, 8        WTF??
//  "ld", 0xdd60, 0xdff0, OP_REG_IHALF_REG8, Z80_LD, 8        WTF??
//  "ld", 0xdd64, 0xdf64, OP_REG_IHALF_REG_IHALF, Z80_LD, 8   WTF??
  { Z80_LD,   OP_INDEX_HL_NUMBER8,    0x36,   0xff,   0x00, 10, 10 },
  { Z80_LD,   OP_REG8_NUMBER8,        0x06,   0xc7,   0x00,  7,  7 },
  { Z80_LD,   OP_REG8_INDEX,          0xdd46, 0xdfc7, 0x00, 19, 19 },
  { Z80_LD,   OP_INDEX_REG8,          0xdd70, 0xdff8, 0x00, 19, 19 },
  { Z80_LD,   OP_INDEX_NUMBER8,       0xdd36, 0xdfff, 0x00, 19, 19 },
  { Z80_LD,   OP_A_INDEX_BC,          0x0a,   0xff,   0x00,  7,  7 },
  { Z80_LD,   OP_A_INDEX_DE,          0x1a,   0xff,   0x00,  7,  7 },
  { Z80_LD,   OP_A_INDEX_ADDRESS,     0x3a,   0xff,   0x00, 13, 13 },
  { Z80_LD,   OP_INDEX_BC_A,          0x02,   0xff,   0x00,  7,  7 },
  { Z80_LD,   OP_INDEX_DE_A,          0x12,   0xff,   0x00,  7,  7 },
  { Z80_LD,   OP_INDEX_ADDRESS_A,     0x32,   0xff,   0x00, 13, 13 },
  { Z80_LD,   OP_IR_A,                0xed47, 0xfff7, 0x00,  9,  9 },
  { Z80_LD,   OP_A_IR,                0xed57, 0xfff7, 0x00,  9,  9 },
  { Z80_LD,   OP_REG16_ADDRESS,       0x01,   0xcf,   0x00, 10, 10 },
  { Z80_LD,   OP_XY_ADDRESS,          0xdd21, 0xdfff, 0x00, 14, 14 },
  { Z80_LD,   OP_HL_INDEX_ADDRESS,    0x2a,   0xff,   0x00, 16, 16 },
  { Z80_LD,   OP_REG16_INDEX_ADDRESS, 0xed4b, 0xffcf, 0x00, 20, 20 },
  { Z80_LD,   OP_XY_INDEX_ADDRESS,    0xdd2a, 0xdfff, 0x00, 20, 20 },
  { Z80_LD,   OP_INDEX_ADDRESS_HL,    0x22,   0xff,   0x00, 16, 16 },
  { Z80_LD,   OP_INDEX_ADDRESS_REG16, 0xed43, 0xffcf, 0x00, 20, 20 },
  { Z80_LD,   OP_INDEX_ADDRESS_XY,    0xdd22, 0xdfff, 0x00, 20, 20 },
  { Z80_LD,   OP_SP_HL,               0xf9,   0xff,   0x00,  6,  6 },
  { Z80_LD,   OP_SP_XY,               0xddf9, 0xdfff, 0x00, 10, 10 },
  { Z80_LD,   OP_A_REG_IHALF,         0xdd7c, 0xdffe, 0x00, 10, 10 },
  { Z80_LD,   OP_B_REG_IHALF,         0xdd44, 0xdffe, 0x00, 10, 10 },
  { Z80_LD,   OP_C_REG_IHALF,         0xdd4c, 0xdffe, 0x00, 10, 10 },
  { Z80_LD,   OP_D_REG_IHALF,         0xdd54, 0xdffe, 0x00, 10, 10 },
  { Z80_LD,   OP_E_REG_IHALF,         0xdd5c, 0xdffe, 0x00, 10, 10 },
  { Z80_LDD,  OP_NONE16,              0xeda8, 0xffff, 0x00, 16, 16 },
  { Z80_LDDR, OP_NONE16,              0xedb8, 0xffff, 0x00, 16, 21 },
  { Z80_LDI,  OP_NONE16,              0xeda0, 0xffff, 0x00, 16, 16 },
  { Z80_LDIR, OP_NONE16,              0xedb0, 0xffff, 0x00, 16, 21 },
  { Z80_NEG,  OP_NONE16,              0xed44, 0xffff, 0x00,  8,  8 },
  { Z80_NOP,  OP_NONE,                0x00,   0xff,   0x00,  4,  4 },
  { Z80_OR,   OP_INDEX_HL,            0xb6,   0xff,   0x00,  7,  7 },
  { Z80_OR,   OP_REG8,                0xb0,   0xf8,   0x00,  4,  4 },
  { Z80_OR,   OP_REG_IHALF,           0xddb4, 0xdffe, 0x00,  8,  8 },
  { Z80_OR,   OP_NUMBER8,             0xf6,   0xff,   0x00,  7,  7 },
  { Z80_OR,   OP_INDEX,               0xddb6, 0xdfff, 0x00, 19, 19 },
  { Z80_OUT,  OP_INDEX_ADDRESS8_A,    0xd3,   0xff,   0x00, 11, 11 },
  { Z80_OUT,  OP_INDEX_C_ZERO,        0xed71, 0xffff, 0x00, 12, 12 },
  { Z80_OUT,  OP_INDEX_C_REG8,        0xed41, 0xffc7, 0x00, 12, 12 },
  { Z80_OUTD, OP_NONE16,              0xedab, 0xffff, 0x00, 16, 16 },
  { Z80_OTDR, OP_NONE16,              0xedbb, 0xffff, 0x00, 16, 21 },
  { Z80_OUTI, OP_NONE16,              0xeda3, 0xffff, 0x00, 16, 16 },
  { Z80_OTIR, OP_NONE16,              0xedb3, 0xffff, 0x00, 16, 21 },
  { Z80_POP,  OP_REG16P,              0xc1,   0xcf,   0x00, 10, 10 },
  { Z80_POP,  OP_XY,                  0xdde1, 0xdfff, 0x00, 14, 14 },
  { Z80_PUSH, OP_REG16P,              0xc5,   0xcf,   0x00, 11, 11 },
  { Z80_PUSH, OP_XY,                  0xdde5, 0xdfff, 0x00, 15, 15 },
  { Z80_RES,  OP_BIT_INDEX_HL,        0xcb86, 0xffc7, 0x00, 15, 15 },
  { Z80_RES,  OP_BIT_REG8,            0xcb80, 0xffc0, 0x00,  8,  8 },
  { Z80_RET,  OP_NONE,                0xc9,   0xff,   0x00, 10, 10 },
  { Z80_RET,  OP_COND,                0xc0,   0xc7,   0x00,  5, 11 },
  { Z80_RETI, OP_NONE16,              0xed4d, 0xffff, 0x00, 14, 14 },
  { Z80_RETN, OP_NONE16,              0xed45, 0xffff, 0x00, 14, 14 },
  { Z80_RLA,  OP_NONE,                0x17,   0xff,   0x00,  4,  4 },
  { Z80_RL,   OP_INDEX_HL_CB,         0xcb16, 0xffff, 0x00, 15, 15 },
  { Z80_RL,   OP_REG8_CB,             0xcb10, 0xfff8, 0x00,  8,  8 },
  { Z80_RL,   OP_INDEX_LONG,          0xddcb, 0xdfff, 0x16, 23, 23 },
  { Z80_RLCA, OP_NONE,                0x07,   0xff,   0x00,  4,  4 },
  { Z80_RLC,  OP_INDEX_HL_CB,         0xcb06, 0xffff, 0x00, 15, 15 },
  { Z80_RLC,  OP_REG8_CB,             0xcb00, 0xfff8, 0x00,  8,  8 },
  { Z80_RLC,  OP_INDEX_LONG,          0xddcb, 0xdfff, 0x06, 23, 23 },
  { Z80_RLD,  OP_NONE16,              0xed6f, 0xffff, 0x00, 18, 18 },
  { Z80_RRA,  OP_NONE,                0x1f,   0xff,   0x00,  4,  4 },
  { Z80_RR,   OP_INDEX_HL_CB,         0xcb1e, 0xffff, 0x00, 15, 15 },
  { Z80_RR,   OP_REG8_CB,             0xcb18, 0xfff8, 0x00,  8,  8 },
  { Z80_RR,   OP_INDEX_LONG,          0xddcb, 0xdfff, 0x1e, 23, 23 },
  { Z80_RRCA, OP_NONE,                0x0f,   0xff,   0x00,  4,  4 },
  { Z80_RRC,  OP_INDEX_HL_CB,         0xcb0e, 0xffff, 0x00, 15, 15 },
  { Z80_RRC,  OP_REG8_CB,             0xcb08, 0xfff8, 0x00,  8,  8 },
  { Z80_RRC,  OP_INDEX_LONG,          0xddcb, 0xdfff, 0x0e, 23, 23 },
  { Z80_RRD,  OP_NONE16,              0xed67, 0xffff, 0x00, 18, 18 },
  { Z80_RST,  OP_RESTART_ADDRESS,     0xc7,   0xc7,   0x00, 11, 11 },
  { Z80_SBC,  OP_A_INDEX_HL,          0x9e,   0xff,   0x00,  7,  7 },
  { Z80_SBC,  OP_A_REG8,              0x98,   0xf8,   0x00,  4,  4 },
  { Z80_SBC,  OP_A_REG_IHALF,         0xdd9c, 0xdffe, 0x00,  8,  8 },
  { Z80_SBC,  OP_A_NUMBER8,           0xde,   0xff,   0x00,  7,  7 },
  { Z80_SBC,  OP_A_INDEX,             0xdd9e, 0xdfff, 0x00, 19, 19 },
  { Z80_SBC,  OP_HL_REG16_2,          0xed42, 0xffcf, 0x00, 15, 15 },
  { Z80_SCF,  OP_NONE,                0x37,   0xff,   0x00,  4,  4 },
  { Z80_SET,  OP_BIT_INDEX_HL,        0xcbc6, 0xffc7, 0x00, 15, 15 },
  { Z80_SET,  OP_BIT_REG8,            0xcbc0, 0xffc0, 0x00,  8,  8 },
  { Z80_SLA,  OP_INDEX_HL_CB,         0xcb26, 0xffff, 0x00, 15, 15 },
  { Z80_SLA,  OP_REG8_CB,             0xcb20, 0xfff8, 0x00,  8,  8 },
  { Z80_SLA,  OP_INDEX_LONG,          0xddcb, 0xdfff, 0x26, 23, 23 },
  { Z80_SRA,  OP_INDEX_HL_CB,         0xcb2e, 0xffff, 0x00, 15, 15 },
  { Z80_SRA,  OP_REG8_CB,             0xcb28, 0xfff8, 0x00,  8,  8 },
  { Z80_SRA,  OP_INDEX_LONG,          0xddcb, 0xdfff, 0x2e, 23, 23 },
  { Z80_SLL,  OP_INDEX_HL_CB,         0xcb36, 0xffff, 0x00, 15, 15 },
  { Z80_SLL,  OP_REG8_CB,             0xcb30, 0xfff8, 0x00,  8,  8 },
  { Z80_SLL,  OP_INDEX_LONG,          0xddcb, 0xdfff, 0x36, 23, 23 },
  { Z80_SRL,  OP_INDEX_HL_CB,         0xcb3e, 0xffff, 0x00, 15, 15 },
  { Z80_SRL,  OP_REG8_CB,             0xcb38, 0xfff8, 0x00,  8,  8 },
  { Z80_SRL,  OP_INDEX_LONG,          0xddcb, 0xdfff, 0x3e, 23, 23 },
  { Z80_STOP, OP_NONE24,              0xdddd, 0xffff, 0x00,  0,  0 },
  { Z80_SUB,  OP_INDEX_HL,            0x96,   0xff,   0x00,  7,  7 },
  { Z80_SUB,  OP_REG8,                0x90,   0xf8,   0x00,  4,  4 },
  { Z80_SUB,  OP_REG_IHALF,           0xdd94, 0xdffe, 0x00,  8,  8 },
  { Z80_SUB,  OP_NUMBER8,             0xd6,   0xff,   0x00,  7,  7 },
  { Z80_SUB,  OP_INDEX,               0xdd96, 0xdfff, 0x00, 19, 19 },
  { Z80_XOR,  OP_INDEX_HL,            0xae,   0xff,   0x00,  7,  7 },
  { Z80_XOR,  OP_REG8,                0xa8,   0xf8,   0x00,  4,  4 },
  { Z80_XOR,  OP_REG_IHALF,           0xddac, 0xdffe, 0x00,  8,  8 },
  { Z80_XOR,  OP_NUMBER8,             0xee,   0xff,   0x00,  7,  7 },
  { Z80_XOR,  OP_INDEX,               0xddae, 0xdfff, 0x00, 19, 19 },
  { Z80_BIT,  OP_BIT_INDEX_HL,        0xcb46, 0xffc7, 0x00, 12, 12 },
  { Z80_BIT,  OP_BIT_REG8,            0xcb40, 0xffc0, 0x00,  8,  8 },
  { Z80_BIT,  OP_BIT_INDEX,           0xddcb, 0xdfff, 0x00, 20, 23 },
  { Z80_NONE }
};

struct _table_z80 table_z80_4_byte[] =
{
  { Z80_RES, OP_BIT_INDEX_V2,   0x86, 0xc7, 0x00, 23, 23 },
  { Z80_RES, OP_BIT_INDEX_REG8, 0x80, 0xc0, 0x00, 23, 23 },
  { Z80_RL,  OP_BIT_INDEX_REG8, 0x10, 0xf8, 0x00, 23, 23 },
  { Z80_RLC, OP_BIT_INDEX_REG8, 0x00, 0xf8, 0x00, 23, 23 },
  { Z80_RR,  OP_BIT_INDEX_V2,   0x1e, 0xff, 0x00, 23, 23 },
  { Z80_RR,  OP_BIT_INDEX_REG8, 0x18, 0xf8, 0x00, 23, 23 },
  { Z80_RRC, OP_BIT_INDEX_REG8, 0x08, 0xf8, 0x00, 23, 23 },
  { Z80_SET, OP_BIT_INDEX_V2,   0xc6, 0xc7, 0x00, 23, 23 },
  { Z80_SET, OP_BIT_INDEX_REG8, 0xc0, 0xc0, 0x00, 23, 23 },
  { Z80_SLA, OP_BIT_INDEX_REG8, 0x20, 0xf8, 0x00, 23, 23 },
  { Z80_SRA, OP_BIT_INDEX_REG8, 0x28, 0xf8, 0x00, 23, 23 },
  { Z80_SLL, OP_BIT_INDEX_REG8, 0x30, 0xf8, 0x00, 23, 23 },
  { Z80_SRL, OP_BIT_INDEX_REG8, 0x38, 0xf8, 0x00, 23, 23 },
  { Z80_NONE }
};

