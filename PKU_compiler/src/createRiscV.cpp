#include "koopa.h"
#include <iostream>
#include <assert.h>
using namespace std;
void parse_string(const char* str)
{

    // 解析字符串 str, 得到 Koopa IR 程序
    koopa_program_t program;
    koopa_error_code_t ret = koopa_parse_from_string(str, &program);
    assert(ret == KOOPA_EC_SUCCESS);  // 确保解析时没有出错
    // 创建一个 raw program builder, 用来构建 raw program
    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    // 将 Koopa IR 程序转换为 raw program
    koopa_raw_program_t raw = koopa_build_raw_program(builder, program);
    // 释放 Koopa IR 程序占用的内存
    koopa_delete_program(program);
    
    // 使用 for 循环遍历函数列表
    for (size_t i = 0; i < raw.funcs.len; ++i) {
        // 正常情况下, 列表中的元素就是函数, 我们只不过是在确认这个事实
        // 当然, 你也可以基于 raw slice 的 kind, 实现一个通用的处理函数
        assert(raw.funcs.kind == KOOPA_RSIK_FUNCTION);
        // 获取当前函数
        koopa_raw_function_t func = (koopa_raw_function_t) raw.funcs.buffer[i];
        // 进一步处理当前函数
        for (size_t j = 0; j < func->bbs.len; ++j) 
        {
            assert(func->bbs.kind == KOOPA_RSIK_BASIC_BLOCK);
            koopa_raw_basic_block_t bb = (koopa_raw_basic_block_t) func->bbs.buffer[j];
            // 进一步处理当前基本块
            for (size_t k = 0; k < bb->insts.len; ++k){
                koopa_raw_value_t value = (koopa_raw_value_t)bb->insts.buffer[k];
                cout<<value->kind.tag<<endl;
                //表达式类型：binary
                if(value->kind.tag == KOOPA_RVT_BINARY)
                {

                }
            }
        }
    }


    // 处理完成, 释放 raw program builder 占用的内存
    // 注意, raw program 中所有的指针指向的内存均为 raw program builder 的内存
    // 所以不要在 raw program 处理完毕之前释放 builder
    koopa_delete_raw_program_builder(builder);

}