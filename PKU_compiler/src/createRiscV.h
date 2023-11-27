#pragma once
#ifndef SYSY_MAKE_TEMPLATE_CREATERISCV_H
#define SYSY_MAKE_TEMPLATE_CREATERISCV_H

#endif //SYSY_MAKE_TEMPLATE_CREATERISCV_H
#include "string"
#include "koopa.h"
using namespace std;
//读取Koopa程序字符串生产IR内存
void parse_string(const char* str);

// 处理二元表达式
void Visit(const koopa_raw_binary_t& oper);