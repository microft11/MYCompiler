#include "ast.hpp"
#include <string>
#include "sstream"
using namespace std;

//格式化生成运算式
std::string generate_binary_operation(int leftnum,int rightnum,string oper)
{
    ostringstream tmp;
    tmp<< "\t%"<<NAME_NUMBER<<" = "<<oper<<" %"<<leftnum<<", %"<<rightnum<<endl;
    NAME_NUMBER ++;
    return tmp.str();
}
std::string CompUnitAST::DumpAST() const {
    return "CompUnitAST { \n" + func_def->DumpAST() + "}";
}

std::string CompUnitAST::DumpKoopa() const {
    return func_def->DumpKoopa();
}

std::string FuncDefAST::DumpAST() const {
    return "FuncDefAST { \n" + func_type->DumpAST() + ", " + "ident=" + ident + ", " + block->DumpAST() + " }";
}

std::string FuncDefAST::DumpKoopa() const {
    return "fun @" + ident + "():" + func_type->DumpKoopa() + "{\n" + block->DumpKoopa() + "\n}";
}

std::string FuncTypeAST::DumpAST() const {
    return "FuncTypeAST { \n" + name + " }";
}

std::string FuncTypeAST::DumpKoopa() const {
    return std::string("i32");
}

std::string BlockAST::DumpAST() const {
    return "BlockAST { \n" + stmt->DumpAST() + " }";
}

std::string BlockAST::DumpKoopa() const 
{
    CONSTVAL_MAP.push_back(unordered_map<string, int>());
    string rslt =  "%entry:\n" + stmt->DumpKoopa();
    CONSTVAL_MAP.pop_back();
    return rslt;
}

std::string BlockItemAST::DumpKoopa() const
{
    ostringstream oss;
    oss << stmt->DumpKoopa();
    cerr << oss.str() << std::endl;
    if (next != nullptr)
        oss << next->DumpKoopa();
    return oss.str();
}

std::string StmtAST::DumpAST() const 
{
    if (Is_LVal == 0)
        return "StmtAST { return, \n" + num->DumpAST() + " }";
    else {
        cerr << "Not implied val" << std::endl;
        assert(0);
    }
}

std::string StmtAST::DumpKoopa() const {
    if (Is_LVal == 0)
        return num->DumpKoopa()+"\tret %"+ to_string(NAME_NUMBER-1);
    else {
        cerr << "Not implied val in this" << std::endl;
        assert(0);
    }
}

// Decl ::= ConstDecl | VarDecl;
std::string DeclAST::DumpKoopa() const {
    if (Is_Const)
        return decl->DumpKoopa();
    else {
        cerr << "下一次就会实现了（可能" << std::endl;
        assert(0);
    }
}

// ConstDef  ::= IDENT "=" ConstInitVal;
// 在ConstDefAST存下了所有常量式
std::string ConstDefAST::DumpKoopa() const {
    unordered_map<std::string, int> & lastmap = CONSTVAL_MAP.back();
    lastmap[name] = exp->Calc();
    if (next != nullptr)
        next->DumpKoopa();
    return "";
}

std::string NumberAST::DumpAST() const {

    return "NumberAST { int \n" + std::to_string(val) + " }";
}

// Number ::= INT_CONST;
// 把一元运算符改成二元的形式
std::string NumberAST::DumpKoopa() const {
    //cout<<"Number:"<<NAME_NUMBER<<endl;
    return "\t%"+to_string(NAME_NUMBER++) +"= add 0, "+to_string(val)+"\n";
}

std::string ExpAST::DumpAST() const {
    return "ExpAST{ \n" + unary_exp->DumpAST() + "}";
}

std::string ExpAST::DumpKoopa() const {
    return unary_exp->DumpKoopa();
}

std::string PrimaryExpAST::DumpAST() const {
    return "PrimaryExpAST{  \n"+p_exp->DumpAST() +"}";
}

std::string PrimaryExpAST::DumpKoopa() const {
    return p_exp->DumpKoopa();
}

std::string UnaryExpAST::DumpAST() const {
    std::string oper;
    switch (type) {
        case UnaryOp::Positive:
            oper = "+";
            break;
        case UnaryOp::Negative:
            oper = "-";
            break;
        case UnaryOp::LogicalFalse:
            oper = "!";
            break;
    }

    return "UnaryExpAST{  \n"+oper+"("+u_exp->DumpAST()+")}";
}

// UnaryExp    ::= PrimaryExp | UnaryOp UnaryExp;
// 但是并没有写成两个构建函数
// 因为可以用递归的方法把所有情况都枚举到
std::string UnaryExpAST::DumpKoopa() const {
    std::string rslt = u_exp->DumpKoopa();
    switch (type) {
        case UnaryOp::Positive:
            return u_exp->DumpKoopa();
        case UnaryOp::Negative:
            rslt += "\t%"+to_string(NAME_NUMBER) +" = sub 0, %"+to_string(NAME_NUMBER-1)+"\n";
            NAME_NUMBER++;
            //cout<<"Negative:"<<NAME_NUMBER<<endl;
            return rslt;
        case UnaryOp::LogicalFalse:
            rslt += "\t%"+to_string(NAME_NUMBER) +" = eq 0, %"+to_string(NAME_NUMBER-1)+"\n";
            NAME_NUMBER++;
            ///cout<<"LogicalFalse:"<<NAME_NUMBER<<endl;
            return rslt;

    }
}

// MulExp ::= UnaryExp | MulExp ("*" | "/" | "%") UnaryExp;
std::string MulExpAST::DumpAST() const {

    if(type == MulType::NotMul){
        return "MulExpAST{  \n"+u_exp->DumpAST()+"}";
    }
    else{
        std::string oper;
        switch (type) {
            case MulType::Mul:
                oper="*";
                break;
            case MulType::Div:
                oper = "/";
                break;
            case MulType::Mod:
                oper = "%";
                break;
            default:
                break;
        }

        return "MulExpAST{  "+m_exp->DumpAST()+"\n"+oper+"\n"+u_exp->DumpAST()+"}";

    }
}

std::string MulExpAST::DumpKoopa() const {
    ostringstream rslt;
    if(type==MulType::NotMul){
        return u_exp->DumpKoopa();
    }
    else{
        rslt << m_exp->DumpKoopa();
        int leftnum = NAME_NUMBER-1;
        rslt <<u_exp->DumpKoopa();
        int rightnum = NAME_NUMBER-1;
        string oper;
        switch (type) {
            case MulType::Mul:
                oper="mul";
                break;
            case MulType::Div:
                oper = "div";
                break;
            case MulType::Mod:
                oper = "mod";
                break;
            default:
                break;
        }
        rslt<< generate_binary_operation(leftnum,rightnum,oper);

        return rslt.str();
    }

}

// AddExp  ::= MulExp | AddExp ("+" | "-") MulExp;
std::string AddExpAST::DumpAST() const {
    if (type == AddType::NotAdd) {
        return "AddExpAST{  \n"+m_exp->DumpAST()+"}";
    } 
    else {
        std::string oper;
        switch(type) {
            case AddType::Add:
                oper = "+";
                break;
            case AddType::Sub:
                oper = "-";
                break;
        }
        return  "AddExpAST{  "+a_exp->DumpAST()+"\n"+oper+"\n"+m_exp->DumpAST()+"}";
    }
}

std::string AddExpAST::DumpKoopa() const {
    ostringstream reslt;
    if(type == AddType::NotAdd){
        return m_exp->DumpKoopa();
    }
    else{
        string oper;
        if(type == AddType::Add){
            oper="add";
        }
        else if (type ==AddType::Sub){
            oper = "sub";
        }
        reslt<<a_exp->DumpKoopa();
        int leftnum = NAME_NUMBER-1;
        reslt<<m_exp->DumpKoopa();
        int rightnum = NAME_NUMBER-1;
        reslt<< generate_binary_operation(leftnum,rightnum,oper);

        return reslt.str();
    }
}

// RelExp ::= AddExp | RelExp ("<" | ">" | "<=" | ">=") AddExp;
std::string RelExpAST::DumpAST() const {
    string oper;
    if (type == RelType::NoRel) {
        return "RelExpAST{  \n"+a_exp->DumpAST()+"}";
    }
    else {
        switch (type) {
        case RelType::Less:
            oper = "<";
            break;
        case RelType::Bigger:
            oper = ">";
            break;
        case RelType::LessEq:
            oper = "<=";
            break;
        case RelType::BiggerEq:
            oper = ">=";
            break;
        default:
            break;
        }
        return "RelExpAST{  "+r_exp->DumpAST()+"\n"+oper+"\n"+a_exp->DumpAST()+"}";
    }
}

std::string RelExpAST::DumpKoopa() const {
    string oper= "";
    switch (type) {
        case RelType::Less:
            oper = "lt";
            break;
        case RelType::Bigger:
            oper = "gt";
            break;
        case RelType::LessEq:
            oper = "le";
            break;
        case RelType::BiggerEq:
            oper = "ge";
            break;
        default:
            break;
    }
    ostringstream reslt;
    if(type == RelType::NoRel)
        return a_exp->DumpKoopa();
    else{
        reslt<<r_exp->DumpKoopa();
        int leftnum = NAME_NUMBER-1;
        reslt<<a_exp->DumpKoopa();
        int rightnum =NAME_NUMBER-1;
        reslt<< generate_binary_operation(leftnum,rightnum,oper);
        return reslt.str();
    }
}

// EqExp  ::= RelExp | EqExp ("==" | "!=") RelExp;
std::string EqExpAST::DumpAST() const {
    std::string oper;
    if (type == EqType::NoEq) {
        return "EqExpAST{  \n"+r_exp->DumpAST()+"}";
    }
    else {
        switch (type) {
        case EqType::Equal:
            oper = "==";
            break;
        case EqType::NotEqual:
            oper = "!=";
            break;
        default:
            break;
        }
        return "EqExpAST{  "+ e_exp->DumpAST()+"\n"+oper+"\n"+ r_exp->DumpAST()+"}";
    }
}

std::string EqExpAST::DumpKoopa() const {
    ostringstream reslt;
    string oper;
    switch (type) {
        case EqType::Equal:
            oper = "eq";
            break;
        case EqType::NotEqual:
            oper = "ne";
            break;
        default:
            break;
    }

    if (type ==EqType::NoEq){
        return r_exp->DumpKoopa();
    }
    else{
        reslt<<e_exp->DumpKoopa();
        int leftnum = NAME_NUMBER-1;
        reslt<<r_exp->DumpKoopa();
        int rightnum =NAME_NUMBER-1;
        reslt<< generate_binary_operation(leftnum,rightnum,oper);
        return reslt.str();
    }
}

std::string LAndExpAST::DumpAST() const {
    if (type == AndOrType::NoLogic) {
        return "LAndExpAST{  \n"+e_exp->DumpAST()+"}";
    }
    else {
        return "LAndExpAST{  "+ l_exp->DumpAST()+"\n" + "&&" + "\n"+ e_exp->DumpAST()+"}";
    }
}

std::string LAndExpAST::DumpKoopa() const {
    ostringstream reslt;
    if(type==AndOrType::NoLogic)
        return e_exp->DumpKoopa();
    else{
        reslt<<l_exp->DumpKoopa();
        int leftnum = NAME_NUMBER-1;
        reslt<<e_exp->DumpKoopa();
        int rightnum =NAME_NUMBER-1;

        reslt<<"\t%"<<NAME_NUMBER<<" = eq 0, %"<<leftnum<<endl;
        //下一个leftnum需要更新
        leftnum = NAME_NUMBER,NAME_NUMBER++;

        reslt<<"\t%"<<NAME_NUMBER<<" = eq 0, %"<<rightnum<<endl;
        //下一个rightnum需要更新
        rightnum = NAME_NUMBER,NAME_NUMBER++;

        //and 说明两个都为真才是真（在这里两个值都为0）
        //所以是left or right取反

        reslt<< generate_binary_operation(leftnum,rightnum,"or");
        reslt<<"\t%"<<NAME_NUMBER<<" = eq 0, %"<<NAME_NUMBER-1<<endl;
        NAME_NUMBER++;

        return reslt.str();
    }
}

std::string LOrExpAST::DumpAST() const {
    if (type == AndOrType::NoLogic) {
        return "LAndExpAST{  \n"+And_exp->DumpAST()+"}";
    }
    else {
        return "LAndExpAST{  "+ Or_exp->DumpAST()+"\n" + "||" + "\n"+ And_exp->DumpAST()+"}";
    }
}

std::string LOrExpAST::DumpKoopa() const {
    ostringstream reslt;
    if(type==AndOrType::NoLogic)
        return And_exp->DumpKoopa();
    else{
        reslt<<Or_exp->DumpKoopa();
        int leftnum = NAME_NUMBER-1;
        reslt<<And_exp->DumpKoopa();
        int rightnum =NAME_NUMBER-1;

        //和0比较取布尔值
        reslt<<"\t%"<<NAME_NUMBER<<" = eq 0, %"<<leftnum<<endl;
        //下一个leftnum需要更新
        leftnum = NAME_NUMBER,NAME_NUMBER++;

        reslt<<"\t%"<<NAME_NUMBER<<" = eq 0, %"<<rightnum<<endl;
        //下一个rightnum需要更新
        rightnum = NAME_NUMBER,NAME_NUMBER++;

        //or即如果两个都为假（在这里值为1）则假否则为真
        //所以是leftnum and rightnum 然后取反

        reslt<< generate_binary_operation(leftnum,rightnum,"and");
        reslt<<"\t%"<<NAME_NUMBER<<" = eq 0, %"<<NAME_NUMBER-1<<endl;
        NAME_NUMBER++;
        return reslt.str();
    }
}

std::string LValAST::DumpKoopa() const {
    ostringstream oss;
    oss << "\t%" << NAME_NUMBER ++ << "= add 0, " << Calc() << endl;
    return oss.str();
}