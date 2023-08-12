/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/assembler.h"
#include "common/Memory.h"

Memory::Memory() :
  pages        (NULL),
  low_address  (0xffffffff),
  high_address (0),
  entry_point  (0xfffffff),
  endian       (ENDIAN_LITTLE),
  size         (~((uint32_t)0))
{
}

Memory::~Memory()
{
  MemoryPage *page = pages;

  while (page != NULL)
  {
    MemoryPage *next = page->next;
    delete page;
    page = next;
  }

  pages = NULL;
}

void memory_clear(Memory *memory)
{
  MemoryPage *page;
  MemoryPage *next;

  page = memory->pages;
  while (page != NULL)
  {
    next = page->next;
    memset(page->bin, 0, PAGE_SIZE);
    page->offset_min = PAGE_SIZE;
    page->offset_max = 0;
    page = next;
  }
}

int memory_in_use(Memory *memory, uint32_t address)
{
  MemoryPage *page;

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address && address < page->address + PAGE_SIZE)
    {
      return 1;
    }

    page = page->next;
  }

  return 0;
}

int memory_get_page_address_min(Memory *memory, uint32_t address)
{
  MemoryPage *page;

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address && address < page->address + PAGE_SIZE)
    {
      return page->address + page->offset_min;
    }

    page = page->next;
  }

  print_error_internal(NULL, __FILE__, __LINE__);

  return 0;
}

int memory_get_page_address_max(Memory *memory, uint32_t address)
{
  MemoryPage *page;

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address && address < page->address + PAGE_SIZE)
    {
      return page->address + page->offset_max;
    }

    page = page->next;
  }

  print_error_internal(NULL, __FILE__, __LINE__);

  return 0;
}

int memory_page_size(Memory *memory)
{
  return PAGE_SIZE;
}

static uint8_t read_byte(Memory *memory, uint32_t address)
{
  MemoryPage *page;

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address && address < page->address + PAGE_SIZE)
    {
      return page->bin[address-page->address];
    }
    page = page->next;
  }

  return 0;
}

static int read_debug(Memory *memory, uint32_t address)
{
  MemoryPage *page;

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address && address < page->address + PAGE_SIZE)
    {
      return page->debug_line[address-page->address];
    }
    page = page->next;
  }

  return -1;
}

static void write_byte(Memory *memory, uint32_t address, uint8_t data)
{
  MemoryPage *page;

  if (memory->pages == NULL)
  {
    memory->pages = new MemoryPage(address);
  }

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address &&
        address < (uint64_t)page->address + PAGE_SIZE)
    {
      break;
    }

    if (page->next == NULL)
    {
      page->next = new MemoryPage(address);
    }

    page = page->next;
  }

  if (memory->low_address > address) memory->low_address = address;
  if (memory->high_address < address) memory->high_address = address;

  page->set_data(address, data);
}

static void write_debug(Memory *memory, uint32_t address, int data)
{
  MemoryPage *page;

  if (memory->pages == NULL)
  {
    memory->pages = new MemoryPage(address);
  }

  page = memory->pages;

  while (page != NULL)
  {
    if (address >= page->address && address < page->address + PAGE_SIZE)
    {
      break;
    }

    if (page->next == NULL)
    {
      page->next = new MemoryPage(address);
    }

    page = page->next;
  }

  page->set_debug(address, data);
}

uint8_t memory_read(AsmContext *asm_context, uint32_t address)
{
  if (address >= asm_context->memory.size)
  {
    printf("Warning: Data read address %d overran %d byte boundary at %s:%d\n",
      address,
      asm_context->memory.size,
      asm_context->tokens.filename,
      asm_context->tokens.line);

    return 0;
  }

  return read_byte(&asm_context->memory, address);
}

uint8_t memory_read_inc(AsmContext *asm_context)
{
  return read_byte(&asm_context->memory, asm_context->address++);
}

void memory_write(
  AsmContext *asm_context,
  uint32_t address,
  uint8_t data,
  int line)
{
  if (address >= asm_context->memory.size)
  {
    printf("Warning: Data write address %d overran %d byte boundary at %s:%d\n",
      address,
      asm_context->memory.size,
      asm_context->tokens.filename,
      asm_context->tokens.line);

    return;
  }

  write_byte(&asm_context->memory, address, data);
  write_debug(&asm_context->memory, address, line);
}

void memory_write_inc(AsmContext *asm_context, uint8_t data, int line)
{
  write_byte(&asm_context->memory, asm_context->address, data);
  write_debug(&asm_context->memory, asm_context->address, line);
  asm_context->address++;
}

int memory_debug_line(AsmContext *asm_context, uint32_t address)
{
  return read_debug(&asm_context->memory, address);
}

void memory_debug_line_set(AsmContext *asm_context, uint32_t address, int value)
{
  write_debug(&asm_context->memory, address, value);
}

int memory_debug_line_m(Memory *memory, uint32_t address)
{
  return read_debug(memory, address);
}

void memory_debug_line_set_m(Memory *memory, uint32_t address, int value)
{
  write_debug(memory, address, value);
}

void memory_dump(Memory *memory)
{
  MemoryPage *page = memory->pages;

  printf("------ memory dump (debug) ---------\n");
  printf(" low_address: 0x%08x\n", memory->low_address);
  printf("high_address: 0x%08x\n", memory->high_address);
  printf("      endian: %d\n", memory->endian);
  printf("        size: 0x%x\n", memory->size);

  while (page != NULL)
  {
    printf("  page: %p next=%p address=0x%08x offset_min=%d offset_max=%d\n",
      page,
      page->next,
      page->address,
      page->offset_min,
      page->offset_max);

    page = page->next;
  }
}

uint8_t memory_read_m(Memory *memory, uint32_t address)
{
  return read_byte(memory, address);
}

uint16_t memory_read16_m(Memory *memory, uint32_t address)
{
  if (memory->endian == ENDIAN_LITTLE)
  {
    return read_byte(memory, address) |
          (read_byte(memory, address + 1) << 8);
  }
    else
  {
    return (read_byte(memory, address) << 8) |
           (read_byte(memory, address + 1));
  }
}

uint32_t memory_read32_m(Memory *memory, uint32_t address)
{
  if (memory->endian == ENDIAN_LITTLE)
  {
    return read_byte(memory, address) |
          (read_byte(memory, address + 1) << 8) |
          (read_byte(memory, address + 2) << 16) |
          (read_byte(memory, address + 3) << 24);
  }
    else
  {
    return (read_byte(memory, address) << 24) |
           (read_byte(memory, address + 1) << 16) |
           (read_byte(memory, address + 2) << 8) |
           (read_byte(memory, address + 3));
  }
}

void memory_write_m(Memory *memory, uint32_t address, uint8_t data)
{
  write_byte(memory, address, data);
}

void memory_write16_m(Memory *memory, uint32_t address, uint16_t data)
{
  if (memory->endian == ENDIAN_LITTLE)
  {
    write_byte(memory, address + 0, data & 0xff);
    write_byte(memory, address + 1, data >> 8);
  }
    else
  {
    write_byte(memory, address + 0, data >> 8);
    write_byte(memory, address + 1, data & 0xff);
  }
}

void memory_write32_m(Memory *memory, uint32_t address, uint32_t data)
{
  if (memory->endian == ENDIAN_LITTLE)
  {
    write_byte(memory, address + 0, data & 0xff);
    write_byte(memory, address + 1, (data >> 8) & 0xff);
    write_byte(memory, address + 2, (data >> 16) & 0xff);
    write_byte(memory, address + 3, (data >> 24) & 0xff);
  }
    else
  {
    write_byte(memory, address + 0, (data >> 24) & 0xff);
    write_byte(memory, address + 1, (data >> 16) & 0xff);
    write_byte(memory, address + 2, (data >> 8) & 0xff);
    write_byte(memory, address + 3, data & 0xff);
  }
}
