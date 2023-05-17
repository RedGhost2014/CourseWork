#include "AST.hpp"

SyntaxUnit& AbstractSyntaxTree::getHead()
{
	return head;
}

void SyntaxUnit::push(SyntaxUnit* su)
{
	this->branch.push_back(su);
}

void AbstractSyntaxTree::push(SyntaxUnit* unit)
{
	if (dynamic_cast<VariableDeclReference*>(unit))
	{
		wcout << "Ya varDecl" << endl;
	}
	if (dynamic_cast<CompoundStatement*>(unit))
	{
		wcout << "Ya CompoundStmt" << endl;
		//stack.push_back(&unit->getBranch());
		//stack.back()->push_back(&(unit->getBranch()));
		//head.getBranch().back()->getBranch();
	}
	//else
	//stack.back()->push_back(unit);

	//head.getBranch().push_back(unit);




	/*wstring element = unit->getValue();
	su->setFilename(head->getTree().back()->getValue());
	if (element == L"{" || element == L"[" || element == L"(")
	{
		stack.back()->push_back(su);
		stack.push_back(&su->getTree());
	}
	else if (element == L"}" || element == L"]" || element == L")")
	{
		if (stack.size() > 1)
			stack.pop_back();

		stack.back()->push_back(su);
	}
	else
	{
		stack.back()->push_back(su);
	}*/
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

void FunctionCallReference::setFunction(Function* f)
{
	function = f;
}

void ConstantDeclReference::setConstant(wstring _constant)
{
	constant = _constant;
}

void ConstantDeclReference::setType(BasicAbstractType* t)
{
	constantType = t;
}