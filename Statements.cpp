#include "Statements.hpp"
#include <string>
#include <vector>

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
  for (auto s: _statements)
    s->print();
}

void Statements::evaluate(SymTab &symTab) {
  for (auto s: _statements)
    s->evaluate(symTab);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
  _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
  TypeDescriptor *rhs = rhsExpression()->evaluate(symTab);
  symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
  return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
  return _rhsExpression;
}

void AssignmentStatement::print() {
  std::cout << _lhsVariable << " = ";
  _rhsExpression->print();
  std::cout << std::endl;
}

// PrintStatment

PrintStatement::PrintStatement(std::vector <ExprNode *> testList) { for (int i = 0; i < testList.size(); i++) _testList.push_back(testList[i]); };

void PrintStatement::evaluate(SymTab &symTab)
{
  for (int i = 0; i < _testList.size(); i++)
    v.push_back(_testList[i]->evaluate(symTab));
}

void PrintStatement::print() 
{
  std::cout << "print ";
  for (int i = 0; i < v.size(); i++)
    {
      if (v[i]->type() == TypeDescriptor::INTEGER)
	{
	  NumberDescriptor *nDescV = dynamic_cast<NumberDescriptor *>(v[i]);
	  std::cout << nDescV->value.intValue << " ";
	}
      else if (v[i]->type() == TypeDescriptor::DOUBLE)
	{
	  NumberDescriptor *nDescV = dynamic_cast<NumberDescriptor *>(v[i]);
	  std::cout << nDescV->value.doubleValue << " ";
	}
      else if (v[i]->type() == TypeDescriptor::STRING)
	{
	  StringDescriptor *sDescV = dynamic_cast<StringDescriptor *>(v[i]);
	  std::cout << sDescV->stringValue << " ";
	}
    }
  std::cout << std::endl;
}

// ForStatement

ForStatement::ForStatement() : _block{nullptr}, _rString{NULL} {}

ForStatement::ForStatement(std::vector <ExprNode *> testList, Statements *block, std::string varName) : _block(block), _varName{varName} {
  for (int i = 0; i < testList.size(); i++)
    _testList.push_back(testList[i]);
} 

void ForStatement::evaluate(SymTab &symTab)
{
  int s = _testList.size();
  Range *rangeInstance;
  NumberDescriptor *itr = new NumberDescriptor(TypeDescriptor::INTEGER);
  if (s == 1)
    { 
      TypeDescriptor *TD = _testList[0]->evaluate(symTab);
      NumberDescriptor *ND = dynamic_cast<NumberDescriptor *>(TD);
      rangeInstance = new Range(ND->value.intValue);
      itr->value.intValue = 0;
      _rString = std::to_string(ND->value.intValue);
    }
  else if (s == 2)
    {
      TypeDescriptor *TD1 = _testList[0]->evaluate(symTab);
      TypeDescriptor *TD2 = _testList[1]->evaluate(symTab);
      NumberDescriptor *ND1 = dynamic_cast<NumberDescriptor *>(TD1);
      NumberDescriptor *ND2 = dynamic_cast<NumberDescriptor *>(TD2);
      rangeInstance = new Range(ND1->value.intValue, ND2->value.intValue);
      itr = ND1;
      _rString = std::to_string(ND1->value.intValue); _rString += ", "; _rString +=  std::to_string(ND2->value.intValue);
    }
  else if (s == 3)
    {
      TypeDescriptor *TD1 = _testList[0]->evaluate(symTab);
      TypeDescriptor *TD2 = _testList[1]->evaluate(symTab);
      TypeDescriptor *TD3 = _testList[2]->evaluate(symTab);
      NumberDescriptor *ND1 = dynamic_cast<NumberDescriptor *>(TD1);
      NumberDescriptor *ND2 = dynamic_cast<NumberDescriptor *>(TD2);
      NumberDescriptor *ND3 = dynamic_cast<NumberDescriptor *>(TD3);
      rangeInstance = new Range(ND1->value.intValue, ND2->value.intValue, ND3->value.intValue);
      itr = ND1;
      _rString = std::to_string(ND1->value.intValue); _rString += ", "; _rString += std::to_string(ND2->value.intValue); _rString += ", "; _rString += std::to_string(ND3->value.intValue);
    }
  
  int count = 0;
  while (rangeInstance->condition())
    {
      symTab.setValueFor(_varName, itr);
      _block->evaluate(symTab);
      itr->value.intValue = rangeInstance->next();
      count++;
    }
  itr->value.intValue = itr->value.intValue - rangeInstance->stepValue();
}

void ForStatement::print()
{
  //std::cout << "for " << _varName << " in range (" << _rString << ") {\n";
  std::cout << "Result (for statement):\n";
  _block->print();
  std::cout << std::endl << std::endl;
}

// IfStatement

IfStatement::IfStatement(std::vector <ExprNode *> testList, std::vector <Statements *> blocks) {
  for (int i = 0; i < testList.size(); i++)
    _testList.push_back(testList[i]);
  for (int j = 0; j < blocks.size(); j++)
    _blocks.push_back(blocks[j]);
}

void IfStatement::evaluate(SymTab &symTab)
{
  for (int i = 0; i < _testList.size(); i++)
    {
      TypeDescriptor *TD = _testList[i]->evaluate(symTab);
      NumberDescriptor *ND = dynamic_cast<NumberDescriptor *>(TD);
      int b = ND->value.intValue;
      if (b == 1)
	{
	  _blocks[i]->evaluate(symTab);
	  return;
	}
    }
  
  if (_blocks.size() - _testList.size() == 1)
    _blocks[_blocks.size()-1]->evaluate(symTab);
}

void IfStatement::print()
{

  std::cout << "Result (if statement):\n";
  for (int i = 0; i < _blocks.size(); i++)
    _blocks[i]->print();

  std::cout << std::endl << std::endl;
}

// Range

Range::Range(int rangeValue): _rangeValue{rangeValue}, _initValue{0}, _stepValue{1} {}

Range::Range(int initValue, int rangeValue): _rangeValue{rangeValue}, _initValue{initValue}, _stepValue{1} {}

Range::Range(int initValue, int rangeValue, int stepValue): _rangeValue{rangeValue}, _initValue{initValue}, _stepValue{stepValue} {}

bool Range::condition() {
  return _initValue < _rangeValue;
}

int Range::stepValue() {
  return _stepValue;
}

int Range::next() {
  _initValue = _initValue + _stepValue;
  return _initValue;
}




				  
				   
		       
