/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2016 by Michael Kohn
 *
 */

#include "table/cell.h"

struct _table_cell table_cell[] =
{
  // Load / Store
  { "lqd",    0x34000000, 0xff000000, OP_RT_S10_RA,    FLAG_NONE, 0, 0 },
  { "lqx",    0x38800000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "lqa",    0x30800000, 0xff800000, OP_RT_ADDRESS,   FLAG_NONE, 0, 0 },
  { "lqr",    0x33800000, 0xff800000, OP_RT_RELATIVE,  FLAG_NONE, 0, 0 },
  { "stqd",   0x24000000, 0xff000000, OP_RT_S10_RA,    FLAG_NONE, 0, 0 },
  { "stqx",   0x28800000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "stqa",   0x20800000, 0xff800000, OP_RT_ADDRESS,   FLAG_NONE, 0, 0 },
  { "stqr",   0x23800000, 0xff800000, OP_RT_RELATIVE,  FLAG_NONE, 0, 0 },
  { "cbd",    0x3e800000, 0xffe00000, OP_RT_S7_RA,     FLAG_NONE, 0, 0 },
  { "cbx",    0x3a800000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "chd",    0x3ea00000, 0xffe00000, OP_RT_S7_RA,     FLAG_NONE, 0, 0 },
  { "chx",    0x3aa00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "cwd",    0x3ec00000, 0xffe00000, OP_RT_S7_RA,     FLAG_NONE, 0, 0 },
  { "cwx",    0x3ac00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "cdd",    0x3ee00000, 0xffe00000, OP_RT_S7_RA,     FLAG_NONE, 0, 0 },
  { "cdx",    0x3ae00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  // Constant formation instructions
  { "ilh",    0x41800000, 0xff800000, OP_RT_U16,       FLAG_NONE, 0, 0 },
  { "ilhu",   0x41000000, 0xff800000, OP_RT_U16,       FLAG_NONE, 0, 0 },
  { "il",     0x40800000, 0xff800000, OP_RT_U16,       FLAG_NONE, 0, 0 },
  { "ila",    0x42000000, 0xfe000000, OP_RT_U18,       FLAG_NONE, 0, 0 },
  { "iohl",   0x60800000, 0xff800000, OP_RT_U16,       FLAG_NONE, 0, 0 },
  // Integer and logical instructions
  { "ah",     0x19000000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "ahi",    0x1d000000, 0xff000000, OP_RT_RA_S10,    FLAG_NONE, 0, 0 },
  { "a",      0x18000000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "ai",     0x1c000000, 0xff000000, OP_RT_RA_S10,    FLAG_NONE, 0, 0 },
  { "sfh",    0x09000000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "sfhi",   0x0d000000, 0xff000000, OP_RT_RA_S10,    FLAG_NONE, 0, 0 },
  { "sf",     0x08000000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "sfi",    0x0c000000, 0xff000000, OP_RT_RA_S10,    FLAG_NONE, 0, 0 },
  { "addx",   0x68000000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "cg",     0x18400000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "cgx",    0x68400000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "sfx",    0x68200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "bg",     0x08400000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "bgx",    0x68600000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpy",    0x78800000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpyu",   0x79800000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpyi",   0x74000000, 0xff000000, OP_RT_RA_S10,    FLAG_NONE, 0, 0 },
  { "mpyui",  0x75000000, 0xff000000, OP_RT_RA_S10,    FLAG_NONE, 0, 0 },
  { "mpya",   0xc0000000, 0xf0000000, OP_RT_RA_RB_RC,  FLAG_NONE, 0, 0 },
  { "mpyh",   0x78a00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpys",   0x78e00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpyhh",  0x78c00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpyhha", 0x68c00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpyhhu", 0x79c00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "mpyhhua",0x69c00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "clz",    0x54a00000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "cntb",   0x56800000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "fsmb",   0x36c00000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "fsmh",   0x36a00000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "fsm",    0x36800000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "fsmbi",  0x32800000, 0xff800000, OP_RT_U16,       FLAG_NONE, 0, 0 },
  { "gbb",    0x36400000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "gbh",    0x36200000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "gb",     0x36000000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "avgb",   0x1a600000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "absdb",  0x0a600000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "sumb",   0x4a600000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "xsbh",   0x56c00000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "xshw",   0x55c00000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "xswd",   0x54c00000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "and",    0x18200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "andc",   0x58200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "andbi",  0x16000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "andhi",  0x15000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "andi",   0x14000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "or",     0x08200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "orc",    0x59200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "orbi",   0x06000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "orhi",   0x05000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "ori",    0x04000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "orx",    0x3e000000, 0xffe00000, OP_RT_RA,        FLAG_NONE, 0, 0 },
  { "xor",    0x48200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "xorbi",  0x46000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "xorhi",  0x45000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "xori",   0x44000000, 0xff000000, OP_RT_RA_U10,    FLAG_NONE, 0, 0 },
  { "nand",   0x19200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "nor",    0x09200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "eqv",    0x49200000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "selb",   0x80000000, 0xf0000000, OP_RT_RA_RB_RC,  FLAG_NONE, 0, 0 },
  { "shufb",  0xb0000000, 0xf0000000, OP_RT_RA_RB_RC,  FLAG_NONE, 0, 0 },
  // Shift and rotate instructions
  { "shlh",   0x0be00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "shlhi",  0x0fe00000, 0xffe00000, OP_RT_RA_U7,     FLAG_NONE, 0, 0 },
  { "shl",    0x0bb00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "shli",   0x0f600000, 0xffe00000, OP_RT_RA_U7,     FLAG_NONE, 0, 0 },
  { "shlqbi", 0x3bb00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "shlqbii",0x3fb00000, 0xffe00000, OP_RT_RA_U7,     FLAG_NONE, 0, 0 },
  { "shlqby", 0x3be00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },
  { "shlqbyi",0x3fe00000, 0xffe00000, OP_RT_RA_U7,     FLAG_NONE, 0, 0 },
  { "shlqbybi",0x39e00000, 0xffe00000, OP_RT_RA_RB,     FLAG_NONE, 0, 0 },

  { NULL, 0, 0, 0, 0 }
};

