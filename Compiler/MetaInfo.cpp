#include "MetaInfo.hpp"


MetaInformaton MetaInfo;


wstring IName::getName()
{
	return name;
}

void IName::setName(wstring _name)
{
	name = _name;
}




void BasicAbstractType::setCountOfPtrs(size_t count)
{
	countOfPtrs = count;
}

void BasicAbstractType::setIsStatic(bool condition)
{
	m_isStatic = condition;
}

void BasicAbstractType::setIsConst(bool condition)
{
	m_isConst = condition;
}
void BasicAbstractType::setIsRef(bool condition)
{
	m_isRef = condition;
}

void BasicAbstractType::setIsVolatile(bool condition)
{
	m_isVolatile = condition;
}

void BasicAbstractType::setSize(size_t _size)
{
	size = _size;
}

size_t BasicAbstractType::getSize()
{
	return size;
}

size_t BasicAbstractType::getCountOfPointers()
{
	return countOfPtrs;
}

vector<wstring>& BasicAbstractType::getSynonims()
{
	return synonyms;
}

bool BasicAbstractType::isStatic() { return m_isStatic; };
bool BasicAbstractType::isConst() { return m_isConst; };
bool BasicAbstractType::isRef() { return m_isRef; };
bool BasicAbstractType::isVolatile() { return m_isVolatile; }












PrimitiveType::PrimitiveType()
{
	size = 1;

	m_isRef = false;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	m_isStatic = false;

	countOfPtrs = 0;
}

PrimitiveType::PrimitiveType(wstring _name)
{
	size = 1;
	name = _name;

	m_isRef = false;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	m_isStatic = false;

	countOfPtrs = 0;
}

PrimitiveType::PrimitiveType(wstring _name, size_t _size)
{
	name = _name;
	size = _size;

	m_isRef = false;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	m_isStatic = false;

	countOfPtrs = 0;
}

void PrimitiveType::setIsUnsigned(bool condition)
{
	m_isUnsigned = condition;
}


PrimitiveType::PrimitiveType(const PrimitiveType& rhs)
{
	name = rhs.name;
	size = rhs.size;

	synonyms = rhs.synonyms;

	m_isConst = rhs.m_isConst;
	m_isUnsigned = rhs.m_isUnsigned;
	m_isVolatile = rhs.m_isVolatile;
	m_isStatic = rhs.m_isStatic;
	m_isRef = rhs.m_isRef;

	countOfPtrs = rhs.countOfPtrs;
}

PrimitiveType& PrimitiveType::operator=(const PrimitiveType& rhs)
{
	name = rhs.name;
	size = rhs.size;

	synonyms = rhs.synonyms;

	m_isConst = rhs.m_isConst;
	m_isUnsigned = rhs.m_isUnsigned;
	m_isVolatile = rhs.m_isVolatile;
	m_isStatic = rhs.m_isStatic;
	m_isRef = rhs.m_isRef;

	countOfPtrs = rhs.countOfPtrs;
	return *this;
}


PrimitiveType* PrimitiveType::clone()
{
	return new PrimitiveType(*this);
}











CompositeType::CompositeType()
{
	size = 1;

	m_isRef = false;
	m_isConst = false;
	m_isVolatile = false;
	m_isStatic = false;

	m_isDefined = false;
	this->internalTypes = nullptr;

	countOfPtrs = 0;
}

CompositeType::CompositeType(wstring _name)
{
	size = 1;
	name = _name;


	m_isRef = false;
	m_isConst = false;
	m_isVolatile = false;
	m_isStatic = false;

	m_isDefined = false;
	this->internalTypes = nullptr;

	countOfPtrs = 0;
}

CompositeType::~CompositeType()
{
	if (internalTypes != nullptr)
	{
		for (size_t i = 0; i < internalTypes->size(); i++)
		{
			delete (*internalTypes)[i];
		}
	}
}

CompositeType::CompositeType(const CompositeType& rhs)
{
	name = rhs.name;
	size = rhs.size;

	synonyms = rhs.synonyms;

	m_isConst = rhs.m_isConst;
	m_isVolatile = rhs.m_isVolatile;
	m_isStatic = rhs.m_isStatic;
	m_isRef = rhs.m_isRef;
	m_isDefined = rhs.m_isDefined;

	countOfPtrs = rhs.countOfPtrs;

	if (internalTypes != nullptr)
	{
		for (size_t i = 0; i < internalTypes->size(); i++)
		{
			delete (*internalTypes)[i];
		}
	}

	internalTypes = nullptr;

	if (rhs.internalTypes == nullptr)
	{
		return;
	}

	internalTypes = new vector<Variable*>;

	vector<Variable*>* rhsv = rhs.internalTypes;

	for (size_t i = 0; i < rhsv->size(); i++)
	{
		internalTypes->push_back(new Variable(*(rhsv[0][i])));
		//internalTypes->push_back((rhsv[0][i]));
	}
}

CompositeType& CompositeType::operator=(const CompositeType& rhs)
{
	name = rhs.name;
	size = rhs.size;

	synonyms = rhs.synonyms;

	m_isConst = rhs.m_isConst;
	m_isVolatile = rhs.m_isVolatile;
	m_isStatic = rhs.m_isStatic;
	m_isRef = rhs.m_isRef;

	m_isDefined = rhs.m_isDefined;

	countOfPtrs = rhs.countOfPtrs;

	if (internalTypes != nullptr)
	{
		for (size_t i = 0; i < internalTypes->size(); i++)
		{
			delete (*internalTypes)[i];
		}
	}

	internalTypes = nullptr;

	if (rhs.internalTypes == nullptr)
	{
		return *this;
	}

	internalTypes = new vector<Variable*>;

	vector<Variable*>* rhsv = rhs.internalTypes;

	for (size_t i = 0; i < rhsv->size(); i++)
	{
		internalTypes->push_back(new Variable(*(rhsv[0][i])));
	}
	return *this;
}


bool CompositeType::isDefined()
{
	return m_isDefined;
}

void CompositeType::setIsDefined(bool condition)
{
	m_isDefined = condition;
}

vector<Variable*>*& CompositeType::getInternal()
{
	return internalTypes;
}


CompositeType* CompositeType::clone()
{
	return new CompositeType(*this);
}







BasicAbstractType* Variable::getType()
{
	return type;
}

void Variable::setType(BasicAbstractType* t)
{
	type = t;
}

Variable::Variable(const Variable& rhs)
{
	PrimitiveType* primitive = dynamic_cast<PrimitiveType*>(rhs.type);
	CompositeType* composite = dynamic_cast<CompositeType*>(rhs.type);
	name = rhs.name;
	if (type != nullptr)
	{
		delete type;
	}

	if (primitive)
	{
		type = primitive;
		//type = new PrimitiveType(*primitive);
	}
	else if (composite)
	{
		type = composite;
		//type = new CompositeType(*composite);
	}
}





Function::Function()
{
	m_isDefined = false;
	stackSize = 0;
}

vector<BasicAbstractType*>& Function::getReturnTypes()
{
	return returnTypes;
}

vector<Function*>& Function::getFunctionOverloads()
{
	return functionOverloads;
}

vector<Variable*>& Function::getFunctionArguments()
{
	return functionArguments;
}

bool Function::isDefined()
{
	return m_isDefined;
}

void Function::setDefinitionStatus(bool status)
{
	m_isDefined = status;
}

size_t Function::getStackSize()
{
	return stackSize;
}

void Function::setStackSize(size_t newsize)
{
	stackSize = newsize;
}



int BasicAbstractOperator::getAssociativity()
{
	return associativity;
}

size_t BasicAbstractOperator::getPrecedence()
{
	return precedence;
}

void BasicAbstractOperator::setAssociativity(int _associativity)
{
	associativity = _associativity;
}

bool BasicAbstractOperator::isBuiltIn()
{
	return _isBuiltIn;
}

bool BasicAbstractOperator::isCanBeOverloaded()
{
	return canBeOverloaded;
}


UnaryOperator::UnaryOperator(wstring op, size_t _precedence, int _associativity, bool _canBeOverloaded, bool isBuiltIn)
{
	name = op;
	precedence = _precedence;

	canBeOverloaded = _canBeOverloaded;
	associativity = _associativity;
	_isBuiltIn = isBuiltIn;

	type = nullptr;
}

BasicAbstractType* UnaryOperator::getType()
{
	return type;
}

void UnaryOperator::setType(BasicAbstractType* t)
{
	type = t;
}


BinaryOperator::BinaryOperator(wstring op, size_t _precedence, int _associativity, bool _canBeOverloaded, bool isBuiltIn)
{
	name = op;
	precedence = _precedence;

	canBeOverloaded = _canBeOverloaded;
	associativity = _associativity;
	_isBuiltIn = isBuiltIn;

	leftType = nullptr;
	rightType = nullptr;
}

BasicAbstractType* BinaryOperator::getLeftType()
{
	return leftType;
}

BasicAbstractType* BinaryOperator::getRightType()
{
	return rightType;
}

void BinaryOperator::setLeftType(BasicAbstractType* t)
{
	leftType = t;
}

void BinaryOperator::setRightType(BasicAbstractType* t)
{
	rightType = t;
}



AbstractScopeMetaInformation::AbstractScopeMetaInformation() {}

vector<BasicAbstractType*>& AbstractScopeMetaInformation::getExistedTypes() { return existedTypes; };
vector<Variable*>& AbstractScopeMetaInformation::getExistedVariables() { return existedVariables; };
vector<Function*>& AbstractScopeMetaInformation::getExistedFunctions() { return existedFunctions; };
vector<BasicAbstractOperator*>& AbstractScopeMetaInformation::getExistedOperators() { return existedOperators; };

void GlobalScopeMetaInformation::pushFunction(Function* f)
{
	existedFunctions.push_back(f);
}

void GlobalScopeMetaInformation::pushType(BasicAbstractType* t)
{
	existedTypes.push_back(t);
}

void GlobalScopeMetaInformation::pushVariable(Variable* v)
{
	existedVariables.push_back(v);
}

void GlobalScopeMetaInformation::pushOperator(BasicAbstractOperator* op)
{
	existedOperators.push_back(op);
}





ClassScopeMetaInformation::ClassScopeMetaInformation(LexicalUnit* lu) : start(lu) {}
ClassScopeMetaInformation::~ClassScopeMetaInformation()
{
	BasicAbstractType* currentType = MetaInfo.getTypeByName(start->getValue());

	CompositeType* currentCompositeType = dynamic_cast<CompositeType*>(currentType);

	if (currentCompositeType == nullptr)
		return;

	vector<Variable*>*& intertnaltypes = currentCompositeType->getInternal();

	if (intertnaltypes == nullptr)
	{
		intertnaltypes = new vector<Variable*>;
	}

	size_t resultSize = 0;
	for (size_t i = 0; i < existedVariables.size(); i++)
	{
		BasicAbstractType* currentVType = existedVariables[i]->getType();

		if (currentVType->getName() == currentCompositeType->getName())
		{
			if (currentVType->getCountOfPointers() == 0)
			{
				Log::pushError(L"Composite type can't contain themselves.", start);
			}
			else
			{
				resultSize += currentVType->getSize();
				delete currentVType;
				existedVariables[i]->setType(currentCompositeType);
			}
		}
		else
		{
			resultSize += existedVariables[i]->getType()->getSize();
		}
		intertnaltypes->push_back(existedVariables[i]);
	}
	currentCompositeType->setSize(resultSize);
}


void ClassScopeMetaInformation::pushFunction(Function* f)
{
	Log::pushError(L"Composite type can't contain functions.", start);
	//existedFunctions.push_back(f);
}

void ClassScopeMetaInformation::pushType(BasicAbstractType* t)
{
	existedTypes.push_back(t);
}

void ClassScopeMetaInformation::pushVariable(Variable* v)
{
	existedVariables.push_back(v);
}

void ClassScopeMetaInformation::pushOperator(BasicAbstractOperator* op)
{
	existedOperators.push_back(op);
}




FunctionScopeMetaInformation::FunctionScopeMetaInformation(Function* currentFunction, LexicalUnit* _start)
	: basicFunction(currentFunction), start(_start) {}

FunctionScopeMetaInformation::~FunctionScopeMetaInformation()
{
	size_t resultSize = 0;
	for (Variable* var : existedVariables)
	{
		resultSize += var->getType()->getSize();
	}
	basicFunction->setStackSize(resultSize);

}

Function* FunctionScopeMetaInformation::getBasicFunction() { return basicFunction; }

void FunctionScopeMetaInformation::pushFunction(Function* f)
{
	Log::pushError(L"Function can't contain functions.", start);
	//existedFunctions.push_back(f);
}

void FunctionScopeMetaInformation::pushType(BasicAbstractType* t)
{
	existedTypes.push_back(t);
}

void FunctionScopeMetaInformation::pushVariable(Variable* v)
{
	existedVariables.push_back(v);
}

void FunctionScopeMetaInformation::pushOperator(BasicAbstractOperator* op)
{
	existedOperators.push_back(op);
}














void MetaInformaton::pushScope(AbstractScopeMetaInformation* scope)
{
	metaStack.push_back(scope);
}

void MetaInformaton::popScope()
{
	if (metaStack.size() > 1)
	{
		delete metaStack.back();
		metaStack.pop_back();
	}
}

AbstractScopeMetaInformation* MetaInformaton::back()
{
	return metaStack.back();
}

BasicAbstractType* MetaInformaton::getTypeByName(wstring name)
{
	BasicAbstractType* result = nullptr;

	for (AbstractScopeMetaInformation* meta : metaStack)
	{
		vector<BasicAbstractType*>& types = meta->getExistedTypes();
		for (size_t i = 0; i < types.size(); i++)
		{
			if (types[i]->getName() == name)
			{
				result = types[i];
				return result;
			}
			// TODO: synonims
			/*else
			{
				vector<wstring>& synonims = types[i]->getSynonims();
				for (size_t j = 0; j < synonims.size(); j++)
				{
					if(synonims[j] == name)
					{
						result = types[i];
						return result;
					}
				}
			}*/
		}
	}
	return result;
}

Function* MetaInformaton::getFunctionByName(wstring name)
{
	Function* result = nullptr;

	for (AbstractScopeMetaInformation* meta : metaStack)
	{
		vector<Function*>& functions = meta->getExistedFunctions();
		for (size_t i = 0; i < functions.size(); i++)
		{
			if (functions[i]->getName() == name)
			{
				result = functions[i];
				return result;
			}
			// TODO: synonims
		}
	}
	return result;
}

BasicAbstractOperator* MetaInformaton::getBinaryOperatorByName(wstring name)
{
	BasicAbstractOperator* result = nullptr;

	for (AbstractScopeMetaInformation* meta : metaStack)
	{
		vector<BasicAbstractOperator*>& operators = meta->getExistedOperators();
		for (size_t i = 0; i < operators.size(); i++)
		{
			BinaryOperator* concreteBinaryOp = dynamic_cast<BinaryOperator*>(operators[i]);

			if (concreteBinaryOp && concreteBinaryOp->getName() == name)
			{
				result = operators[i];
				return result;
			}
		}
	}
	return result;
}


BasicAbstractOperator* MetaInformaton::getUnaryOperatorByName(wstring name)
{
	BasicAbstractOperator* result = nullptr;

	for (AbstractScopeMetaInformation* meta : metaStack)
	{
		vector<BasicAbstractOperator*>& operators = meta->getExistedOperators();
		for (size_t i = 0; i < operators.size(); i++)
		{
			UnaryOperator* concreteUnaryOp = dynamic_cast<UnaryOperator*>(operators[i]);

			if (concreteUnaryOp && concreteUnaryOp->getName() == name)
			{
				result = operators[i];
				return result;
			}
		}
	}
	return result;
}


BasicAbstractOperator* MetaInformaton::getUnaryOperatorByNameWithAssociativity(wstring name, int _assoc)
{
	BasicAbstractOperator* result = nullptr;

	for (AbstractScopeMetaInformation* meta : metaStack)
	{
		vector<BasicAbstractOperator*>& operators = meta->getExistedOperators();
		for (size_t i = 0; i < operators.size(); i++)
		{
			UnaryOperator* concreteUnaryOp = dynamic_cast<UnaryOperator*>(operators[i]);

			if (concreteUnaryOp && concreteUnaryOp->getName() == name && concreteUnaryOp->getAssociativity() == _assoc)
			{
				result = operators[i];
				return result;
			}
		}
	}
	return result;
}

//BasicAbstractOperator* MetaInformaton::getOperatorByNameAndType(wstring name, const BasicAbstractType* leftType, const BasicAbstractType* rightType)
//{
//	BasicAbstractOperator* result = nullptr;
//
//	for (AbstractScopeMetaInformation* meta : metaStack)
//	{
//		vector<BasicAbstractOperator*>& operators = meta->getExistedOperators();
//		for (size_t i = 0; i < operators.size(); i++)
//		{
//			BinaryOperator* concreteBinaryOp = dynamic_cast<BinaryOperator*>(operators[i]);
//
//			if (concreteBinaryOp && concreteBinaryOp->getName() == name)
//			{
//				if ((isPrimitiveType((BasicAbstractType*)leftType) || concreteBinaryOp->getLeftType() == leftType)
//					&& (isPrimitiveType((BasicAbstractType*)rightType) || concreteBinaryOp->getRightType() == rightType))
//				{
//					result = operators[i];
//					return result;
//				}
//			}
//		}
//	}
//	return result;
//}
//
//BasicAbstractOperator* MetaInformaton::getOperatorByNameAndType(wstring name, const BasicAbstractType* type)
//{
//	BasicAbstractOperator* result = nullptr;
//
//	for (AbstractScopeMetaInformation* meta : metaStack)
//	{
//		vector<BasicAbstractOperator*>& operators = meta->getExistedOperators();
//		for (size_t i = 0; i < operators.size(); i++)
//		{
//			UnaryOperator* concreteUnaryOp = dynamic_cast<UnaryOperator*>(operators[i]);
//
//			if (concreteUnaryOp && concreteUnaryOp->getName() == name)
//			{
//				// TODO: Вероятно лажа и нужно переделать на конкретное сравнение.
//				if (isPrimitiveType((BasicAbstractType*)type) || concreteUnaryOp->getType() == type)
//				{
//					result = operators[i];
//					return result;
//				}
//			}
//		}
//	}
//	return result;
//}
//
//
//BasicAbstractOperator* MetaInformaton::getOperatorByNameAndTypeWithAssociativity(wstring name, const BasicAbstractType* type, int _assoc)
//{
//	BasicAbstractOperator* result = nullptr;
//
//	for (AbstractScopeMetaInformation* meta : metaStack)
//	{
//		vector<BasicAbstractOperator*>& operators = meta->getExistedOperators();
//		for (size_t i = 0; i < operators.size(); i++)
//		{
//			UnaryOperator* concreteUnaryOp = dynamic_cast<UnaryOperator*>(operators[i]);
//
//			if (concreteUnaryOp && concreteUnaryOp->getName() == name && concreteUnaryOp->getAssociativity() == _assoc)
//			{
//				// TODO: Вероятно лажа и нужно переделать на конкретное сравнение.
//				if (isPrimitiveType((BasicAbstractType*)type) || concreteUnaryOp->getType() == type)
//				{
//					result = operators[i];
//					return result;
//				}
//			}
//		}
//	}
//	return result;
//}

Variable* MetaInformaton::getVariableByName(wstring name)
{
	for (int scopeindex = metaStack.size() - 1; scopeindex >= 0; scopeindex--)
	{
		AbstractScopeMetaInformation* currentscope = metaStack[scopeindex];
		vector<Variable*>& vars = currentscope->getExistedVariables();
		for (size_t i = 0; i < vars.size(); i++)
		{
			if (vars[i]->getName() == name)
			{
				return vars[i];
			}
		}

		FunctionScopeMetaInformation* functionScope = dynamic_cast<FunctionScopeMetaInformation*>(currentscope);

		if (functionScope != nullptr)
		{
			vector<Variable*>& args = functionScope->getBasicFunction()->getFunctionArguments();
			for (size_t i = 0; i < args.size(); i++)
			{
				if (args[i]->getName() == name)
				{
					return args[i];
				}
			}
		}
	}
	return nullptr;
}

bool MetaInformaton::isPrimitiveType(BasicAbstractType* t)
{
	return dynamic_cast<PrimitiveType*>(t);
}

MetaInformaton::MetaInformaton()
{
	AbstractScopeMetaInformation* toplevel = new GlobalScopeMetaInformation();

	toplevel->pushType(new PrimitiveType(L"char", 1));
	toplevel->pushType(new PrimitiveType(L"bool", 1));
	toplevel->pushType(new PrimitiveType(L"short", 2));
	toplevel->pushType(new PrimitiveType(L"int", 4));
	toplevel->pushType(new PrimitiveType(L"float", 4));
	toplevel->pushType(new PrimitiveType(L"double", 4));
	toplevel->pushType(new PrimitiveType(L"long", 8));


	int leftAssociativity = 1;
	int rightAssociativity = -1;

	// https://en.cppreference.com/w/cpp/language/operator_precedence

	// Scope Resolution
	toplevel->pushOperator(new UnaryOperator(L"::", 1, leftAssociativity, false, true));

	// Member Access
	toplevel->pushOperator(new BinaryOperator(L".", 2, leftAssociativity, false, true));
	toplevel->pushOperator(new BinaryOperator(L"->", 2, leftAssociativity, false, true));

	// Postfix Increment/Decrement

	// Test pseudo Operator
	//toplevel->pushOperator(new UnaryOperator(L"?", 2, leftAssociativity, true, true));

	toplevel->pushOperator(new UnaryOperator(L"++", 2, leftAssociativity, true, true));
	toplevel->pushOperator(new UnaryOperator(L"--", 2, leftAssociativity, true, true));

	// Prefix Increment/Decrement
	toplevel->pushOperator(new UnaryOperator(L"++", 3, rightAssociativity, true, true));
	toplevel->pushOperator(new UnaryOperator(L"--", 3, rightAssociativity, true, true));

	toplevel->pushOperator(new UnaryOperator(L"+", 3, rightAssociativity, true, true));
	toplevel->pushOperator(new UnaryOperator(L"-", 3, rightAssociativity, true, true));
	toplevel->pushOperator(new UnaryOperator(L"!", 3, rightAssociativity, true, true));
	toplevel->pushOperator(new UnaryOperator(L"~", 3, rightAssociativity, true, true));

	toplevel->pushOperator(new UnaryOperator(L"*", 3, rightAssociativity, false, true));
	toplevel->pushOperator(new UnaryOperator(L"&", 3, rightAssociativity, false, true));
	toplevel->pushOperator(new UnaryOperator(L"sizeof", 3, rightAssociativity, false, true));

	toplevel->pushOperator(new BinaryOperator(L"*", 5, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"/", 5, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"%", 5, leftAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L"+", 6, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"-", 6, leftAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L"<<", 7, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L">>", 7, leftAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L"<", 9, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"<=", 9, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L">", 9, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L">=", 9, leftAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L"==", 10, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"!=", 10, leftAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L"&", 11, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"^", 12, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"|", 13, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"&&", 14, leftAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"||", 15, leftAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L"+=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"-=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"*=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"/=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"%=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"<<=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L">>=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"&=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"^=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"|=", 16, rightAssociativity, true, true));
	toplevel->pushOperator(new BinaryOperator(L"=", 16, rightAssociativity, true, true));

	toplevel->pushOperator(new BinaryOperator(L",", 17, leftAssociativity, false, true));


	this->pushScope(toplevel);
}

vector<AbstractScopeMetaInformation*>& MetaInformaton::getMetaStack()
{
	return metaStack;
}