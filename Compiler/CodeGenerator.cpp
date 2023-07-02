#include "CodeGenerator.hpp"

Register::Register(wstring _regname)
{
	name = _regname;
	isLock = false;
}

void Register::lock()
{
	isLock = true;
}

void Register::unlock()
{
	isLock = false;
}

bool Register::isLocked()
{
	return isLock;
}

size_t AssemblerX86::nestedTabulations = 0;

AssemblerX86::AssemblerX86() :
	dwordptr(L"DWORD PTR "),
	wordptr(L"WORD PTR "),
	byteptr(L"BYTE PTR ")
{
	reg_eax = new Register(L"eax");
	reg_ebx = new Register(L"ebx");
	reg_ecx = new Register(L"ecx");
	reg_edx = new Register(L"edx");
	reg_esi = new Register(L"esi");
	reg_edi = new Register(L"edi");
	reg_ebp = new Register(L"ebp");
	reg_esp = new Register(L"esp");


	basicRegisters.push_back(reg_eax);
	basicRegisters.push_back(reg_ecx);
	basicRegisters.push_back(reg_edx);
	basicRegisters.push_back(reg_esi);
	basicRegisters.push_back(reg_edi);
	basicRegisters.push_back(reg_ebx);
}

Register* AssemblerX86::getNextFreeRegister()
{
	for (Register* reg : basicRegisters)
	{
		if (!reg->isLocked())
		{
			return reg;
		}
	}
	return nullptr;
}

inline Register* AssemblerX86::getEAX()
{
	return reg_eax;
}

inline Register* AssemblerX86::getEBX()
{
	return reg_ebx;
}

inline Register* AssemblerX86::getECX()
{
	return reg_ecx;
}

inline Register* AssemblerX86::getEDX()
{
	return reg_edx;
}

inline Register* AssemblerX86::getESI()
{
	return reg_esi;
}

inline Register* AssemblerX86::getEDI()
{
	return reg_edi;
}

inline Register* AssemblerX86::getESP()
{
	return reg_esp;
}

inline Register* AssemblerX86::getEBP()
{
	return reg_ebp;
}

void AssemblerX86::unlockAllRegisters()
{
	for (Register* reg : basicRegisters)
	{
		reg->unlock();
	}
}

void AssemblerX86::bindOutputFlowController(BaseFlow<wstring>* _output)
{
	output = _output;
}

void AssemblerX86::printtabs(size_t count)
{
	if (count == 0)
		return;

	wstring tabs;
	for (size_t i = 0; i < count; i++)
	{
		tabs += L"\t";
	}
	*output << tabs;
}




















void AssemblerX86::add(wstring lhs, wstring rhs)
{
	printtabs(nestedTabulations);
	*output << L"add " << lhs << L", " << rhs << L"\n";
}


void AssemblerX86::sub(wstring lhs, wstring rhs)
{
	printtabs(nestedTabulations);
	*output << L"sub " << lhs << L", " << rhs << L"\n";
}


void AssemblerX86::lea(wstring lhs, wstring rhs)
{
	printtabs(nestedTabulations);
	*output << L"lea " << lhs << L", " << rhs << L"\n";
}

void AssemblerX86::mov(wstring lhs, wstring rhs)
{
	printtabs(nestedTabulations);
	*output << L"mov " << lhs << L", " << rhs << L"\n";
}


void AssemblerX86::push(wstring lhs)
{
	printtabs(nestedTabulations);
	*output << L"push " << lhs << L"\n";
}

void AssemblerX86::pop(wstring lhs)
{
	printtabs(nestedTabulations);
	*output << L"pop " << lhs << L"\n";
}

void AssemblerX86::call(wstring lhs)
{
	printtabs(nestedTabulations);
	*output << L"call " << lhs << L"\n";
}

void AssemblerX86::neg(wstring lhs)
{
	printtabs(nestedTabulations);
	*output << L"neg " << lhs << L"\n";
}

void AssemblerX86::dec(wstring lhs)
{
	printtabs(nestedTabulations);
	*output << L"dec " << lhs << L"\n";
}

void AssemblerX86::inc(wstring lhs)
{
	printtabs(nestedTabulations);
	*output << L"inc " << lhs << L"\n";
}

void AssemblerX86::ret()
{
	printtabs(nestedTabulations);
	*output << L"ret" << L"\n";
}


wstring AssemblerX86::getPtrType(Variable* var)
{
	BasicAbstractType* t = var->getType();
	if (t->getSize() >= 4)
	{
		return dwordptr;
	}
	else if (t->getSize() == 2)
	{
		return wordptr;
	}
	else if (t->getSize() == 1)
	{
		return byteptr;
	}
}
















CodeGenerator::CodeGenerator()
{
	output = nullptr;
}

void CodeGenerator::bindOutputFlowController(BaseFlow<wstring>* _output)
{
	output = _output;
	as.bindOutputFlowController(output);
}

void CodeGenerator::bindMangler(BasicMangler* _m)
{
	mangler = _m;
}

void CodeGenerator::generateCodeBasedOnAST(AbstractSyntaxTree* ast)
{
	stack.push_back(ast->getHead().getBranch());

	vector<size_t> sequenceIndexStack;

	vector<SyntaxUnit*> stackNestedUnits;

	size_t i = 0;

	TranslationUnit* tu = nullptr;
	FunctionDeclReference* fdr = nullptr;
	CompoundStatement* cs = nullptr;

	while (1)
	{
		if (!stack.empty() && i >= stack.back().size())
		{
			if (!stack.empty())
				stack.pop_back();


			if (!stackNestedUnits.empty())
			{
				if (fdr = dynamic_cast<FunctionDeclReference*>(stackNestedUnits.back()))
				{
					makeFunctionEpilogue(fdr);
					as.nestedTabulations--;
					makeFunctionEnd(fdr);
				}
				stackNestedUnits.pop_back();
			}


			if (!sequenceIndexStack.empty())
			{
				i = sequenceIndexStack.back();
				sequenceIndexStack.pop_back();
			}
		}

		if (stack.empty())
			break;

		if (i < stack.back().size() && (tu = dynamic_cast<TranslationUnit*>(stack.back()[i])))
		{
			makeHeader(tu);
			stack.push_back(tu->getBranch());

			stackNestedUnits.push_back(tu);
			i++;
			sequenceIndexStack.push_back(i);
			i = 0;

			continue;
		}
		else if (i < stack.back().size() && (fdr = dynamic_cast<FunctionDeclReference*>(stack.back()[i])))
		{
			makeFunctionStart(fdr);
			as.nestedTabulations++;
			makeFunctionPrologue(fdr);

			stack.push_back(fdr->getBranch());
			stackNestedUnits.push_back(fdr);

			i++;
			sequenceIndexStack.push_back(i);
			i = 0;

			continue;
		}
		else if (i < stack.back().size() && (cs = dynamic_cast<CompoundStatement*>(stack.back()[i])))
		{
			// TODO: checkSemantic(cs->getBranch());
			as.unlockAllRegisters();
			makeRPNTree(cs);
			sendToFlow(L"\n");
		}

		i++;
	}
	sendToFlow(L"END");
}

void CodeGenerator::makeRPNTree(CompoundStatement* cs)
{
	vector<SyntaxUnit*> subsequence = cs->getBranch();

	if (subsequence.empty())
		return;

	vector<SyntaxUnit*> operands;

	OperatorReference* opref = nullptr;
	FunctionCallReference* fcr = nullptr;

	for (size_t i = 0; i < subsequence.size(); i++)
	{
		if (opref = dynamic_cast<OperatorReference*>(subsequence[i]))
		{
			UnaryOperator* unaryop = nullptr;
			BinaryOperator* binaryop = nullptr;

			if (unaryop = dynamic_cast<UnaryOperator*>(opref->getOperator()))
			{
				SyntaxUnit* stackTop = operands.back();

				makeUnaryOperatorAction(stackTop, unaryop, operands);

			}
			else if (binaryop = dynamic_cast<BinaryOperator*>(opref->getOperator()))
			{
				// crutch
				if (binaryop->getName() == L",")
					continue;

				SyntaxUnit* stackTop = operands.back();
				SyntaxUnit* stackUnderTop = operands[operands.size() - 2];

				makeBinaryOperatorAction(stackUnderTop, binaryop, stackTop, operands);
			}
		}
		else if (fcr = dynamic_cast<FunctionCallReference*>(subsequence[i]))
		{
			makeFunctionCall(fcr, operands);
		}
		else
		{
			operands.push_back(subsequence[i]);
		}
	}
}

void CodeGenerator::makeUnaryOperatorAction(SyntaxUnit* lhs, UnaryOperator* op, vector<SyntaxUnit*>& operands)
{
	ConstantDeclReference* constant = nullptr;
	VariableDeclReference* var = nullptr;

	TempValueReference* temp = nullptr;

	Register* reg = as.getNextFreeRegister();

	if (op->isBuiltIn())
	{
		if (op->getName() == L"-")
		{
			if (temp = dynamic_cast<TempValueReference*>(lhs))
			{
				// Example: -(3 + a)
				as.neg(temp->getRegister()->getName());
				return;
			}
			else if (constant = dynamic_cast<ConstantDeclReference*>(lhs))
			{
				// Example: -5
				as.mov(reg->getName(), L"-" + constant->getSignature());
			}
			else if (var = dynamic_cast<VariableDeclReference*>(lhs))
			{
				// Example: -i
				as.mov(reg->getName(), as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				as.neg(reg->getName());
			}
		}
		else if (op->getName() == L"++")
		{
			if (constant = dynamic_cast<ConstantDeclReference*>(lhs))
			{
				// Constant not a lvalue => TODO: error
				// Example: 5++
			}
			else if (var = dynamic_cast<VariableDeclReference*>(lhs))
			{
				if (MetaInfo.isPureIntegralType(var->getVariable()->getType()))
				{
					as.inc(as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
					return;
				}
				else
				{
					as.mov(reg->getName(), as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");

					as.add(reg->getName(), Utils::toWstring(var->getVariable()->getType()->getSize(), 10));

					as.mov(as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]", reg->getName());
				}
			}
		}
		else if (op->getName() == L"--")
		{
			if (constant = dynamic_cast<ConstantDeclReference*>(lhs))
			{
				// Constant not a lvalue => TODO: error
				// Example: 5--
			}
			else if (var = dynamic_cast<VariableDeclReference*>(lhs))
			{
				if (MetaInfo.isPureIntegralType(var->getVariable()->getType()))
				{
					as.dec(as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
					return;
				}
				else
				{
					as.mov(reg->getName(), as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");

					as.sub(reg->getName(), Utils::toWstring(var->getVariable()->getType()->getSize(), 10));

					as.mov(as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]", reg->getName());
				}
			}
		}
	}

	reg->lock();
	TempValueReference* tmpresult = new TempValueReference(reg);
	operands.pop_back();
	operands.push_back(tmpresult);
}

void CodeGenerator::makeBinaryOperatorAction(SyntaxUnit* lhs, BinaryOperator* op, SyntaxUnit* rhs, vector<SyntaxUnit*>& operands)
{
	ConstantDeclReference* constant = nullptr;
	VariableDeclReference* var = nullptr;

	TempValueReference* tempLhs = nullptr;
	TempValueReference* tempRhs = nullptr;

	Register* reg = as.getNextFreeRegister();

	if (op->isBuiltIn())
	{
		if (op->getName() == L"+")
		{
			if (tempLhs = dynamic_cast<TempValueReference*>(lhs))
			{
				if (tempRhs = dynamic_cast<TempValueReference*>(rhs))
				{
					as.add(tempLhs->getRegister()->getName(), tempRhs->getRegister()->getName());
					tempRhs->getRegister()->unlock();
				}
				else if (var = dynamic_cast<VariableDeclReference*>(rhs))
				{
					as.add(tempLhs->getRegister()->getName(), as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}
				else if (constant = dynamic_cast<ConstantDeclReference*>(rhs))
				{
					as.add(tempLhs->getName(), constant->getSignature());
				}
				operands.pop_back();
				return;
			}
			else
			{
				if (constant = dynamic_cast<ConstantDeclReference*>(lhs))
				{
					as.mov(reg->getName(), constant->getSignature());
				}
				else if (var = dynamic_cast<VariableDeclReference*>(lhs))
				{
					as.mov(reg->getName(), as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}

				if (tempRhs = dynamic_cast<TempValueReference*>(rhs))
				{
					as.add(reg->getName(), tempRhs->getRegister()->getName());
					tempRhs->getRegister()->unlock();
				}
				else if (var = dynamic_cast<VariableDeclReference*>(rhs))
				{
					as.add(reg->getName(), as.dwordptr + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}
				else if (constant = dynamic_cast<ConstantDeclReference*>(rhs))
				{
					as.add(reg->getName(), constant->getSignature());
				}
			}
		}
		else if (op->getName() == L"-")
		{
			if (tempLhs = dynamic_cast<TempValueReference*>(lhs))
			{
				if (tempRhs = dynamic_cast<TempValueReference*>(rhs))
				{
					as.sub(tempLhs->getRegister()->getName(), tempRhs->getRegister()->getName());
					tempRhs->getRegister()->unlock();
				}
				else if (var = dynamic_cast<VariableDeclReference*>(rhs))
				{
					as.sub(tempLhs->getRegister()->getName(), as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}
				else if (constant = dynamic_cast<ConstantDeclReference*>(rhs))
				{
					as.sub(tempLhs->getName(), constant->getSignature());
				}
				operands.pop_back();
				return;
			}
			else
			{
				if (constant = dynamic_cast<ConstantDeclReference*>(lhs))
				{
					as.mov(reg->getName(), constant->getSignature());
				}
				else if (var = dynamic_cast<VariableDeclReference*>(lhs))
				{
					as.mov(reg->getName(), as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}

				if (tempRhs = dynamic_cast<TempValueReference*>(rhs))
				{
					as.sub(reg->getName(), tempRhs->getRegister()->getName());
					tempRhs->getRegister()->unlock();
				}
				else if (var = dynamic_cast<VariableDeclReference*>(rhs))
				{
					as.sub(reg->getName(), as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}
				else if (constant = dynamic_cast<ConstantDeclReference*>(rhs))
				{
					as.sub(reg->getName(), constant->getSignature());
				}
			}
		}


		else if (op->getName() == L"=")
		{
			if (tempRhs = dynamic_cast<TempValueReference*>(rhs))
			{
				if (var = dynamic_cast<VariableDeclReference*>(lhs))
				{
					as.mov(as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]", tempRhs->getRegister()->getName());
				}
			}
			else if ((var = dynamic_cast<VariableDeclReference*>(lhs)) && (constant = dynamic_cast<ConstantDeclReference*>(rhs)))
			{
				as.mov(as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]", constant->getSignature());
			}
			else
			{
				if (var = dynamic_cast<VariableDeclReference*>(rhs))
				{
					as.mov(reg->getName(), as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
				}
				else if (constant = dynamic_cast<ConstantDeclReference*>(rhs))
				{
					as.mov(reg->getName(), constant->getSignature());
				}
				if (var = dynamic_cast<VariableDeclReference*>(lhs))
				{
					as.mov(as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]", reg->getName());
				}
			}
		}
	}
	else
	{
		// TODO: call operator
	}

	reg->lock();
	TempValueReference* tmpresult = new TempValueReference(reg);

	operands.pop_back();
	operands.pop_back();

	operands.push_back(tmpresult);
}

void CodeGenerator::makeFunctionCall(FunctionCallReference* fcl, vector<SyntaxUnit*>& operands)
{
	Function* f = fcl->getFunction();

	if (!f)
	{
		//TODO: error
		return;
	}

	vector<Variable*>& arguments = f->getFunctionArguments();

	ConstantDeclReference* constant = nullptr;
	VariableDeclReference* var = nullptr;
	TempValueReference* temp = nullptr;

	Register* reg = as.getNextFreeRegister();

	size_t j = operands.size();
	
	// thats probably weird
	for (size_t i = 0; i < arguments.size(); i++)
	{
		if (constant = dynamic_cast<ConstantDeclReference*>(operands[j - 1]))
		{
			as.push(constant->getSignature());
		}
		else if (var = dynamic_cast<VariableDeclReference*>(operands[j - 1]))
		{
			as.mov(reg->getName(), as.getPtrType(var->getVariable()) + mangler->mangleVariableName(var->getSignature()) + L"[" + as.ebp + L"]");
			as.push(reg->getName());
		}
		else if (temp = dynamic_cast<TempValueReference*>(operands[j - 1]))
		{
			as.push(temp->getRegister()->getName());
		}
		operands.pop_back();
		j--;
	}
	
	as.call(mangler->mangleFunction(f) + L"\t; " + f->getName());
	as.add(as.esp, Utils::toWstring(arguments.size() * 4, 10));

	TempValueReference* tmpresult = new TempValueReference(as.getEAX());
	as.getEAX()->lock();
	operands.push_back(tmpresult);
}

void CodeGenerator::sendToFlow(wstring _data)
{
	*output << _data << L"\n";
}

void CodeGenerator::makeHeader(TranslationUnit* tu)
{
	sendToFlow(L"; Source: " + tu->getSignature());
	sendToFlow(L"\t.686P");
	sendToFlow(L"\t.model flat\n");
}

void CodeGenerator::makeFunctionStart(FunctionDeclReference* fdr)
{
	vector<Variable*>& stack = fdr->getFunction()->getFunctionStackVars();

	sendToFlow(L"_TEXT segment\n");

	size_t currentOffset = 0;

	for (int i = stack.size() - 1; i >= 0; i--)
	{
		currentOffset += stack[i]->getType()->getSize();
		sendToFlow(mangler->mangleVariableName(stack[i]->getName()) + L" = -" + Utils::toWstring(currentOffset, 10));
	}

	currentOffset = 8;

	vector<Variable*>& args = fdr->getFunction()->getFunctionArguments();

	for (size_t i = 0; i < args.size(); i++)
	{
		sendToFlow(mangler->mangleVariableName(args[i]->getName()) + L" = " + Utils::toWstring(currentOffset, 10));
		currentOffset += 4;
	}
	sendToFlow(L"\n" + mangler->mangleFunction(fdr->getFunction()) + L" proc\n");
}

void CodeGenerator::makeFunctionPrologue(FunctionDeclReference* fdr)
{
	as.push(as.ebp);
	as.mov(as.ebp, as.esp);

	as.sub(as.esp, Utils::toWstring(fdr->getFunction()->getStackSize(), 10));

	sendToFlow(L"\n");
}


void CodeGenerator::makeFunctionEpilogue(FunctionDeclReference* fdr)
{
	as.mov(as.esp, as.ebp);
	as.pop(as.ebp);
	as.ret();
}

void CodeGenerator::makeFunctionEnd(FunctionDeclReference* fdr)
{
	sendToFlow(L"\n" + mangler->mangleFunction(fdr->getFunction()) + L" endp\n");
	sendToFlow(L"_TEXT ends\n");
}