# Compiler

Sinamin Compiler Documentation
 
Problem Statement
The syntax analyzer, which checks if the tokens, generated from the lexical analyzer, are syntactically correct. The syntax analyzer determines the program’s underlying structure prior to semantic analysis and translation.

How to use
1. Double-click and open "SyntaxAnalyzer.exe". There are on-screen directions from there.
2. When the syntax analyzer is finished, the output file will be in the same directory as the executable. The program assumes your extension is four characters long, including the period.

Design
1. Main and Lexical Analyzer
Everything in the lexical analysis portion and main driver is the same other than the addition of a line counting method and disabling printing the tokens to the console. We disabled the token list printing to the console and assigned line numbers per token. Also, right after getting the token list, the program calls "analyzeSyntax" with the SyntaxAnalyzer object (in main) to analyze the token list given. It should be noted that the Lexical Analyzer will output to the same text file every time the program runs. (Although the Syntax Analyzer will write to a new text file every time).
2. Syntax Analyzer

The syntax analyzer is made up of two separate files: "SyntaxAnalyzer.h" and "SyntaxAnalyzer.cpp". The .cpp contains all implementation for the procedure lists and the .h includes everything else needed for the syntax analysis. 	
1. When called, analyzeSyntax initializes the input stack, the received tokens queue, and the file that output will be written to.
2. As soon as initialization is complete, the syntax analyzer grabs the first token by calling "simulatedLexerCall". "simulatedLexerCall " grabs a token from the token list (synonymous to calling lexer). The first procedure is then called, and "simulatedLexerCall ", "getCurrentTokenInfo" (provides the token information to know which procedure to call next), and the other procedures are called in order until completed.
			2a. If tokens are valid, procedures are printed upon accessing them.
2b. If any errors occur during the program, the analyzer attempts to	 re-continue by going to the next procedure it falls into.
In order to make the recursive descent parser work, we had to remove all left recursions and prevent backtracking issues by means of left factorization. How we fixed this is listed below.

Rewriting the grammar RAT15 to remove Left Recursion.
Left recursion was found in the following production rules:
<Expression> ::= <Expression> + <Term> | <Expression> - <Term> | <Term>
<Term> ::= <Term> * <Factor> | <Term> / <Factor> | <Factor>

Breaking down the production rules: 
<Expression>  ->  <Expression> + <Term>
<Expression>  ->  <Expression> - <Term>
<Expression>  ->  <Term>
<Term>  ->  <Term> * <Factor>
<Term>  ->  <Term> / <Factor>
<Term>  ->  <Factor>
Resolving the Left Recursion:
From Rule 1 and 3:          
<Expression>  ->  <Term> <Expression'>
<Expression>  ->  + <Term> <Expression'> | <Empty>

From Rule 2 and 3:
<Expression>  ->  <Term> <Expression'>
<Expression>  ->   - <Term> <Expression'> | <Empty>

From Rule 4 and 6:
<Term>  ->  <Factor> <Term'>
<Term>  ->  * <Factor> <Term'>

From Rule 5 and 6:
<Term>  ->  <Factor> <Term'>
<Term>  ->  / <Factor> <Term'>

Collecting all the rules together:
<Expression>  ->  <Term> <Expression'>
<Expression>  ->  + <Term> <Expression'> | - <Term> <Expression'> | <Empty>
<Term>  ->  <Factor> <Term'>
<Term>  ->  * <Factor> <Term'> | / <Factor> <Term'> | <Empty>

*Note*  <Expression'> is named <expressionPrime> and <Term'> is named <termPrime> in the code.

Solving Backtracking using Left Factorization:
Backtracking was found in the following grammar:


         α                        β                           α                                       
       |-------------------------------|  |-----|  |-------------------------------|  |-------------------------|                    
<If> ::= if (<Condition>) <Statement> endif | if (<Condition>) <Statement> else <Statement> endif
  		


                                          

<If >  if (<Condition>) <Statement> <if >
<if >  endif | else <Statement> endif
*Note* <if'> is named <ifRulePrime> in the code.

Any Limitation
None
Any shortcomings
None




					TEST CASES
# ===================================================
# TEST CASE 1 (n < 20)
# ===================================================

/* this is comment for this sample code which
   converts  Fahrenheit into Celcius 
 */

function convert [fahr:int]
{
           return 5*(fahr -32)/9;
}

@@
     int    low, high, step;   /* declarations */

 @@
     read(low, high, step);
     while (low < high)  
         {  write (low);
             write (convert [low]);
             low := low + step;
         } 
# ---------------------------------------------------------------------------------------
# Analyzed Syntax for Test Case 1
# ---------------------------------------------------------------------------------------
-----------------------------------------------------------
Token: Keyword       Lexeme: function      Line: 5
<Rat15S> ::= <Opt Function Definitions>   @@  <Opt Declaration List> @@ <Statement List>  
<Opt Function Definitions> ::= <Function Definitions> | <Empty>
<Function Definitions>  ::= <Function> | <Function> <Function Definitions>
<Function> ::= function  <Identifier>  [ <Opt Parameter List> ]   <Opt Declaration List>  <Body>
-----------------------------------------------------------
Token: Identifier    Lexeme: convert       Line: 5
-----------------------------------------------------------
Token: Separator     Lexeme: [             Line: 5
-----------------------------------------------------------
Token: Identifier    Lexeme: fahr          Line: 5
<Opt Parameter List> ::=  <Parameter List>   |  <Empty>
<Parameter List>  ::=  <Parameter>  | <Parameter> , <Parameter List>
<Parameter> ::=  <Identifier> : <Qualifier>
-----------------------------------------------------------
Token: Operator      Lexeme: :             Line: 5
-----------------------------------------------------------
Token: Keyword       Lexeme: int           Line: 5
<Qualifier> ::= int   |  boolean   |  real
-----------------------------------------------------------
Token: Separator     Lexeme: ]             Line: 5
-----------------------------------------------------------
Token: Separator     Lexeme: {             Line: 6
<Opt Declaration List> ::= <Declaration List>   | <Empty>
<Empty> ::= epsilon
<Body>  ::=  {  < Statement List> }
-----------------------------------------------------------
Token: Keyword       Lexeme: return        Line: 7
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Return> ::=  return; | return <Expression> ;
-----------------------------------------------------------
Token: Integer       Lexeme: 5             Line: 7
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: *             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 7
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Identifier    Lexeme: fahr          Line: 7
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: -             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
-----------------------------------------------------------
Token: Integer       Lexeme: 32            Line: 7
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Operator      Lexeme: /             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
-----------------------------------------------------------
Token: Integer       Lexeme: 9             Line: 7
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: }             Line: 8
-----------------------------------------------------------
Token: Separator     Lexeme: @@            Line: 10
-----------------------------------------------------------
Token: Keyword       Lexeme: int           Line: 11
<Opt Declaration List> ::= <Declaration List>   | <Empty>
<Declaration List>  := <Declaration> ;  | <Declaration> ; <Declaration List>
<Declaration> ::=  <Qualifier> <IDs>
<Qualifier> ::= int   |  boolean   |  real
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 11
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
-----------------------------------------------------------
Token: Separator     Lexeme: ,             Line: 11
-----------------------------------------------------------
Token: Identifier    Lexeme: high          Line: 11
-----------------------------------------------------------
Token: Separator     Lexeme: ,             Line: 11
-----------------------------------------------------------
Token: Identifier    Lexeme: step          Line: 11
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 11
-----------------------------------------------------------
Token: Separator     Lexeme: @@            Line: 13
-----------------------------------------------------------
Token: Keyword       Lexeme: read          Line: 14
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Read> ::= read ( <IDs> );
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 14
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 14
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
-----------------------------------------------------------
Token: Separator     Lexeme: ,             Line: 14
-----------------------------------------------------------
Token: Identifier    Lexeme: high          Line: 14
-----------------------------------------------------------
Token: Separator     Lexeme: ,             Line: 14
-----------------------------------------------------------
Token: Identifier    Lexeme: step          Line: 14
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 14
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 14
-----------------------------------------------------------
Token: Keyword       Lexeme: while         Line: 15
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<While> ::= while ( <Condition>  )  <Statement>
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 15
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 15
<Condition> ::= <Expression>  <relationalOperator>   <Expression>
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: <             Line: 15
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
<relationalOperator> ::=   = |  !=  |   >   | < | => | <=
-----------------------------------------------------------
Token: Identifier    Lexeme: high          Line: 15
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 15
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: {             Line: 16
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Compound> ::= {  <Statement List>  }
-----------------------------------------------------------
Token: Keyword       Lexeme: write         Line: 16
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Write> ::=   write ( <Expression> );
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 16
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 16
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 16
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 16
-----------------------------------------------------------
Token: Keyword       Lexeme: write         Line: 17
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Write> ::=   write ( <Expression> );
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 17
-----------------------------------------------------------
Token: Identifier    Lexeme: convert       Line: 17
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: [             Line: 17
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 17
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
-----------------------------------------------------------
Token: Separator     Lexeme: ]             Line: 17
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 17
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 17
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 18
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: :=            Line: 18
-----------------------------------------------------------
Token: Identifier    Lexeme: low           Line: 18
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: +             Line: 18
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
-----------------------------------------------------------
Token: Identifier    Lexeme: step          Line: 18
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 18
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: }             Line: 19
Reached EOF with no errors found.

# ===================================================
# TEST CASE 2 (n < 10)
# ===================================================

/* 2t.txt Made by Us */
function addTheStuff []
{jack := jacked + threeD;}
	@@
			real 	joe, mondata; /* funny stuff */
@@   return(key);
	while(not = true) 
{ value := 0 - 2;
}

# ---------------------------------------------------------------------------------------
# Analyzed Syntax for Test Case 2
# ---------------------------------------------------------------------------------------
-----------------------------------------------------------
Token: Keyword       Lexeme: function      Line: 2
<Rat15S> ::= <Opt Function Definitions>   @@  <Opt Declaration List> @@ <Statement List>  
<Opt Function Definitions> ::= <Function Definitions> | <Empty>
<Function Definitions>  ::= <Function> | <Function> <Function Definitions>
<Function> ::= function  <Identifier>  [ <Opt Parameter List> ]   <Opt Declaration List>  <Body>
-----------------------------------------------------------
Token: Identifier    Lexeme: addTheStuff   Line: 2
-----------------------------------------------------------
Token: Separator     Lexeme: [             Line: 2
-----------------------------------------------------------
Token: Separator     Lexeme: ]             Line: 2
<Opt Parameter List> ::=  <Parameter List>   |  <Empty>
-----------------------------------------------------------
Token: Separator     Lexeme: {             Line: 3
<Opt Declaration List> ::= <Declaration List>   | <Empty>
<Empty> ::= epsilon
<Body>  ::=  {  < Statement List> }
-----------------------------------------------------------
Token: Identifier    Lexeme: jack          Line: 3
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: :=            Line: 3
-----------------------------------------------------------
Token: Identifier    Lexeme: jacked        Line: 3
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: +             Line: 3
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
-----------------------------------------------------------
Token: Identifier    Lexeme: threeD        Line: 3
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 3
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: }             Line: 3
-----------------------------------------------------------
Token: Separator     Lexeme: @@            Line: 4
-----------------------------------------------------------
Token: Keyword       Lexeme: real          Line: 5
<Opt Declaration List> ::= <Declaration List>   | <Empty>
<Declaration List>  := <Declaration> ;  | <Declaration> ; <Declaration List>
<Declaration> ::=  <Qualifier> <IDs>
<Qualifier> ::= int   |  boolean   |  real
-----------------------------------------------------------
Token: Identifier    Lexeme: joe           Line: 5
<IDs> ::=  <Identifier>    | <Identifier>, <IDs>
-----------------------------------------------------------
Token: Separator     Lexeme: ,             Line: 5
-----------------------------------------------------------
Token: Identifier    Lexeme: mondata       Line: 5
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 5
-----------------------------------------------------------
Token: Separator     Lexeme: @@            Line: 5
-----------------------------------------------------------
Token: Keyword       Lexeme: return        Line: 6
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Return> ::=  return; | return <Expression> ;
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 6
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Identifier    Lexeme: key           Line: 6
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 6
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 6
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Keyword       Lexeme: while         Line: 7
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<While> ::= while ( <Condition>  )  <Statement>
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 7
-----------------------------------------------------------
Token: Identifier    Lexeme: not           Line: 7
<Condition> ::= <Expression>  <relationalOperator>   <Expression>
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: =             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
<relationalOperator> ::=   = |  !=  |   >   | < | => | <=
-----------------------------------------------------------
Token: Keyword       Lexeme: true          Line: 7
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 7
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: {             Line: 8
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Compound> ::= {  <Statement List>  }
-----------------------------------------------------------
Token: Identifier    Lexeme: value         Line: 8
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: :=            Line: 8
-----------------------------------------------------------
Token: Integer       Lexeme: 0             Line: 8
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: -             Line: 8
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
-----------------------------------------------------------
Token: Integer       Lexeme: 2             Line: 8
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 8
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: }             Line: 9
Reached EOF with no errors found.

# ===================================================
# TEST CASE 3 (n > 20)
# ===================================================

/* if a 
jack could
would chuck
i would chuck the would if he could chuck * */
function youcouldntevenunderstandme [law2ka		: real]
{	while(jackedman != true)

		{
			wecoulddoit = all + night + long;
		int wewill := 0;
			real coulddoit;
		coulddoit := 2342.2;

			wewill := wewill + coulddoit *(2 -wewill); }
@@
	if(jim := 0) 	{make := jim - weight; }
@@
	hes := over + weight;

# ---------------------------------------------------------------------------------------
# Analyzed Syntax for Test Case 3
# ---------------------------------------------------------------------------------------
-----------------------------------------------------------
Token: Keyword       Lexeme: function      Line: 5
<Rat15S> ::= <Opt Function Definitions>   @@  <Opt Declaration List> @@ <Statement List>  
<Opt Function Definitions> ::= <Function Definitions> | <Empty>
<Function Definitions>  ::= <Function> | <Function> <Function Definitions>
<Function> ::= function  <Identifier>  [ <Opt Parameter List> ]   <Opt Declaration List>  <Body>
-----------------------------------------------------------
Token: Identifier    Lexeme: youcouldntevenunderstandmeLine: 5
-----------------------------------------------------------
Token: Separator     Lexeme: [             Line: 5
-----------------------------------------------------------
Token: Identifier    Lexeme: law2ka        Line: 5
<Opt Parameter List> ::=  <Parameter List>   |  <Empty>
<Parameter List>  ::=  <Parameter>  | <Parameter> , <Parameter List>
<Parameter> ::=  <Identifier> : <Qualifier>
-----------------------------------------------------------
Token: Operator      Lexeme: :             Line: 5
-----------------------------------------------------------
Token: Keyword       Lexeme: real          Line: 5
<Qualifier> ::= int   |  boolean   |  real
-----------------------------------------------------------
Token: Separator     Lexeme: ]             Line: 5
-----------------------------------------------------------
Token: Separator     Lexeme: {             Line: 6
<Opt Declaration List> ::= <Declaration List>   | <Empty>
<Empty> ::= epsilon
<Body>  ::=  {  < Statement List> }
-----------------------------------------------------------
Token: Keyword       Lexeme: while         Line: 6
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<While> ::= while ( <Condition>  )  <Statement>
-----------------------------------------------------------
Token: Separator     Lexeme: (             Line: 6
-----------------------------------------------------------
Token: Identifier    Lexeme: jackedman     Line: 6
<Condition> ::= <Expression>  <relationalOperator>   <Expression>
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: !=            Line: 6
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
<relationalOperator> ::=   = |  !=  |   >   | < | => | <=
-----------------------------------------------------------
Token: Keyword       Lexeme: true          Line: 6
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Separator     Lexeme: )             Line: 6
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Separator     Lexeme: {             Line: 8
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Compound> ::= {  <Statement List>  }
-----------------------------------------------------------
Token: Identifier    Lexeme: wecoulddoit   Line: 9
<Statement List> ::=  <Statement> | <Statement> <Statement List>
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: =             Line: 9

ERROR: Expected: :=
Found the following: Token "Operator" and Lexeme "=" on line 9.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Identifier    Lexeme: all           Line: 9
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: +             Line: 9

ERROR: Expected: :=
Found the following: Token "Operator" and Lexeme "+" on line 9.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Identifier    Lexeme: night         Line: 9
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: +             Line: 9

ERROR: Expected: :=
Found the following: Token "Operator" and Lexeme "+" on line 9.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Identifier    Lexeme: long          Line: 9
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 9

ERROR: Expected: :=
Found the following: Token "Operator" and Lexeme ";" on line 9.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Keyword       Lexeme: int           Line: 10

ERROR: Expected: }
Found the following: Token "Keyword" and Lexeme "int" on line 10.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Identifier    Lexeme: wewill        Line: 10
<Statement> ::=  <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
<Assign> ::=   <Identifier> := <Expression> ;
-----------------------------------------------------------
Token: Operator      Lexeme: :=            Line: 10
-----------------------------------------------------------
Token: Integer       Lexeme: 0             Line: 10
<expression> ::= <term> <expressionPrime>
<term> ::= <Factor> <termPrime>
<Factor> ::= - <Primary> | <Primary>
<Primary> ::= <Identifier> | <Integer> | <Identifier> [<IDs>]  | ( <Expression> )  | <Real> | true | false
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 10
<termPrime> ::= * <Factor> <termPrime> | / <Factor> <termPrime> | <Empty>
<Empty> ::= epsilon
<expressionPrime>  ::= + <Term> <expressionPrime>  | - <Term> <expressionPrime>  | <Empty>
<Empty> ::= epsilon
-----------------------------------------------------------
Token: Keyword       Lexeme: real          Line: 11

ERROR: Expected: }
Found the following: Token "Keyword" and Lexeme "real" on line 11.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Identifier    Lexeme: coulddoit     Line: 11

ERROR: Expected: @@
Found the following: Token "Identifier" and Lexeme "coulddoit" on line 11.
Attempting to continue syntax analysis.
-----------------------------------------------------------
Token: Operator      Lexeme: ;             Line: 11
Reached EOF with errors found.



END OF TEST CASES
