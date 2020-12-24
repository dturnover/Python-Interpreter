#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"

class Tokenizer {

public:
  Tokenizer(std::ifstream &inStream);
  Token getToken();
  void ungetToken();
  void printProcessedTokens();
  // helper functions
  std::string checkStringDouble ();
  std::string checkStringSingle();
  std::string checkNumber ();
  bool isDouble(std::string s);
  bool getDents();
private:
  Token lastToken;
  bool ungottenToken;
  bool parsingANewLine;
  bool foundEOF;
  std::ifstream &inStream;
  std::vector<Token> _tokens; 
private:
  std::string readName();
  int readInteger(std::string s);
  double readDouble(std:: string s);
};

#endif //EXPRINTER_TOKENIZER_HPP
