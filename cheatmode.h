#pragma once

#include <string>
#include <string_view>
#include <algorithm>

#include "split.h"

#include "utf8.h"

void seinmode(std::string src, std::string& tgt)
{
	tgt.clear();
	std::vector<std::string> temp_vec = split(src, std::string(" "));
	for(auto i : temp_vec)
	{
		if(i == "nothing")
			{ tgt.append("嘸"); continue; } else
		if(i == "die")
			{ tgt.append("死"); continue; } else
		if(i == "betray")
			{ tgt.append("叛"); continue; } else
		if(i == "increment" or i == "inc")
			{ tgt.append("增"); continue; } else
		if(i == "decrement" or i == "dec")
			{ tgt.append("低"); continue; } else
		if(i == "add")
			{ tgt.append("加"); continue; } else
		if(i == "sub")
			{ tgt.append("減"); continue; } else
		if(i == "div")
			{ tgt.append("除"); continue; } else
		if(i == "mul")
			{ tgt.append("乘"); continue; } else
		if(i == "mod")
			{ tgt.append("餘"); continue; } else
		if(i == "factl")
			{ tgt.append("階"); continue; } else
		if(i == "log_e" or i == "loge" or i == "ln" or i == "log")
			{ tgt.append("基"); continue; } else
		if(i == "root")
			{ tgt.append("根"); continue; } else
		if(i == "pow")
			{ tgt.append("冪"); continue; } else
		if(i == "report")
			{ tgt.append("报"); continue; } else
		if(i == "respond")
			{ tgt.append("答"); continue; } else
		if(i == "interpret")
			{ tgt.append("譯"); continue; } else
		if(i == "transpl")
			{ tgt.append("移"); continue; } else
		if(i == "twin")
			{ tgt.append("孖"); continue; } else
		if(i == "cousin")
			{ tgt.append("哥"); continue; } else
		if(i == "swap")
			{ tgt.append("換"); continue; } else
		if(i == "swap3")
			{ tgt.append("紡"); continue; } else
		if(i == "rotate")
			{ tgt.append("擰"); continue; } else
		if(i == "abandon")
			{ tgt.append("棄"); continue; } else
		if(i == "destroy")
			{ tgt.append("殲"); continue; } else
		if(i == "where")
			{ tgt.append("安"); continue; } else
		if(i == "fall")
			{ tgt.append("落"); continue; } else
		if(i == "jump")
			{ tgt.append("跳"); continue; } else
		if(i == "0")
			{ tgt.append("零"); continue; } else
		if(i == "1")
			{ tgt.append("壹"); continue; } else
		if(i == "2")
			{ tgt.append("貳"); continue; } else
		if(i == "3")
			{ tgt.append("參"); continue; } else
		if(i == "4")
			{ tgt.append("肆"); continue; } else
		if(i == "5")
			{ tgt.append("伍"); continue; } else
		if(i == "6")
			{ tgt.append("陸"); continue; } else
		if(i == "7")
			{ tgt.append("柒"); continue; } else
		if(i == "8")
			{ tgt.append("捌"); continue; } else
		if(i == "9")
			{ tgt.append("玖"); continue; } else
		if(i == "10")
			{ tgt.append("拾"); continue; } else
		if(i == "type")
			{ tgt.append("類"); continue; } else
		if(i == "round")
			{ tgt.append("整"); continue; } else
		if(i == "bend")
			{ tgt.append("彎"); continue; } else
		if(i == "register")
			{ tgt.append("宣"); continue; } else
		if(i == "conclude")
			{ tgt.append("終"); continue; } else
		if(i == "invoke")
			{ tgt.append("嗌"); continue; } else
		if(i == "swindle")
			{ tgt.append("諞"); continue; } else
		if(i == "bigger")
			{ tgt.append("越"); continue; } else
		if(i == "is_it_so" or i == "isitso" or i == "?")
			{ tgt.append("敢"); continue; }
		else{ tgt.append( i ) ; continue; }
	}
	return;
}