# pragma once
#include <vector>
using namespace std;

// 命名if跳转标签
extern int IF_COUNT;

/// 用于记录当前寄存器用到几号
/// %NAME_NUMBER 意思是下一个空着的变量符而不是最后一个已用的变量
extern int NAME_NUMBER;

// 记录当前块是否已经有ret
extern vector<int> BLOCK_RET_RECORDER;