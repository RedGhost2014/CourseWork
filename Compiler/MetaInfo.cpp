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











Type::Type()
{
	m_isRef = false;
	m_isDefined = false;

	internalTypes = nullptr;
	size = 1;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	countOfPtrs = 0;
}

Type::Type(wstring name)
{
	internalTypes = nullptr;
	size = 1;
	this->name = name;
	m_isConst = false;
	m_isUnsigned = false;
	m_isVolatile = false;
	countOfPtrs = 0;
}

vector<wstring>& Type::getSynonims()
{
	return synonyms;
}

Type::Type(wstring _name, size_t _size)
{
	name = _name;
	size = _size;
}
Type::Type(wstring _name, size_t _size, bool defined)
{
	name = _name;
	size = _size;
	m_isDefined = defined;
}

Type::Type(const Type& rhs)
{
	name = rhs.name;
	size = rhs.size;
	m_isConst = rhs.m_isConst;
	m_isUnsigned = rhs.m_isUnsigned;
	m_isVolatile = rhs.m_isVolatile;
	countOfPtrs = rhs.countOfPtrs;

	if (rhs.internalTypes != nullptr)
	{
		this->internalTypes = new vector<Variable*>;
		for (size_t i = 0; i < rhs.internalTypes->size(); i++)
		{
			internalTypes->push_back(rhs.internalTypes[0][i]);
		}
	}
}

Type& Type::operator=(const Type& rhs)
{
	name = rhs.name;
	size = rhs.size;
	m_isConst = rhs.m_isConst;
	m_isUnsigned = rhs.m_isUnsigned;
	m_isVolatile = rhs.m_isVolatile;
	countOfPtrs = rhs.countOfPtrs;

	if (rhs.internalTypes != nullptr)
	{
		this->internalTypes = new vector<Variable*>;
		for (size_t i = 0; i < rhs.internalTypes->size(); i++)
		{
			internalTypes->push_back(rhs.internalTypes[0][i]);
		}
	}
	else
		internalTypes = nullptr;

	return *this;
}



void Type::setSize(size_t _size)
{
	size = _size;
}

size_t Type::getSize() { return size; }
vector<Variable*>* Type::getInternal() { return internalTypes; }

Type& Type::setIsConst(bool b) 
{ 
	m_isConst = b;
	return *this;
}
Type& Type::setIsUnsigned(bool b)
{ 
	m_isUnsigned = b;
	return *this;
}
Type& Type::setIsVolatile(bool b) 
{
	m_isVolatile = b;
	return *this;
}
Type& Type::setIsRef(bool b)
{
	m_isRef = b;
	return *this;
}

Type& Type::setIsDefined(bool b)
{
	m_isDefined = b;
	return *this;
}

bool Type::isDefined()
{
	return m_isDefined;
}

Type& Type::setIsCountOfPtrs(size_t count) 
{ 
	countOfPtrs = count; 
	return *this;
}




Type& Variable::getType()
{
	return type;
}
void Variable::setType(Type& t)
{
	type = t;
}






Function::Function()
{
	m_isDefined = false;
	stackSize = 0;
}

vector<Type*>& Function::getReturnTypes() 
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

vector<Type*>& AbstractScopeMetaInformation::getExistedTypes() { return existedTypes; };
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

void GlobalScopeMetaInformation::pushType(Type* t)
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
	Type* currentT = MetaInfo.getTypeByName(start->getValue());
	vector<Variable*>* intertnaltypes = currentT->getInternal();

	if (intertnaltypes == nullptr)
	{
		intertnaltypes = new vector<Variable*>;
	}

	size_t resultSize = 0;
	for (size_t i = 0; i < existedVariables.size(); i++)
	{
		intertnaltypes->push_back(existedVariables[i]);
		resultSize += existedVariables[i]->getType().getSize();
	}
	currentT->setSize(resultSize);
}


void ClassScopeMetaInformation::pushFunction(Function* f)
{
	Log::pushError(L"Composite type can't contain functions.", start);
	//existedFunctions.push_back(f);
}

void ClassScopeMetaInformation::pushType(Type* t)
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
		resultSize += var->getType().getSize();
	}
	basicFunction->setStackSize(resultSize);
}

Function* FunctionScopeMetaInformation::getBasicFunction() { return basicFunction; }

void FunctionScopeMetaInformation::pushFunction(Function* f)
{
	Log::pushError(L"Function can't contain functions.", start);
	//existedFunctions.push_back(f);
}

void FunctionScopeMetaInformation::pushType(Type* t)
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

Type* MetaInformaton::getTypeByName(wstring name)
{
	Type* result = nullptr;

	for (AbstractScopeMetaInformation* meta : metaStack)
	{
		vector<Type*>& types = meta->getExistedTypes();
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

	toplevel->pushType(new Type(L"char", 1, true));
	toplevel->pushType(new Type(L"bool", 1, true));
	toplevel->pushType(new Type(L"short", 2, true));
	toplevel->pushType(new Type(L"int", 4, true));
	toplevel->pushType(new Type(L"float", 4, true));
	toplevel->pushType(new Type(L"double", 4, true));
	toplevel->pushType(new Type(L"long", 8, true));

	this->pushScope(toplevel);
}

vector<AbstractScopeMetaInformation*>& MetaInformaton::getMetaStack()
{
	return metaStack;
}