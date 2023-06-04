#include "SyntaxAnalyzer.hpp"
#include "AST.hpp"
#include "RPNCreator.hpp"

SyntaxAnalyzer::SyntaxAnalyzer()
{
	this->tree = new AbstractSyntaxTree;
}

void SyntaxAnalyzer::addSourceFileToAST(File* f)
{
	SyntaxUnit* su = new SyntaxUnit;
	su->setName(f->getName());
	tree->getHead().getBranch().push_back(su);
}

void SyntaxAnalyzer::consumeLexicTree(AbstractLexicTree* alt)
{
	bypassLexicTree(alt->getHead()->getTree()[0]);
}

AbstractSyntaxTree* SyntaxAnalyzer::detachTree()
{
	AbstractSyntaxTree* currenttree = this->tree;
	this->tree = nullptr;
	return currenttree;
}

AbstractSyntaxTree* SyntaxAnalyzer::getTree()
{
	return tree;
}

void SyntaxAnalyzer::bypassLexicTree(LexicalUnit* head)
{
	vector<LexicalUnit*> parserow;

	vector<LexicalUnit*>& currentLexicalRow = head->getTree();


	size_t i = 0;
	while (i < currentLexicalRow.size())
	{
		wstring tmp = currentLexicalRow[i]->getValue();
		parserow.push_back(currentLexicalRow[i]);
		if (tmp == L"{" || tmp == L";")
		{
			//printLexicRow(parserow);

			while (!parserow.empty())
			{
				analyze(parserow);
				leftDropTillSemi(parserow);
			}
		}
		i++;
	}
	if (!parserow.empty())
	{
		//printLexicRow(parserow);
		analyze(parserow);
	}
}


void SyntaxAnalyzer::analyze(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow[0]->getValue() == L"}")
	{
		MetaInfo.popScope();
		tree->pop();
		lexicrow.erase(lexicrow.begin());
	}
	if (lexicrow.empty() || expectValue(lexicrow[0], L";"))
		return;

	if (isDeclarator(lexicrow)) {}
	else
	{

	}


	bool wasSemiColumn = false;
	for (size_t i = 0; i < lexicrow.size(); i++)
	{
		wstring tmp = lexicrow[i]->getValue();

		// more crutches to god of crutches
		if (tmp == L";")
		{
			wasSemiColumn = true;
		}

		if (tmp == L"{" && !wasSemiColumn)
		{
			bypassLexicTree(lexicrow[i]);
		}
	}
}


vector<wstring> SyntaxAnalyzer::qualifiers = {
	L"const", L"register", L"signed", L"static", L"unsigned", L"volatile", L"nodiscard", L"immutable", L"public", L"private"
};

vector<wstring> SyntaxAnalyzer::typequalifiers = {
	L"const", L"signed", L"register", L"static", L"unsigned", L"volatile", L"public", L"private"
};

bool SyntaxAnalyzer::isBelongToAlphabet(wstring literal, vector<wstring>& alphabet)
{
	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (literal == alphabet[i])
			return true;
	}
	return false;
}

bool SyntaxAnalyzer::expectValue(LexicalUnit* target, wstring expected)
{
	return (target->getValue() == expected);
}

bool SyntaxAnalyzer::expectKey(LexicalUnit* target, wstring expected)
{
	return (target->getKey() == expected);
}


void SyntaxAnalyzer::printLexicRow(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow.empty())
		return;
	static int z = 0;
	wcout << L"lexicrow[" << z << L"] = ";
	for (size_t i = 0; i < lexicrow.size(); i++)
	{
		wcout << lexicrow[i]->getValue() << " ";
	}
	wcout << endl;
	z++;
}

void SyntaxAnalyzer::leftDropTillSemi(vector<LexicalUnit*>& lexicrow)
{
	size_t i = 0;
	while (!lexicrow.empty() && lexicrow[i]->getValue() != L";")
	{
		lexicrow.erase(lexicrow.begin());
	}
	if (!lexicrow.empty())
		lexicrow.erase(lexicrow.begin());
}

bool SyntaxAnalyzer::isDefinition(vector<LexicalUnit*>& lexicrow)
{
	if (lexicrow.empty())
		return false;
	return lexicrow.back()->getValue() == L"{";
}

BasicAbstractType* SyntaxAnalyzer::retrieveType(vector<LexicalUnit*>& lexicrow, size_t* countOfParsedUnits)
{
	bool isStatic = false;
	bool isRef = false;

	size_t countOfUnsigned = 0;
	size_t countOfVolatile = 0;
	size_t countOfPtrs = 0;
	size_t countOfConst = 0;
	BasicAbstractType* pureType = nullptr;
	LexicalUnit* pureTypeName = nullptr;

	// resulted count of parsed lexical units also stands for start position for next parse
	size_t i = *countOfParsedUnits;

	while (i < lexicrow.size())
	{
		wstring key = lexicrow[i]->getKey();
		wstring value = lexicrow[i]->getValue();

		pureType = MetaInfo.getTypeByName(value);

		if (key == L"Identifier")
		{
			pureTypeName = lexicrow[i];
			break;
		}

		if (key == L"Keyword" && pureType)
		{
			pureTypeName = lexicrow[i];
			break;
		}
		if (key == L"Keyword")
		{
			if (value == L"static")
			{
				if (!isStatic)
					isStatic = true;
				else
				{
					Log::pushError(L"Storage class can be declared only once.", lexicrow[i]);
				}
			}
			else if (value == L"const")
			{
				countOfConst++;
			}
			else if (value == L"unsigned")
			{
				countOfUnsigned++;
			}
			else if (value == L"volatile")
			{
				countOfVolatile++;
			}
		}
		i++;
	}

	if (pureType == nullptr)
	{
		return nullptr;
	}

	if (dynamic_cast<PrimitiveType*>(MetaInfo.getTypeByName(pureTypeName->getValue())) && countOfUnsigned > 1)
	{
		countOfUnsigned = 1;
		Log::pushWarning(L"Qualifier \"unsigned\" repeated more that 1 time.", lexicrow[0]);
	}
	else if (dynamic_cast<CompositeType*>(MetaInfo.getTypeByName(pureTypeName->getValue())) && countOfUnsigned > 0)
	{
		countOfUnsigned = 0;
		Log::pushWarning(L"Qualifier \"unsigned\" can be over Composite Type but does nothing.", lexicrow[0]);
	}


	if (countOfConst > 1)
	{
		countOfConst = 1;
		Log::pushWarning(L"Qualifier \"const\" repeated more that 1 time.", lexicrow[0]);
	}
	if (countOfVolatile > 1)
	{
		countOfVolatile = 1;
		Log::pushWarning(L"Qualifier \"volatile\" repeated more that 1 time.", lexicrow[0]);
	}

	i++;

	while (i < lexicrow.size() && lexicrow[i]->getKey() == L"Operator")
	{
		wstring opvalue = lexicrow[i]->getValue();
		if (opvalue == L"," || opvalue == L"(")
		{
			break;
		}
		if (opvalue == L"*")
		{
			if (isRef)
			{
				Log::pushError(ErrorMessage::syntaxError + L"Reference cannot be over pointer.", lexicrow[i]);
			}
			countOfPtrs++;
		}
		else if (opvalue == L"&")
		{
			if (!isRef)
			{
				isRef = true;
			}
			else
			{
				Log::pushError(ErrorMessage::syntaxError + L"Rvalue reference does not supported.", lexicrow[i]);
			}
		}
		else
		{
			Log::pushError(ErrorMessage::syntaxError + L"\'*\' or \'&\' expected.", lexicrow[i]);
		}
		i++;
	}

	BasicAbstractType* newType = nullptr;
	
	PrimitiveType* possiblePrimitive = dynamic_cast<PrimitiveType*>(MetaInfo.getTypeByName(pureTypeName->getValue()));
	CompositeType* possibleComposite = dynamic_cast<CompositeType*>(MetaInfo.getTypeByName(pureTypeName->getValue()));

	if (possiblePrimitive)
	{
		newType = new PrimitiveType(*possiblePrimitive);
		PrimitiveType* primitive = dynamic_cast<PrimitiveType*>(newType);

		if (countOfUnsigned > 0)
			primitive->setIsUnsigned(true);
	}
	else if (possibleComposite)
	{
		newType = new CompositeType(*possibleComposite);
	}

	newType->setCountOfPtrs(countOfPtrs);
	if (countOfPtrs > 0)
		newType->setSize(4);
	if (countOfConst > 0)
		newType->setIsConst(true);
	if (countOfVolatile > 0)
		newType->setIsVolatile(true);

	newType->setIsRef(isRef);
	newType->setIsStatic(isStatic);

	*countOfParsedUnits = i;

	return newType;
}

bool SyntaxAnalyzer::isTypedefDeclarator(vector<LexicalUnit*>& lexicrow)
{
	size_t i = 0;

	if (!expectValue(lexicrow[i], L"typedef"))
		return false;

	lexicrow.erase(lexicrow.begin());

	size_t countOfParsed = 0;
	BasicAbstractType* what = retrieveType(lexicrow, &countOfParsed);

	if (what == nullptr)
		return false;

	if (i < lexicrow.size() && MetaInfo.getTypeByName(lexicrow[i]->getValue()) != nullptr)
	{
		Log::pushError(ErrorMessage::syntaxError + L"Type declarator \"" + lexicrow[i]->getValue() + L"\" already defined.", lexicrow[i]);
		return false;
	}
	//what->setName();
	what->getSynonims().push_back(lexicrow[i]->getValue());
	MetaInfo.back()->pushType(what);

	return true;
}

bool SyntaxAnalyzer::isClassDeclarator(vector<LexicalUnit*>& lexicrow)
{
	// TODO: Check Flag
	static bool isFixit = true;

	wstring keyword = lexicrow[0]->getValue();
	if ((keyword == L"struct" || keyword == L"class"))
	{
		if (lexicrow[1]->getKey() != L"Identifier")
		{
			Log::pushError(ErrorMessage::syntaxError + L"Identifier expected.", lexicrow[1]);
			return false;
		}
		if (expectValue(lexicrow[2], L";"))
		{
			wstring name = lexicrow[1]->getValue();
			if (MetaInfo.getTypeByName(name) == nullptr)
				MetaInfo.back()->pushType(new CompositeType(name));
			return true;
		}
		else if (expectValue(lexicrow[2], L"{"))
		{
			wstring name = lexicrow[1]->getValue();
			BasicAbstractType* t = MetaInfo.getTypeByName(name);
			CompositeType* actualT = dynamic_cast<CompositeType*>(t);
			if (t == nullptr)
			{
				actualT = new CompositeType(name);
				MetaInfo.back()->pushType(actualT);
			}
			else if (actualT && actualT->isDefined())
			{
				Log::pushError(L"Type already defined.", lexicrow[1]);
			}
			actualT->setIsDefined(true);

			AbstractScopeMetaInformation* scope = new ClassScopeMetaInformation(lexicrow[1]);
			MetaInfo.pushScope(scope);
			return true;
		}
		// TODO: Check Flag && Insert Semicolumn function
		else if (isFixit)
		{
			LexicalUnit* s = new LexicalUnit(lexicrow[1]->getLine());
			s->setFilename(lexicrow[1]->getFilename());
			s->setKey(L"End of line");
			s->setValue(L";");

			auto it = lexicrow.begin();
			lexicrow.insert(it + 2, s);

			Log::pushWarning(ErrorMessage::syntaxError + L"\';\' missed but emplaced by --fixit.", lexicrow[1]);

			wstring name = lexicrow[1]->getValue();
			if (!MetaInfo.getTypeByName(name))
				MetaInfo.back()->pushType(new CompositeType(name));



			//leftDropTillSemi(lexicrow);

			return true;
		}
		else
			Log::pushError(ErrorMessage::syntaxError + L"\';\' or \'{\' expected.", lexicrow[1]);
	}
	return false;
}

bool SyntaxAnalyzer::isVariableDeclarator(vector<LexicalUnit*>& lexicrow)
{
	size_t i = 0;
	BasicAbstractType* t = retrieveType(lexicrow, &i);
	if (t == nullptr)
	{
		return false;
	}

	if (i < lexicrow.size() && expectKey(lexicrow[i], L"Identifier"))
	{
		size_t j = i + 1;

		if (j < lexicrow.size() && (expectValue(lexicrow[j], L"(") || expectValue(lexicrow[j], L",")))
		{
			return false;
		}
		else if (j < lexicrow.size() && (!expectValue(lexicrow[j], L";") && !expectValue(lexicrow[j], L"=")))
		{
			Log::pushError(ErrorMessage::syntaxError + L"\';\' or \'=\' expected.", lexicrow[1]);
			return false;
		}

		if (MetaInfo.getVariableByName(lexicrow[i]->getValue()) != nullptr)
		{
			Log::pushError(wstring(L"Identifier \"") + lexicrow[i]->getValue() + L"\" already defined.", lexicrow[i]);
			return false;
		}

		Variable* var = new Variable;
		var->setName(lexicrow[i]->getValue());

		var->setType(t);

		MetaInfo.back()->pushVariable(var);

		if (expectValue(lexicrow[j], L"="))
		{	
			if (dynamic_cast<ClassScopeMetaInformation*>(MetaInfo.back()))
			{
				Log::pushError(L"Assigment does not allowed in class declaration.", lexicrow[j]);
				return true;
			}

			CompoundStatement* cs = new CompoundStatement(lexicrow[j]->getLine());
			this->getTree()->push(cs);

			vector<LexicalUnit*> subsequence;
			for (size_t i = j - 1; i < lexicrow.size(); i++)
			{
				subsequence.push_back(lexicrow[i]);
			}
			ReversePolishNotationCreator rpnc;
			rpnc.createRPN(subsequence, this->tree);

			this->tree->pop();
		}

		return true;
	}

	return false;
}









void SyntaxAnalyzer::parseFunctionParentheses(vector<LexicalUnit*>& internalLexicRow, vector<Variable*>& functionArgumentsToFill)
{
	size_t parseIndex = 0;

	while (parseIndex < internalLexicRow.size())
	{
		BasicAbstractType* t = retrieveType(internalLexicRow, &parseIndex);

		if (t == nullptr)
			break;

		Variable* var = new Variable;
		var->setType(t);

		if (expectKey(internalLexicRow[parseIndex], L"Identifier"))
		{
			var->setName(internalLexicRow[parseIndex]->getValue());
			parseIndex++;
		}

		functionArgumentsToFill.push_back(var);
	}
}


bool SyntaxAnalyzer::isFunctionDeclarator(vector<LexicalUnit*>& lexicrow)
{
	static bool isFixit = false;

	size_t i = 0;
	BasicAbstractType* mainType = retrieveType(lexicrow, &i);

	if (mainType == nullptr)
	{
		return false;
	}

	if (i < lexicrow.size() && (expectKey(lexicrow[i], L"Identifier") || expectValue(lexicrow[i], L",")))
	{
		size_t j = i + 1;
		// One-return-type-function
		if (j < lexicrow.size() && expectKey(lexicrow[i], L"Identifier") && expectValue(lexicrow[j], L"("))
		{
			Function* f = new Function;

			f->setName(lexicrow[i]->getValue());
			f->getReturnTypes().push_back(mainType);

			vector<LexicalUnit*>& functionParenthesis = lexicrow[j]->getTree();

			parseFunctionParentheses(functionParenthesis, f->getFunctionArguments());

			j += 2;

			if (j < lexicrow.size())
			{
				// TODO: Make check for diff functions have same arguments count/type
				if (expectValue(lexicrow[j], L";"))
				{
					Function* possibleSynonim = MetaInfo.getFunctionByName(f->getName());
					if (possibleSynonim)
						possibleSynonim->getFunctionOverloads().push_back(f);
					else
						MetaInfo.back()->pushFunction(f);
					return true;
				}
				else if (expectValue(lexicrow[j], L"{"))
				{
					Function* possibleSynonim = MetaInfo.getFunctionByName(f->getName());
					if (possibleSynonim)
						possibleSynonim->getFunctionOverloads().push_back(f);
					else
						MetaInfo.back()->pushFunction(f);

					f->setDefinitionStatus(true);

					FunctionDeclReference* functiondecl = new FunctionDeclReference;
					functiondecl->setFunction(f);
					this->tree->push(functiondecl);

					AbstractScopeMetaInformation* scope = new FunctionScopeMetaInformation(f, lexicrow[i]);
					MetaInfo.pushScope(scope);
					return true;
				}
				// TODO: Check Flag && Insert Semicolumn function
				else if (isFixit)
				{

				}
				else
					Log::pushError(ErrorMessage::syntaxError + L"\';\' or \'{\' expected.", lexicrow[j]);
			}
		}
		// Two-return-type-function
		else if (expectValue(lexicrow[i], L","))
		{
			i++;
			BasicAbstractType* additionalType = retrieveType(lexicrow, &i);
			if (additionalType == nullptr)
			{
				Log::pushError(ErrorMessage::syntaxError + L"Additional type expected.", lexicrow[i]);
				return false;
			}

			j = i + 1;

			if (j < lexicrow.size() && expectKey(lexicrow[i], L"Identifier") && expectValue(lexicrow[j], L"("))
			{
				Function* f = new Function;

				f->setName(lexicrow[i]->getValue());
				f->getReturnTypes().push_back(mainType);
				f->getReturnTypes().push_back(additionalType);

				vector<LexicalUnit*>& functionParenthesis = lexicrow[j]->getTree();

				parseFunctionParentheses(functionParenthesis, f->getFunctionArguments());
				j += 2;

				if (j < lexicrow.size())
				{
					// TODO: Make check for diff functions have same arguments count/type
					if (expectValue(lexicrow[j], L";"))
					{
						Function* possibleSynonim = MetaInfo.getFunctionByName(f->getName());
						if (possibleSynonim)
							possibleSynonim->getFunctionOverloads().push_back(f);
						else
							MetaInfo.back()->pushFunction(f);
						return true;
					}
					else if (expectValue(lexicrow[j], L"{"))
					{
						Function* possibleSynonim = MetaInfo.getFunctionByName(f->getName());
						if (possibleSynonim)
							possibleSynonim->getFunctionOverloads().push_back(f);
						else
							MetaInfo.back()->pushFunction(f);

						f->setDefinitionStatus(true);

						FunctionDeclReference* functiondecl = new FunctionDeclReference;
						functiondecl->setFunction(f);
						this->tree->push(functiondecl);

						AbstractScopeMetaInformation* scope = new FunctionScopeMetaInformation(f, lexicrow[i]);
						MetaInfo.pushScope(scope);
						return true;
					}
					// TODO: Check Flag && Insert Semicolumn function
					else if (isFixit)
					{

					}
					else
						Log::pushError(ErrorMessage::syntaxError + L"\';\' or \'{\' expected.", lexicrow[j]);
				}
			}
		}
		else
		{
			// TODO: errors
		}
	}
	return false;
}

bool SyntaxAnalyzer::isDeclarator(vector<LexicalUnit*>& lexicrow)
{
	if (isTypedefDeclarator(lexicrow))
	{
		return true;
	}
	else if (isClassDeclarator(lexicrow))
	{
		if (isDefinition(lexicrow))
		{
			wcout << L"Defined new type: " << lexicrow[1]->getValue() << endl;
		}
		else
		{
			wcout << L"Declared new type: " << lexicrow[1]->getValue() << endl;
		}
		return true;
	}
	else if (isVariableDeclarator(lexicrow))
	{
		wcout << L"Declared new variable: ";
		for (size_t i = 0; i < lexicrow.size(); i++)
		{
			wcout << lexicrow[i]->getValue() << L" ";
		}
		wcout << endl;
		return true;
	}
	else if (isFunctionDeclarator(lexicrow))
	{
		wcout << L"Declared new function: ";
		for (size_t i = 0; i < lexicrow.size(); i++)
		{
			wcout << lexicrow[i]->getValue() << L" ";
		}
		wcout << endl;
		return true;
	}

	return false;
}