#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <string_view>

struct vm_t
{
	uint32_t type  : 8;
	uint32_t value : 24;
}; // the lowest byte holds the type

enum class type_t: uint8_t
{
	EMPTY = 0x00,
	INT24 = 0x01,
   UINT24 = 0x02,

   INT48L = 0x03,
   INT48H = 0x04,

    F24   = 0x05,

    ADDR  = 0x06,

    CUSTOM= 0x80,

	ERROR = 0xff
};

enum insn_t: uint16_t
{
    ZERO = 0x0000,
    COMM = 0x0001,

    NOTHN= 0x5638,

    DIE  = 0x6b7b,

    PHOLD= 0xfffd,
    ERROR= 0xFFFF
};

struct command_t
{
    insn_t instruction;
    vm_t   value = {0x0, 0x0};
};