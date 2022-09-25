# include <iostream>
# include <string>
# include <vector>
# include <cstdlib>
# include <string.h>
# include <sstream> // stringstream
# include <math.h>
# include <iomanip>
# include <cstdio>
# include <stdio.h> 
# include <cctype> // isalpha() isdigit()
# include <cmath>


using namespace std;

struct Information{
  string variable_Name;
  string variable_Type;

  bool isVariable, isFunction;
  vector< float > vArray;
  vector< string > function_vArray;
  vector< string > token;
};

int gLine = 1;

class Exception {
  public:
  string mEMessage;
  Exception() { }

  Exception( string token ) {
    mEMessage = token;
  } // Exception()
}; // Exception

class LexicalErrorException : public Exception {
  public:
  LexicalErrorException( char temp_Ch ) {
    cout << "> Line " << gLine << " : unrecognized token with first char : '" << temp_Ch << "'" << endl;
  } // LexicalErrorException();

  LexicalErrorException( string temp_St ) {
    cout << "> Line " << gLine << " : unrecognized token with first char : '" << temp_St << "'" << endl;
  } // LexicalErrorException();
}; // LexicalErrorException

class SyntaxErrorException : public Exception {
  public:
  SyntaxErrorException( char temp_Ch ) {
    cout << "> Line " << gLine << " : unexpected token : '" << temp_Ch << "'" << endl;
  } // SyntaxErrorException()
  
  SyntaxErrorException( string temp_St ) {
    cout << "> Line " << gLine << " : unexpected token : '" << temp_St << "'" << endl;
  } // SyntaxErrorException()	
}; // SyntaxErrorException

class UndefinedErrorException : public Exception {
  public:
  UndefinedErrorException( char temp_Ch ) {
    cout << "> Line " << gLine << " : undefined identifier : '" << temp_Ch << "'" << endl;
  } // UndefinedErrorException()

  UndefinedErrorException( string temp_St ) {
    cout << "> Line " << gLine << " : undefined identifier : '" << temp_St << "'" << endl;
  } // UndefinedErrorException() 
}; // UndefinedErrorException

class End : public Exception {
  public:
  End() {
    cout << "> Our-C exited ..." << endl; 
  } // End()
}; // End

class Analyzer {
  public :
  bool mFuncD;
  bool mIsCin, mIsCout ;
  bool mIsSigned ;
  vector< Information > mCList ;

  string GetToken() {
    string token = "";
    char trash[128];
    
    SkipWhiteSpace();
    char temp_Ch = cin.peek();
    string tokenType = CheckStartType( temp_Ch );
    while ( IsID( temp_Ch ) || IsNum( temp_Ch ) || IsOther( temp_Ch ) || temp_Ch == '_' ) {
    // ==========================================================================================
      if ( tokenType == "IDENT" && ( IsID( temp_Ch ) || temp_Ch == '_' || IsNum( temp_Ch ) ) ) {
        if ( temp_Ch == '.' )
          return token;

        temp_Ch = GetNextChar();
        token += temp_Ch;
      } // if
      // =========================================================================================
      else if ( tokenType == "NUM" && ( IsNum( temp_Ch ) || temp_Ch == '.' ) ) {
        if ( ( temp_Ch == '.' && token != "" && IsNoDotNum( token ) ) ||
             ( IsNum( temp_Ch ) && temp_Ch != '.' ) || ( token == "" && IsNum( temp_Ch ) ) ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
        } // if
        else
          return token;
      } // else if
      // =========================================================================================   
      else if ( tokenType == "OTHER" && IsOther( temp_Ch ) ) {
        char f_Char = temp_Ch;
          
          // > or >= or >>
        if ( f_Char == '>' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' || cin.peek() == '>' ) {
          // >= or >>
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          
          return token;
        } // if
        // < or <= or <<    
        else if ( f_Char == '<' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' || cin.peek() == '<' ) {
          // <= or << or <>
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          
          return token;
        } // else if
        // = or ==
        else if ( f_Char == '=' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' ) {
              // ==
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          
          return token;
        } // else if
        else if ( f_Char == '!' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' ) {
              // !=
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          
          return token;
        } // else if
        else if ( f_Char == '\'' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          
          if ( cin.peek() == '\n' )
            throw LexicalErrorException( f_Char );
          else {
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // else

          if ( cin.peek() != '\'' )
            throw LexicalErrorException( '\'' );

          temp_Ch = GetNextChar();
          token += temp_Ch;
          return token;
        } // else if
        else if ( f_Char == '"' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;

          while ( cin.peek() != '"' ) {
            if ( cin.peek() == '\n' )
              throw LexicalErrorException( "\"" );
            if ( cin.peek() == '\\' ) {
              temp_Ch = GetNextChar();

              if ( cin.peek() == '"' ) {
                token += temp_Ch;
                temp_Ch = GetNextChar();
                token += temp_Ch;
              } // if
              else
                token += temp_Ch;
            } // if
            else {
              temp_Ch = GetNextChar();
              token += temp_Ch;
            } // else
          } // while
          
          temp_Ch = GetNextChar();
          token += temp_Ch;
          return token;
        } // else if
        else if ( f_Char == '&' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '&' ) {
              // &&
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          
          return token;
        } // else if
        else if ( f_Char == '|' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '|' ) {
            // ||
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          
          return token;
        } // else if
        else if ( f_Char == ';' || f_Char == ',' || f_Char == '?' || f_Char == ':' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          return token;
        } // else if 
        else if ( f_Char == '+' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' ) {
            // +=
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          else if ( cin.peek() == '+' ) {
            // ++
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if

          return token;
        } // else if
        else if ( f_Char == '-' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' ) {
            // -=
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if
          else if ( cin.peek() == '-' ) {
            // --
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if

          return token;
        } // else if
        // * or *= or % or %=
        else if ( f_Char == '*' || f_Char == '%' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          if ( cin.peek() == '=' ) {
            // *= %=
            temp_Ch = GetNextChar();
            token += temp_Ch;
          } // if

          return token;
        } // else if
        else if ( f_Char == '^' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          return token;
        } // else if
        else if ( f_Char == '/' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          
          if ( cin.peek() == '/' ) {
            temp_Ch = GetNextChar();
            token += temp_Ch;
            gLine ++;
            char trash[128];
            cin.getline( trash, 128 );
            token = "";
            SkipWhiteSpace();
            tokenType = CheckStartType( cin.peek() );
          } // if
          else if ( cin.peek() == '=' ) {
            temp_Ch = GetNextChar();
            token += temp_Ch;
            return token;
          } // else if
          else
            return token;
        } // else if
        // .
        else if ( f_Char == '.' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          
          if ( !IsNum( cin.peek() || cin.peek() == '.' ) )
            throw LexicalErrorException( token ); 
        
          tokenType = "NUM";
        } // else if
        // ( or ) or [ or ] or { or } 
        else if ( f_Char == '(' || f_Char == ')' || f_Char == '[' || 
                  f_Char == ']' || f_Char == '{' || f_Char == '}' ) {
          temp_Ch = GetNextChar();
          token += temp_Ch;
          return token;
        } // else if 
        else
          throw LexicalErrorException( temp_Ch ); 
      } // else if
      else
        return token;
    
      temp_Ch = cin.peek();
    } // while
    
    return token;
  } // GetToken()
  
  bool IsWhiteSpace( char temp_Ch ) {
    if ( temp_Ch == '\t' || temp_Ch == ' ' || temp_Ch == '\n' )
      return true;
    return false;
  } // IsWhiteSpace()

  void SkipWhiteSpace() {
    char temp;
    while ( cin.peek() != EOF && IsWhiteSpace( cin.peek() ) ) {
      temp = GetNextChar();
      if ( temp == '\n' )
        gLine ++;
    } // while
  } // SkipWhiteSpace()
  
  char GetNextChar() {
    char nextChar;
    nextChar = cin.get();
    return nextChar;
  } // GetNextChar()

  string CheckStartType( char f_Char ) {
    if ( IsID( f_Char ) )
      return "IDENT";
    else if ( IsNum( f_Char ) ) 
      return "NUM";
    else if ( IsOther( f_Char ) )
      return "OTHER";
    else
      throw LexicalErrorException( f_Char );
    return "ERROR";
  } // CheckStartType()

  bool IsID( char temp_Ch ) {
    return isalpha( temp_Ch ); // check Letter or not ( from cctype )
  } // IsID()
  
  bool IsNum( char temp_Ch ) {
    char temp;
    if ( temp_Ch == '.' ) { // first char is . 
      temp = cin.get();
      if ( isdigit( cin.peek() ) ) {
        cin.putback( temp );
        return true;      
      } // if
      else {
        cin.putback( temp );
        return false;
      } // else
    } // if
    else // others, just check it
      return isdigit( temp_Ch );
    // isdigit from cctype
  } // IsNum()

  bool IsOther( char temp_Ch ) {
    if ( temp_Ch == '=' || temp_Ch == '>' || temp_Ch == '<' || temp_Ch == ':' || temp_Ch == ';' ||
         temp_Ch == '+' || temp_Ch == '-' || temp_Ch == '*' || temp_Ch == '/' || temp_Ch == '.' ||
         temp_Ch == '(' || temp_Ch == ')' || temp_Ch == '[' || temp_Ch == ']' || temp_Ch == '{' ||
         temp_Ch == '}' || temp_Ch == '%' || temp_Ch == '^' || temp_Ch == '!' || temp_Ch == '&' ||
         temp_Ch == '|' || temp_Ch == '?' || temp_Ch == ',' || temp_Ch == '\'' || temp_Ch == '"' )
      return true;
    else
      return false;
  } // IsOther()

  bool IsNoDotNum( string temp_St ) {
    for ( int i = 0 ; i < temp_St.size() ; i ++ ) {
      if ( temp_St[i] == '.' )
        return false;
    } // for
    
    return true;
  } // IsNoDotNum()

  string PeekToken() {
    string token = GetToken();
    for ( int i = token.size() - 1 ; i >= 0 ; i -- )
      cin.putback( token.at( i ) );
    return token;
  } // PeekToken()

  // ===================================================================================
  string CheckWhatType( string temp_St ) {
    if ( temp_St == "" )
      return "ERROR";
    else if ( temp_St == "int" || temp_St == "void" || temp_St == "float" ||
              temp_St == "char" || temp_St == "bool" || temp_St == "string" ||
              temp_St == "if" || temp_St == "do" || temp_St == "while" ||
              temp_St == "else" || temp_St == "return" || temp_St == "cin" ||
              temp_St == "cout" )
      return "KEEPWORD";
    else if ( ( temp_St == "true" ) || ( temp_St == "false" ) )
      return "NUM";
    else if ( IsID( temp_St.at( 0 ) ) )
      return "IDENT";
    else if ( isdigit( temp_St.at( 0 ) ) || ( temp_St.at( 0 ) == '.' ) || ( temp_St.at( 0 ) == '\'' ) ||
              ( temp_St.at( 0 ) == '"' ) )
      return "NUM";
    else
      return "OTHER";
  } // CheckWhatType()

  bool IsIDExisted( string token, Information &info ) {
    for ( int i = 0 ; i < mCList.size() ; i ++ ) {
      if ( mCList.at( i ).variable_Name == token ) {
        int nLine = gLine;
        if ( mCList.at( i ).isFunction ) {
          if ( PeekToken() != "(" ) {   
            if ( gLine > nLine ) {
              /*
              for ( int j = 0 ; j < gLine - nLine ; j ++ )
                cin.putback( '\n' );
              */
              cin.putback( 'Z' );
            } // if

            gLine = nLine;
            throw UndefinedErrorException( token );
            // if names are same but now token is variable but the name in list is function
            // So throw Undefined Error to it.
          } // if

          return true;
        } // if
        else if ( mCList.at( i ).isVariable ) {
          if ( PeekToken() == "(" ) {
            if ( gLine > nLine ) {
              /*
              for ( int j = 0 ; j < gLine - nLine ; j ++ )
                cin.putback( '\n' );
              */
              cin.putback( 'Z' );
            } // if

            gLine = nLine;
            
            throw UndefinedErrorException( token ); 
            // if names are same but now token is function but the name in list is variable
            // So throw Undefined Error to it.
          } // if

          return true;
        } // else if
      } // if
    } // for

    // if we went into Compound Statement, then is function declaration now.
    if ( !mFuncD )
      return false;
    
    // if it is function declaration now, we will also check the
    // now info to check the id declartion is in the function
    // or not
    for ( int i = 0 ; i < info.function_vArray.size() ; i ++ ) {
      if ( info.function_vArray.at( i ) == token )
        return true;
    } // for

    return false;
    // no pass declartion and now declaration, so return false
  } // IsIDExisted()

  void IDDefinition( Information &info ) {
    vector< Information > tempIList;
    bool redefined = false;

    // definition come, check redefinition
    for ( int i = 0 ; i < mCList.size() ; i ++ ) {
      if ( mCList.at( i ).variable_Name == info.variable_Name && !redefined ) {
        // first time come in, and it is the same variable
        mCList.erase( mCList.begin() + i );
        mCList.push_back( info );
        redefined = true;
        // delete last one, and redefined the old variable
        if ( info.isFunction ) {
          cout << "New definition of " << info.variable_Name;
          cout << "() entered ..." << endl;
        } // if
        else {
          cout << "New definition of " << info.variable_Name;
          cout << " entered ..." << endl;
        } // else
      } // if
    } // for

    // new definition, not redefined
    if ( !redefined ) {
      mCList.push_back( info );
      if ( info.isFunction ) {
        cout << "Definition of " << info.variable_Name;
        cout << "() entered ..." << endl;
      } // if
      else {
        cout << "Definition of " << info.variable_Name;
        cout << " entered ..." << endl;
      } // else
    } // if

    // it is a variable declaration, setting it.
    if ( !mFuncD && !info.function_vArray.empty() ) {
      Information tempI;
      tempI.variable_Name = info.function_vArray.front();
      info.function_vArray.erase( info.function_vArray.begin() ); // delete already out varaible
      tempI.function_vArray.assign( info.function_vArray.begin(), info.function_vArray.end() );
      tempI.variable_Type = info.variable_Type;
      tempI.isFunction = false;
      tempI.isVariable = true;
      tempI.token.assign( info.token.begin(), info.token.end() );

      IDDefinition( tempI );
    } // if
  } // IDDefinition()

  bool IsFuctionOrVariableExisted( string token, string type ) {
    for ( int i = 0 ; i < mCList.size() ; i ++ ) {
      if ( mCList.at( i ).variable_Name == token ) {
        // same name, and check it is same type or not
        if ( mCList.at( i ).isFunction && type == "FUNCTION" )
          return true;
        else if ( mCList.at( i ).isVariable && type == "VARIABLE" )
          return true;
      } // if
    } // for

    // Maybe same name, but there are not same type in name list
    return false;
  } // IsFuctionOrVariableExisted()

  void ListVariable( string variableName ) {
    cout << "> ";
    int index = 0;
    for ( int i = 0 ; i < mCList.size() ; i ++ ) {
      if ( mCList.at( i ).variable_Name == variableName ) {
        index = i;
        i = mCList.size() ;
      } // if
    } // for

    cout << mCList.at( index ).variable_Type << " " << mCList.at( index ).variable_Name;
    // [] there is array
    for ( int i = 0 ; i < mCList.at( index ).token.size() ; i ++ ) {
      if ( ( mCList.at( index ).token.at( i ) == mCList.at( index ).variable_Name ) &&
           ( mCList.at( index ).token.at( i + 1 ) == "[" ) ) {
        cout << mCList.at( index ).token.at( i + 1 ) << " ";
        cout << mCList.at( index ).token.at( i + 2 ) << " ";
        cout << mCList.at( index ).token.at( i + 3 ) << " ;" << endl;
        return;
      } // if
    } // for

    cout << " ;" << endl; // no array so put back ;
  } // ListVariable()

  void ListFunction( string functionName ) {
    cout << "> ";

    
    // IsStart : the start of one function or statement or not
    // isWhileOfIfAndNobrackets : if or while then true
    // spaceNumOnStart : the num of space to put in front of statement

    bool isStart = false;
    bool isWhileOrIfAndNobrackets = false;
    bool fucStart = true;
    bool isDoWhile = false;
    int spaceNumOnStart = 0;
    // .............................................................
    bool stop = false; // to check when to stop
    for ( int i = 0 ; i < mCList.size() && !stop ; i ++ ) {
      if ( mCList.at( i ).variable_Name == functionName ) {
        // if got the same function, then stop
        stop = true;
        // ...................................
        Information tempInfo = mCList.at( i );

        for ( int j = 0 ; j < tempInfo.token.size() ; j ++ ) {
          if ( !isStart ) {
            // first time of one statement or function
            for ( int k = 0 ; k < spaceNumOnStart ; k ++ )
              cout << " ";
        
            isStart = true;
          } // if

          // cout the token we have or statement
          cout << tempInfo.token.at( j );
          // ...................................

          // many special case for output downward
          if ( tempInfo.token.at( j ) != ";" && tempInfo.token.at( j ) != "{"  && 
               tempInfo.token.at( j ) != "}" && tempInfo.token.at( j ) != "--" &&
               tempInfo.token.at( j ) != "++" && 
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) != "," ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) != "[" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) != "--" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) != "++" ) ) {

            if ( ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "(" ) &&
                 tempInfo.token.at( j ) != "if" && tempInfo.token.at( j ) != "while" )
              ; // do nothing with it
            else if ( tempInfo.token.at( j ) == "(" &&
                      ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == ")" ) )
              cout << " ";
            else {
              if ( tempInfo.token.at( j ) == "]" &&
                   ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "," ) ) 
                ;
              else {
                if ( tempInfo.token.at( j ) == ")" && isWhileOrIfAndNobrackets ) {
                  if ( ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == ";" ) ) {
                    // it is a dowhile problem
                    cout << " ";
                    isDoWhile = true;
                  } // if
                  else if ( ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) != "{" ) ) {
                    // if we got {, then change line and set the space +2, because it is
                    // a function or a if while..... statements
                    cout << endl;
                    spaceNumOnStart += 2;
                    isStart = false;
                    // so make the start flag false, because it is not the first line of tokenList
                  } // else if
                  else {
                    cout << " ";
                    isWhileOrIfAndNobrackets = false;
                    // it is not while of if ..... so BYEBYE
                  } // else
                } // if
                else
                  cout << " ";
                // other else just cout " "
              } // else
            } // else
          } // if

          // .............................................................................................
          
          if ( ( tempInfo.token.at( j ) == "<<" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "++" ) )
            cout << " ";

          if ( ( tempInfo.token.at( j ) == "<<" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "--" ) )
            cout << " ";

          if ( ( tempInfo.token.at( j ) == ">>" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "++" ) )
            cout << " ";

          if ( ( tempInfo.token.at( j ) == ">>" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "--" ) )
            cout << " ";
          
          // .............................................................................................
          if ( ( tempInfo.token.at( j ) == "++" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && 
                 ( CheckWhatType( tempInfo.token.at( j + 1 ) ) != "IDENT" ) ) )
            cout << " ";
          
          if ( ( tempInfo.token.at( j ) == "--" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && 
                 ( CheckWhatType( tempInfo.token.at( j + 1 ) ) != "IDENT" ) ) )
            cout << " ";
          // .............................................................................................
          
          if ( ( tempInfo.token.at( j ) == "+" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "++" ) )
            cout << " ";

          if ( ( tempInfo.token.at( j ) == "-" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "++" ) )
            cout << " ";

          if ( ( tempInfo.token.at( j ) == "+" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "--" ) )
            cout << " ";

          if ( ( tempInfo.token.at( j ) == "-" ) &&
               ( ( j + 1 ) != tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "--" ) )
            cout << " ";
          
          // .............................................................................................
          if ( tempInfo.token.at( j ) == "while" || tempInfo.token.at( j ) == "if" )
            // if we saw the while or if, we need to set the flag to true
            isWhileOrIfAndNobrackets = true; 

          if ( tempInfo.token.at( j ) == "(" )
            fucStart = false;
          
          if ( tempInfo.token.at( j ) == ";" ) {
            // the easiest problem, find ; then change line ~~~~~~~~
            cout << endl;

            if ( isWhileOrIfAndNobrackets && !isDoWhile )
              // if we got no brackets( already done ), so space -2 to another problem
              spaceNumOnStart -= 2;

            // reset it ~
            isDoWhile = false;
            isWhileOrIfAndNobrackets = false;
            isStart = false;
            // ..............................
          } // if

          if ( tempInfo.token.at( j ) == "{" ) {
            // if () { while() { ..........
            cout << endl;
            // not first line anymore
            isStart = false;
            // make sure to plus two spaces ~~~~~~~~~~~~~
            spaceNumOnStart += 2;
          } // if

          if ( ( j + 1 ) < tempInfo.token.size() && tempInfo.token.at( j + 1 ) == "}" )
            // end of one {, need to minus two spaces for it.
            spaceNumOnStart -= 2;

          if ( tempInfo.token.at( j ) == "}" ) {
            // same problem, but this is when we face it.
            cout << endl;
            isStart = false;
          } // if
        } // for
      } // if
    } // for

    
    /*
    bool stop = false;
    
    // check the head start and count the space we need to cout
    bool isStart = false;
    int spaceNumOnStart = 0;
    // ....................................

    // the big & bracketNum is to check we finished one line bracket or not
    bool counting = false; // count again
    int bracketNum = 0;
    int bigBracketNum = 0;
    // ....................................

    // change line or not
    bool newLine = false; 
    // ....................................

    // do while ( Special case for it, i dont know how to do with others )
    bool isDoWhile = false;
    // ....................................

    for ( int i = 0 ; i < mCList.size() && !stop ; i ++ ) {
      if ( mCList.at( i ).variable_Name == functionName ) {
        // find the same function name
        for ( int j = 0 ; j < mCList.at( i ).token.size() ; j ++ ) {
          // output the token that defined in function

          // if we face with }, then it is the end of one statement
          // so we do not need the space now
          if ( mCList.at( i ).token[j] == "}" )
            spaceNumOnStart -= 2;
          // ......................................................

          // if it is a new line or start, we need to cout tab for it
          if ( newLine || isStart ) {
            for ( int k = 0 ; k < spaceNumOnStart ; k ++ )
              cout << " "; // space
            
            isStart = false; // there is space, so it is not start now

            if ( newLine ) {
              // if we are new line now
              spaceNumOnStart -= 2;
              newLine = false; // initiate, we have done it
            } // if
          } // if

          // downside are many special case, i dont like them
          if ( ( mCList.at( i ).token[j] == "if" ) || ( mCList.at( i ).token[j] == "while" && !isDoWhile ) )
            // now is if or while, so we need to check there is a brackets behind or not
            counting = true;
          else if ( mCList.at( i ).token[j] == "do" )
            isDoWhile = true;
          // .................................................................................

          // dowhile special case dealing with
          if ( isDoWhile ) {
            if ( mCList.at( i ).token[j] == "{" )
              // there is a big bracket
              bigBracketNum ++;
            else if ( mCList.at( i ).token[j] == "}" )
              // ther is a right big bracket
              bigBracketNum --; 
          } // if
          // dealing with the brackets ...................

          // if or while situation, need to find backward is still some brackets or not
          if ( counting ) {
            if ( mCList.at( i ).token[j] == "(" )
              bracketNum ++;
            else if ( mCList.at( i ).token[j] == ")" )
              bracketNum --;
          } // if
          // ..........................................................................

          // counting brackets and space finished, so output the token
          cout << mCList.at( i ).token[j]; 

          if ( !isDoWhile && mCList.at( i ).token[j] == "}" ) {
            // we face the }, but now is not dowhile,
            // so backward must need to cout endl
            cout << endl;
            // .....................................................

            // if we change line, then it is a new start
            isStart = true;
          } // if
          else if ( mCList.at( i ).token[j] == "{" || mCList.at( i ).token[j] == ";" ) {
            if ( mCList.at( i ).token[j] == "{" )
              spaceNumOnStart += 2;
            // we face the ; or {, so it is definitely a change line
            cout << endl;
            // .....................................................

            // if we change line, then it is a new start
            isStart = true;
          } // else if
          else if ( mCList.at( i ).token[j + 1] == "{" && mCList.at( i ).token[j] == "else" ) {
            // special case : else { then change line
            cout << endl;
            // .....................................................

            // newLine, but not at the start of the statement
            spaceNumOnStart += 2;
            newLine = true;
            // .....................................................
          } // else if
          else if ( bracketNum == 0 && counting && ( mCList.at( i ).token[j] == "while" &&
                                                     mCList.at( i ).token[j] == "if" ) ) {
            if ( mCList.at( i ).token[j] == "{" && mCList.at( i ).token[j] == ";" )
              cout << " ";
            else {
              cout << endl;
              // newLine, but not at the start of the statement
              spaceNumOnStart += 2;
              newLine = true;
            } // else

            counting = false;
          } // else if
          else if ( bigBracketNum == 0 && mCList.at( i ).token[j] == "}" && isDoWhile ) {
            isDoWhile = false;
            cout << " ";
          } // else if
          else {
            // two special case for punction do nothing for it ( space or tab )
            if ( ( mCList.at( i ).token[j] != "if" && mCList.at( i ).token[j] != "while" &&
                   mCList.at( i ).token[j] != "<" && mCList.at( i ).token[j] != "&&" &&
                   mCList.at( i ).token[j] != "?" && mCList.at( i ).token[j] != ":" &&
                   mCList.at( i ).token[j] != "*" && mCList.at( i ).token[j] != "/" &&
                   mCList.at( i ).token[j] != "%" && mCList.at( i ).token[j] != ">" &&
                   mCList.at( i ).token[j] != "-=" && mCList.at( i ).token[j] != "/=" &&
                   mCList.at( i ).token[j] != "||" && mCList.at( i ).token[j] != "+=" &&
                   mCList.at( i ).token[j] != "*=" && mCList.at( i ).token[j] != "%=" &&
                   mCList.at( i ).token[j] != "+" && mCList.at( i ).token[j] != "-" &&
                   mCList.at( i ).token[j] != "(" ) && mCList.at( i ).token[j + 1] == "(" )
              ;
              // do nothing for it ( space or tab ) 
            else if ( mCList.at( i ).token[j+1] == "++" || mCList.at( i ).token[j+1] == "--" ||
                      ( mCList.at( i ).token[j] == "++" && mCList.at( i ).token[j+1] != ";" ) ||
                      mCList.at( i ).token[j+1] == "[" || mCList.at( i ).token[j+1] == "," ||
                      ( mCList.at( i ).token[j] == "--" && mCList.at( i ).token[j+1] != ";" ) ||
                      mCList.at( i ).token[j] == "!" || mCList.at( i ).token[j] == "&" )
              ;
              // do nothing for it ( space or tab )
            else if ( mCList.at( i ).token[j] == "(" && mCList.at( i ).token[j + 1] == ")" )
              ;
            else
              cout << " ";
          } // else
        } // for

        stop = true; // find it so stop the for loop
      } // if
    } // for
    */
  } // ListFunction()

  void ListAllVariables() {
    vector< string > tokenList; // store variable token name
    cout << "> ";
    for ( int i = 0 ; i < mCList.size() ; i ++ ) {
      if ( mCList.at( i ).isVariable )
        tokenList.push_back( mCList.at( i ).variable_Name );
    } // for

    for ( int i = 0 ; i < tokenList.size() ; i ++ ) {
      for ( int j = i + 1 ; j < tokenList.size() ; j ++ ) {
        if ( tokenList.at( i ) > tokenList.at( j ) ) {
          string tempToken = tokenList.at( i );
          tokenList.at( i ) = tokenList.at( j );
          tokenList.at( j ) = tempToken;
        } // if
      } // for
    } // for

    for ( int i = 0 ; i < tokenList.size() ; i ++ )
      cout << tokenList.at( i ) << endl;
  } // ListAllVariables()

  void ListAllFunctions() {
    vector< string > tokenList; // store function token name
    cout << "> ";
    for ( int i = 0 ; i < mCList.size() ; i ++ ) {
      if ( mCList.at( i ).isFunction )
        tokenList.push_back( mCList.at( i ).variable_Name );
    } // for

    for ( int i = 0 ; i < tokenList.size() ; i ++ ) {
      for ( int j = i + 1 ; j < tokenList.size() ; j ++ ) {
        if ( tokenList.at( i ) > tokenList.at( j ) ) {
          string tempToken = tokenList.at( i );
          tokenList.at( i ) = tokenList.at( j );
          tokenList.at( j ) = tempToken;
        } // if
      } // for
    } // for

    for ( int i = 0 ; i < tokenList.size() ; i ++ )
      cout << tokenList.at( i ) << "()" << endl;
  } // ListAllFunctions()
  // ===================================================================================
  bool User_Input() {
    mIsSigned = false;
    mIsCin = false;
    mIsCout = false;
    mFuncD = false;

    string token, tokenType;
    Information info;
    info.isFunction = false;
    info.isVariable = false;
    
    token = PeekToken();
    if ( token == "Done" ) {
      token = GetToken(); // Done
      token = GetToken(); // (
      if ( token != "(" )
        throw SyntaxErrorException( token );
      
      token = GetToken(); // )
      if ( token != ")" )
        throw SyntaxErrorException( token );

      token = GetToken(); // ;
      if ( token != ";" )
        throw SyntaxErrorException( token );
      
      // Once get Done, all Syntax must be correct
      return false;
    } // if
    else if ( token == "ListVariable" ) {
      token = GetToken(); // ListVariable
      token = GetToken(); // (
      if ( token != "(" )
        throw SyntaxErrorException( token );
      
      // string
      string variableName = GetToken();
      string variableType = CheckWhatType( variableName );
      // delete the " && "
      variableName.erase( variableName.end() - 1 );
      variableName.erase( variableName.begin() );
      if ( variableType != "NUM" )
      // variable is not string or constant
        throw SyntaxErrorException( variableName );
      else if ( !IsFuctionOrVariableExisted( variableName, "VARIABLE" ) )
        throw UndefinedErrorException( variableName );
      // ......

      token = GetToken(); // )
      if ( token != ")" )
        throw SyntaxErrorException( token );

      token = GetToken(); // ;
      if ( token != ";" )
        throw SyntaxErrorException( token );
      
      // if it come to here, then it is a ListVariable time
      ListVariable( variableName );
      cout << "Statement executed ..." << endl;
    } // else if
    else if ( token == "ListFunction" ) {
      token = GetToken(); // ListFunction
      token = GetToken(); // (
      if ( token != "(" )
        throw SyntaxErrorException( token );
      
      // string
      string functionName = GetToken();
      string functionType = CheckWhatType( functionName );
      // delete the " && "
      functionName.erase( functionName.end() - 1 );
      functionName.erase( functionName.begin() );
      if ( functionType != "NUM" )
      // function is not string or constant
        throw SyntaxErrorException( functionName );
      else if ( !IsFuctionOrVariableExisted( functionName, "FUNCTION" ) )
        throw UndefinedErrorException( functionName );
      // ......

      token = GetToken(); // )
      if ( token != ")" )
        throw SyntaxErrorException( token );

      token = GetToken(); // ;
      if ( token != ";" )
        throw SyntaxErrorException( token );
      
      // if it come to here, then it is a ListFunction time
      ListFunction( functionName );
      cout << "Statement executed ..." << endl;
    } // else if 
    else if ( token == "ListAllVariables" ) {
      token = GetToken(); // ListAllVariables
      token = GetToken(); // (
      if ( token != "(" )
        throw SyntaxErrorException( token );
      
      token = GetToken(); // )
      if ( token != ")" )
        throw SyntaxErrorException( token );

      token = GetToken(); // ;
      if ( token != ";" )
        throw SyntaxErrorException( token );
      
      // if it come to here, then it is a ListFunction time
      ListAllVariables();
      cout << "Statement executed ..." << endl;
    } // else if
    else if ( token == "ListAllFunctions" ) {
      token = GetToken(); // ListAllFunctions
      token = GetToken(); // (
      if ( token != "(" )
        throw SyntaxErrorException( token );
      
      token = GetToken(); // )
      if ( token != ")" )
        throw SyntaxErrorException( token );

      token = GetToken(); // ;
      if ( token != ";" )
        throw SyntaxErrorException( token );
      
      // if it come to here, then it is a ListFunction time
      ListAllFunctions();
      cout << "Statement executed ..." << endl;
    } // else if
    else if ( token == "void" || token == "int" || token == "char" || 
              token == "float" || token == "string" || token == "bool" ) {

      Definition( info );
      // It is Definition way;
      cout << "> ";
      IDDefinition( info );
    } // else if
    else {
      Statement( info );
      cout << "> Statement executed ..." << endl;
    } // else

    info.function_vArray.clear();
    info.token.clear();

    return true;
  } // User_Input()

  void Definition( Information &info ) {
    // VOID Identifier function_definition_without_ID
    // or
    // type_specifier Identifier function_definition_or_declarators
    
    string token;

    if ( PeekToken() == "void" ) {
      // (VOID) Identifier function_definition_without_ID
      token = GetToken(); // void
      info.token.push_back( token );

      token = GetToken(); // is void so anyway get next
      if ( CheckWhatType( token ) == "IDENT" ) {
        // (VOID Identifier) function_definition_without_ID
        info.variable_Name = token;
        info.isFunction = true; // void so it is func
        info.token.push_back( token );
        Function_definiton_without_ID( info );
      } // if
      else
        throw SyntaxErrorException( token );
    } // if
    else {
      // type_specifier Identifier function_definition_or_declarators
      Type_specifer( info );

      token = GetToken(); // ID
      if ( CheckWhatType( token ) == "IDENT" ) {
        info.variable_Name = token;
        info.token.push_back( token );
        Function_definition_or_declarators( info );
      } // if
      else
        throw SyntaxErrorException( token );
    } // else
  } // Definition()

  void Function_definition_or_declarators( Information &info ) {
    // function_definition_without_ID | rest_of_declarators
    string token;

    if ( PeekToken() == "(" ) {
      Function_definiton_without_ID( info );
      info.isFunction = true;
    } // if
    else {
      Rest_of_declarators( info );
      info.isVariable = true;
    } // else
  } // Function_definition_or_declarators()

  void Function_definiton_without_ID( Information &info ) {
    // '(' [ VOID | formal_parameter_list ] ')' compound_statement
    string token = GetToken(); // (
    if ( token != "(" ) 
      throw SyntaxErrorException( token );
    else
      info.token.push_back( token );
    
    if ( PeekToken() == "void" ) {
      // '(' VOID ')' compound_statement
      token = GetToken();
      info.token.push_back( token );
    } // if
    else if ( PeekToken() == "int" || PeekToken() == "char" || PeekToken() == "float" ||
              PeekToken() == "string" || PeekToken() == "bool" )
      Formal_parameter_list( info ); // '(' formal_parameter_list ')' compound_statement

    token = GetToken(); // )
    if ( token != ")" ) 
      throw SyntaxErrorException( token );
    else
      info.token.push_back( token );
    
    Compound_statement( info );
  } // Function_definiton_without_ID()

  void Formal_parameter_list( Information &info ) {
    // type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
    // { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
    string token;
    Type_specifer( info );
    if ( PeekToken() == "&" ) {
      // ...... & .....
      token = GetToken();
      info.token.push_back( token );
    } // if

    token = GetToken();
    if ( CheckWhatType( token ) == "IDENT" ) {
      // ...... Identifer .....
      info.token.push_back( token );
      // new ID come in the function, so push it
      info.function_vArray.push_back( token );
    } // if
    else
      throw SyntaxErrorException( token );
    
    if ( PeekToken() == "[" ) {
      // ..... [ '[' Constant ']' ] .....
      token = GetToken(); // [
      info.token.push_back( token );

      token = GetToken(); // check next is Constant, just get it or error
      if ( CheckWhatType( token ) == "NUM" ) // Constant == Num
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );

      token = GetToken(); // check next is ], just get it or error
      if ( token == "]" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // if

    // ...... { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
    while ( PeekToken() == "," ) {
      token = GetToken();
      info.token.push_back( token ); // ,

      Type_specifer( info ); // ..... type_specifier .....

      if ( PeekToken() == "&" ) {
        // ..... [ '&' ] .....
        token = GetToken();
        info.token.push_back( token );
      } // if
      
      token = GetToken();
      if ( CheckWhatType( token ) == "IDENT" ) {
        // ..... Identifier .....
        info.token.push_back( token );
        // new ID come in the function, so push it
        info.function_vArray.push_back( token );
      } // if
      else
        throw SyntaxErrorException( token );
      
      if ( PeekToken() == "[" ) {
        // ..... [ '[' Constant ']' ]
        token = GetToken(); // [
        info.token.push_back( token );

        token = GetToken(); // check next is Constant, just get it or error
        if ( CheckWhatType( token ) == "NUM" ) // Constant == Num
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );

        token = GetToken(); // check next is ], just get it or error
        if ( token == "]" )
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );
      } // if
    } // while
  } // Formal_parameter_list()

  void Type_specifer( Information &info ) {
    // INT | CHAR | FLOAT | STRING | BOOL
    string token = GetToken();
    if ( token == "int" || token == "char" || token == "float" ||
         token == "string" || token == "bool" ) {
      info.token.push_back( token );
      info.variable_Type = token; // Variable type definition
    } // if
    else
      throw SyntaxErrorException( token );
  } // Type_specifer()

  void Compound_statement( Information &info ) {
    // '{' { declaration | statement } '}'
    string token;

    // if come in Compound Statement, then it is a function declaration
    mFuncD = true;

    token = GetToken();
    if ( token == "{" ) {
      // { .....
      info.token.push_back( token );
    } // if
    else
      throw SyntaxErrorException( token );

    while ( PeekToken() != "}" ) {
      // ...... { declaration | statement } .....
      // keep getting until the } comes
      if ( PeekToken() == "int" || PeekToken() == "char" || PeekToken() == "float" ||
           PeekToken() == "string" || PeekToken() == "bool" )
        Declaration( info );
      else
        Statement( info );
    } // while

    token = GetToken();
    if ( token == "}" ) {
      // ..... }
      info.token.push_back( token );
    } // if
    else
      throw SyntaxErrorException( token );
  } // Compound_statement()

  void Declaration( Information &info ) {
    // type_specifier Identifier rest_of_declarators
    string token;
    Type_specifer( info );

    if ( CheckWhatType( PeekToken() ) == "IDENT" ) {
      // ..... Identifier .....
      token = GetToken();
      info.token.push_back( token );
      // new ID come in the function, so push it
      info.function_vArray.push_back( token );

      // ..... rest_of_declarators
      Rest_of_declarators( info );
    } // if
    else {
      token = GetToken();
      throw SyntaxErrorException( token );
    } // else
  } // Declaration()

  void Rest_of_declarators( Information &info ) {
    // [ '[' Constant ']' ] { ',' Identifier [ '[' Constant ']' ] } ';'
    string token;

    if ( PeekToken() == "[" ) {
      // [ '[' Constant ']' ] .....
      token = GetToken();
      info.token.push_back( token ); // [

      token = GetToken();
      if ( CheckWhatType( token ) == "NUM" ) // NUM == CONSTANT
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      token = GetToken();
      if ( token == "]" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // if

    while ( PeekToken() == "," ) {
      // ..... { ',' Identifier [ '[' Constant ']' ] } .....
      token = GetToken();
      info.token.push_back( token ); // ,

      token = GetToken();
      if ( CheckWhatType( token ) == "IDENT" ) {
        info.token.push_back( token );
        // new ID come in the function, so push it
        info.function_vArray.push_back( token );
      } // if
      else
        throw SyntaxErrorException( token );
      
      if ( PeekToken() == "[" ) {
        // ..... [ '[' Constant ']' ] .....
        token = GetToken();
        info.token.push_back( token ); // [

        token = GetToken();
        if ( CheckWhatType( token ) == "NUM" ) // NUM == CONSTANT
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );
      
        token = GetToken();
        if ( token == "]" )
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );
      } // if
    } // while()

    if ( PeekToken() == ";" ) {
      // ..... ;
      token = GetToken();
      info.token.push_back( token );
    } // if
    else {
      token = GetToken();
      throw SyntaxErrorException( token );
    } // else
  } // Rest_of_declarators()

  void Statement( Information &info ) {
    // ';'     // the null statement
    // | expression ';'  /* expression here should not be empty */
    // | RETURN [ expression ] ';'
    // | compound_statement
    // | IF '(' expression ')' statement [ ELSE statement ]
    // | WHILE '(' expression ')' statement
    // | DO statement WHILE '(' expression ')' ';'
    string token;

    if ( PeekToken() == ";" ) {
      // ; The easiest one
      token = GetToken();
      info.token.push_back( token );
    } // if
    else if ( PeekToken() == "return" ) {
      // RETURN [ expression ] ';'
      token = GetToken(); // return
      info.token.push_back( token );
      
      if ( PeekToken() != ";" )
        // ..... [ expression ] .....
        Expression( info );
      
      token = GetToken();
      if ( token == ";" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // else if 
    else if ( PeekToken() == "{" ) {
      // compound_statement
      Compound_statement( info );
    } // else if
    else if ( PeekToken() == "if" ) {
      // IF '(' expression ')' statement [ ELSE statement ]
      token = GetToken(); // if
      info.token.push_back( token );

      token = GetToken();
      // ..... '(' .....
      if ( token == "(" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      Expression( info );

      token = GetToken();
      // ..... ')' .....
      if ( token == ")" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      Statement( info );

      if ( PeekToken() == "else" ) {
        // ..... [ ELSE statement ]
        token = GetToken(); // else
        info.token.push_back( token );

        Statement( info );
      } // if
    } // else if
    else if ( PeekToken() == "while" ) {
      // WHILE '(' expression ')' statement
      token = GetToken(); // while
      info.token.push_back( token );

      token = GetToken(); // (
      if ( token == "(" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      // while ( expression .....
      Expression( info );

      token = GetToken(); // )
      if ( token == ")" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      // while ( expression ) .....
      Statement( info );
    } // else if
    else if ( PeekToken() == "do" ) {
      // DO statement WHILE '(' expression ')' ';'
      token = GetToken(); // do
      info.token.push_back( token );

      // DO statement ......
      Statement( info );

      token = GetToken(); // while
      if ( token == "while" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      // DO statement while ......
      token = GetToken(); // (
      if ( token == "(" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      // DO statement while ( ......
      Expression( info );
      // DO statement while ( expression .....
      token = GetToken(); // )
      if ( token == ")" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      // DO statement while ( expression ) .....

      token = GetToken(); // ;
      if ( token == ";" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // else if
    else {
      // expression ';'
      // upside if is about basic expression first token
      Expression( info );

      token = GetToken(); // ..... ;
      if ( token == ";" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // else
  } // Statement()

  void Expression( Information &info ) {
    // basic_expression { ',' basic_expression }
    string token;

    Basic_expression( info );

    while ( PeekToken() == "," ) {
      // ..... { ',' basic_expression }
      token = GetToken();
      info.token.push_back( token );

      Basic_expression( info );
    } // while
  } // Expression()

  void Basic_expression( Information &info ) {
    // Identifier rest_of_Identifier_started_basic_exp
    // | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
    // | sign { sign } signed_unary_exp romce_and_romloe
    // | ( Constant | '(' expression ')' ) romce_and_romloe
    string token;

    if ( CheckWhatType( PeekToken() ) == "IDENT" ) {
      // Identifier rest_of_Identifier_started_basic_exp
      token = GetToken(); // id
      if ( !IsIDExisted( token, info ) )
        throw UndefinedErrorException( token );
      else
        info.token.push_back( token );
      
      Rest_of_Identifier_started_basic_exp( info );
    } // if
    else if ( PeekToken() == "cin" || PeekToken() == "cout" ) {
      // KeepWord Cin Cout
      token = GetToken();
      info.token.push_back( token );

      if ( token == "cout" ) {
        mIsCout = true;
        mIsCin = false;
      } // if
      else {
        mIsCin = true;
        mIsCout = false;
      } // else

      Romce_and_romloe( info );
      mIsCout = false;
      mIsCin = false;
    } // else if
    else if ( PeekToken() == "++" || PeekToken() == "-- " ) {
      // ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
      token = GetToken(); // ++ || --
      info.token.push_back( token );
      
      token = GetToken(); // ID
      if ( CheckWhatType( token ) != "IDENT" )
        throw SyntaxErrorException( token );
      else if ( !IsIDExisted( token, info ) )
        throw UndefinedErrorException( token );
      else
        info.token.push_back( token );
      
      Rest_of_PPMM_Identifier_started_basic_exp( info );  
    } // else if
    else if ( PeekToken() == "+" || PeekToken() == "-" || PeekToken() == "!" ) {
      // sign { sign } signed_unary_exp romce_and_romloe
      // mIsSigned is false
      Sign( info );
      // mIsSigned is true
      while ( PeekToken() == "+" || PeekToken() == "-" || PeekToken() == "!" )
        Sign( info );
      
      Signed_unary_exp( info );
      // if we finished signed with constant, then we set it to false
      // when next time into the function upside, then it will not sign
      mIsSigned = false;

      Romce_and_romloe( info );
    } // else if
    else if ( ( CheckWhatType( PeekToken() ) == "NUM" ) || PeekToken() == "(" ) {
      // ( Constant | '(' expression ')' ) romce_and_romloe
      if ( CheckWhatType( PeekToken() ) == "NUM" ) {
        // Constant .....
        token = GetToken();
        info.token.push_back( token );
      } // if
      else if ( PeekToken() == "(" ) {
        // ( .....
        token = GetToken();
        info.token.push_back( token );

        Expression( info );
        token = GetToken(); // )
        if ( token == ")" )
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );
        // ( expression ) .....
      } // else if
      else {
        token = GetToken();
        throw SyntaxErrorException( token );
      } // else
      
      Romce_and_romloe( info );
    } // else if 
    else {
      token = GetToken();
      throw SyntaxErrorException( token );
    } // else
  } // Basic_expression()

  void Rest_of_Identifier_started_basic_exp( Information &info ) {
    // [ '[' expression ']' ] ( assignment_operator basic_expression 
    // | [ PP | MM ] romce_and_romloe )
    // | '(' [ actual_parameter_list ] ')' romce_and_romloe
    string token;
    if ( PeekToken() == "(" ) {
      // | '(' [ actual_parameter_list ] ')' romce_and_romloe
      token = GetToken();
      info.token.push_back( token );

      if ( PeekToken() != ")" )
        Actual_parameter_list( info );
      
      token = GetToken();
      if ( token != ")" ) {
        token = token;
        throw SyntaxErrorException( token );
      } // if

      info.token.push_back( token );

      Romce_and_romloe( info );
    } // if
    else {
      // [ '[' expression ']' ] ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
      if ( PeekToken() == "[" ) {
        // [ '[' expression ']' ] .....
        token = GetToken(); // [
        info.token.push_back( token );

        Expression( info );

        token = GetToken();
        if ( token == "]" )
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );
      } // if

      if ( PeekToken() == "=" || PeekToken() == "*=" || PeekToken() == "/=" ||
           PeekToken() == "%=" || PeekToken() == "+=" || PeekToken() == "-=" ) {
        // ..... assignment_operator basic_expression .....
        Assignment_operator( info );

        Basic_expression( info );
      } // if
      else {
        // [ PP | MM ] romce_and_romloe
        if ( PeekToken() == "++" || PeekToken() == "--" ) {
          token = GetToken();
          info.token.push_back( token );
        } // if

        Romce_and_romloe( info );
      } // else
    } // else
  } // Rest_of_Identifier_started_basic_exp()

  void Rest_of_PPMM_Identifier_started_basic_exp( Information &info ) {
    // [ '[' expression ']' ] romce_and_romloe
    string token; 
    if ( PeekToken() == "[" ) {
      // [ '[' expression ']' ] .....
      token = GetToken(); // [
      info.token.push_back( token );

      Expression( info );

      token = GetToken();
      if ( token == "]" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // if

    Romce_and_romloe( info );
  } // Rest_of_PPMM_Identifier_started_basic_exp()

  void Sign( Information &info ) {
    // '+' | '-' | '!'
    string token = GetToken();
    if ( token == "+" || token == "-" || token == "!" )
      info.token.push_back( token );
    else
      throw SyntaxErrorException( token );
    
    mIsSigned = true; // Sign completed so true
  } // Sign()

  void Signed_unary_exp( Information &info ) {
    // Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']' ]
    // | Constant 
    // | '(' expression ')'
    string token = GetToken();

    if ( CheckWhatType( token ) == "IDENT" ) {
      // Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']' ]
      if ( !IsIDExisted( token, info ) ) {
        throw UndefinedErrorException( token );
      } // if

      info.token.push_back( token );

      if ( PeekToken() == "(" ) {
        // ..... '(' [ actual_parameter_list ] ')' .....
        token = GetToken(); // (
        info.token.push_back( token );

        if ( PeekToken() != ")" )
          Actual_parameter_list( info );
        
        token = GetToken();
        if ( token == ")" )
          info.token.push_back( token );
        else {
          throw SyntaxErrorException( token );
        } // else
      } // if
      else if ( PeekToken() == "[" ) {
        // ..... '[' expression ']' ] .....
        token = GetToken();
        info.token.push_back( token );

        Expression( info );

        token = GetToken();
        if ( token == "]" )
          info.token.push_back( token );
        else {
          throw SyntaxErrorException( token );
        } // else
      } // else if
    } // if
    else if ( CheckWhatType( token ) == "NUM" ) {
      // Constant
      if ( token == "true" || token == "false" || !mIsSigned )
        info.token.push_back( token );
      else
        info.token.back() += token; 
    } // else if
    else if ( token == "(" ) {
      // '(' expression ')'
      info.token.push_back( token );
      Expression( info );

      token = GetToken();
      if ( token == ")" )
        info.token.push_back( token );
      else throw SyntaxErrorException( token );
    } // else if
    else {
      token = token;
      throw SyntaxErrorException( token );
    } // else
  } // Signed_unary_exp()

  void Romce_and_romloe( Information &info ) {
    // rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
    string token;

    // Special case for the cin cout problem
    if ( !mIsCout && !mIsCin )
      Rest_of_maybe_logical_OR_exp( info );
    else
      Rest_of_maybe_shift_exp( info );
    
    if ( PeekToken() == "?" ) {
      // ..... [ '?' basic_expression ':' basic_expression ]
      token = GetToken(); // ?
      info.token.push_back( token );

      Basic_expression( info );

      token = GetToken(); // :
      if ( token == ":" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
      
      Basic_expression( info );
    } // if
  } // Romce_and_romloe()

  void Assignment_operator( Information &info ) {
    // '=' | TE | DE | RE | PE | ME
    string token = GetToken();
    if ( token == "=" || token == "*=" || token == "/=" ||
         token == "%=" || token == "+=" || token == "-=" )
      info.token.push_back( token );
    else
      throw SyntaxErrorException( token );
  } // Assignment_operator()

  void Actual_parameter_list( Information &info ) {
    // basic_expression { ',' basic_expression }
    string token;

    Basic_expression( info );

    while ( PeekToken() == "," ) {
      // ..... { ',' basic_expression }
      token = GetToken();
      info.token.push_back( token );

      Basic_expression( info );
    } // while
  } // Actual_parameter_list()

  void Rest_of_maybe_logical_OR_exp( Information &info ) {
    // rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
    string token;

    Rest_of_maybe_logical_AND_exp( info );

    while ( PeekToken() == "||" ) {
      token = GetToken(); // ||
      info.token.push_back( token );

      Maybe_logical_And_exp( info );
    } // while
  } // Rest_of_maybe_logical_OR_exp()

  void Rest_of_maybe_shift_exp( Information &info ) {
    // rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    string token;

    // Special Case For Cin Cout
    if ( !mIsCout && !mIsCin )
      Rest_of_maybe_additive_exp( info );
    // ..................................

    while ( PeekToken() == ">>" || PeekToken() == "<<" ) {
      // ..... { ( LS | RS ) maybe_additive_exp }
      token = GetToken(); // >> || <<
      // now is cin, but there is not a peektoken that is >>
      if ( mIsCin && ( token != ">>" && token != ";" ) )
        throw SyntaxErrorException( token );
      // now is cout, but there is not a peektoken that is <<
      else if ( mIsCout && ( token != "<<" && token != ";" ) )
        throw SyntaxErrorException( token );
      
      info.token.push_back( token );
      Maybe_additive_exp( info );
    } // while
  } // Rest_of_maybe_shift_exp()

  void Rest_of_maybe_logical_AND_exp( Information &info ) {
    // rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    string token;

    Rest_of_maybe_bit_OR_exp( info );

    while ( PeekToken() == "&&" ) {
      // ..... { AND maybe_bit_OR_exp }
      token = GetToken(); // &&
      info.token.push_back( token );

      Maybe_bit_OR_exp( info );
    } // while
  } // Rest_of_maybe_logical_AND_exp()

  void Maybe_logical_And_exp( Information &info ) {
    // maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    string token;

    Maybe_bit_OR_exp( info );

    while ( PeekToken() == "&&" ) {
      // { AND maybe_bit_OR_exp }
      token = GetToken(); // &&
      info.token.push_back( token );

      Maybe_bit_OR_exp( info );
    } // while
  } // Maybe_logical_And_exp()

  void Rest_of_maybe_additive_exp( Information &info ) {
    // rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
    string token;

    Rest_of_maybe_mult_exp( info );

    while ( PeekToken() == "+" || PeekToken() == "-" ) {
      // ..... { ( '+' | '-' ) maybe_mult_exp }
      token = GetToken(); // + || -
      info.token.push_back( token );

      Maybe_mult_exp( info );
    } // while
  } // Rest_of_maybe_additive_exp()

  void Maybe_additive_exp( Information &info ) {
    // maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
    string token;

    Maybe_mult_exp( info );

    while ( PeekToken() == "+" || PeekToken() == "-" ) {
      // ..... { ( '+' | '-' ) maybe_mult_exp }
      token = GetToken();
      info.token.push_back( token );

      Maybe_mult_exp( info );
    } // while
  } // Maybe_additive_exp()

  void Rest_of_maybe_bit_OR_exp( Information &info ) {
    // rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    string token;

    Rest_of_maybe_bit_ex_OR_exp( info );

    while ( PeekToken() == "|" ) {
      // ..... { '|' maybe_bit_ex_OR_exp }
      token = GetToken();
      info.token.push_back( token );

      Maybe_bit_ex_OR_exp( info );
    } // while
  } // Rest_of_maybe_bit_OR_exp()

  void Maybe_bit_OR_exp( Information &info ) {
    // maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    string token;

    Maybe_bit_ex_OR_exp( info );

    while ( PeekToken() == "|" ) {
      // ..... { '|' maybe_bit_ex_OR_exp }
      token = GetToken();
      info.token.push_back( token );

      Maybe_bit_ex_OR_exp( info );
    } // while
  } // Maybe_bit_OR_exp()

  void Rest_of_maybe_mult_exp( Information &info ) {
    // { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */
    string token;

    while ( PeekToken() == "*" || PeekToken() == "/" || PeekToken() == "%" ) {
      // { ( '*' | '/' | '%' ) unary_exp }
      token = GetToken();
      info.token.push_back( token );

      Unary_exp( info );
    } // while
  } // Rest_of_maybe_mult_exp()

  void Maybe_mult_exp( Information &info ) {
    // unary_exp rest_of_maybe_mult_exp
    Unary_exp( info );

    Rest_of_maybe_mult_exp( info );
  } // Maybe_mult_exp()

  void Rest_of_maybe_bit_ex_OR_exp( Information &info ) {
    // rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
    string token;

    Rest_of_maybe_bit_AND_exp( info );

    while ( PeekToken() == "^" ) {
      // ..... { '^' maybe_bit_AND_exp }
      token = GetToken(); // ^
      info.token.push_back( token );

      Maybe_bit_AND_exp( info );
    } // while
  } // Rest_of_maybe_bit_ex_OR_exp()

  void Maybe_bit_ex_OR_exp( Information &info ) {
    // maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
    string token;

    Maybe_bit_AND_exp( info );

    while ( PeekToken() == "^" ) {
      token = GetToken(); // ^
      info.token.push_back( token );

      Maybe_bit_AND_exp( info );
    } // while
  } // Maybe_bit_ex_OR_exp()

  void Unary_exp( Information &info ) {
    // sign { sign } signed_unary_exp
    // | unsigned_unary_exp
    // | ( PP | MM ) Identifier [ '[' expression ']' ]
    string token;

    if ( PeekToken() == "++" || PeekToken() == "--" ) {
      // ( PP | MM ) Identifier [ '[' expression ']' ]
      token = GetToken(); // ++ --
      info.token.push_back( token );

      token = GetToken();
      if ( CheckWhatType( token ) != "IDENT" )
        throw SyntaxErrorException( token );
      else if ( !IsIDExisted( token, info ) )
        throw UndefinedErrorException( token );
      else
        info.token.push_back( token );
      
      if ( PeekToken() == "[" ) {
        // ..... [ '[' expression ']' ]
        token = GetToken(); // +
        info.token.push_back( token );

        Expression( info );

        token = GetToken();
        if ( token != "]" )
          throw SyntaxErrorException( token );
        else
          info.token.push_back( token );
      } // if
    } // if
    else if ( PeekToken() == "+" || PeekToken() == "-" || PeekToken() == "!" ) {
      // sign { sign } signed_unary_exp
      Sign( info );

      while ( PeekToken() == "+" || PeekToken() == "-" || PeekToken() == "!" )
        // ..... { sign } ......
        Sign( info );
      
      Signed_unary_exp( info );
      mIsSigned = false;
    } // if
    else 
      Unsigned_unary_exp( info );
  } // Unary_exp()

  void Rest_of_maybe_bit_AND_exp( Information &info ) {
    // rest_of_maybe_equality_exp { '&' maybe_equality_exp }
    string token;

    Rest_of_maybe_equality_exp( info );

    while ( PeekToken() == "&" ) {
      // ...... { '&' maybe_equality_exp }
      token = GetToken();
      info.token.push_back( token );

      Maybe_equality_exp( info );
    } // while
  } // Rest_of_maybe_bit_AND_exp()

  void Maybe_bit_AND_exp( Information &info ) {
    // maybe_equality_exp { '&' maybe_equality_exp }
    string token;

    Maybe_equality_exp( info );

    while ( PeekToken() == "&" ) {
      // ..... { '&' maybe_equality_exp }
      token = GetToken(); // &
      info.token.push_back( token );

      Maybe_equality_exp( info );
    } // while
  } // Maybe_bit_AND_exp()

  void Unsigned_unary_exp( Information &info ) {
    // Identifier [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ]
    // | Constant 
    // | '(' expression ')'
    string token = GetToken();

    if ( CheckWhatType( token ) == "IDENT" ) {
      // Identifier [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ]
      if ( !IsIDExisted( token, info ) )
        throw UndefinedErrorException( token );
      
      info.token.push_back( token ); // ID

      if ( PeekToken() == "(" ) {
        // '(' [ actual_parameter_list ] ')'
        token = GetToken(); // (
        info.token.push_back( token );

        if ( PeekToken() != ")" )
          Actual_parameter_list( info );
        
        token = GetToken(); // )
        if ( token == ")" )
          info.token.push_back( token );
        else
          throw SyntaxErrorException( token );
      } // if
      else if ( PeekToken() == "[" || PeekToken() == "++" || PeekToken() == "--" ) {
        // ..... [ '[' expression ']' ] ......
        if ( PeekToken() == "[" ) {
          token = GetToken(); // [
          info.token.push_back( token );

          Expression( info );

          token = GetToken(); // ]
          if ( token == "]" )
            info.token.push_back( token );
          else
            throw SyntaxErrorException( token );
        } // if

        if ( PeekToken() == "++" || PeekToken() == "--" ) {
          // ..... [ ( PP | MM ) ]
          token = GetToken();
          info.token.push_back( token );
        } // if
      } // else if
      else
        ; // [] no Syntax error
    } // if
    else if ( CheckWhatType( token ) == "NUM" )
      info.token.push_back( token );
    else if ( token == "(" ) {
      // '(' expression ')'
      info.token.push_back( token );

      Expression( info );

      token = GetToken();
      if ( token == ")" )
        info.token.push_back( token );
      else
        throw SyntaxErrorException( token );
    } // else if
    else {
      token = token;
      throw SyntaxErrorException( token );
    } // else
  } // Unsigned_unary_exp()

  void Rest_of_maybe_equality_exp( Information &info ) {
    // rest_of_maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
    string token;

    Rest_of_maybe_relational_exp( info );

    while ( PeekToken() == "==" || PeekToken() == "!=" ) {
      // { ( EQ | NEQ ) maybe_relational_exp }
      token = GetToken(); // == || !=
      info.token.push_back( token );

      Maybe_relational_exp( info );
    } // while
  } // Rest_of_maybe_equality_exp()

  void Maybe_equality_exp( Information &info ) {
    // maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp}
    string token;

    Maybe_relational_exp( info );

    while ( PeekToken() == "==" || PeekToken() == "!=" ) {
      // { ( EQ | NEQ ) maybe_relational_exp}
      token = GetToken(); // == || !=
      info.token.push_back( token );

      Maybe_relational_exp( info );
    } // while
  } // Maybe_equality_exp()

  void Rest_of_maybe_relational_exp( Information &info ) {
    // rest_of_maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
    string token;

    Rest_of_maybe_shift_exp( info );

    while ( PeekToken() == "<" || PeekToken() == ">" || PeekToken() == "<=" || PeekToken() == ">=" ) {
      // ..... { ( '<' | '>' | LE | GE ) maybe_shift_exp }
      token = GetToken(); // < > <= >=
      info.token.push_back( token );

      Maybe_shift_exp( info );
    } // while
  } // Rest_of_maybe_relational_exp()

  void Maybe_relational_exp( Information &info ) {
    // maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
    string token;

    Maybe_shift_exp( info );

    while ( PeekToken() == "<" || PeekToken() == ">" || PeekToken() == "<=" || PeekToken() == ">=" ) {
      // ..... { ( '<' | '>' | LE | GE ) maybe_shift_exp }
      token = GetToken(); // < > <= >=
      info.token.push_back( token );

      Maybe_shift_exp( info );
    } // while
  } // Maybe_relational_exp()

  void Maybe_shift_exp( Information &info ) {
    // maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    string token;

    Maybe_additive_exp( info );

    while ( PeekToken() == ">>" || PeekToken() == "<<" ) {
      // ..... { ( LS | RS ) maybe_additive_exp }
      token = GetToken();
      info.token.push_back( token );

      Maybe_additive_exp( info );
    } // while
  } // Maybe_shift_exp()
  // ===================================================================================
  
  public :
  
  void Analyze() {
    bool end = false;
    while ( !end ) {
      try {
        gLine = 1;
        if ( !User_Input() )
          throw End();
        else {
          char tempC;
          while ( cin.peek() == ' ' || cin.peek() == '\t' ) 
            tempC = GetNextChar();
      
          tempC = GetNextChar() ;
          if ( tempC == '/' && cin.peek() == '/' ) {
            while ( cin.peek() != '\n' ) 
              tempC = GetNextChar();
            tempC = GetNextChar();
          } // else if
          else if ( tempC != '\n' )
            cin.putback( tempC ) ;
        } // else
      } // try
      catch ( End e ) {
        end = true;
      } // catch
      catch ( LexicalErrorException e ) {
        char tempC;
        while ( cin.peek() != '\n' ) 
          tempC = GetNextChar();
        tempC = GetNextChar();
      } // catch
      catch ( SyntaxErrorException e ) {
        char tempC;
        while ( cin.peek() != '\n' ) 
          tempC = GetNextChar();
        tempC = GetNextChar();
      } // catch
      catch ( UndefinedErrorException e ) {
        char tempC;
        while ( cin.peek() != 'Z' && cin.peek() != '\n' ) 
          tempC = GetNextChar();
        tempC = GetNextChar();
      } // catch
    } // while
  } // Analyze()
  
}; // Analyzer

int main() {
  char uTestNum;
  char line;
  scanf( "%c", &uTestNum );
  scanf( "%c", &line );
  cout << "Our-C running ..." << endl; 
  Analyzer a;
  a.Analyze();
} // main()
