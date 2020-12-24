//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include <stack>
#include "Tokenizer.hpp"

std::stack <int> s;

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

int Tokenizer::readInteger(std::string s) {
  // This function is called when it is known that
  // the first character in input is a digit.
  // The function reads and returns all remaining digits.

  int r = std::stoi(s);
  return r;
}

double Tokenizer::readDouble(std::string s) {
  // This function is called when it is known that
  // there is a period between the digits
  // The function reads and returns a max of 2 decimal places afterwards

  double r = std::stod(s);
  return r;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, parsingANewLine{true}, foundEOF(false), inStream{stream}, lastToken{} {}
  
Token Tokenizer::getToken() {
  
  if(ungottenToken) {
    ungottenToken = false;
    return lastToken;
  }
  
  char c;
  char temp;

  if (s.empty())
    s.push(0);

  int numSpaces = 0;
  Token token;
  Token space;
  
  while ( inStream.get(c) && isspace(c) && c != '\n')
    numSpaces++;
  
  if (parsingANewLine == true && c != '\n' && c != '#')
    {
      if (numSpaces > s.top())
	{
	  s.push(numSpaces);
	  space.INDENT() = true;
	  _tokens.push_back(space); 

	  inStream.putback(c);
	  parsingANewLine = false;

	  return lastToken = space;
	}
      else if (numSpaces < s.top())
	{
	  space.DEDENT() = true;
	  _tokens.push_back(space);
	  s.pop();

	  if (numSpaces == s.top())
	    parsingANewLine = false;
	  else if (numSpaces > s.top())
	    {                                                                                                                           
              std::cout << "Indentation error, mismatched spaces in suite. Spaces counted: " << numSpaces << ", spaces expected: " << s.top() << std::endl;
              exit(1);                                                                                                                  
            }

	  if (inStream.eof())
	    foundEOF = true;

	  inStream.putback(c);

	  return lastToken = space;	  
	}
    }

  parsingANewLine = false;

  if (inStream.bad())
    {
      std::cout << "Error while reading the input stream in Tokenizer.\n";
      exit(1);
    }

  if( inStream.eof() )
    {
      token.eof() = true;
    }
  else if( c == '\n' )
    {      
      parsingANewLine = true;
      token.eol() = true;
 
    }
  else if( c == '#' )
    {
      while (c != '\n')
	inStream.get(c);
      
      parsingANewLine = true;
      token.eol() = true;
    }
  else if( isdigit(c) )
    { // a integer?
      // put the digit back into the input stream so
      // we read the entire number in a function
      inStream.putback(c);
      std::string num = checkNumber();
      if (isDouble(num))
	token.setDoubleNumber( readDouble(num) );
      else
	token.setWholeNumber( readInteger(num) );
      
    }
  else if( c == '=' )
    {
      token.symbol(c);
      inStream.get(temp);
      if (temp == '=')
	token.s2(temp);
      else
	inStream.putback(temp);
      
    }
  else if( c == '+' || c == '*' || c == '%' || c == '-' )
    token.symbol(c);
  
  else if( c == '/' )
    {
      token.symbol(c);
      inStream.get(temp);
      if (temp == '/')
	token.s2(temp);

    }
  else if( c == ';' || c == ':')
    token.symbol(c);
  
  else if( c == '(' || c == ')')
    token.symbol(c);
  
  else if ( c == '{' || c == '}')
    token.symbol(c);
  
  else if ( c == '<' )
    {
      token.symbol(c);
      inStream.get(temp);
      if (temp == '=')
	token.s2(temp);
      else if (temp == '>')
	token.s2(temp);
      else
	inStream.putback(temp);

    }
  else if ( c == '>' )
    {
      token.symbol(c);
      inStream.get(temp);
      if (temp == '=')
	token.s2(temp);
      else
	inStream.putback(temp);

    }
  else if ( c == '!' )
    {
      inStream.get(temp);
	if (temp == '=')
	  {
	    token.symbol(c);
	    token.s2(temp);
	  }
	else
	  inStream.putback(temp);

    }
  else if ( c == '.' )
    token.symbol(c);
  
  else if ( c == '"' )
    token.setString( checkStringDouble() ); 
  
  else if ( c == '\'')
    token.setString( checkStringSingle() ); 
  
  else if(isalpha(c))
    {  // an identifier?
      // put c back into the stream so we can read the entire name in a function.
      inStream.putback(c);
      token.setName( readName() );
      
    }
  else if ( c == '#' )
    token.symbol(c);
  
  else if ( c == ',' )
    token.symbol(c);
  
  else
    {
      if (foundEOF == true)
	token.eof() = true;
      else
	{
	  std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
	  exit(1);
	}
    }
  
  _tokens.push_back(token);
  
  return lastToken = token;
}

std::string Tokenizer::checkStringDouble ()
{
  std::string s = "";
  char c;

  while (inStream.get(c) && c != '"')
    s += c;

  return s;
}

std::string Tokenizer::checkStringSingle()
{ 
  std::string s = "";
  char c;

  while (inStream.get(c) && c != '\'')
    s += c;

  return s;
}

std::string Tokenizer::checkNumber ()
{ 
  std::string s = "";
  char c;

  while (inStream.get(c) && (isdigit(c) or c == '.' or c == '-'))
    s += c;
      
  if(inStream.good())
    inStream.putback(c);

  return s;	 
}

bool Tokenizer::isDouble(std::string s)
{ // Added helper function
  bool r;
  int pCount = 0;
  for (int i = 0; i < s.length(); i++)
    if (s[i] == '.')
      pCount++;

  if (pCount == 1)
    return true;
  else if (pCount == 0)
    return false;
  else
    {
      std::cout << "Too many decimal points in double. Exiting...\n";
      exit(1);
    }  
}

void Tokenizer::ungetToken() {
  ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
  for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
    iter->print();
    std::cout << std::endl;
  }
}
