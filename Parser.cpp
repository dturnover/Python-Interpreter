
#include<vector>
#include<iostream>

#include <vector>
#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token)
{
  std::cout << where << " " << message << std::endl;
  token.print();
  std::cout << std::endl;
  std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
  tokenizer.printProcessedTokens();
  exit(1);
}

Statements *Parser::statements() {
  // This function is called when we KNOW that we are about to parse
  // a series of assignment statements.
  
  // This function parses the grammar rules:
  
  // <statement> -> <assignStatement> <statement> 
  // <statement> -> Epsilon
  
  
  Statements *stmts = new Statements();
  Token tok = tokenizer.getToken();
  
  
  while (tok.eol())
    tok = tokenizer.getToken();

  while (tok.isName())
    {

      if (tok.isKeyword())
	{
	  if (tok.isKeywordPrint())
	    {
	      PrintStatement *printStmt = printStatement();
	      stmts->addStatement(printStmt);
	      
	      tok = tokenizer.getToken();
	    }
	  else if (tok.isKeywordFor())
	    {
	      ForStatement *forStmt = forStatement();
	      stmts->addStatement(forStmt);
	      
	      tok = tokenizer.getToken();
	    }
	  else if (tok.isKeywordIf())
	    {
	      IfStatement *ifStmt = ifStatement();
	      stmts->addStatement(ifStmt);

	      tok = tokenizer.getToken();
	    }
	}
      else
	{
	  tokenizer.ungetToken();
	  
	  AssignmentStatement *assignStmt = assignStatement();
	  stmts->addStatement(assignStmt);
	  
	  tok = tokenizer.getToken();
	}
      
      while (tok.eol())
	tok = tokenizer.getToken();
      
    }
  tokenizer.ungetToken();
  
  return stmts;
}

AssignmentStatement *Parser::assignStatement()
{
  // Parses the following grammar rule
  // 
  // <assign-stmtement> -> <id> = <expr>
  
  Token id = tokenizer.getToken();
  if (!id.isName())
    die("Parser::assignStatement", "Expected a name token, instead got", id);

  Token assignOp = tokenizer.getToken();
  if (!assignOp.isAssignmentOperator())
    die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

  ExprNode *rightHandSideExpr = orTest();

  Token tok = tokenizer.getToken();
  
  if (!tok.eol())
    die("Parser::statements", "Expected an end of line token, instead got", tok);
  
  return new AssignmentStatement(id.getName(), rightHandSideExpr);
}

PrintStatement *Parser::printStatement()
{
  std::vector <ExprNode *> list = testList();

  tokenizer.ungetToken();
  Token tok = tokenizer.getToken();

  if (!tok.eol() and !tok.eof())
    die("Parser::printStatements","Expected an end of line token, instead got", tok);                                                             
  return new PrintStatement(list);
}

ForStatement *Parser::forStatement()
{
  Token idT = tokenizer.getToken();
  if (!idT.isName())
    die("Parser::forStatement", "Expected a name token, instead got", idT);

  std::string id = idT.getName();
  
  Token tok = tokenizer.getToken();
  if (!tok.isKeywordIn())
    die("Parser::forStatement", "Expected a in keyword token, instead got ", tok);

  tok = tokenizer.getToken();
  if (!tok.isKeywordRange())
    die("Parser::forStatement", "Expected a range keyword token, instead got ", tok);

  tok = tokenizer.getToken();
  if (!tok.isOpenParen())
    die("Parser::forStatement", "Expected an open parenthesis, instead got", tok);

  std::vector <ExprNode *> TL = testList();
  
  tokenizer.ungetToken();
  tok = tokenizer.getToken();
  if (!tok.isClosedParen())                              
    die("Parser::forStatement", "Expected a closed parenthesis, instead got", tok);     

  if (TL.size() > 3 || TL.empty())
    {
      std::cout << "Parser::forStatement incorrect test list size\n";
      exit(1);
    }
  
  tok = tokenizer.getToken();
  if (!tok.isColon())
    die("Parser::forStatement", "Expected a colon, instead got", tok);

  Statements *block = suite();
  
  return new ForStatement(TL, block, id);
}

IfStatement *Parser::ifStatement()
{
  std::vector <ExprNode *> TL;
  std::vector <Statements *> blocks;
  
  ExprNode *test = orTest();
  TL.push_back(test);
  
  Token tok = tokenizer.getToken();
  if (!tok.isColon())
    die("Parser::ifStatement", "Expected a colon, instead got", tok);

  Statements *block = suite();
  blocks.push_back(block);  

  tok = tokenizer.getToken();
  if (tok.isKeywordElif())
    {
      while (tok.isKeywordElif())
	{
	  test = orTest();
	  TL.push_back(test);

	  tok = tokenizer.getToken();
	  if (!tok.isColon())
	    die("Parser::ifStatement", "Expected a colon, instead got", tok);
	  
	  block = suite();
	  blocks.push_back(block);

	  tok = tokenizer.getToken();
	}
    }

  if (tok.isKeywordElse())
    {
      tok = tokenizer.getToken();
      if (!tok.isColon())
	die("Parser::ifStatement", "Expected a colon, instead got", tok);
      
      block = suite();
      blocks.push_back(block);
      
      tok = tokenizer.getToken(); 
    }

  tokenizer.ungetToken();
  
  return new IfStatement(TL,blocks);
}

ExprNode *Parser::arithExpr()
{
  // This function parses the grammar rules:
  
  // arith_expr: term {(’+’|’-’) term}*
  
  ExprNode *left = term(); 
  Token tok = tokenizer.getToken();
  while (tok.isAdditionOperator() || tok.isSubtractionOperator())
    {
      InfixExprNode *p = new InfixExprNode(tok);
      p->left() = left;
      p->right() = term(); 
      left = p;
      tok = tokenizer.getToken();
    }
  tokenizer.ungetToken();
  return left;
}


ExprNode *Parser::term()
{
  // This function parses the grammar rules:
  
  // term: factor {(’*’|’/’|’%’|’//’) factor}*
  
  ExprNode *left = factor(); 
  Token tok = tokenizer.getToken();
  
  while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isIntDivisionOperator()) { // added int division
    InfixExprNode *p = new InfixExprNode(tok);
    p->left() = left;
    p->right() = factor(); 
    left = p;
    tok = tokenizer.getToken();
  }
  tokenizer.ungetToken();
  return left;
}

ExprNode *Parser::factor()
{
  // This function parses the grammar rules:

  // factor: {'-'} factor | atom

  Token tok = tokenizer.getToken();
  ExprNode *left;
  
  if (tok.isSubtractionOperator())
    {
      Token op;
      op.symbol('*');
      Token flip;
      flip.setWholeNumber(-1);

      InfixExprNode *p = new InfixExprNode(op);
      p->left() = new WholeNumber(flip);
      p->right() = factor();
      left = p;
    }
  else
    {
      tokenizer.ungetToken();
      left = atom();
    }

  return left;
}

ExprNode *Parser::atom()
{
  // This function parses the grammar rules:
  
  // atom: ID | NUMBER | STRING+ | ’(’ test ’)’
  
  Token tok = tokenizer.getToken();
  
  if( tok.isName() )
    return new Variable(tok);
  else if ( tok.isString() )
    return new String(tok);
  else if ( tok.isWholeNumber() )
    return new WholeNumber(tok);
  else if ( tok.isDoubleNumber() )
    return new DoubleNumber(tok);
  else if ( tok.isOpenParen() ) {
    ExprNode *p = orTest(); 
    Token token = tokenizer.getToken();
    if (!token.isClosedParen())
      die("Parser::factor", "Expected close-parenthesis, instead got", token);
    return p;
  }
  
  die("Parser::factor", "Unexpected token", tok);
  
  return nullptr;  // Will not reach this statement!
}

Statements *Parser::suite()
{
  Token tok = tokenizer.getToken();
  
  if (!tok.eol())
    die("Parser::suite", "Expected an end of line token, instead got", tok);

  while (tok.eol())
    tok = tokenizer.getToken();
  
  if (!tok.INDENT())
    die("Parser::suite", "Expected an INDENT token, instead got", tok);
  
  Statements *block = statements();

  while (tok.eol())
    {
      tok = tokenizer.getToken();
      tokenizer.ungetToken();
    }
  
  tok = tokenizer.getToken();
  
  if (!tok.DEDENT())
    die("Parser::suite", "Expected a DEDENT token, instead got", tok);

  return block;
}

std::vector <ExprNode *> Parser::testList()
{
  std::vector <ExprNode *> TL;
  Token tok;

  do  {

    ExprNode *test = orTest();
    TL.push_back(test);
    tok = tokenizer.getToken();

  } while (tok.isComma());
  
  return TL;
}

ExprNode *Parser::orTest() 
{
  ExprNode *left = andTest();
  Token tok = tokenizer.getToken();
  while (tok.isKeywordOr())
    {
      InfixExprNode *p = new InfixExprNode(tok);
      p->left() = left;
      p->right() = andTest();
      left = p;
      tok = tokenizer.getToken();
    }
  tokenizer.ungetToken();

  return left;
}

ExprNode *Parser::andTest()
{
  ExprNode *left = notTest();
  Token tok = tokenizer.getToken();
  while (tok.isKeywordAnd())
    {
      InfixExprNode *p = new InfixExprNode(tok);
      p->left() = left;
      p->right() = notTest();
      left = p;
      tok = tokenizer.getToken();
    }
  tokenizer.ungetToken();

  return left;
}

ExprNode *Parser::notTest()
{
  ExprNode *left;
  Token tok = tokenizer.getToken();
  
  if (tok.isKeywordNot())
    {
      InfixExprNode *p = new InfixExprNode(tok);

      p->left() = nullptr;
      p->right() = notTest();
      left = p;
    }
  else
    {
      tokenizer.ungetToken();
      left = comparison();
    }

  return left;
}

ExprNode *Parser::comparison()
{
  // This function parses the grammar rules:
  // comparison: arith_expr {comp_op arith_expr}*
  
  ExprNode *left = arithExpr(); 
  Token tok = tokenizer.getToken();
  while (tok.isEqualToOperator() || tok.isNotEqualToOperator() || tok.isGreaterThanOperator() || tok.isLessThanOperator() || tok.isGreaterEqualOperator() || tok.isLessEqualOperator()) 
    {
      InfixExprNode *p = new InfixExprNode(tok);
      p->left() = left;
      p->right() = arithExpr(); 
      left = p;
      tok = tokenizer.getToken();
    }
  tokenizer.ungetToken();

  return left;
}
