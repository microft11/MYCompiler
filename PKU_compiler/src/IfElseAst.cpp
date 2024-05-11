#include "IfElseAst.hpp"
#include "sstream"

int IF_COUNT = 0;

vector<int> BLOCK_RET_RECORDER;

string IfElseAST::DumpKoopa() const {
	bool output_end_signal = false;

	if (BLOCK_RET_RECORDER.back() != 0)
		return "";

	ostringstream oss;
	oss << sequence->DumpKoopa();
	oss << "\tbr %" << NAME_NUMBER - 1 << ", %COMPILER_then_" << IF_COUNT;

	if (elseexp != nullptr)
		oss << ", %COMPILER_else_" << IF_COUNT << endl;
	IF_COUNT ++;

	oss << endl << endl;

	oss << "%COMPILER_then_" << IF_COUNT - 1 << ":" << endl;
	BLOCK_RET_RECORDER.push_back(0);
	oss << ifexp->DumpKoopa();
	if (BLOCK_RET_RECORDER.back() == 0)
	{
		oss << "\tjump %COMPILER_end_" << IF_COUNT - 1 << endl << endl;
		output_end_signal = true;
	}
	BLOCK_RET_RECORDER.pop_back();

	if (elseexp != nullptr)
	{
		BLOCK_RET_RECORDER.push_back(0);
		oss << "%COMPILER_else_" << IF_COUNT - 1 << endl;
		oss << elseexp->DumpKoopa();
		if (BLOCK_RET_RECORDER.back() == 0)
		{
			oss << "\tjump %COMPILER_end_" << IF_COUNT - 1 << endl << endl;
			output_end_signal = true;
		}
	}

	if (output_end_signal)
		oss << "%COMPILER_end_" << IF_COUNT - 1 << endl;
	
	return oss.str();
}