// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {


#if 0
  static int VE_GetInt( const string& command, int& value ) {
    if( command.IsHexNumber() ) {
      command.ToInt32( 16 );
      return True;
    }

    if( command.IsNumber() ) {
      command.ToInt32( 10 );
      return True;
    }

    return False;
  }





  static int VE_GetIntVar( const string& command, int index, int& value ) {
    zCPar_Symbol* symbol = parser->GetSymbol( command );
    if( symbol && symbol->type == zPAR_TYPE_INT ) {
      symbol->GetValue( value, index );
      return True;
    }

    return False;
  }






  //
  static int EditVariable( const string& command, zSTRING& message ) {
    uint wordNr = 1;
    int index = 0;


    //
    string symbolName = command.GetWordSmart( wordNr++, true );
    zCPar_Symbol* symbol = parser->GetSymbol( symbolName );
    if( !symbol ) {
      message = "Variable is not exists";
      return False;
    }

    //
    string s_operator = command.GetWordSmart( wordNr++, true );
    if( s_operator == "[" ) {
      string s_index = command.GetWordSmart( wordNr++, true );
      if( !VE_GetInt( s_index, index ) ) {
        if( !VE_GetIntVar( s_index, 0, index ) ) {
          message = "Index is not exists";
          return False;
        }
      }

      s_operator = command.GetWordSmart( wordNr++, true );
      if( s_operator != "]" ) {
        message = "Close bracket not found";
        return False;
      }

      s_operator = command.GetWordSmart( wordNr++, true );
    }

    //
    if( s_operator == "=" ) {
      int value = 0;

      string s_value = command.GetWordSmart( wordNr++, true );
      if( !VE_GetInt( s_value, value ) ) {

        // 
        int index = 0;
        string s_index = command.GetWordSmart( wordNr++, true );
        if( s_index == "[" ) {
          s_index = command.GetWordSmart( wordNr++, true );
          if( !VE_GetInt( s_index, index ) ) {
            if( !VE_GetIntVar( s_index, 0, index ) ) {
              message = "Index is not exists";
              return False;
            }
          }
        }

        // 
        if( !VE_GetIntVar( s_value, index, value ) ) {

        }
      }
    }


    return True;
  }
// #endif

  static bool VariableEditor_CheckTokensLength( Array<string>& tokens, uint& iterator, zSTRING& message ) {
    if( iterator >= tokens.GetNum() ) {
      message = "Variable index not exists";
      return false;
    }

    return true;
  }



  static bool VariableEditor_GetArrayIndex( const string& command, int& value, zSTRING& message ) {
    if( command.IsHexNumber() ) {
      command.ToInt32( 16 );
      return true;
    }

    if( command.IsNumber() ) {
      command.ToInt32( 10 );
      return true;
    }

    zCPar_Symbol* symbol = parser->GetSymbol( command );
    if( symbol && symbol->type == zPAR_TYPE_INT ) {
      symbol->GetValue( value, 0 );
      return true;
    }

    message = "Variable index not exists";
    return false;
  }



  static bool VariableEditor_GetSymbolName( Array<string>& tokens, string& symbolName, uint& iterator, zSTRING& message ) {
    if( iterator > tokens.GetNum() ) {
      message = "Variable name not exists";
      return false;
    }

    string symbolName = tokens[iterator++];
    return true;
  }



  static bool VariableEditor_GetSymbolName( Array<string>& tokens, uint& index, uint& iterator, zSTRING& message ) {
    if( !VariableEditor_CheckTokensLength( tokens, iterator, message ) )
      return false;

    if( tokens[iterator] == "[" ) {
      if( !VariableEditor_CheckTokensLength( tokens, iterator, message ) )
        return false;

      iterator++;

      VariableEditor_GetArrayIndex(  );
    }

    return true;
  }
#endif






  struct zTVariableEditor {
    string Command;
    string LeftValue;
    string RightValue;
    zCPar_Symbol* LeftSymbol;
    uint LeftIndex;
    uint RightIndex;
    string Message;
    uint Iterator;

    zTVariableEditor( string command );
    string NextToken();
    string PrevToken();
    string CurrentToken();
    bool Parse();
    bool ParseSymbol( zCPar_Symbol*& symbol );
    int ParseIndex();
    bool ParseAssign();
    int GetLeftIndex();
    int GetRightIndex();
  };



  zTVariableEditor::zTVariableEditor( string command ) {
    Command  = command;
    Iterator = 0;
  }



  string zTVariableEditor::NextToken() {
    return Command.GetWordSmart( ++Iterator );
  }



  string zTVariableEditor::PrevToken() {
    return Command.GetWordSmart( --Iterator );
  }



  string zTVariableEditor::CurrentToken() {
    return Command.GetWordSmart( Iterator );
  }




  inline string GetSymbolValue( zCPar_Symbol* symbol, int index ) {
    if( symbol->type == zPAR_TYPE_VOID )
      return "void";

    if( symbol->type == zPAR_TYPE_FLOAT ) {
      float value;
      symbol->GetValue( value, index );
      return value;
    }

    if( symbol->type == zPAR_TYPE_INT ) {
      int value;
      symbol->GetValue( value, index );
      return value;
    }

    if( symbol->type == zPAR_TYPE_STRING ) {
      zSTRING value;
      symbol->GetValue( value, index );
      return value;
    }

    if( symbol->type == zPAR_TYPE_CLASS )
      return symbol->GetName();

    if( symbol->type == zPAR_TYPE_FUNC )
      return symbol->GetName();

    if( symbol->type == zPAR_TYPE_PROTOTYPE )
      return symbol->GetName();

    if( symbol->type == zPAR_TYPE_INSTANCE )
      return symbol->GetName();
  }



  inline string GetSymbolValue( zCPar_Symbol* symbol ) {
    string data;
    int ele = symbol->ele;

    for( int i = 0; i < ele; i++ )
      data += GetSymbolValue( symbol, i ) + "; ";
    
    return data;
  }




  bool zTVariableEditor::Parse() {
    if( !ParseSymbol( LeftSymbol ) )
      return false;

    if( !ParseAssign() ) {
      Message = LeftIndex != Invalid ?
        GetSymbolValue( LeftSymbol, LeftIndex ) :
        GetSymbolValue( LeftSymbol );

      return true;
    }



    return true;
  }



  bool zTVariableEditor::ParseSymbol( zCPar_Symbol*& symbol ) {
    string symbolName = NextToken();
    symbol = parser->GetSymbol( symbolName );
    
    if( !LeftSymbol ) {
      Message = "Unknown symbol";
      return false;
    }

    LeftIndex = ParseIndex();

    return true;
  }




  int zTVariableEditor::ParseIndex() {
    string s_operator = NextToken();
    if( s_operator != "[" ) {
      PrevToken();
      return Invalid;
    }

    string s_symbol = NextToken();
    NextToken();

    if( s_symbol.IsNumber() )
      return s_symbol.ToInt32();
    
    zCPar_Symbol* symbol = parser->GetSymbol( s_symbol );
    if( symbol && symbol->type == zPAR_TYPE_INT ) {
      int value;
      symbol->GetValue( value, 0 );
      return value;
    }

    return Invalid;
  }



  bool zTVariableEditor::ParseAssign() {
    string s_operator = NextToken();
    if( s_operator != "=" )
      return false;

    string s_value = NextToken();
    if( s_value.IsNumber() ) {
      if( LeftSymbol->type == zPAR_TYPE_INT ) {
        LeftSymbol->SetValue( (int)s_value.ToInt32(), GetLeftIndex() );
        return true;
      }

      if( LeftSymbol->type == zPAR_TYPE_FLOAT ) {
        LeftSymbol->SetValue( (float)s_value.ToReal32(), GetLeftIndex() );
        return true;
      }
    }
    else if( s_value == "\"" ) {
      if( LeftSymbol->type == zPAR_TYPE_STRING ) {
        zSTRING text = Command.GetPattern( "\"", "\"" );
        LeftSymbol->SetValue( text, GetLeftIndex() );
        return true;
      }
    }
    else {
      zCPar_Symbol* rightSymbol;
      if( ParseSymbol( rightSymbol ) ) {
        Message = "Unknown right value";
        return false;
      }


      if( LeftSymbol->type == rightSymbol->type ) {
        RightIndex = ParseIndex();

        if( LeftSymbol->type == zPAR_TYPE_FLOAT ) {
          float value;
          rightSymbol->GetValue( value, GetRightIndex() );
          LeftSymbol->SetValue( value, GetLeftIndex() );
          Message = string::Combine( "%z = %f", LeftSymbol->GetName(), value );
        }
        else if( LeftSymbol->type == zPAR_TYPE_STRING ) {
          zSTRING value;
          rightSymbol->GetValue( value, GetRightIndex() );
          LeftSymbol->SetValue( value, GetLeftIndex() );
          Message = string::Combine( "%z = %z", LeftSymbol->GetName(), value );
        }
        else {
          int value;
          rightSymbol->GetValue( value, GetRightIndex() );
          LeftSymbol->SetValue( value, GetLeftIndex() );
          Message = string::Combine( "%z = %i", LeftSymbol->GetName(), value );
        }
        
        return true;
      }
    }

    Message = "Different types";
    return false;
  }



  int zTVariableEditor::GetLeftIndex() {
    return LeftIndex != Invalid ? 0 : LeftIndex;
  }



  int zTVariableEditor::GetRightIndex() {
    return RightIndex != Invalid ? 0 : RightIndex;
  }







  static int VariableEditor( const string& command, zSTRING& message ) {
    /*Array<string> tokens;
    for( uint i = 1; true; i++ ) {
      string token = command.GetWordSmart( 1, true );
      if( token.IsEmpty() )
        break;

      tokens += token;
    }
    
    string symbolName = NullStr;
    string rightValue = NullStr;
    uint   iterator   = 0;
    uint   index      = 0;*/

    zTVariableEditor variableEditor( command );
    variableEditor.Parse();
    return True;
  }
}