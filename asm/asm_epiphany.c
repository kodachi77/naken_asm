/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "asm_common.h"
#include "asm_epiphany.h"
#include "assembler.h"
#include "disasm_epiphany.h"
#include "tokens.h"
#include "eval_expression.h"
#include "table_epiphany.h"

enum
{
  OPERAND_NONE,
  OPERAND_REG,
  OPERAND_NUMBER,
  OPERAND_ADDRESS,
  OPERAND_INDEX_REG,
  OPERAND_INDEX_REG_IMM,
  OPERAND_INDEX_REG_REG,
};

struct _operand
{
  uint8_t type;
  uint8_t reg;
  int value;
  uint8_t use_32_bit_instruction;
  uint8_t reg_is_negative;
};

static int get_register(const char *token)
{
  if (token[0] == 'r' || token[1] == 'R')
  {
    const char *s = token + 1;
    int n = 0, count = 0;

    while(*s != 0)
    {
      if (*s < '0' || *s > '9') { return -1; }
      n = (n * 10) + (*s - '0');
      count++;

      // Disallow leading 0's on registers
      if (n == 0 && count >1) { return -1; }

      s++;
    }

    // This token was just r or R.
    if (count == 0) { return -1; }

    return n;
  }

  // A1-A4 = R0-R3
  if (token[0] == 'a' || token[1] == 'A')
  {
    if (token[2] == 0 && token[1] >= '1' && token[1] <= '4')
    {
      return token[1] - '1';
    }
  }

  // V1-V8 = R4-R11
  if (token[0] == 'v' || token[1] == 'V')
  {
    if (token[2] == 0 && token[1] >= '1' && token[1] <= '8')
    {
      return (token[1] - '1') + 4;
    }
  }

  if (strcasecmp(token, "sb") == 0) { return 9; }
  if (strcasecmp(token, "sl") == 0) { return 10; }
  if (strcasecmp(token, "fp") == 0) { return 11; }
  if (strcasecmp(token, "sp") == 0) { return 13; }
  if (strcasecmp(token, "lr") == 0) { return 14; }

  return -1;
}

int parse_instruction_epiphany(struct _asm_context *asm_context, char *instr)
{
  char instr_case[TOKENLEN];
  char token[TOKENLEN];
  int token_type;
  struct _operand operands[3];
  int operand_count;
  int offset;
  uint32_t reg_combo;
  uint32_t value;
  int n;

  lower_copy(instr_case, instr);
  operand_count = 0;
  memset(operands, 0, sizeof(operands));

  // Parse operands
  while(1)
  {
    token_type = tokens_get(asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOL || token_type == TOKEN_EOF)
    {
      break;
    }

    // if (IS_TOKEN(token,';')) { break; }

    if (operand_count != 0)
    {
      if (IS_NOT_TOKEN(token,','))
      {
        print_error_unexp(token, asm_context);
        return -1;
      }

      token_type = tokens_get(asm_context, token, TOKENLEN);
    }

    if (operand_count == 3)
    {
      print_error_unexp(token, asm_context);
      return -1;
    }

    n = get_register(token);

    if (n != -1)
    {
      operands[operand_count].type = OPERAND_REG;
      operands[operand_count].reg = n;
    }
      else
    if (IS_TOKEN(token,'#'))
    {
      if (eval_expression(asm_context, &n) != 0)
      {
        if (asm_context->pass == 1)
        {
          eat_operand(asm_context);
          memory_write(asm_context, asm_context->address, 1, asm_context->line);
          operands[operand_count].use_32_bit_instruction = 1;
        }
          else
        {
          print_error_illegal_expression(instr, asm_context);
          return -1;
        }

        if (memory_read(asm_context, asm_context->address) != 0)
        {
          operands[operand_count].use_32_bit_instruction = 1;
        }

        operands[operand_count].type = OPERAND_NUMBER;
        operands[operand_count].value = n;
      }
    }
      else
    if (IS_TOKEN(token,'['))
    {
      token_type = tokens_get(asm_context, token, TOKENLEN);

      n = get_register(token);

      if (n == -1)
      {
        print_error_unexp(token, asm_context);
        return -1;
      }

      operands[operand_count].type = OPERAND_INDEX_REG;
      operands[operand_count].reg = n;

      token_type = tokens_get(asm_context, token, TOKENLEN);
      if (IS_TOKEN(token,','))
      {
        token_type = tokens_get(asm_context, token, TOKENLEN);
        if (IS_TOKEN(token,'#'))
        {
          if (eval_expression(asm_context, &n) != 0)
          {
            if (asm_context->pass == 1)
            {
              eat_operand(asm_context);
              memory_write(asm_context, asm_context->address, 1, asm_context->line);
              operands[operand_count].use_32_bit_instruction = 1;
            }
              else
            {
              print_error_illegal_expression(instr, asm_context);
              return -1;
            }
          }

          if (memory_read(asm_context, asm_context->address) != 0)
          {
            operands[operand_count].use_32_bit_instruction = 1;
          }

          operands[operand_count].type = OPERAND_INDEX_REG_IMM;
          operands[operand_count].value = n;

          token_type = tokens_get(asm_context, token, TOKENLEN);

          if (IS_NOT_TOKEN(token,']'))
          {
            print_error_unexp(token, asm_context);
            return -1;
          }
        }
          else
        if (IS_TOKEN(token,'-'))
        {
          token_type = tokens_get(asm_context, token, TOKENLEN);

          n = get_register(token);

          if (n == -1)
          {
            print_error_unexp(token, asm_context);
            return -1;
          }

          operands[operand_count].type = OPERAND_INDEX_REG_REG;
          operands[operand_count].value = n;
          operands[operand_count].reg_is_negative = 1;
        }
          else
        {
          token_type = tokens_get(asm_context, token, TOKENLEN);

          n = get_register(token);

          if (n == -1)
          {
            print_error_unexp(token, asm_context);
            return -1;
          }

          operands[operand_count].type = OPERAND_INDEX_REG_REG;
          operands[operand_count].value = n;
        }
      }
        else
      if (IS_NOT_TOKEN(token,']'))
      {
        print_error_unexp(token, asm_context);
        return -1;
      }
    }
      else
    {
      tokens_push(asm_context, token, token_type);

      if (eval_expression(asm_context, &n) != 0)
      {
        if (asm_context->pass == 1)
        {
          eat_operand(asm_context);
        }
          else
        {
          print_error_illegal_expression(instr, asm_context);
          return -1;
        }
      }

      operands[operand_count].type = OPERAND_ADDRESS;
      operands[operand_count].value = n;
    }

    operand_count++;
  }

  // Get opcodes
  n = 0;
  while(table_epiphany[n].instr != NULL)
  {
    if (strcmp(table_epiphany[n].instr, instr_case) == 0)
    {
      switch(table_epiphany[n].type)
      {
        case OP_NONE:
        {
          // Should never get in here.
          break;
        }
        case OP_BRANCH_16:
        {
          if (operand_count == 1 && operands[0].type == OPERAND_ADDRESS)
          {
            offset = operands[0].value - (asm_context->address + 2);

            if ((offset & 1) != 0)
            {
              print_error("Address not on an odd boundary", asm_context);
              return -1;
            }

            if (offset >= -256 && offset <= 255)
            {
              add_bin16(asm_context, table_epiphany[n].opcode | (((uint16_t)offset) << 8), IS_OPCODE);
              return 2;
            }
          }
          break;
        }
        case OP_BRANCH_32:
        {
          if (operand_count == 1 && operands[0].type == OPERAND_ADDRESS)
          {
            offset = operands[0].value - (asm_context->address + 4);

            if ((offset & 1) != 0)
            {
              print_error("Address not on an odd boundary", asm_context);
              return -1;
            }

            if (check_range(asm_context, "Offset", offset, -0x800000, 0x7fffff) == -1)
            {
              return -1;
            }

            add_bin32(asm_context, table_epiphany[n].opcode | (((uint32_t)offset) << 8), IS_OPCODE);

            return 4;
          }
          break;
        }
        case OP_DISP_IMM3_16:
        {
          if (operand_count == 2 &&
              operands[0].type == OPERAND_REG &&
              operands[1].type == OPERAND_INDEX_REG_IMM)
          {
            if (operands[1].use_32_bit_instruction == 1) { break; }
            if (operands[1].value < 0 || operands[1].value > 7) { break; }
            if (operands[0].reg > 7) { break; }
            if (operands[1].reg > 7) { break; }

            reg_combo = (operands[0].reg << 13) |
                        (operands[1].reg << 10);

            add_bin16(asm_context, table_epiphany[n].opcode | reg_combo | (operands[1].value << 7), IS_OPCODE);
            return 2;
          }
          break;
        }
        case OP_DISP_IMM11_32:
        {
          if (operand_count == 2 &&
              operands[0].type == OPERAND_REG &&
              operands[1].type == OPERAND_INDEX_REG_IMM)
          {
            if (check_range(asm_context, "Immediate", operands[1].value, -2047, 2047) == -1) { return -1; }

            reg_combo = ((operands[0].reg & 0x7) << 13) |
                        ((operands[1].reg & 0x7) << 10) |
                        ((operands[0].reg >> 3) << 29) |
                        ((operands[1].reg >> 3) << 26);

            if (operands[1].value < 0)
            {
              operands[1].value = -operands[1].value;
              value = 0x01000000;
            }
              else
            {
              value = 0;
            }

            value |= ((operands[1].value & 0x7) << 7) |
                     (((operands[1].value >> 3) & 0xff) << 16);

            add_bin32(asm_context, table_epiphany[n].opcode | reg_combo | value, IS_OPCODE);
            return 4;
          }
          break;
        }
        case OP_INDEX_16:
        {
          break;
        }
        case OP_INDEX_32:
        {
          break;
        }
        case OP_POST_MOD_16:
        {
          break;
        }
        case OP_POST_MOD_DISP_32:
        {
          break;
        }
        case OP_REG_IMM_16:
        {
          break;
        }
        case OP_REG_IMM_32:
        {
          break;
        }
        case OP_REG_2_IMM_16:
        {
          break;
        }
        case OP_REG_2_IMM_32:
        {
          break;
        }
        case OP_REG_2_IMM5_16:
        {
          break;
        }
        case OP_REG_2_IMM5_32:
        {
          break;
        }
        case OP_REG_3_16:
        {
          if (operand_count == 3 &&
              operands[0].type == OPERAND_REG && operands[0].value < 8 &&
              operands[1].type == OPERAND_REG && operands[1].value < 8 &&
              operands[2].type == OPERAND_REG && operands[2].value < 8)
          {
            reg_combo = (operands[0].reg << 13) |
                        (operands[1].reg << 10) |
                        (operands[2].reg << 7);
            add_bin16(asm_context, table_epiphany[n].opcode | reg_combo, IS_OPCODE);
            return 2;
          }
          break;
        }
        case OP_REG_3_32:
        {
          if (operand_count == 3 &&
              operands[0].type == OPERAND_REG &&
              operands[1].type == OPERAND_REG &&
              operands[2].type == OPERAND_REG)
          {
            reg_combo = ((operands[0].reg & 0x7) << 13) |
                        ((operands[1].reg & 0x7) << 10) |
                        ((operands[2].reg & 0x7) << 7) |
                        ((operands[0].reg >> 3) << 29) |
                        ((operands[1].reg >> 3) << 26) |
                        ((operands[2].reg >> 3) << 23);
            add_bin32(asm_context, table_epiphany[n].opcode | reg_combo, IS_OPCODE);
            return 4;
          }
          break;
        }
        case OP_REG_2_16:
        {
          if (operand_count == 2 &&
              operands[0].type == OPERAND_REG && operands[0].value < 8 &&
              operands[1].type == OPERAND_REG && operands[1].value < 8)
          {
            reg_combo = (operands[0].reg << 13) |
                        (operands[1].reg << 10);
            add_bin16(asm_context, table_epiphany[n].opcode | reg_combo, IS_OPCODE);
            return 2;
          }
          break;
        }
        case OP_REG_2_32:
        {
          if (operand_count == 2 &&
              operands[0].type == OPERAND_REG &&
              operands[1].type == OPERAND_REG)
          {
            reg_combo = ((operands[0].reg & 0x7) << 13) |
                        ((operands[1].reg & 0x7) << 10) |
                        ((operands[0].reg >> 3) << 29) |
                        ((operands[1].reg >> 3) << 26);
            add_bin32(asm_context, table_epiphany[n].opcode | reg_combo, IS_OPCODE);
            return 4;
          }
          break;
        }
        case OP_REG_1_16:
        {
          break;
        }
        case OP_REG_1_32:
        {
          break;
        }
        case OP_NUM_16:
        {
          if (operand_count == 1)
          {
            //add_bin16(asm_context, table_epiphany[n].opcode, IS_OPCODE);
            return 2;
          }
          break;
        }
        case OP_NONE_16:
        {
          if (operand_count == 0)
          {
            add_bin16(asm_context, table_epiphany[n].opcode, IS_OPCODE);
            return 2;
          }
          break;
        }
        case OP_NONE_32:
        {
          if (operand_count == 0)
          {
            add_bin32(asm_context, table_epiphany[n].opcode, IS_OPCODE);
            return 4;
          }
          break;
        }
        default:
          printf("Internal error %s:%d\n", __FILE__, __LINE__);
          return -1;
          break;
      }
    }

    n++;
  }

  print_error_unknown_operand_combo(instr, asm_context);

  return -1;
}

