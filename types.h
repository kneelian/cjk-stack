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

    DECREMENT= 0x4f4e, // 低

    ADD      = 0x52a0, // 加

    NOTHING  = 0x5638, // 嘸

    INCREMENT= 0x589e, // 增

    TWIN     = 0x5b56, // 孖

    WHERE    = 0x5b89, // 安

    SWAP     = 0x63db, // 換

    ROTATE   = 0x64f0, // 擰

    ABANDON  = 0x68c4, // 棄

    DIE      = 0x6b7b, // 死

    DESTROY  = 0x6bb2, // 殲

    SWAP3    = 0x7d21, // 紡

    FALL     = 0x843d, // 落

    JUMP     = 0x8df3, // 跳

    PSH_ZERO = 0x96f6, // 零

    PSH_ONE  = 0x58f9, // 壹

    PSH_TWO  = 0x8cb3, // 貳

    PSH_THREE= 0x53c3, // 參

    PSH_FOUR = 0x8086, // 肆

    PSH_FIVE = 0x4f0d, // 伍

    PSH_SIX  = 0x9678, // 陸

    PSH_SEVEN= 0x67d2, // 柒

    PSH_EIGHT= 0x634c, // 捌

    PSH_NINE = 0x7396, // 玖

    PSH_TEN  = 0x62fe, // 拾

    PHOLD= 0xfffd,
    ERROR= 0xFFFF
};

struct command_t
{
    insn_t instruction;
    vm_t   value = {0x0, 0x0};
};