//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
  Token();
  
  bool &eof()  { return _eof; }
  bool &eol()  { return _eol; }

  bool eof() const { return _eof; }
  bool eol() const  { return _eol; }

  bool &INDENT() { return _INDENT; }
  bool &DEDENT() { return _DEDENT; }

  bool INDENT() const { return _INDENT; }
  bool DEDENT() const { return _DEDENT; }
  
  void symbol(char c) { _symbol = c; }
  char symbol() { return _symbol; }
  void s2(char c) { _s2 = c; }
  char s2() { return _s2; }

  bool isComma() const { return _symbol == ','; }
  bool isHashtag() const { return _symbol == '#'; }
  bool isPeriod() const { return _symbol == '.'; }
  bool isSemiColon() const { return _symbol == ';'; }
  bool isColon() const { return _symbol == ':'; }
  bool isAssignmentOperator() const { return _symbol == '=' and _s2 != '='; }

  bool isOpenParen() const  { return _symbol == '('; }
  bool isClosedParen() const { return _symbol == ')'; }
  bool isOpenCurly() const { return _symbol == '{'; }
  bool isClosedCurly() const { return _symbol == '}'; }
  
  bool isMultiplicationOperator() const { return _symbol == '*'; }
  bool isAdditionOperator() const       { return _symbol == '+'; }
  bool isSubtractionOperator() const    { return _symbol == '-'; }
  bool isModuloOperator() const         { return _symbol == '%'; }
  bool isDivisionOperator() const       { return _symbol == '/' and _s2 != '/'; } 
  bool isIntDivisionOperator() const    { return _symbol == '/' and _s2 == '/'; } 
  bool isArithmeticOperator() const {
    return isMultiplicationOperator() ||
      isAdditionOperator() ||
      isSubtractionOperator() ||
      isModuloOperator() ||
      isDivisionOperator() ||
      isIntDivisionOperator();
  }
  bool isGreaterThanOperator() const { return _symbol == '>' and _s2 != '='; } 
  bool isLessThanOperator() const { return _symbol == '<' and _s2 != '=' and _s2 != '>'; }   
  bool isGreaterEqualOperator() const { return _symbol == '>' and _s2 == '='; }
  bool isLessEqualOperator() const { return _symbol == '<' and _s2 == '='; } 
  bool isEqualToOperator() const { return _symbol == '=' and _s2 == '='; } 
  bool isNotEqualToOperator() const { return ((_symbol == '!' and _s2 == '=') or (_symbol == '<' and _s2 == '>')); } 
  bool isRelationalOperator() const {
    return isGreaterThanOperator() ||
      isLessThanOperator() ||
      isGreaterEqualOperator() ||
      isLessEqualOperator() ||
      isEqualToOperator() ||
      isNotEqualToOperator();
  }
  
  bool isKeywordPrint() const { return _name == "print"; } 
  bool isKeywordFor() const { return _name == "for"; }
  bool isKeywordIf() const { return _name == "if"; }
  bool isKeywordElif() const { return _name == "elif"; }
  bool isKeywordElse() const { return _name == "else"; }
  bool isKeywordIn() const { return _name == "in"; }
  bool isKeywordRange() const { return _name == "range"; }
  bool isKeywordOr() const { return _name == "or"; }
  bool isKeywordAnd() const { return _name == "and"; }
  bool isKeywordNot() const { return _name == "not"; }
  bool isKeyword() const {
    return isKeywordPrint() ||
      isKeywordFor() ||
      isKeywordIf() ||
      isKeywordElif() ||
      isKeywordElse() ||
      isKeywordIn() ||
      isKeywordRange() ||
      isKeywordOr() ||
      isKeywordAnd() ||
      isKeywordNot();
  }
  
  bool isName() const                   { return _name.length() > 0; }
  std::string getName() const                  { return _name; }
  void setName(std::string n) { _name = n; }
  
  bool &isWholeNumber() { return _isWholeNumber; }
  bool isWholeNumber() const { return _isWholeNumber; }
  int getWholeNumber() const { return _wholeNumber; }
  void setWholeNumber(int n) {
    _wholeNumber = n;
    isWholeNumber() = true;
  }

  bool &isDoubleNumber() { return _isDoubleNumber; }
  bool isDoubleNumber() const { return _isDoubleNumber; }
  double getDoubleNumber() const { return _doubleNumber; }
  void setDoubleNumber(double n) {
    _doubleNumber = n;
    isDoubleNumber() = true;
  }

  bool isQuotationSingle() const { return _symbol == '\'';}
  bool isQuotationDouble() const { return _symbol == '"'; }
  bool isStringOperator() const {
    return isQuotationSingle() ||
      isQuotationDouble();
  }

  bool &isString() { return _isString; }
  bool isString() const { return _isString; }
  std::string getString() const { return _string; }
  void setString(std::string s) {
    _string = s;
    isString() = true;

  }

  void print() const;
  
private:
  std::string _name;
  bool _eof, _eol;
  bool _isWholeNumber, _isDoubleNumber, _isString;
  bool _INDENT, _DEDENT;
  char _symbol;
  char _s2;
  int _wholeNumber;
  double _doubleNumber;
  std::string _string;
};

#endif //EXPRINTER_TOKEN_HPP
