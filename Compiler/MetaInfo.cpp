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




void BasicAbstractType::setIsCountOfPtrs(size_t count)
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

size_t BasicAbstractType::getSize()
{
	return size;
};

void BasicAbstractType::setSize(size_t _size)
{
	size = _size;
}

vector<wstring>& BasicAbstractType::getSynonims()
{
	return synonyms;
}

//BasicAbstractType* BasicAbstractType::clone()
//{
//	return new BasicAbstractType(*this);
//}














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

vector<Variable*>* CompositeType::getInternal()
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
	if (type != nullptr)
	{
		delete type;
	}

	if (primitive)
	{
		type = new PrimitiveType(*primitive);
	}
	else if (composite)
	{
		type = new CompositeType(*composite);
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





















AbstractScopeMetaInformation::AbstractScopeMetaInformation() {}

vector<BasicAbstractType*>& AbstractScopeMetaInformation::getExistedTypes() { return existedTypes; };
vector<Variable*>& AbstractScopeMetaInformation::getExistedVariables() { return existedVariables;  };
vector<Function*>& AbstractScopeMetaInformation::getExistedFunctions() { return existedFunctions; };

//void ScopeMetaInformation::printDefinedTypes()
//{
//	wcout << L"Defined Types:" << endl;
//	for (size_t i = 0; i < definedTypes.size(); i++)
//	{
//		wcout << L"Type[" << i << L"] = (" << definedTypes[i].getName() << ", " << definedTypes[i].getSize() << L")" << endl;
//	}
//}
//
//void ScopeMetaInformation::printDeclaredTypes()
//{
//	wcout << L"Declared Types: " << declaredTypes.size() << endl;
//	for (size_t i = 0; i < declaredTypes.size(); i++)
//	{
//		wcout << L"Type[" << i << L"] = (" << declaredTypes[i].getName() << ", " << declaredTypes[i].getSize() << L")" << endl;
//	}
//}

//
//bool ScopeMetaInformation::isTypeExist(wstring _typename)
//{
//	for (Type& v : declaredTypes)
//	{
//		if (v.getName() == _typename)
//			return true;
//	}
//
//	for (Type& v : definedTypes)
//	{
//		if (v.getName() == _typename)
//			return true;
//	}
//
//	return false;
//}
//
//
//bool MetaInformaton::isTypeExist(wstring _typename)
//{
//	for (ScopeMetaInformation& v : metaStack)
//	{
//		if (v.isTypeExist(_typename))
//		{
//			return true;
//		}
//	}
//	return false;
//}

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




ClassScopeMetaInformation::ClassScopeMetaInformation(LexicalUnit* lu) : start(lu) {}
ClassScopeMetaInformation::~ClassScopeMetaInformation() 
{
	BasicAbstractType* currentT = MetaInfo.getTypeByName(start->getValue());

	CompositeType* ct = dynamic_cast<CompositeType*>(currentT);

	if (ct == nullptr)
		return;

	vector<Variable*>* intertnaltypes = ct->getInternal();

	if (intertnaltypes == nullptr)
	{
		intertnaltypes = new vector<Variable*>;
	}

	size_t resultSize = 0;
	for (size_t i = 0; i < existedVariables.size(); i++)
	{
		intertnaltypes->push_back(existedVariables[i]);
		resultSize += existedVariables[i]->getType()->getSize();
	}
	currentT->setSize(resultSize);
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

Variable* MetaInformaton::getVariableByName(wstring name)
{
	for (size_t scopeindex = metaStack.size() - 1; scopeindex > 0; scopeindex--)
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

	this->pushScope(toplevel);
}

vector<AbstractScopeMetaInformation*>& MetaInformaton::getMetaStack()
{
	return metaStack;
}