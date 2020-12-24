//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include <math.h>
#include "Expr.hpp"

// ArithExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor *InfixExprNode::evaluate(SymTab &symTab) {
  // Evaluates an infix expression using a post-order traversal of the expression tree.

  bool cascade = false;
  TypeDescriptor *lValueType;

  if (left() != nullptr)
    {
      cascade = true;
      lValueType = left()->evaluate(symTab);
    }
  TypeDescriptor * rValueType = right()->evaluate(symTab);
  
  if (cascade == false || (lValueType->type() == TypeDescriptor::INTEGER and rValueType->type() == TypeDescriptor::INTEGER))
    {
      NumberDescriptor *lValueN;
      int lValueV;
      if (cascade)
	{
	  lValueN = dynamic_cast<NumberDescriptor *>(lValueType);
	  lValueV = lValueN->value.intValue;
	}
      NumberDescriptor *rValueN = dynamic_cast<NumberDescriptor *>(rValueType);
      int rValueV = rValueN->value.intValue;
      
      std::cout << "InfixExprNode::evaluate: " << lValueV << " " << token().symbol() << " " << rValueV << std::endl;      
      if( token().isAdditionOperator() )
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV + rValueV;
	  return desc;
	} 
      else if(token().isSubtractionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV - rValueV;
	  return desc;
	}
      else if(token().isMultiplicationOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV * rValueV;
	  return desc;
	}
      else if(token().isDivisionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  double dblL = lValueV;
	  double dblR = rValueV;
	  double dblV = dblL / dblR;
	  desc->value.doubleValue = dblV;

	  return desc;
	}
      else if(token().isIntDivisionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = round(lValueV / rValueV); 
	  return desc;
	}
      else if( token().isModuloOperator() )
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV % rValueV;
	  return desc;
	}
      else if (token().isGreaterThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV > rValueV;
	  return desc;
	}
      else if (token().isLessThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV < rValueV;
	  return desc;
	}
      else if (token().isGreaterEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV >= rValueV;
	  return desc;
	}
      else if (token().isLessEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV <= rValueV;
	  return desc;
	}
      else if (token().isEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV == rValueV;
	  return desc;
	}
      else if (token().isNotEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = lValueV !=  rValueV;
	  return desc;
	}
      else if (token().isKeywordOr())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = (lValueV == 1 || rValueV == 1);
	  return desc;
	}
      else if (token().isKeywordAnd())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  desc->value.intValue = (lValueV == 1 && rValueV == 1);
	  return desc;
	}
      else if (token().isKeywordNot())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	  if (rValueV == 1)
	    desc->value.intValue = 0;
	  else if (rValueV == 0)
	    desc->value.intValue = 1;

	  return desc;
	}
    }
  else if (lValueType->type() == TypeDescriptor::DOUBLE and rValueType->type() == TypeDescriptor::DOUBLE)
    {
      NumberDescriptor *lValueN;
      double lValueV;
      if (cascade)
	{
	  lValueN = dynamic_cast<NumberDescriptor *>(lValueType);
	  lValueV = lValueN->value.doubleValue;
	}
      NumberDescriptor *rValueN = dynamic_cast<NumberDescriptor *>(rValueType);
      double rValueV = rValueN->value.doubleValue;
      
      std::cout << "InfixExprNode::evaluate: " << lValueV << " " << token().symbol() << " " << rValueV << std::endl;
      
      if( token().isAdditionOperator() )
        {
          NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
          desc->value.doubleValue = lValueV + rValueV;
          return desc;
        }
      else if(token().isSubtractionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV - rValueV;
	  return desc;
	}
      else if(token().isMultiplicationOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV * rValueV;
	  return desc;
	}
      else if(token().isDivisionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV / rValueV;
	  return desc;
	}
      else if(token().isIntDivisionOperator())
	{
	  std::cout << "Unable to perform integer divison on non ints. Exiting...\n";
	  exit(1);
	}
      else if( token().isModuloOperator() )
	{
	  std::cout << "Unable to perform modulo operation on non ints. Exiting...\n";
	  exit(1);
	}
      else if (token().isGreaterThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV > rValueV;
	  return desc;
	}
      else if (token().isLessThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV < rValueV;
	  return desc;
	}
      else if (token().isGreaterEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV >= rValueV;
	  return desc;
	}
      else if (token().isLessEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV <= rValueV;
	  return desc;
	}
      else if (token().isEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV == rValueV;
	  return desc;
	}
      else if (token().isNotEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV !=  rValueV;
	  return desc;
	}
    }
  else if (lValueType->type() == TypeDescriptor::DOUBLE and rValueType->type() == TypeDescriptor::INTEGER)
    {
      NumberDescriptor *lValueN;
      double lValueV;
      if (cascade)
	{
	  lValueN = dynamic_cast<NumberDescriptor *>(lValueType);
	  lValueV = lValueN->value.doubleValue;
	}
      NumberDescriptor *rValueN = dynamic_cast<NumberDescriptor *>(rValueType);
      int rValueV = rValueN->value.intValue;

      std::cout << "InfixExprNode::evaluate: " << lValueV << " " << token().symbol() << " " << rValueV << std::endl;
      
      if( token().isAdditionOperator() )
        {
          NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
          desc->value.doubleValue = lValueV + rValueV;
          return desc;
        }
      else if(token().isSubtractionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV - rValueV;
	  return desc;
	}
      else if(token().isMultiplicationOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV * rValueV;
	  return desc;
	}
      else if(token().isDivisionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV / rValueV;
	  return desc;
	}
      else if(token().isIntDivisionOperator())
        {
	  std::cout << "Unable to perform integer divison on non ints. Exiting...\n";
	  exit(1);
	}
      else if( token().isModuloOperator() )
	{
	  std::cout << "Unable to use perform modulo operation on non ints. Exiting...\n";
	  exit(1);
	}
      else if (token().isGreaterThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV > rValueV;
	  return desc;
	}
      else if (token().isLessThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV < rValueV;
	  return desc;
	}
      else if (token().isGreaterEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV >= rValueV;
	  return desc;
	}
      else if (token().isLessEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV <= rValueV;
	  return desc;
	}
      else if (token().isEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV == rValueV;
	  return desc;
	}
      else if (token().isNotEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV !=  rValueV;
	  return desc;
	}
    }
  else if (lValueType->type() == TypeDescriptor::INTEGER and rValueType->type() == TypeDescriptor::DOUBLE)
    {
      
      NumberDescriptor *lValueN;
      int lValueV;
      if (cascade)
	{
	  lValueN = dynamic_cast<NumberDescriptor *>(lValueType);
	  lValueV = lValueN->value.intValue;
	}
      NumberDescriptor *rValueN = dynamic_cast<NumberDescriptor *>(rValueType);
      double rValueV = rValueN->value.doubleValue;
      
      std::cout << "InfixExprNode::evaluate: " << lValueV << " " << token().symbol() << " " << rValueV << std::endl;

      if( token().isAdditionOperator() )
        {
          NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
          desc->value.doubleValue = lValueV + rValueV;
          return desc;
        }
      else if(token().isSubtractionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV - rValueV;
	  return desc;
	}
      else if(token().isMultiplicationOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV * rValueV;
	  return desc;
	}
      else if(token().isDivisionOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV / rValueV;
	  return desc;
	}
      else if(token().isIntDivisionOperator())
        {
          std::cout << "Unable to perform integer divison on non ints. Exiting...\n";
          exit(1);
        }
      else if( token().isModuloOperator() )
	{
	  std::cout << "Unable to perform modulo operation on non ints. Exiting...\n";
	  exit(1);
	}
      else if (token().isGreaterThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV > rValueV;
	  return desc;
	}
      else if (token().isLessThanOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV < rValueV;
	  return desc;
	}
      else if (token().isGreaterEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV >= rValueV;
	  return desc;
	}
      else if (token().isLessEqualOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV <= rValueV;
	  return desc;
	}
      else if (token().isEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV == rValueV;
	  return desc;
	}
      else if (token().isNotEqualToOperator())
	{
	  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	  desc->value.doubleValue = lValueV !=  rValueV;
	  return desc;
	}
    }
  else if (lValueType->type() == TypeDescriptor::STRING and rValueType->type() == TypeDescriptor::STRING)
    {
      StringDescriptor *lValueS = dynamic_cast<StringDescriptor *>(lValueType);
      StringDescriptor *rValueS = dynamic_cast<StringDescriptor *>(rValueType);
      std::string lValueV = lValueS->stringValue;
      std::string rValueV = rValueS->stringValue;
      
      std::cout << "InfixExprNode::evaluate: " << lValueV << " " << token().symbol() << " " << rValueV << std::endl;
      
      if( token().isAdditionOperator() )
        {
          StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
          desc->stringValue = lValueV + rValueV;
          return desc;
        }
      else
	{
	  std::cout << "Invalid operator for strings. Exiting...\n";
	  exit(1);
	}
    }

  std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
  token().print();
  std::cout << std::endl;
  exit(2);
}

void InfixExprNode::print()
{
  if (_left != nullptr)
    _left->print();
  token().print();
  _right->print();
}

// WholeNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
  token().print();
}

TypeDescriptor *WholeNumber::evaluate(SymTab &symTab) {
  
  std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;

  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
  desc->value.intValue = token().getWholeNumber();
  return desc;
}

// DoubleNumber (added by me)
DoubleNumber::DoubleNumber(Token token): ExprNode{token} {}

void DoubleNumber::print() {
  token().print();
}

TypeDescriptor *DoubleNumber::evaluate(SymTab &symTab) {
  
  std::cout << "DoubleNumber::evaluate: returning " << token().getDoubleNumber() << std::endl;
  
  NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
  desc->value.doubleValue = token().getDoubleNumber();
  return desc;
}

// String (added by me)
String::String(Token token): ExprNode{token} {}

void String::print() {
  token().print();
}

TypeDescriptor *String::evaluate(SymTab &symTab) {
  
  std::cout << "DoubleNumber::evaluate: returning " << token().getString() << std::endl;
  
  StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
  desc->stringValue = token().getString();
  return desc;
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
  token().print();
}

TypeDescriptor *Variable::evaluate(SymTab &symTab) {
  if( ! symTab.isDefined(token().getName())) {
    std::cout << "Use of undefined variable, " << token().getName() << std::endl;
    exit(1);
  }
  
  TypeDescriptor *value = symTab.getValueFor(token().getName());
  NumberDescriptor *nDesc = dynamic_cast<NumberDescriptor *>(value);
  if (value->type() == TypeDescriptor::INTEGER)
    std::cout << "Variable::evaluate: returning " << nDesc->value.intValue << std::endl;
  else if (value->type() == TypeDescriptor::DOUBLE)
    std::cout << "Variable::evaluate: return " << nDesc->value.doubleValue << std::endl;
 
  return value;
}


