/**************************************************************************************
 * Syntax Analyzer.h
 *
 * Programmer 1		: Cameron Walters
 * Programmer 2		: Sina Amini
 *************************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>

#include "Token.h"

using namespace std;

/***************************************************************
 * SyntaxAnalyzer
 * ------------------------------------------------------------
 * Class:
 *
 ***************************************************************/
class SyntaxAnalyzer
{
	public:

		SyntaxAnalyzer()
		{
			error 			= false;
			print_production 	= false;
			current_line 	= 0;
		}

		~SyntaxAnalyzer()
		{

		}

		/***************************************************************
		 * SyntaxAnalyzer::		analyzeSyntax
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will make sure the vector list of tokens
		 *		coming in isn't empty, cause if its empty there's no
		 *		reason to run. after found populated list it will then
		 *		open and start the process of the output file. once
		 *		open it then enters the second if which heads to the
		 *		start of the list and then calls a simulated Lexer call
		 *		which grabs the first token in list then takes it to the
		 *		rat15S which is the analyzer of Syntax and begins the
		 *		Syntax Analyzer as a whole. Once it has found an error
		 *		or finishes it will then output to the console where
		 *		the file was saved and then exit the function.
		 ***************************************************************/
		void analyzeSyntax(const vector<Token>& t_list,
								 string 		in_name,
								 bool 			print = true)
		{
			if(!t_list.empty())
			{
				in_name.erase(in_name.length()-4, in_name.length());

				string out_name = in_name.append("_output.txt");

				out_file.open(out_name.c_str());

				if(out_file.is_open())
				{
					print_production = print;

					//Set up internal token list (to be identical to given token list
					for(unsigned int index = 0; index < t_list.size(); ++index)
					{
						received_tokens.push_back(t_list.at(index));
					}

					//Get first lexeme and call first nonterminal in grammar
					simulatedLexerCall();
					rat15S();

					out_file << "Reached EOF with ";

					if(!error)
					{
						out_file << "no ";
					}

					out_file << "errors found." << endl;

					out_file.close();

					cout << "Finished." << endl;
					cout << "\nCheck output file \"" << out_name << "\" for your results, "
							"which is within the same directory as this executable." << endl;
				}
				else
				{
					cout << "Error when creating and writing to output file." << endl;
				}
			}
			else
			{
				cout << "No tokens to analyze." << endl;
			}
		}

		/***************************************************************
		 * SyntaxAnalyzer::		emptyLists
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will empty the list and clear them for use
		 ***************************************************************/
		//Clear lists (and reset error flag)
		void emptyLists()
		{
			input.clear();
			received_tokens.clear();

			error = false;
		}

	private:

		deque<Token> 	input;            		//use this "stack" to list characters grabbed from token list
		deque<Token> 	received_tokens;  		//for getting the token list from the lexer; should be deque
		bool 		 	print_production;          //production print toggle/switch
		bool 			error;                  //error toggle (to let the user know an error was found -
												// - while analyzing the syntax
		string 			current_lexeme;			//for getting the current lexeme (set by getCurTokenInfo())
		string 			current_token;			//for getting the current token type (set by getCurTokenInfo())
		int 			current_line;           //for getting the current token's line number
		ofstream 		out_file;               //for writing the syntax analysis to an output file


		/***************************************************************
		 * SyntaxAnalyzer::		simulatedLexerCall
		 * ------------------------------------------------------------
		 * Function:
		 *		This will grab a token and it to the stack
		 ***************************************************************/
		void simulatedLexerCall(bool print = true)
		{
			if(!received_tokens.empty())
			{
				//Add to input stack, dequeue received tokens list
				input.push_back(received_tokens.front());
				received_tokens.pop_front();

				//Set new current lexeme and type
				getCurrentTokenInfo();
			}
		}

		/***************************************************************
		 * SyntaxAnalyzer::		errorExpectedThis
		 * ------------------------------------------------------------
		 * Function:
		 *		This is a error messaging function, simply pass in
		 *		the string that is expect and a general error message
		 *		is then output to the outfile. Then the error variable
		 *		will then be turned back to true. Then it will call
		 *		the function simulatedLexerCall to continue the program.
		 ***************************************************************/
		void errorExpectedThis(string expectedToken)
		{
			out_file << "\nERROR: Expected: " << expectedToken << endl;

			out_file << "Found the following: Token \"" << current_token << "\" and Lexeme \"" << current_lexeme
					 << "\" on line " << current_line << ".\nAttempting to continue syntax analysis.\n";

			error = true;

			simulatedLexerCall();
		}

		/***************************************************************
		 * SyntaxAnalyzer::		getCurTokenInfo
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will get the current token's type and
		 *		lexeme while the input stack isn't empty. then prints
		 *		the lexeme to the outfile or outputs a error message
		 ***************************************************************/
		void getCurrentTokenInfo(bool print = true)
		{
			if(!input.empty())
			{
				Token temp 		= input.back();

				current_lexeme 	= temp.getActualValueOfToken();
				current_token 	= temp.getTypeOfToken();
				current_line	= temp.getLineNumber();


				if(print)
				{
					out_file <<"-----------------------------------------------------------" << endl;
					out_file << left << "Token: " << setw(14) << current_token
							 << "Lexeme: " << setw(14) << current_lexeme <<
							 	"Line: " << current_line << endl;
				}
			}
			else
			{
				cout << "Input deque is empty.\n";
			}
		}

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
		void rat15S();

		/***************************************************************
		 * SyntaxAnalyzer::		optDeclarationList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is a helper function to the function
		 *		definition function. It will output what is expected
		 *		and check to see if Function is the token and if it is
		 *		it will call the functiondefinition() and if not
		 *		it will report a error message.
		 ***************************************************************/
		void optFunctionDefinition();

		/***************************************************************
		 * SyntaxAnalyzer::		functionDefinition
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will output the expected token if print is
		 *		turned on. it will then call function() once and if needed
		 *		it will continue calling it till its wrong.
		 ***************************************************************/
		void functionDefinition();

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
		void function();

		/***************************************************************
		 * SyntaxAnalyzer::		optParameterList
		 * ------------------------------------------------------------
		 * Function:
		 *		This is the main Function Syntax function, meaning it
		 *		checks to make sure the function's parameters are
		 *		meant to standard. it will check for what needs to be
		 *		in place. if what is expected isn't an error will be called
		 *		and if it is meant it will grab the next token and do a
		 *		similar syntax check inside the function in which is called.
		 ***************************************************************/
		void optParameterList();

		/***************************************************************
		 * SyntaxAnalyzer::		ParameterList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for the syntax to a parameter
		 *		list meaning there must be a parameter then a recursive
		 *		call back to parameterlist meaning another parameter.
		 *		also checks to make sure there is a comma in between each.
		 ***************************************************************/
		void ParameterList();

		/***************************************************************
		 * SyntaxAnalyzer::		parameter
		 * ------------------------------------------------------------
		 * Function:
		 *		This function checks to make sure the parameters that
		 *		are of the current token are correct to syntax standards.
		 *		if correct a qualifier function should be called. if not
		 *		it will send an our a corresponding error message.
		 ***************************************************************/
		void parameter();


		/***************************************************************
		 * SyntaxAnalyzer::		qualifier
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the qualifier is
		 *		correct in the sense if it is a int or boolean and if
		 *		it is then grabs the next token. if not it will output
		 *		a error message.
		 ***************************************************************/
		void qualifier();

		/***************************************************************
		 * SyntaxAnalyzer::		body
		 * ------------------------------------------------------------
		 * Function:
		 *		this function will check to see if there a statement list
		 *		with the proper enclosing separators. if so it will
		 *		continue to statementlist() if not will output error.
		 ***************************************************************/
		void body();

		/***************************************************************
		 * SyntaxAnalyzer::		helperDeclarationList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function helps with dealing with declaration list
		 *		in the sense it does a first check from a type. if correct
		 *		it will then call declarationlist() if not sends out a
		 *		corresponding error message.
		 ***************************************************************/
		void optDeclarationList();

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
		void declarationList();

		/***************************************************************
		 * SyntaxAnalyzer::		declaration
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the declaration has
		 *		met standard and is a correct qualifier or id.
		 ***************************************************************/
		void declaration();

		/***************************************************************
		 * SyntaxAnalyzer::		iDs
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the current token
		 *		is correct of an identifier location. if correct
		 *		it will then grab the next token and check that, if
		 *		wrong will output a error messgae.
		 ***************************************************************/
		void IDs();


		/***************************************************************
		 * SyntaxAnalyzer::		statementList
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if the current token
		 *		its a identifier or a keyword to correspond to to a
		 *		function. it will first try once and then can try again
		 *		if successful will head to statement()
		 ***************************************************************/
		void statementList();

		/***************************************************************
		 * SyntaxAnalyzer::		statement
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will now call the correct function to the
		 *		correct keyword to make sure the following token is
		 *		correct. meaning if 'if' is called it will check the
		 *		syntax of the next few tokens to make sure its correct.
		 ***************************************************************/
		void statement();

		/***************************************************************
		 * SyntaxAnalyzer::		compound
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check to see if a { is in place them
		 *		call the next token into the statement list function
		 *		and once that is finished to make sure to have } at the
		 *		end or output which bracket is missing.
		 ***************************************************************/
		void compound();

		/***************************************************************
		 * SyntaxAnalyzer::		assign
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will make sure proper assigning variable
		 *		sytnax is followed meaning there is a := followed by an
		 *		expression and leading with a identifier.
		 ***************************************************************/
		void assign();

		/***************************************************************
		 * SyntaxAnalyzer::		If
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will make sure proper If statement
		 *		sytnax is followed meaning some kind of condition and
		 *		then a statement or ifRulePrime.
		 ***************************************************************/
		void If();

		/***************************************************************
		 * SyntaxAnalyzer::		ifRulePrime
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is made to deal with left-factorization
		 *		using the endif statement in between to give a correct
		 *		and easy way to deal with left-factorization.
		 ***************************************************************/
		void ifRulePrime();

		/***************************************************************
		 * SyntaxAnalyzer::		SAreturn
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is meant to check proper syntax if a
		 *		return is token is found and how it must be properly
		 *		Executed.
		 ***************************************************************/
		void SAreturn();

		/***************************************************************
		 * SyntaxAnalyzer::		write
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check proper syntax of a write function
		 *		meaning it checks for write then ( and then calls
		 *		expression function to check a proper expression and then
		 *		checks to see if it ends with a ).
		 ***************************************************************/
		void write();

		/***************************************************************
		 * SyntaxAnalyzer::		read
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is meant to handle read function calls
		 *		meaning a token read has been found and now must have a
		 *		( and call IDs function and then end with a )
		 ***************************************************************/
		void read();

		/***************************************************************
		 * SyntaxAnalyzer::		whileSyntax
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is meant to handle the while syntax,
		 *		this mean it checks for proper while format, meaning
		 *		if theres a ( and condition then an end )
		 ***************************************************************/
		void whileSyntax();

		/***************************************************************
		 * SyntaxAnalyzer::		condition
		 * ------------------------------------------------------------
		 * Function:
		 *		a Conditional statement means there is a expression then
		 *		some kind of relational operator then another expression
		 *		it will check this by calling each function one after
		 *		another.
		 ***************************************************************/
		void condition();

		/***************************************************************
		 * SyntaxAnalyzer::		relationalOperator
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for proper relational
		 *		operators and will output what needs to be each time.
		 ***************************************************************/
		void relationalOperator();

		/***************************************************************
		 * SyntaxAnalyzer::		expression
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check for a proper expression meaning
		 *		what will be called it uses term() and expressionPrime()
		 ***************************************************************/
		void expression();

		/***************************************************************
		 * SyntaxAnalyzer::		expressionPrime
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is used to handle left hand recursion,
		 *		meaning it will add and subtract expressions.
		 ***************************************************************/
		void expressionPrime();

		/***************************************************************
		 * SyntaxAnalyzer::		term
		 * ------------------------------------------------------------
		 * Function:
		 *		This function will check proper factor format of a new
		 *		term meaning it will process a term.
		 ***************************************************************/
		void term();

		/***************************************************************
		 * SyntaxAnalyzer::		termPrime
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is for left hand recursion and will correctly
		 *		make sure a term has proper syntax. needed because there
		 *		are small cases that need to be handled.
		 ***************************************************************/
		void termPrime();

		/***************************************************************
		 * SyntaxAnalyzer::		factor
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is used to make sure there is a primary
		 *		then another primary when factor is called and will
		 *		call primary() to make sure what is being used is correct
		 ***************************************************************/
		void factor();

		/***************************************************************
		 * SyntaxAnalyzer::		primary
		 * ------------------------------------------------------------
		 * Function:
		 *		This function is used to make sure the token is a
		 *		identifier or integer, or IDs or expression or real or
		 *		true or false. and will check to make sure the format is
		 *		correct per type of primary.
		 ***************************************************************/
		void primary();

		/***************************************************************
		 * SyntaxAnalyzer::		TheListIsEmpty
		 * ------------------------------------------------------------
		 * Function:
		 *		This is a error message output meaning the list or
		 *		spot is empty or also known as epsilon
		 ***************************************************************/
		void TheListIsEmpty();


};
