#pragma once

#include <string>
#include "utf8.h"

std::string hanzi_to_9999(int32_t num);
uint32_t hanzi_digit(int32_t num);

void to_hanzi_f24(std::string& tgt, float num)
{
	tgt.clear();
	if(num == 0) { utf8::append(0x3007, tgt); return; }

	if(num < 0)
	{
		utf8::append(0x8CA0, tgt);
		num = 0 - num;
	}

	int32_t whole  = int32_t(num);
	float   remain = num - whole;
	int32_t top = whole / 10000;
	int32_t bot = whole % 10000;
	if(top)
	{
		tgt.append( hanzi_to_9999(top) );
		utf8::append(0x842C, tgt);
	}

	tgt.append( hanzi_to_9999(bot) );
	if(remain)
	{
		utf8::append(0xff1a, tgt);
		int32_t rem_whole = int32_t(remain * 10000);

		utf8::append(hanzi_digit(rem_whole/1000), tgt);
		if(rem_whole % 1000 == 0) { goto break_if; }

		rem_whole %= 1000;
		utf8::append(hanzi_digit(rem_whole/100), tgt);
		if(rem_whole % 100 == 0) { goto break_if; }

		rem_whole %= 100;
		utf8::append(hanzi_digit(rem_whole/10), tgt);
		if(rem_whole % 10 == 0) { goto break_if; }

		rem_whole %= 10;
		utf8::append(hanzi_digit(rem_whole), tgt);
		if(rem_whole == 0) { goto break_if; }

		break_if:;
	}
	else
	{
		utf8::append(0xff1a, tgt);
		utf8::append(0x3007, tgt);
	}

	return;
}

void to_hanzi(std::string& tgt, int32_t num)
{
	tgt.clear();
	if(num == 0) { utf8::append(0x3007, tgt); return; }
	int32_t top = num / 10000;
	int32_t bot = num % 10000;
	if(top)
	{
		tgt.append( hanzi_to_9999(top) );
		utf8::append(0x842C, tgt);
	}

	tgt.append( hanzi_to_9999(bot) );
	
	return;
}

std::string hanzi_to_9999(int32_t num)
{
	std::string temp = "";
	if(num/1000)
	{
		utf8::append(hanzi_digit(num/1000), temp);
		utf8::append(0x5343, temp);
	}
	num %= 1000;
	if(num/100)
	{
		utf8::append(hanzi_digit(num/100), temp);
		utf8::append(0x767E, temp);
	}
	num %= 100;
	if(num/10)
	{
		utf8::append(hanzi_digit(num/10), temp);
		utf8::append(0x5341, temp);
	}
	num %= 10;
	if(num)
	{
		utf8::append(hanzi_digit(num), temp);
	}
	return temp;

}

uint32_t hanzi_digit(int32_t num)
{
	switch(num)
	{
		case 0:
			return 0x3007;
		case 1:
			return 0x4E00;
		case 2:
			return 0x4E8C;
		case 3:
			return 0x4E09;
		case 4:
			return 0x56DB;
		case 5:
			return 0x4E94;
		case 6:
			return 0x516D;
		case 7:
			return 0x4E03;
		case 8:
			return 0x516B;
		case 9:
			return 0x4E5D;
		default:
			throw;
	}
}