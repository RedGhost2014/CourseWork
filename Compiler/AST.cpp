#include "AST.hpp"

wstring ISignature::getSignature()
{
	return signature;
}

void ISignature::setSignature(wstring _signature)
{
	signature = _signature;
}



AbstractSyntaxTree::AbstractSyntaxTree()
{
	this->stack.push_back(&head.getBranch());
}

SyntaxUnit& AbstractSyntaxTree::getHead()
{
	return head;
}

void SyntaxUnit::push(SyntaxUnit* su)
{
	this->branch.push_back(su);
}

void AbstractSyntaxTree::pop()
{
	if (stack.size() > 1)
		stack.pop_back();
}

void AbstractSyntaxTree::print()
{
	treePrint(&head);
}

void AbstractSyntaxTree::treePrint(SyntaxUnit* su)
{
	static size_t recursionDepthLevel = 0;
	vector<SyntaxUnit*>& v = su->getBranch();

	for (size_t i = 0; i < v.size(); i++)
	{
		for (size_t i = 0; i < recursionDepthLevel; i++)
		{
			wcout << "\t";
		}
		v[i]->print();
		wcout << endl;

		recursionDepthLevel++;
		treePrint(v[i]);
		recursionDepthLevel--;
	}
}

void AbstractSyntaxTree::push(SyntaxUnit* unit)
{
	stack.back()->push_back(unit);
	if (dynamic_cast<VariableDeclReference*>(unit))
	{
		//wcout << "Ya varDeclRef" << endl;
	}
	else if (dynamic_cast<CompoundStatement*>(unit) || dynamic_cast<ReturnStatement*>(unit))
	{
		//wcout << "Ya CompoundStmt" << endl;
		stack.push_back(&unit->getBranch());
	}
	else if (dynamic_cast<TranslationUnit*>(unit))
	{
		stack.push_back(&unit->getBranch());
	}
	else if (dynamic_cast<FunctionDeclReference*>(unit))
	{
		stack.push_back(&unit->getBranch());
	}
}

void TranslationUnit::print()
{
	wcout << name << L" <" << signature << L">";
}

void CompoundStatement::print()
{
	wcout << name << L" < Line: " << stringnumber << L" >";
}

void ReturnStatement::print()
{
	wcout << name << L" < Line: " << stringnumber << L" >";
}

vector<SyntaxUnit*>& SyntaxUnit::getBranch()
{
	return branch;
}


void VariableDeclReference::setVariable(Variable* _var)
{
	var = _var;
}


void FunctionDeclReference::setFunction(Function* f)
{
	function = f;
}

Function* FunctionDeclReference::getFunction()
{
	return function;
}



void FunctionCallReference::setFunction(Function* f)
{
	function = f;
}

Function* FunctionCallReference::getFunction()
{
	return function;
}

void ConstantDeclReference::setType(BasicAbstractType* t)
{
	constantType = t;
}

BasicAbstractType* ConstantDeclReference::getType()
{
	return constantType;
}

void OperatorReference::setOperator(BasicAbstractOperator* _op)
{
	op = _op;

	if (dynamic_cast<BinaryOperator*>(_op))
	{
		name = L"BinaryOperatorRef";
	}
	else if (dynamic_cast<UnaryOperator*>(_op))
	{
		name = L"UnaryOperatorRef";
	}
	else
	{
		name = L"UnresolvedOperatorRef";
	}
}

BasicAbstractOperator* OperatorReference::getOperator()
{
	return op;
}

void OperatorReference::print()
{
	wcout << name << L"(" << signature << L")";
}

void VariableDeclReference::print()
{
	wcout << name << L"(" << signature << L")";
}

Variable* VariableDeclReference::getVariable()
{
	return var;
}


void FunctionDeclReference::print()
{
	wcout << name << L"(" << signature << L")";
}

void FunctionCallReference::print()
{
	wcout << name << L"(" << signature << L")";
}

void ConstantDeclReference::print()
{
	wcout << name << L"(" << signature << L")";
}