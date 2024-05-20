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

enum type_t: uint8_t
{
	EMPTY_T = 0x00,
	INT24_T = 0x01,
   UINT24_T = 0x02,

   INT48L_T = 0x03,
   INT48H_T = 0x04,

    F24_T   = 0x05,

    ADDR_T  = 0x06,

    CUSTOM_T= 0x80,

    COMM_T  = 0xfe,

	ERROR_T = 0xff
};

enum insn_t: uint16_t
{
    ZERO = 0x0000,
    COMM = 0x0001,

    ADD  = 0x52a0,     // 加

    NOTHING  = 0x5638, // 嘸

    INCREMENT= 0x589e, // 增

    ROTATE   = 0x64f0, // 擰

    ABANDON  = 0x68c4, // 棄

    DIE      = 0x6b7b, // 死

    DESTROY  = 0x6bb2, // 殲

    PSH_ZERO = 0x96f6, // 零

    PHOLD= 0xfffd,
    ERROR= 0xFFFF
};

struct command_t
{
    insn_t instruction;
    vm_t   value = {0x0, 0x0};
};