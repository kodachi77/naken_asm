/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2019 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "table/sh4.h"

struct _table_sh4 table_sh4[] =
{
  { "add",     0x300c, 0xf00f, OP_REG_REG, 0 },
  { "add",     0x7000, 0xf000, OP_IMM_REG, 0 },
  { "addc",    0x300e, 0xf00f, OP_REG_REG, 0 },
  { "addv",    0x300f, 0xf00f, OP_REG_REG, 0 },
  { "and",     0x2009, 0xf00f, OP_REG_REG, 0 },
  { "and",     0xc900, 0xff00, OP_IMM_R0, 0 },
  { "and.b",   0xcd00, 0xff00, OP_IMM_AT_R0_GBR, 0 },
  { "bf",      0x8b00, 0xff00, OP_BRANCH_S9, 0 },
  { "bf/s",    0x8f00, 0xff00, OP_BRANCH_S9, 0 },
  { "bra",     0xa000, 0xf000, OP_BRANCH_S13, 0 },
  { "braf",    0x0023, 0xf0ff, OP_REG, 0 },
  { "brk",     0x003b, 0xffff, OP_NONE, 0 },
  { "bsr",     0xb000, 0xf000, OP_BRANCH_S13, 0 },
  { "bsrf",    0x0003, 0xf0ff, OP_REG, 0 },
  { "bt",      0x8900, 0xff00, OP_BRANCH_S9, 0 },
  { "bt/s",    0x8d00, 0xff00, OP_BRANCH_S9, 0 },
  { "clrmac",  0x0028, 0xffff, OP_NONE, 0 },
  { "clrs",    0x0048, 0xffff, OP_NONE, 0 },
  { "clrt",    0x0008, 0xffff, OP_NONE, 0 },
  { "cmp/eq",  0x3000, 0xf00f, OP_REG_REG, 0 },
  { "cmp/eq",  0x8800, 0xff00, OP_IMM_R0, 0 },
  { "cmp/ge",  0x3003, 0xf00f, OP_REG_REG, 0 },
  { "cmp/gt",  0x3007, 0xf00f, OP_REG_REG, 0 },
  { "cmp/hi",  0x3006, 0xf00f, OP_REG_REG, 0 },
  { "cmp/hs",  0x3002, 0xf00f, OP_REG_REG, 0 },
  { "cmp/pl",  0x4015, 0xf0ff, OP_REG, 0 },
  { "cmp/pz",  0x4011, 0xf0ff, OP_REG, 0 },
  { "cmp/str", 0x200c, 0xf00f, OP_REG_REG, 0 },
  { "div0s",   0x2007, 0xf00f, OP_REG_REG, 0 },
  { "div0u",   0x0019, 0xffff, OP_NONE, 0 },
  { "div1",    0x3004, 0xf00f, OP_REG_REG, 0 },
  { "dmuls.l", 0x300d, 0xf00f, OP_REG_REG, 0 },
  { "dmulu.l", 0x3005, 0xf00f, OP_REG_REG, 0 },
  { "dt",      0x4010, 0xf0ff, OP_REG, 0 },
  { "exts.b",  0x600e, 0xf00f, OP_REG_REG, 0 },
  { "exts.w",  0x600f, 0xf00f, OP_REG_REG, 0 },
  { "extu.b",  0x600c, 0xf00f, OP_REG_REG, 0 },
  { "extu.w",  0x600d, 0xf00f, OP_REG_REG, 0 },
  { "fabs",    0xf05d, 0xf1ff, OP_DREG, 0 },
  { "fabs",    0xf05d, 0xf0ff, OP_FREG, 0 },
  { "fadd",    0xf000, 0xf11f, OP_DREG_DREG, 0 },
  { "fadd",    0xf000, 0xf00f, OP_FREG_FREG, 0 },
  { "fcmp/eq", 0xf004, 0xf00f, OP_FREG_FREG, 0 },
  { "fcmp/gt", 0xf005, 0xf00f, OP_DREG_DREG, 0 },
  { "fcnvds",  0xf0bd, 0xf1ff, OP_DREG_FPUL, SPECIAL_REG_FPUL },
  { "fcnvsd",  0xf0ad, 0xf1ff, OP_FPUL_DREG, 0 },
  { "fdiv",    0xf003, 0xf11f, OP_DREG_DREG, 0 },
  { "fdiv",    0xf003, 0xf00f, OP_FREG_FREG, 0 },
  { "fipr",    0xf0ed, 0xf0ff, OP_FVREG_FVREG, 0 },
  { "flds",    0xf01d, 0xf0ff, OP_FREG_FPUL, SPECIAL_REG_FPUL },
  { "fldi0",   0xf08d, 0xf0ff, OP_FREG, 0 },
  { "fldi1",   0xf09d, 0xf0ff, OP_FREG, 0 },
  { "float",   0xf02d, 0xf1ff, OP_FPUL_DREG, 0 },
  { "float",   0xf02d, 0xf0ff, OP_FPUL_FREG, 0 },
  { "fmac",    0xf00e, 0xf00f, OP_FR0_FREG_FREG, 0 },
  { "fmov",    0xf00c, 0xf11f, OP_DREG_DREG, 0 },
  { "fmov",    0xf10c, 0xf11f, OP_DREG_XDREG, 0 },
  { "fmov",    0xf00a, 0xf01f, OP_DREG_AT_REG, 0 },
  { "fmov",    0xf00b, 0xf01f, OP_DREG_AT_MINUS_REG, 0 },
  { "fmov",    0xf007, 0xf01f, OP_DREG_AT_R0_REG, 0 },
  { "fmov.s",  0xf00c, 0xf00f, OP_FREG_FREG, 0 },
  { "fmov.s",  0xf00a, 0xf00f, OP_FREG_AT_REG, 0 },
  { "fmov.s",  0xf00b, 0xf00f, OP_FREG_AT_MINUS_REG, 0 },
  { "fmov.s",  0xf007, 0xf00f, OP_FREG_AT_R0_REG, 0 },
  { "fmov",    0xf01c, 0xf11f, OP_XDREG_DREG, 0 },
  { "fmov",    0xf11c, 0xf11f, OP_XDREG_XDREG, 0 },
  { "fmov",    0xf01a, 0xf01f, OP_XDREG_AT_REG, 0 },
  { "fmov",    0xf01b, 0xf01f, OP_XDREG_AT_MINUS_REG, 0 },
  { "fmov",    0xf017, 0xf01f, OP_XDREG_AT_R0_REG, 0 },
  { "fmov",    0xf008, 0xf10f, OP_AT_REG_DREG, 0 },
  { "fmov",    0xf009, 0xf10f, OP_AT_REG_PLUS_DREG, 0 },
  { "fmov",    0xf006, 0xf10f, OP_AT_R0_REG_DREG, 0 },
  { "fmov.s",  0xf008, 0xf00f, OP_AT_REG_FREG, 0 },
  { "fmov.s",  0xf009, 0xf00f, OP_AT_REG_PLUS_FREG, 0 },
  { "fmov.s",  0xf006, 0xf00f, OP_AT_R0_REG_FREG, 0 },
  { "fmov",    0xf108, 0xf10f, OP_AT_REG_XDREG, 0 },
  { "fmov",    0xf109, 0xf10f, OP_AT_REG_PLUS_XDREG, 0 },
  { "fmov",    0xf106, 0xf10f, OP_AT_R0_REG_XDREG, 0 },
  { "fmul",    0xf002, 0xf11f, OP_DREG_DREG, 0 },
  { "fmul",    0xf002, 0xf00f, OP_FREG_FREG, 0 },
  { "fneg",    0xf04d, 0xf1ff, OP_DREG, 0 },
  { "fneg",    0xf04d, 0xf0ff, OP_FREG, 0 },
  { "frchg",   0xfbfd, 0xffff, OP_NONE, 0 },
  { "fschg",   0xf3fd, 0xffff, OP_NONE, 0 },
  { "fsqrt",   0xf04d, 0xf1ff, OP_DREG, 0 },
  { "fsqrt",   0xf06d, 0xf0ff, OP_FREG, 0 },
  { "fsts",    0xf00d, 0xf0ff, OP_FPUL_FREG, 0 },
  { "fsub",    0xf001, 0xf11f, OP_DREG_DREG, 0 },
  { "fsub",    0xf001, 0xf00f, OP_FREG_FREG, 0 },
  { "ftrc",    0xf03d, 0xf1ff, OP_DREG_FPUL, SPECIAL_REG_FPUL },
  { "ftrc",    0xf03d, 0xf0ff, OP_FREG_FPUL, SPECIAL_REG_FPUL },
  { "ftrv",    0xf1fd, 0xf3ff, OP_XMTRX_FVREG, 0 },
  { "jmp",     0x402b, 0xf0ff, OP_AT_REG, 0 },
  { "jsr",     0x400b, 0xf0ff, OP_AT_REG, 0 },
  { "ldc",     0x401e, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_GBR },
  { "ldc",     0x400e, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_SR },
  { "ldc",     0x402e, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_VBR },
  { "ldc",     0x403e, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_SSR },
  { "ldc",     0x404e, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_SPC },
  { "ldc",     0x40fa, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_DBR },
  { "ldc",     0x408e, 0xf08f, OP_REG_REG_BANK, 0 },
  { "ldc.l",   0x4017, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_GBR },
  { "ldc.l",   0x4007, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_SR },
  { "ldc.l",   0x4027, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_VBR },
  { "ldc.l",   0x4037, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_SSR },
  { "ldc.l",   0x4047, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_SPC },
  { "ldc.l",   0x40f6, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_DBR },
  { "ldc.l",   0x4087, 0xf08f, OP_AT_REG_PLUS_REG_BANK, 0 },
  { "ldc",     0x406a, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_FPSCR },
  { "ldc.l",   0x4066, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_FPSCR },
  { "ldc",     0x405a, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_FPUL },
  { "ldc.l",   0x4056, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_FPUL },
  { "ldc",     0x400a, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_MACH },
  { "ldc.l",   0x4006, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_MACH },
  { "ldc",     0x401a, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_MACL },
  { "ldc.l",   0x4016, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_MACL },
  { "ldc",     0x402a, 0xf0ff, OP_REG_SPECIAL, SPECIAL_REG_PR },
  { "ldc.l",   0x4026, 0xf0ff, OP_AT_REG_PLUS_SPECIAL, SPECIAL_REG_PR },
  { "ldtlb",   0x0038, 0xffff, OP_NONE, 0 },
  { "mac.l",   0x000f, 0xf00f, OP_AT_REG_PLUS_AT_REG_PLUS, 0 },
  { "mac.w",   0x400f, 0xf00f, OP_AT_REG_PLUS_AT_REG_PLUS, 0 },
  { "mov",     0x6003, 0xf00f, OP_REG_REG, 0 },
  { "mov",     0xe000, 0xf000, OP_IMM_REG, 0 },
  { "mov.b",   0x2000, 0xf00f, OP_REG_AT_REG, 0 },
  { "mov.b",   0x2004, 0xf00f, OP_REG_AT_MINUS_REG, 0 },
  { "mov.b",   0x0004, 0xf00f, OP_REG_AT_R0_REG, 0 },
  { "mov.b",   0xc000, 0xff00, OP_R0_AT_DISP_GBR, 1 },
  { "mov.b",   0x8000, 0xff00, OP_R0_AT_DISP_REG, 1 },
  { "mov.b",   0x6000, 0xf00f, OP_AT_REG_REG, 0 },
  { "mov.b",   0x6004, 0xf00f, OP_AT_REG_PLUS_REG, 0 },
  { "mov.b",   0x000c, 0xf00f, OP_AT_R0_REG_REG, 0 },
  { "mov.b",   0xc400, 0xff00, OP_AT_DISP_GBR_R0, 1 },
  { "mov.b",   0x8400, 0xff00, OP_AT_DISP_REG_R0, 1 },
  { "mov.l",   0x2002, 0xf00f, OP_REG_AT_REG, 0 },
  { "mov.l",   0x2006, 0xf00f, OP_REG_AT_MINUS_REG, 0 },
  { "mov.l",   0x0006, 0xf00f, OP_REG_AT_R0_REG, 0 },
  { "mov.l",   0xc200, 0xff00, OP_R0_AT_DISP_GBR, 4 },
  { "mov.l",   0x1000, 0xff00, OP_R0_AT_DISP_REG, 4 },
  { "mov.l",   0x6002, 0xf00f, OP_AT_REG_REG, 0 },
  { "mov.l",   0x6006, 0xf00f, OP_AT_REG_PLUS_REG, 0 },
  { "mov.l",   0x000e, 0xf00f, OP_AT_R0_REG_REG, 0 },
  { "mov.l",   0xc600, 0xff00, OP_AT_DISP_GBR_R0, 4 },
  { "mov.l",   0xd000, 0xf000, OP_AT_DISP_PC_REG, 4 },
  { "mov.l",   0x5000, 0xf000, OP_AT_DISP_REG_REG, 4 },
  { NULL,      0x0000, 0x0000, 0, 0 },
};

struct _operand_type_sh4 operand_type_sh4[] =
{
  { 0,               0, 0,                0,  0 }, // OP_NONE
  { 1,         SH4_REG, 8,                0,  0 }, // OP_REG
  { 1,        SH4_FREG, 8,                0,  0 }, // OP_FREG
  { 1,        SH4_DREG, 9,                0,  0 }, // OP_DREG
  { 2,         SH4_REG, 4,          SH4_REG,  8 }, // OP_REG_REG
  { 2,        SH4_FREG, 4,         SH4_FREG,  8 }, // OP_FREG_FREG
  { 2,        SH4_DREG, 5,         SH4_DREG,  9 }, // OP_DREG_DREG
  { 2,        SH4_DREG, 5,        SH4_XDREG,  9 }, // OP_DREG_XDREG
  { 2,       SH4_XDREG, 5,         SH4_DREG,  9 }, // OP_XDREG_DREG
  { 2,       SH4_XDREG, 5,        SH4_XDREG,  9 }, // OP_XDREG_XDREG
  { 2,       SH4_FVREG, 8,        SH4_FVREG, 10 }, // OP_FVREG_FVREG
  { 2,               0, 0,                0,  0 }, // OP_IMM_REG
  { 2,               0, 0,                0,  0 }, // OP_IMM_R0
  { 2,               0, 0,                0,  0 }, // OP_IMM_AT_R0_GBR
  { 1,               0, 0,                0,  0 }, // OP_BRANCH_S9
  { 1,               0, 0,                0,  0 }, // OP_BRANCH_S13
  { 2,               0, 0,                0,  0 }, // OP_FREG_FPUL
  { 2,               0, 0,                0,  0 }, // OP_DREG_FPUL
  { 2,               0, 0,                0,  0 }, // OP_FPUL_FREG
  { 2,               0, 0,                0,  0 }, // OP_FPUL_DREG
  { 3,               0, 0,                0,  0 }, // OP_FR0_FREG_FREG
  { 2,        SH4_FREG, 4,       SH4_AT_REG,  8 }, // OP_FREG_AT_REG
  { 2,        SH4_DREG, 5,       SH4_AT_REG,  8 }, // OP_DREG_AT_REG
  { 2,        SH4_FREG, 4, SH4_AT_MINUS_REG,  8 }, // OP_FREG_AT_MINUS_REG
  { 2,        SH4_DREG, 5, SH4_AT_MINUS_REG,  8 }, // OP_DREG_AT_MINUS_REG
  { 2,        SH4_FREG, 4,    SH4_AT_R0_REG,  8 }, // OP_FREG_AT_R0_REG
  { 2,        SH4_DREG, 5,    SH4_AT_R0_REG,  8 }, // OP_DREG_AT_R0_REG
  { 2,       SH4_XDREG, 5,       SH4_AT_REG,  8 }, // OP_XDREG_AT_REG
  { 2,       SH4_XDREG, 5, SH4_AT_MINUS_REG,  8 }, // OP_XDREG_AT_MINUS_REG
  { 2,       SH4_XDREG, 5,    SH4_AT_R0_REG,  8 }, // OP_XDREG_AT_R0_REG
  { 2,      SH4_AT_REG, 4,         SH4_DREG,  9 }, // OP_AT_REG_DREG
  { 2, SH4_AT_REG_PLUS, 4,         SH4_DREG,  9 }, // OP_AT_REG_PLUS_DREG
  { 2,   SH4_AT_R0_REG, 4,         SH4_DREG,  9 }, // OP_AT_R0_REG_DREG
  { 2,      SH4_AT_REG, 4,         SH4_FREG,  8 }, // OP_AT_REG_FREG
  { 2, SH4_AT_REG_PLUS, 4,         SH4_FREG,  8 }, // OP_AT_REG_PLUS_FREG
  { 2,   SH4_AT_R0_REG, 4,         SH4_FREG,  8 }, // OP_AT_R0_REG_FREG
  { 2,      SH4_AT_REG, 4,        SH4_XDREG,  9 }, // OP_AT_REG_XDREG
  { 2, SH4_AT_REG_PLUS, 4,        SH4_XDREG,  9 }, // OP_AT_REG_PLUS_XDREG
  { 2,   SH4_AT_R0_REG, 4,        SH4_XDREG,  9 }, // OP_AT_R0_REG_XDREG
  { 2,               0, 0,        SH4_FVREG, 10 }, // OP_XMTRX_FVREG
  { 1,      SH4_AT_REG, 8,                0,  0 }, // OP_AT_REG
  { 2,         SH4_REG, 8,      SH4_SPECIAL,  0 }, // OP_REG_SPECIAL
  { 2,         SH4_REG, 8,                0,  0 }, // OP_REG_REG_BANK
  { 2, SH4_AT_REG_PLUS, 8,      SH4_SPECIAL,  0 }, // OP_AT_REG_PLUS_SPECIAL
  { 2, SH4_AT_REG_PLUS, 8,                0,  0 }, // OP_AT_REG_PLUS_REG_BANK
  { 2, SH4_AT_REG_PLUS, 4,  SH4_AT_REG_PLUS,  8 }, // OP_AT_REG_PLUS_AT_REG_PLUS
  { 2,         SH4_REG, 4,       SH4_AT_REG,  8 }, // OP_REG_AT_REG
  { 2,         SH4_REG, 4, SH4_AT_MINUS_REG,  8 }, // OP_REG_AT_MINUS_REG
  { 2,         SH4_REG, 4,    SH4_AT_R0_REG,  8 }, // OP_REG_AT_R0_REG
  { 2,                  0, 4,             0,  8 }, // OP_R0_AT_DISP_GBR
  { 2,                  0, 4,             0,  8 }, // OP_R0_AT_DISP_REG
  { 2,         SH4_AT_REG, 4,       SH4_REG,  8 }, // OP_AT_REG_REG
  { 2,    SH4_AT_REG_PLUS, 4,       SH4_REG,  8 }, // OP_AT_REG_PLUS_REG
  { 2,      SH4_AT_R0_REG, 4,       SH4_REG,  8 }, // OP_AT_R0_REG_REG
  { 2,                  0, 0,             0,  0 }, // OP_AT_DISP_GBR_R0
  { 2,                  0, 0,             0,  0 }, // OP_AT_DISP_REG_R0
  { 2,                  0, 0,             0,  0 }, // OP_AT_DISP_PC_REG
  { 2,                  0, 0,             0,  0 }, // OP_AT_DISP_REG_REG
};

char *sh4_specials[] =
{
  "???",
  "FPUL",
  "GBR",
  "SR",
  "VBR",
  "SSR",
  "SPC",
  "DBR",
  "FPSCR",
  "MACL",
  "MACH",
  "PR",
  "XMTRX",
  NULL,
};
