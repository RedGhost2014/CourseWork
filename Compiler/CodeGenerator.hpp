#pragma once
#include "AST.hpp"
#include "Flow.hpp"
#include "Utils.hpp"
#include "Mangler.hpp"


class Register : public IName
{
public:
	Register(wstring);
	~Register() = default;

	// This is more like mutex logic that depends if register already contain some value or not

	void lock();
	void unlock();
	bool isLocked();

private:
	bool isLock;
};


class AssemblerX86
{
public:
	AssemblerX86();
	~AssemblerX86() = default;
	void bindOutputFlowController(BaseFlow<wstring>* output);
	void printtabs(size_t count);

	void add(wstring lhs, wstring rhs);
	void sub(wstring lhs, wstring rhs);
	void mov(wstring lhs, wstring rhs);
	void lea(wstring lhs, wstring rhs);

	void neg(wstring lhs);
	void inc(wstring lhs);
	void dec(wstring lhs);

	void push(wstring lhs);
	void pop(wstring lhs);
	void call(wstring lhs);
	
	void ret();

	inline Register* getEAX();
	inline Register* getEBX();
	inline Register* getECX();
	inline Register* getEDX();
	inline Register* getESI();
	inline Register* getEDI();
	inline Register* getEBP();
	inline Register* getESP();

#define esp getESP()->getName()
#define ebp getEBP()->getName()
#define esi getESI()->getName()
#define edi getEDI()->getName()

#define eax getEAX()->getName()
#define ebx getEBX()->getName()
#define ecx getECX()->getName()
#define edx getEDX()->getName()

	static size_t nestedTabulations;
	Register* getNextFreeRegister();

	void unlockAllRegisters();

	wstring getPtrType(Variable* var);

	const wstring dwordptr;
	const wstring wordptr;
	const wstring byteptr;
private:

	Register* reg_eax;
	Register* reg_ebx;
	Register* reg_ecx;
	Register* reg_edx;
	Register* reg_esi;
	Register* reg_edi;
	Register* reg_ebp;
	Register* reg_esp;

	vector<Register*> basicRegisters;

	BaseFlow<wstring>* output;
};


class CodeGenerator
{
public:
	CodeGenerator();
	~CodeGenerator() = default;
	
	void generateCodeBasedOnAST(AbstractSyntaxTree* ast);
	void bindOutputFlowController(BaseFlow<wstring>* output);
	void bindMangler(BasicMangler* m);

private:
	BaseFlow<wstring>* output;
	AssemblerX86 as;
	BasicMangler* mangler;

	void sendToFlow(wstring _data);

	void makeHeader(TranslationUnit* tu);

	void makeFunctionStart(FunctionDeclReference* fdr);
	void makeFunctionPrologue(FunctionDeclReference* fdr);
	void makeFunctionEpilogue(FunctionDeclReference* fdr);
	void makeFunctionEnd(FunctionDeclReference* fdr);

	void makeRPNTree(CompoundStatement* cs);

	void makeBinaryOperatorAction(SyntaxUnit* lhs, BinaryOperator* op, SyntaxUnit* rhs, vector<SyntaxUnit*>& stack);
	void makeUnaryOperatorAction(SyntaxUnit* lhs, UnaryOperator* op, vector<SyntaxUnit*>& stack);

	void makeFunctionCall(FunctionCallReference* fcl, vector<SyntaxUnit*>& operands);

	vector<vector<SyntaxUnit*>> stack;
};