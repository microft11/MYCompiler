#program once

#include "GlobalVar.h"
#include "ast.hpp"

class IfElseAST : public BaseAST {
public:
	point<BaseAST> sequence;
	point<BaseAST> ifexp;
	point<BaseAST> elseexp;

	IfElseAST() {
		;
	}

	string DumpKoopa() const override;
};
