#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _s2{'\0'},_isWholeNumber{false}, _isDoubleNumber{false}, _isString{false}, _INDENT{false}, _DEDENT{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( INDENT() ) std::cout << " INDENT ";
    else if( DEDENT() ) std::cout << " DEDENT ";
    else if( isKeywordOr() ) std::cout << " or ";
    else if( isKeywordAnd() ) std::cout << " and ";
    else if( isKeywordNot() ) std::cout << " not ";
    else if( isPeriod() ) std::cout << " . ";
    else if( isOpenParen() )  std::cout << " ( " ;
    else if( isClosedParen() )  std::cout << " ) ";
    else if ( isOpenCurly() ) std::cout << " { ";
    else if ( isClosedCurly() ) std::cout << " } ";
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << " ; " ;
    else if( isColon() ) std::cout << " : ";
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isIntDivisionOperator() ) std::cout << " // ";
    else if ( isGreaterThanOperator() ) std::cout << " > ";
    else if ( isLessThanOperator() ) std::cout << " < ";
    else if ( isGreaterEqualOperator() ) std::cout << " >= ";
    else if ( isLessEqualOperator() ) std::cout << " <= ";
    else if ( isEqualToOperator() ) std::cout << " == ";
    else if ( isNotEqualToOperator() ) std::cout << " != ";
    else if ( isStringOperator() ) std::cout << " \" ";
    else if ( isHashtag() ) std::cout << " # ";
    else if ( isComma() ) std::cout << " , ";
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isDoubleNumber() ) std::cout << getDoubleNumber();
    else if( isString() ) std::cout << getString();
    else std::cout << "Uninitialized token.\n";
}
