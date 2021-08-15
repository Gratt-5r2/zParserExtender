// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {


  bool WordIsFloat( const string& word ) {
    return word.IsNumber() && word.Search( "." ) != Invalid;
  }


  bool WordIsInteger( const string& word ) {
    return word.IsNumber() && !WordIsFloat( word );
  }


	void zCParser::DeclareVaFuncCall( zSTRING& funcName ) {
    zSTRING leafNodeName = funcName;
    zSTRING word;
    Match( Z "(" );

    // Is a format line - main argument of this function.
    // Should add this argument in the end of procedure.
    zCPar_TreeNode* formatLeaf = CreateStringLeaf();

    while( true ) {
      ReadWord( word );
      if( word != "," ) {
        PrevWord();
        break;
      }

      // Is a string line
      ReadWord( word );
      PrevWord();
      if( word == "\"" ) {
        treenode->SetNext( CreateStringLeaf() );
        while( treenode->next ) treenode = treenode->next;
        continue;
      }

      // Is a float value
      if( WordIsFloat( word ) ) {
        treenode->SetNext( CreateFloatLeaf() );
        treenode = treenode->next;
        continue;
      }

      // Is an integer value
      if( WordIsInteger( word ) ) {
        treenode->SetNext( ParseExpression() );
        while( treenode->next ) treenode = treenode->next;
        PrevWord();
        continue;
      }

      // Is a variable
      zCPar_Symbol* sym = GetLocalSymbol( word );
      switch( sym->type ) {
        case zPAR_TYPE_INT:
          treenode->SetNext( ParseExpression() );
          while( treenode->next ) treenode = treenode->next;
          PrevWord();
          break;
        case zPAR_TYPE_FLOAT:
          treenode->SetNext( CreateFloatLeaf() );
          treenode = treenode->next;
          break;
        case zPAR_TYPE_STRING:
          treenode->SetNext( CreateStringLeaf() );
          while( treenode->next ) treenode = treenode->next;
          break;
        default: // case zPAR_TYPE_INSTANCE:
          ReadWord( aword );
          treenode = CreateLeaf( zPAR_TOK_PUSHINST, treenode );
          treenode->name = word;
          treenode->value = GetBaseClass( sym );
          treenode->info = zPAR_TYPE_INSTANCE;
          break;
      }

    }

    treenode->SetNext( formatLeaf );
    while( treenode->next ) treenode = treenode->next;

    Match( Z ")" );

    treenode = CreateLeaf( zPAR_TOK_CALL, treenode );
    treenode->name = leafNodeName;
	}
}