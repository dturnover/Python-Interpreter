#include <iostream>
#include "SymTab.hpp"


void SymTab::setValueFor(std::string vName, TypeDescriptor *value) {
  // Define a variable by setting its initial value.
  if (value->type() == TypeDescriptor::INTEGER)
    {
      NumberDescriptor *nDescValue = dynamic_cast<NumberDescriptor *>(value);
      std::cout << vName << " <- " << nDescValue->value.intValue << std::endl;
    }
  else if (value->type() == TypeDescriptor::DOUBLE)
    {
      NumberDescriptor *nDescValue = dynamic_cast<NumberDescriptor *>(value);
      std::cout << vName << " <- " << nDescValue->value.doubleValue << std::endl;
    }
  else if (value->type() == TypeDescriptor::STRING)
    {
      StringDescriptor *sDescValue = dynamic_cast<StringDescriptor *>(value);
      std::cout << vName << " <- " << sDescValue->stringValue << std::endl;
    }
  
  symTab[vName] = value;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}


TypeDescriptor *SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }

    TypeDescriptor *TD = symTab.find(vName)->second;
    if (TD->type() == TypeDescriptor::INTEGER)
      {
	NumberDescriptor *nDescValue = dynamic_cast<NumberDescriptor *>(symTab.find(vName)->second);
	std::cout << "SymTab::getValueFor: " << vName << " contains " << nDescValue->value.intValue << std::endl; //symTab.find(vName)->second
      }
    else if (TD->type() == TypeDescriptor::DOUBLE)
      {
	NumberDescriptor *nDescValue = dynamic_cast<NumberDescriptor *>(symTab.find(vName)->second);
	std::cout << "SymTab::getValueFor: " << vName << " contains " << nDescValue->value.doubleValue << std::endl;
      }
    else if (TD->type() == TypeDescriptor::STRING)
      {
	StringDescriptor *sDescValue = dynamic_cast<StringDescriptor *>(symTab.find(vName)->second);
	std::cout << "SymTab::getValueFor: " << vName << " contains " << sDescValue->stringValue << std::endl;
      }
    
    return symTab.find(vName)->second;
}

void SymTab::print() {
    for(auto [var, value] : symTab )
      {
	if (value->type() == TypeDescriptor::INTEGER)
	  {
	    NumberDescriptor *nDescValue = dynamic_cast<NumberDescriptor *>(value);
	    std::cout << var << " = " << nDescValue->value.intValue << std::endl;
	  }
	else if (value->type() == TypeDescriptor::DOUBLE)
	  {
	    NumberDescriptor *nDescValue = dynamic_cast<NumberDescriptor *>(value);
	    std::cout << var << " = " << nDescValue->value.doubleValue << std::endl;
	  }
	else if (value->type() == TypeDescriptor::STRING)
	  {
	    StringDescriptor *sDescValue = dynamic_cast<StringDescriptor *>(value);
	    std::cout << var << " = " << sDescValue->stringValue << std::endl;
	  }
      }
}

