#include "RPNCreator.hpp"
void ReversePolishNotationCreator::createRPN(vector<LexicalUnit*>& sequence, AbstractSyntaxTree* treeToFill)
{
	vector<SyntaxUnit*> stack;
	vector<SyntaxUnit*> output;

	resolveLexicSequenceIntoSyntax(sequence, output);

	vector<SyntaxUnit*> rpnoutput;

	for (SyntaxUnit* su : output)
	{
		determineSyntaxUnit(su, stack, rpnoutput);
	}

	while (!stack.empty())
	{
		SyntaxUnit* stacktop = stack.back();
		stack.pop_back();
		rpnoutput.push_back(stacktop);
	}

	for (size_t i = 0; i < rpnoutput.size(); i++)
	{
		treeToFill->push(rpnoutput[i]);
	}
}

void ReversePolishNotationCreator::determineSyntaxUnit(SyntaxUnit* currentUnit, vector<SyntaxUnit*>& stack, vector<SyntaxUnit*>& output)
{
	if (dynamic_cast<ConstantDeclReference*>(currentUnit) || dynamic_cast<VariableDeclReference*>(currentUnit))
	{
		output.push_back(currentUnit);
	}
	else if (dynamic_cast<FunctionCallReference*>(currentUnit))
	{
		stack.push_back(currentUnit);
	}
	else if (dynamic_cast<OperatorReference*>(currentUnit))
	{
		OperatorReference* currentOpRef = dynamic_cast<OperatorReference*>(currentUnit);

		BasicAbstractOperator* currentOp = currentOpRef->getOperator();

		if (currentOp == nullptr)
		{
			stack.push_back(currentOpRef);
			return;
		}

		if (currentOp->getName() == L",")
			return;

		if (!stack.empty())
		{
			OperatorReference* stackTopOperatorRef = dynamic_cast<OperatorReference*>(stack.back());

			while (stackTopOperatorRef && stackTopOperatorRef->getOperator()->getPrecedence() <= currentOp->getPrecedence())
			{
				if (currentOp->getPrecedence() == stackTopOperatorRef->getOperator()->getPrecedence() && currentOp->getAssociativity() < 0)
					break;

				if (stack.empty())
				{
					break;
				}

				output.push_back(stack.back());
				stack.pop_back();

				if (stack.empty())
				{
					break;
				}

				stackTopOperatorRef = dynamic_cast<OperatorReference*>(stack.back());
			}
		}
		stack.push_back(currentOpRef);
	}
	else if (currentUnit->getName() == L"(")
	{
		stack.push_back(currentUnit);
	}
	else if (currentUnit->getName() == L")")
	{
		if (!stack.empty())
		{
			SyntaxUnit* top = nullptr;

			while (1)
			{
				top = stack.back();

				if (top->getName() == L"(")
				{
					break;
				}

				output.push_back(top);
				stack.pop_back();
			}
			stack.pop_back();

			if (!stack.empty() && dynamic_cast<FunctionCallReference*>(stack.back()))
			{
				output.push_back(stack.back());
				stack.pop_back();
			}
		}
	}
}

void ReversePolishNotationCreator::resolveLexicSequenceIntoSyntax(vector<LexicalUnit*>& sequence, vector<SyntaxUnit*>& result)
{
	vector<vector<LexicalUnit*>> sequenceStack;
	vector<size_t> sequenceIndexStack;

	wstring operatorSignatureSequence = L"";

	size_t rollBackIndex = 0;

	SyntaxUnit* last = nullptr;
	SyntaxUnit* lastNonOp = nullptr;
	SyntaxUnit* lastOp = nullptr;

	size_t i = 0;

	sequenceStack.push_back(sequence);

	while (1)
	{
		if (!sequenceStack.empty() && i == sequenceStack.back().size())
		{
			if (!sequenceStack.empty())
				sequenceStack.pop_back();

			if (!sequenceIndexStack.empty())
				i = sequenceIndexStack.back();

			if (!sequenceIndexStack.empty())
				sequenceIndexStack.pop_back();

			if (operatorSignatureSequence != L"")
			{
				OperatorReference* op = new OperatorReference();

				op->setOperator(MetaInfo.getUnaryOperatorByName(operatorSignatureSequence));
				op->setSignature(operatorSignatureSequence);

				result.push_back(op);

				operatorSignatureSequence = L"";
			}
			//lastOp = nullptr;
			//lastNonOp = nullptr;
			//last = nullptr;
		}

		if (sequenceStack.empty())
		{
			break;
		}

		if (sequenceStack.back()[i]->getKey() == L"Number")
		{
			ConstantDeclReference* constant = new ConstantDeclReference();
			constant->setSignature(sequenceStack.back()[i]->getValue());

			if (lastNonOp == nullptr && operatorSignatureSequence != L"")
			{
				resolveUnaryOperatorSignature(operatorSignatureSequence, result);

				operatorSignatureSequence = L"";
			}
			else if (operatorSignatureSequence != L"")
			{
				resolveBinaryOperatorSignature(operatorSignatureSequence, result);

				operatorSignatureSequence = L"";
			}

			result.push_back(constant);

			last = constant;
			lastNonOp = constant;
		}
		else if (sequenceStack.back()[i]->getKey() == L"Identifier")
		{
			VariableDeclReference* var = new VariableDeclReference();
			var->setSignature(sequenceStack.back()[i]->getValue());
			var->setVariable(MetaInfo.getVariableByName(sequenceStack.back()[i]->getValue()));

			// Meet unary operator on start of the sequence
			if (lastNonOp == nullptr && operatorSignatureSequence != L"")
			{
				OperatorReference* op = new OperatorReference();
				op->setOperator(MetaInfo.getUnaryOperatorByName(operatorSignatureSequence));

				result.push_back(op);

				//resolveUnaryOperatorSignature(var, operatorSignatureSequence, result);

				operatorSignatureSequence = L"";
			}
			else if (operatorSignatureSequence != L"")
			{
				resolveBinaryOperatorSignature(operatorSignatureSequence, result);
				operatorSignatureSequence = L"";
			}

			result.push_back(var);

			last = var;
			lastNonOp = var;
		}
		else if (sequenceStack.back()[i]->getKey() == L"Operator")
		{
			if (sequenceStack.back()[i]->getValue() == L"(")
			{
				VariableDeclReference* lastvar = dynamic_cast<VariableDeclReference*>(last);
				if (lastvar)
				{
					FunctionCallReference* functionCall = new FunctionCallReference();

					functionCall->setFunction(MetaInfo.getFunctionByName(lastvar->getSignature()));
					functionCall->setSignature(lastvar->getSignature());

					result.pop_back();
					result.push_back(functionCall);

					last = functionCall;
					lastNonOp = functionCall;

					SyntaxUnit* parenthesis = new SyntaxUnit;
					parenthesis->setName(L"(");
					result.push_back(parenthesis);
				}
				else
				{
					if (operatorSignatureSequence != L"")
					{
						//OperatorReference* op = new OperatorReference;
						//op->setSignature(operatorSignatureSequence);

						resolveBinaryOperatorSignature(operatorSignatureSequence, result);

						/*if (lastNonOp)
							op->setOperator(MetaInfo.getBinaryOperatorByName(operatorSignatureSequence));
						else
							op->setOperator(MetaInfo.getUnaryOperatorByName(operatorSignatureSequence));

						result.push_back(op);*/
					}

					SyntaxUnit* parenthesis = new SyntaxUnit;
					parenthesis->setName(L"(");
					result.push_back(parenthesis);

					lastNonOp = nullptr;
					last = nullptr;
					operatorSignatureSequence = L"";
				}

				sequenceStack.push_back(sequenceStack.back()[i]->getTree());
				i++;
				sequenceIndexStack.push_back(i);
				i = 0;
				continue;
			}
			else if (sequenceStack.back()[i]->getValue() == L")")
			{
				SyntaxUnit* parenthesis = new SyntaxUnit;
				parenthesis->setName(L")");
				result.push_back(parenthesis);
			}
			else
			{
				last = nullptr;
				operatorSignatureSequence += sequenceStack.back()[i]->getValue();
			}
		}
		i++;
	}
}
void ReversePolishNotationCreator::resolveUnaryOperatorSignature(wstring operatorSignatureSequence, vector<SyntaxUnit*>& result)
{
	BasicAbstractOperator* basicOp = MetaInfo.getUnaryOperatorByName(operatorSignatureSequence);

	if (basicOp && basicOp->isBuiltIn())
	{
		OperatorReference* opref = new OperatorReference();

		opref->setOperator(basicOp);
		opref->setSignature(operatorSignatureSequence);

		result.push_back(opref);
		return;
	}
	else
	{
		wstring rhsOPLeftPart = operatorSignatureSequence;
		wstring rhsOPRightPart = L"";

		while (rhsOPLeftPart != L"" || rhsOPRightPart != L"")
		{
			basicOp = MetaInfo.getUnaryOperatorByNameWithAssociativity(rhsOPRightPart, -1);

			if (basicOp)
			{
				OperatorReference* rhsUnaryPostPreRef = new OperatorReference();

				rhsUnaryPostPreRef->setOperator(basicOp);
				rhsUnaryPostPreRef->setSignature(rhsOPRightPart);

				result.push_back(rhsUnaryPostPreRef);
				rhsOPRightPart.clear();

				for (size_t i = 0; i < rhsOPRightPart.length(); i++)
				{
					rhsOPLeftPart.pop_back();
				}
			}

			if (!rhsOPLeftPart.empty())
			{
				rhsOPRightPart += rhsOPLeftPart.front();

				rhsOPLeftPart.erase(rhsOPLeftPart.begin());
			}
			else if (rhsOPRightPart != L"")
			{
				OperatorReference* rhsUnaryPostPreRef = new OperatorReference();
				basicOp = MetaInfo.getUnaryOperatorByNameWithAssociativity(rhsOPRightPart, -1);

				rhsUnaryPostPreRef->setOperator(basicOp);
				rhsUnaryPostPreRef->setSignature(rhsOPRightPart);

				result.push_back(rhsUnaryPostPreRef);

				break;
			}
		}
	}
}


void ReversePolishNotationCreator::resolveBinaryOperatorSignature(wstring operatorSignatureSequence, vector<SyntaxUnit*>& result)
{
	BasicAbstractOperator* basicOp = MetaInfo.getBinaryOperatorByName(operatorSignatureSequence);

	if (basicOp && basicOp->isBuiltIn())
	{
		OperatorReference* opref = new OperatorReference();

		opref->setOperator(basicOp);
		opref->setSignature(operatorSignatureSequence);

		result.push_back(opref);
		return;
	}
	else if (basicOp == nullptr)
	{
		wstring unresolvedSequence = operatorSignatureSequence;
		wstring sequence = unresolvedSequence;

		wstring lhsUnaryPostOp = L"";
		wstring binaryOp = L"";
		wstring rhsUnaryPreOp = L"";

		BasicAbstractOperator* resultBinaryOp = nullptr;
		BasicAbstractOperator* resultLhsUnaryPostOp = nullptr;
		BasicAbstractOperator* resultRhsUnaryPreOp = nullptr;

		while (sequence != L"")
		{
			lhsUnaryPostOp += sequence.front();
			sequence.erase(sequence.begin());
			resultLhsUnaryPostOp = MetaInfo.getUnaryOperatorByNameWithAssociativity(lhsUnaryPostOp, 1);

			if (resultLhsUnaryPostOp)
			{
				//lhsPostOpSignature = lhsUnaryPostOp;
				break;
			}
		}

		if (resultLhsUnaryPostOp == nullptr)
		{
			sequence = unresolvedSequence;
			lhsUnaryPostOp = L"";
		}
		else
		{
			unresolvedSequence = sequence;
		}

		if (lhsUnaryPostOp != L"")
		{
			OperatorReference* lhsUnaryPostOpRef = new OperatorReference();

			lhsUnaryPostOpRef->setOperator(resultLhsUnaryPostOp);
			lhsUnaryPostOpRef->setSignature(lhsUnaryPostOp);
			result.push_back(lhsUnaryPostOpRef);
		}

		while (sequence != L"")
		{
			binaryOp = sequence;

			resultBinaryOp = MetaInfo.getBinaryOperatorByName(binaryOp);

			if (resultBinaryOp)
			{
				break;
			}

			rhsUnaryPreOp.insert(rhsUnaryPreOp.begin(), sequence.back());

			sequence.pop_back();
		}

		if (binaryOp != L"")
		{
			OperatorReference* opBinRef = new OperatorReference();

			opBinRef->setOperator(resultBinaryOp);
			opBinRef->setSignature(binaryOp);
			result.push_back(opBinRef);
		}

		if ((resultRhsUnaryPreOp = MetaInfo.getUnaryOperatorByNameWithAssociativity(rhsUnaryPreOp, -1)) != nullptr)
		{
			OperatorReference* rhsUnaryPostPreRef = new OperatorReference();

			rhsUnaryPostPreRef->setOperator(resultRhsUnaryPreOp);
			rhsUnaryPostPreRef->setSignature(rhsUnaryPreOp);

			result.push_back(rhsUnaryPostPreRef);
		}
		else
		{
			wstring rhsOPLeftPart = rhsUnaryPreOp;
			wstring rhsOPRightPart = L"";

			while (rhsOPLeftPart != L"" || rhsOPRightPart != L"")
			{
				resultRhsUnaryPreOp = MetaInfo.getUnaryOperatorByNameWithAssociativity(rhsOPRightPart, -1);

				if (resultRhsUnaryPreOp)
				{
					OperatorReference* rhsUnaryPostPreRef = new OperatorReference();

					rhsUnaryPostPreRef->setOperator(resultRhsUnaryPreOp);
					rhsUnaryPostPreRef->setSignature(rhsOPRightPart);

					result.push_back(rhsUnaryPostPreRef);
					rhsOPRightPart.clear();

					for (size_t i = 0; i < rhsOPRightPart.length(); i++)
					{
						rhsOPLeftPart.pop_back();
					}
				}

				if (!rhsOPLeftPart.empty())
				{
					rhsOPRightPart += rhsOPLeftPart.front();

					rhsOPLeftPart.erase(rhsOPLeftPart.begin());
				}
				else if (rhsOPRightPart != L"")
				{
					OperatorReference* rhsUnaryPostPreRef = new OperatorReference();
					resultRhsUnaryPreOp = MetaInfo.getUnaryOperatorByNameWithAssociativity(rhsOPRightPart, -1);

					rhsUnaryPostPreRef->setOperator(resultRhsUnaryPreOp);
					rhsUnaryPostPreRef->setSignature(rhsOPRightPart);

					result.push_back(rhsUnaryPostPreRef);

					break;
				}
			}
		}
	}
}