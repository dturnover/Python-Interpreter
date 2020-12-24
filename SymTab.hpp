#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.


struct TypeDescriptor {
  enum types {INTEGER, DOUBLE, STRING};
  
  TypeDescriptor(types type): _type{type} {}
  
  types &type() {return _type; }
  types &value() {return _value; } 

  virtual ~TypeDescriptor() {}
  
private:
  types _value;
  types _type;
};

struct NumberDescriptor: public TypeDescriptor {
  NumberDescriptor(types descType): TypeDescriptor(descType) {} 
  union {
    int intValue;
    double doubleValue;
  } value;
};

struct StringDescriptor: public TypeDescriptor {
  StringDescriptor(types descType): TypeDescriptor(descType) {}
  std::string stringValue;
};

class SymTab {
public:
    
  void setValueFor(std::string vName, TypeDescriptor *value);
  bool isDefined(std::string vName);
  TypeDescriptor *getValueFor(std::string vName);
  void print();

private:
  std::map<std::string, TypeDescriptor *> symTab; 
};

#endif //EXPRINTER_SYMTAB_HPP
