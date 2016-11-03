/**************************************************************************************
 * Syntax Analyzer.cpp
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/


#include "SyntaxAnalyzer.h"

 /***************************************************************
 * SyntaxAnalyzer::		rat15S
 * ------------------------------------------------------------
 * Function:
 *		This function will check for the syntax of what
 *		Separator that needs to be in-between scopes of code
 *		Mainly the function is check to see if there is a @@
 *		token and checks to see if it happens and then if not
 *		will output an error message.
 ***************************************************************/
void SyntaxAnalyzer::rat15S()
{
	if(print_production)
	{
		out_file << "<Rat15S> ::= <Opt Function Definitions>   "
					"@@  <Opt Declaration List> @@ <Statement List>  " << endl;
	}

		optFunctionDefinition();

		if(current_lexeme == "@@")
		{
			simulatedLexerCall();
			optDeclarationList();
			if(current_lexeme == "@@")
			{
				simulatedLexerCall();
				statementList();
			}
			else
			{
				errorExpectedThis("@@");
			}
		}
		else
		{
			errorExpectedThis("@@");
		}


}

/***************************************************************
 * SyntaxAnalyzer::		helperFunctionDefinition
 * ------------------------------------------------------------
 * Function:
 *		This function is a helper function to the function
 *		definition function. It will output what is expected
 *		and check to see if Function is the token and if it is
 *		it will call the functiondefinition() and if not
 *		it will report a error message.
 ***************************************************************/
void SyntaxAnalyzer:: optFunctionDefinition()
{
	if(print_production)
	{
		out_file << "<Opt Function Definitions> ::= <Function "
					"Definitions> | <Empty>" << endl;
	}

	if(current_lexeme == "function")
	{
		functionDefinition();
	}
	else if(current_lexeme != "@@")
	{
		errorExpectedThis("function or nothing");
	}
	else
	{
		TheListIsEmpty();
	}
}

/***************************************************************
 * SyntaxAnalyzer::		functionDefinition
 * ------------------------------------------------------------
 * Function:
 *		This function will output the expected token if print is
 *		turned on. it will then call function() once and if needed
 *		it will continue calling it till its wrong.
 ***************************************************************/
void SyntaxAnalyzer::functionDefinition()
{
	if(print_production)
	{
		out_file << "<Function Definitions>  ::= <Function> "
					"| <Function> <Function Definitions>" << endl;
	}

	//Same as first calling function, then functionDefinition over and over
	do
	{
		function();
	}while(current_lexeme == "function");
}

/***************************************************************
 * SyntaxAnalyzer::		function
 * ------------------------------------------------------------
 * Function:
 *		This is the main Function Syntax function, meaning it
 *		checks to make sure the function's parameters are
 *		meant to standard. it will check for what needs to be
 *		in place. if what is expected isn't an error will be called
 *		and if it is meant it will grab the next token and do a
 *		similar syntax check inside the function in which is called.
 ***************************************************************/
void SyntaxAnalyzer::function()
{
	if(print_production)
	{
		out_file << "<Function> ::= function  <Identifier>  "
					"[ <Opt Parameter List> ]   <Opt Declaration List>  <Body>" << endl;
	}

	if(current_lexeme == "function")
	{
		simulatedLexerCall();

		if(current_token == "Identifier")
		{
			simulatedLexerCall();

			if(current_lexeme == "[")
			{
				simulatedLexerCall();
				optParameterList();

				if(current_lexeme == "]")
				{
					simulatedLexerCall();
					optDeclarationList();
					body();
				}
				else
				{
					errorExpectedThis("]");
				}
			}
			else
			{
				errorExpectedThis("[");
			}
		}
		else
		{
			errorExpectedThis("an identifier");
		}
	}
	else
	{
		errorExpectedThis("function");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		ParameterList
 * ------------------------------------------------------------
 * Function:
 *		This function will check for parameter syntax meaning
 *		it checks for a parameter list or a an empty space
 *		if found for parameterlist meaning theres an identifier
 *		it will enter parameterlist() or send out an error.
 ***************************************************************/
void SyntaxAnalyzer::optParameterList()
{
	if(print_production)
	{
		out_file << "<Opt Parameter List> ::=  <Parameter List>"
					"   |  <Empty>" << endl;
	}

	//to-do
	if(current_token == "Identifier")
	{
		ParameterList();
	}
	else if(current_lexeme != "]")
	{
		errorExpectedThis("Identifier");
	}
}


/***************************************************************
 * SyntaxAnalyzer::		ParameterList
 * ------------------------------------------------------------
 * Function:
 *		This function will check for the syntax to a parameter
 *		list meaning there must be a parameter then a recursive
 *		call back to parameterlist meaning another parameter.
 *		also checks to make sure there is a comma in between each.
 ***************************************************************/
void SyntaxAnalyzer::ParameterList()
{
	if(print_production)
	{
		out_file << "<Parameter List>  ::=  <Parameter>  "
					"| <Parameter> , <Parameter List>" << endl;
	}
		//To help terminate do-while loop
		bool hasComma 		= true;

		//Same as calling declaration first, then ',', ParameterList,
		//and looping back if necessary
		do
		{
			parameter();

			if(current_lexeme == ",")
			{
				simulatedLexerCall();
			}
			else
			{
				hasComma = false;
			}

		}while(hasComma);
}


/***************************************************************
 * SyntaxAnalyzer::		parameter
 * ------------------------------------------------------------
 * Function:
 *		This function checks to make sure the parameters that
 *		are of the current token are correct to syntax standards.
 *		if correct a qualifier function should be called. if not
 *		it will send an our a corresponding error message.
 ***************************************************************/
void SyntaxAnalyzer::parameter()
{
	if(print_production)
	{
		out_file << "<Parameter> ::=  <Identifier> : <Qualifier>" << endl;
	}

	if(current_token == "Identifier")
	{
		simulatedLexerCall();

		if(current_lexeme == ":")
		{
			simulatedLexerCall();
			qualifier();
		}
		else
		{
			errorExpectedThis(":");
		}
	}
	else
	{
		errorExpectedThis("identifier");
	}
}


/***************************************************************
 * SyntaxAnalyzer::		qualifier
 * ------------------------------------------------------------
 * Function:
 *		This function will check to see if the qualifier is
 *		correct in the sense if it is a int or boolean or real
 *		and if it is then grabs the next token. if not it will
 *		output a error message.
 ***************************************************************/
void SyntaxAnalyzer::qualifier()
{
	if(print_production)
	{
		out_file << "<Qualifier> ::= int   |  boolean   |  real" << endl;
	}

	if(current_lexeme == "int" 		||
	   current_lexeme == "boolean" 	||
	   current_lexeme == "real"		 )
	{
		simulatedLexerCall();
	}
	else
	{
		errorExpectedThis("int or boolean or real");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		body
 * ------------------------------------------------------------
 * Function:
 *		this function will check to see if there a statement list
 *		with the proper enclosing separators. if so it will
 *		continue to statementlist() if not will output error.
 ***************************************************************/
void SyntaxAnalyzer::body()
{
	if(print_production)
	{
		out_file << "<Body>  ::=  {  < Statement List> }" << endl;
	}

	if(current_lexeme == "{")
    {
		simulatedLexerCall();
		statementList();

		if(current_lexeme == "}")
    	{
			simulatedLexerCall();
    	}
		else
		{
			errorExpectedThis("}");
		}
	}
	else
	{
		errorExpectedThis("{");
	}
}



/***************************************************************
 * SyntaxAnalyzer::		optDeclarationList
 * ------------------------------------------------------------
 * Function:
 *		This function helps with dealing with declaration list
 *		in the sense it does a first check from a type. if correct
 *		it will then call declarationlist() if not sends out a
 *		corresponding error message.
 ***************************************************************/
void SyntaxAnalyzer::optDeclarationList()
{
	if(print_production)
	{
		out_file << "<Opt Declaration List> ::= <Declaration List>   "
					"| <Empty>" << endl;
	}

	//to-do
	if(current_lexeme == "int" 		||
	   current_lexeme == "boolean" 	||
	   current_lexeme == "real")
	{
		declarationList();
	}
	else if(current_token == "Unknown")
	{
		errorExpectedThis("int, boolean, real, or nothing");
	}
	else
	{
		TheListIsEmpty();
	}
}

/***************************************************************
 * SyntaxAnalyzer::		declarationList
 * ------------------------------------------------------------
 * Function:
 *		This function will check to see after the first
 *		Initial check in the helperDeclarationList() if the
 *		list has the correct syntax meaning checks for correct
 *		Terminating symbols and/or if the variables are bool or int,
 *		if not will send a correct error list.
 ***************************************************************/
void SyntaxAnalyzer::declarationList()
{
	if(print_production)
	{
		out_file << "<Declaration List>  := <Declaration> ;  "
					"| <Declaration> ; <Declaration List>" << endl;
	}
	//To help terminate do-while loop
	bool hasSemicolon 	= false;
	bool completed 		= false;

	//Same as calling declaration first, then ;, declarationList,
	//and looping back if necessary
	do
	{
		declaration();

		if(current_lexeme == ";")
		{
			hasSemicolon = true;

			simulatedLexerCall();

			if(current_lexeme != "int" 		||
			   current_lexeme != "boolean" 	||
			   current_lexeme != "real")
			{
				completed = true;
			}
		}
		else
		{
			hasSemicolon = false;
		}
	}while(!completed && hasSemicolon);

	if(!hasSemicolon)
	{
		errorExpectedThis(";");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		declaration
 * ------------------------------------------------------------
 * Function:
 *		This function will check to see if the declaration has
 *		met standard and is a correct qualifier or id.
 ***************************************************************/
void SyntaxAnalyzer::declaration()
{
	if(print_production)
	{
		out_file << "<Declaration> ::=  <Qualifier> <IDs>" << endl;
	}

	qualifier();
	IDs();
}


/***************************************************************
 * SyntaxAnalyzer::		iDs
 * ------------------------------------------------------------
 * Function:
 *		This function will check to see if the current token
 *		is correct of an identifier location. if correct
 *		it will then grab the next token and check that, if
 *		wrong will output a error messgae.
 ***************************************************************/
void SyntaxAnalyzer::IDs()
{
	if(print_production)
	{
		out_file << "<IDs> ::=  <Identifier>    | <Identifier>, <IDs>" << endl;
	}

	//to-do; possible backtrack
	bool more_id = false;

	//Same as calling identifier, then calling IDs if comma exists
	if(current_token == "Identifier")
	{
		simulatedLexerCall();

		do
		{
			if(current_lexeme == ",")
			{
				simulatedLexerCall();

				if(current_token == "Identifier")
				{
					simulatedLexerCall();

					more_id = true;
				}
				else
				{
					errorExpectedThis("an identifier");
				}
			}
			else
			{
				more_id = false;
			}

		}while(more_id);
	}
	else
	{
		errorExpectedThis("an identifier");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		statementList
 * ------------------------------------------------------------
 * Function:
 *		This function will check to see if the current token
 *		its a identifier or a keyword to correspond to to a
 *		function. it will first try once and then can try again
 *		if successful will head to statement()
 ***************************************************************/
void SyntaxAnalyzer::statementList()
{
	if(print_production)
	{
		out_file << "<Statement List> ::=  <Statement> "
					"| <Statement> <Statement List>" << endl;
	}


	//Same as calling statement, then (if needed)
	// recalling statement list
	do
	{
		statement();
	}while( current_lexeme == "{" 			||
			current_token  == "Identifier" 	||
			current_lexeme == "if" 			||
			current_lexeme == "return" 		||
			current_lexeme == "write" 		||
			current_lexeme == "read" 		||
			current_lexeme == "while");
}

/***************************************************************
 * SyntaxAnalyzer::		statement
 * ------------------------------------------------------------
 * Function:
 *		This function will now call the correct function to the
 *		correct keyword to make sure the following token is
 *		correct. meaning if 'if' is called it will check the
 *		syntax of the next few tokens to make sure its correct.
 ***************************************************************/
void SyntaxAnalyzer::statement()
{
	if(print_production)
	{
		out_file << "<Statement> ::=  <Compound> | <Assign> | <If> "
					"| <Return> | <Write> | <Read> | <While>" << endl;
	}

	//to-do; a backtrack procedure (?)

	if(current_lexeme 		== "{")
	{
		compound();
	}
	else if(current_token 	== "Identifier")
	{
		assign();
	}
	else if(current_lexeme 	== "if")
	{
		If();
	}
	else if(current_lexeme 	== "return")
	{
		SAreturn();
	}
	else if(current_lexeme 	== "write")
	{
		write();
	}
	else if(current_lexeme 	== "read")
	{
		read();
	}
	else if(current_lexeme 	== "while")
	{
		whileSyntax();
	}
	else
	{
		errorExpectedThis("{, an identifier, if, return, write, read, or while");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		compound
 * ------------------------------------------------------------
 * Function:
 *		This function will check to see if a { is in place them
 *		call the next token into the statement list function
 *		and once that is finished to make sure to have } at the
 *		end or output which bracket is missing.
 ***************************************************************/
void SyntaxAnalyzer::compound()
{
	if(print_production)
	{
		out_file << "<Compound> ::= {  <Statement List>  }" << endl;
	}

	if(current_lexeme == "{")
	{
		simulatedLexerCall();
		statementList();

		if(current_lexeme == "}")
		{
			simulatedLexerCall();
		}
		else
		{
			errorExpectedThis("}");
		}
	}
	else
	{
		errorExpectedThis("{");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		assign
 * ------------------------------------------------------------
 * Function:
 *		This function will make sure proper assigning variable
 *		sytnax is followed meaning there is a := followed by an
 *		expression and leading with a identifier.
 ***************************************************************/
void SyntaxAnalyzer::assign()
{
	if(print_production)
	{
		out_file << "<Assign> ::=   <Identifier> := <Expression> ;" << endl;
	}

	if(current_token == "Identifier")
	{
		simulatedLexerCall();

		if(current_lexeme == ":=")
		{
			simulatedLexerCall();
			expression();

			if(current_lexeme == ";")
			{
				simulatedLexerCall();
			}
			else
			{
				errorExpectedThis(";");
			}
		}
		else
		{
			errorExpectedThis(":=");
		}
	}
	else
	{
		errorExpectedThis("an identifier");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		If
 * ------------------------------------------------------------
 * Function:
 *		This function will make sure proper If statement
 *		sytnax is followed meaning some kind of condition and
 *		then a statement or ifRulePrime.
 ***************************************************************/
void SyntaxAnalyzer::If()
{
	if(print_production)
	{
		out_file << "<ifRule> ::= if ( <Condition>  ) "
					"<Statement> <newIfRule>" << endl;
	}

	if(current_lexeme == "if")
	{
		simulatedLexerCall();

		if(current_lexeme == "(")
		{
			simulatedLexerCall();
			condition();

			if(current_lexeme == ")")
			{
				simulatedLexerCall();
				statement();
				ifRulePrime();
			}
			else
			{
				errorExpectedThis(")");
			}
		}
		else
		{
			errorExpectedThis("(");
		}
	}
	else
	{

		errorExpectedThis("if");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		ifRulePrime
 * ------------------------------------------------------------
 * Function:
 *		This function is made to deal with left-factorization
 *		using the endif statement in between to give a correct
 *		and easy way to deal with left-factorization.
 ***************************************************************/
void SyntaxAnalyzer::ifRulePrime()
{
	if(print_production)
	{
		out_file << "<newIfRule> ::= endif | else <Statement> endif" << endl;
	}

	if(current_lexeme == "endif")
	{
		simulatedLexerCall();
	}
	else if(current_lexeme == "else")
	{
		simulatedLexerCall();
		statement();

		if(current_lexeme == "endif")
		{
			simulatedLexerCall();
		}
		else
		{
			errorExpectedThis("endif");
		}
	}
	else
	{
		errorExpectedThis("endif or else");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		SAreturn
 * ------------------------------------------------------------
 * Function:
 *		This function is meant to check proper syntax if a
 *		return is token is found and how it must be properly
 *		Executed.
 ***************************************************************/
void SyntaxAnalyzer::SAreturn()
{
	if(print_production)
	{
		out_file << "<Return> ::=  return; | return <Expression> ;" << endl;
	}

	if(current_lexeme == "return")
	{
		if(current_lexeme != ";")
		{
			simulatedLexerCall();
			expression();
		}

		if(current_lexeme == ";")
		{
			simulatedLexerCall();
		}
		else
		{
			errorExpectedThis(";");
		}
	}
	else
	{
		errorExpectedThis("return");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		write
 * ------------------------------------------------------------
 * Function:
 *		This function will check proper syntax of a write function
 *		meaning it checks for write then ( and then calls
 *		expression function to check a proper expression and then
 *		checks to see if it ends with a ).
 ***************************************************************/
void SyntaxAnalyzer::write()
{
	if(print_production)
	{
		out_file << "<Write> ::=   write ( <Expression> );" << endl;
	}

	if(current_lexeme == "write")
	{
		simulatedLexerCall();

		if(current_lexeme == "(")
		{
			simulatedLexerCall();
			expression();

			if(current_lexeme == ")")
			{
				simulatedLexerCall();

				if(current_lexeme == ";")
				{
					simulatedLexerCall();
				}
				else
				{
					errorExpectedThis(";");
				}
			}
			else
			{
				errorExpectedThis(")");
			}
		}
		else
		{
			errorExpectedThis("(");
		}
	}
	else
	{
		errorExpectedThis("write");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		read
 * ------------------------------------------------------------
 * Function:
 *		This function is meant to handle read function calls
 *		meaning a token read has been found and now must have a
 *		( and call IDs function and then end with a )
 ***************************************************************/
void SyntaxAnalyzer::read()
{
	if(print_production)
	{
		out_file << "<Read> ::= read ( <IDs> );" << endl;
	}

	if(current_lexeme == "read")
	{
		simulatedLexerCall("read");

		if(current_lexeme == "(")
		{
			simulatedLexerCall();
			IDs();

			if(current_lexeme == ")")
			{
				simulatedLexerCall();

				if(current_lexeme == ";")
				{
					simulatedLexerCall();
				}
				else
				{
					errorExpectedThis(";");
				}
			}
			else
			{
				errorExpectedThis(")");
			}
		}
		else
		{
			errorExpectedThis("(");
		}
	}
	else
	{
		errorExpectedThis("read");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		whileSyntax
 * ------------------------------------------------------------
 * Function:
 *		This function is meant to handle the while syntax,
 *		this mean it checks for proper while format, meaning
 *		if theres a ( and condition then an end )
 ***************************************************************/
void SyntaxAnalyzer::whileSyntax()
{
	if(print_production)
	{
		out_file << "<While> ::= while ( <Condition>  )  <Statement>" << endl;
	}

	if(current_lexeme == "while")
	{
		simulatedLexerCall();

		if(current_lexeme == "(")
		{
			simulatedLexerCall();
			condition();

			if(current_lexeme == ")")
			{
				simulatedLexerCall();
				statement();
			}
			else
			{
				errorExpectedThis(")");
			}
		}
		else
		{
			errorExpectedThis("(");
		}
	}
	else
	{
		errorExpectedThis("while");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		condition
 * ------------------------------------------------------------
 * Function:
 *		a Conditional statement means there is a expression then
 *		some kind of relational operator then another expression
 *		it will check this by calling each function one after
 *		another.
 ***************************************************************/
void SyntaxAnalyzer::condition()
{
	if(print_production)
	{
		out_file << "<Condition> ::= <Expression>  "
					"<relationalOperator>   <Expression>" << endl;
	}

	expression();
	relationalOperator();
	expression();
}

/***************************************************************
 * SyntaxAnalyzer::		relationalOperator
 * ------------------------------------------------------------
 * Function:
 *		This function will check for proper relational
 *		operators and will output what needs to be each time.
 ***************************************************************/
void SyntaxAnalyzer::relationalOperator()
{
	if(print_production)
	{
		out_file << "<relationalOperator> ::=   = |  !=  |   >   "
					"| < | => | <=" << endl;
	}

	if(	current_lexeme == "=" 	||
		current_lexeme == "!=" 	||
		current_lexeme == ">" 	||
		current_lexeme == "<" 	||
		current_lexeme == "=>" 	||
		current_lexeme == "<=" 	 )
	{
		simulatedLexerCall();
	}
	else
	{
		errorExpectedThis("=, !=, >, <, =>, <=");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		expression
 * ------------------------------------------------------------
 * Function:
 *		This function will check for a proper expression meaning
 *		what will be called it uses term() and expressionPrime()
 ***************************************************************/
void SyntaxAnalyzer::expression()
{
	if(print_production)
	{
		out_file << "<expression> ::= <term> <expressionPrime>" << endl;
	}

	term();
	expressionPrime();
}

/***************************************************************
 * SyntaxAnalyzer::		expressionPrime
 * ------------------------------------------------------------
 * Function:
 *		This function is used to handle left hand recursion,
 *		meaning it will add and subtract expressions.
 ***************************************************************/
void SyntaxAnalyzer::expressionPrime()
{
	if(print_production)
	{
		out_file << "<expressionPrime>  ::= + <Term> <expressionPrime>"
					"  | - <Term> <expressionPrime>  | <Empty>" << endl;
	}

	if(current_lexeme == "+")
	{
		simulatedLexerCall();
		term();
		expressionPrime();
	}
	else if (current_lexeme == "-")
	{
		simulatedLexerCall();
		term();
		expressionPrime();
	}
	else if(current_token == "Unknown")
	{
		errorExpectedThis("+, -, or nothing");
	}
	else
	{
		TheListIsEmpty();
	}
}

/***************************************************************
 * SyntaxAnalyzer::		term
 * ------------------------------------------------------------
 * Function:
 *		This function will check proper factor format of a new
 *		term meaning it will process a term.
 ***************************************************************/
void SyntaxAnalyzer::term()
{
	if(print_production)
	{
		out_file << "<term> ::= <Factor> <termPrime>" << endl;
	}

	factor();
	termPrime();
}

/***************************************************************
 * SyntaxAnalyzer::		termPrime
 * ------------------------------------------------------------
 * Function:
 *		This function is for left hand recursion and will correctly
 *		make sure a term has proper syntax. needed because there
 *		are small cases that need to be handled.
 ***************************************************************/
void SyntaxAnalyzer::termPrime()
{
	if(print_production)
	{
		out_file << "<termPrime> ::= * <Factor> <termPrime> | "
					"/ <Factor> <termPrime> | <Empty>" << endl;
	}

	if(current_lexeme == "*")
	{
		simulatedLexerCall();
		factor();
		termPrime();
	}
	else if(current_lexeme == "/")
	{
		simulatedLexerCall();
		factor();
		termPrime();
	}
	else
	{
		TheListIsEmpty();
	}
}

/***************************************************************
 * SyntaxAnalyzer::		factor
 * ------------------------------------------------------------
 * Function:
 *		This function is used to make sure there is a primary
 *		then another primary when factor is called and will
 *		call primary() to make sure what is being used is correct
 ***************************************************************/
void SyntaxAnalyzer::factor()
{
	if(print_production)
	{
		out_file << "<Factor> ::= - <Primary> | <Primary>" << endl;
	}

	if(current_lexeme == "-")
	{
		simulatedLexerCall();
		primary();
	}
	else
	{
		primary();
	}
}

/***************************************************************
 * SyntaxAnalyzer::		primary
 * ------------------------------------------------------------
 * Function:
 *		This function is used to make sure the token is a
 *		identifier or integer, or IDs or expression or real or
 *		true or false. and will check to make sure the format is
 *		correct per type of primary.
 ***************************************************************/
void SyntaxAnalyzer::primary()
{
	if(print_production)
	{
		out_file << "<Primary> ::= <Identifier> | <Integer> "
					"| <Identifier> [<IDs>]  | ( <Expression> )  "
					"| <Real> | true | false" << endl;
	}

	if(current_token == "Identifier")
	{
		simulatedLexerCall();

		if(current_lexeme == "[")
		{
			simulatedLexerCall();
			IDs();

			if(current_lexeme == "]")
			{
				simulatedLexerCall();
			}
			else
			{
				errorExpectedThis("]");
			}
		}
	}
	else if(current_token == "Integer" ||
			current_token == "Real")
	{
		simulatedLexerCall();
	}
	else if(current_lexeme == "(")
	{
		simulatedLexerCall();
		expression();

		if(current_lexeme == ")")
		{
			simulatedLexerCall();
		}
		else
		{
			errorExpectedThis(")");
		}
	}
	else if(current_lexeme == "true")
	{
		simulatedLexerCall();
	}
	else if(current_lexeme == "false")
	{
		simulatedLexerCall();
	}
	else
	{
		errorExpectedThis("one or more identifiers, an integer, "
							"an expression, real, true, or false");
	}
}

/***************************************************************
 * SyntaxAnalyzer::		TheListIsEmpty
 * ------------------------------------------------------------
 * Function:
 *		This is a error message output meaning the list or
 *		spot is empty or also known as epsilon
 ***************************************************************/
void SyntaxAnalyzer::TheListIsEmpty()
{
	if(print_production)
	{
		out_file << "<Empty> ::= epsilon" << endl;
	}
}
