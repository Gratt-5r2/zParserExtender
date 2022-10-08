// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  struct NamespaceInfo {
    string Name;
    uint Entries;
    NamespaceInfo( const string& name ) : Entries( 1 ) {
      Name = name;
    }

    NamespaceInfo( const NamespaceInfo& other ) {
      Name = other.Name;
      Entries = other.Entries;
    }
  };

  Array<NamespaceInfo> Namespaces;
  Array<string> UsingNamespaces;
  static char* prevNamespacePos = (char*)Invalid;


  void zCParser::InitializeNamespace( const zSTRING& defaultNamespace ) {
    Namespaces.Clear();
    UsingNamespaces.Clear();
    UsingNamespaces |= zParserExtender.GetUsingNamespaces();
  }


  void zCParser::NamespaceBegin( zSTRING& word ) {
    if( in_func || in_class )
      Error( Z "U:PAR: 'namespace' can not be defined in function or class.", zERROR_LEVEL::zERR_FAULT );

    THISCALL( Ivk_zCParser_ReadWord )(word);
    Namespaces.Create( word );
    THISCALL( Ivk_zCParser_ReadWord )(word);
    ReadWord( word );
  }


  bool zCParser::UsingNamespace( zSTRING& word ) {
    if( in_func || in_class )
      Error( Z "U:PAR: 'using' can not be defined in function or class.", zERROR_LEVEL::zERR_FAULT );

    THISCALL( Ivk_zCParser_ReadWord )(word);
    UsingNamespaces |= word;
    Match( Z ";" );
    ReadWord(word);
    return true;
  }


  void zCParser::NamespaceAddEntry( zSTRING& word ) {
    if( Namespaces.GetNum() == 0 || prevNamespacePos == pc )
      return;

    prevNamespacePos = pc;
    NamespaceInfo& namespaceInfo = Namespaces.GetLast();
    namespaceInfo.Entries++;
  }


  void zCParser::NamespaceRemoveEntry( zSTRING& word ) {
    if( Namespaces.GetNum() == 0 || prevNamespacePos == pc )
      return;

    prevNamespacePos = pc;
    NamespaceInfo& namespaceInfo = Namespaces.GetLast();
    if( --namespaceInfo.Entries == 0 ) {
      Namespaces.RemoveAt( Namespaces.GetNum() - 1 );
      THISCALL( Ivk_zCParser_ReadWord )(word);
      ReadWord(word);
    }
  }


  static uint AddNamespaceAfter = 0;
  static bool NewSymbolExpected = false;
  static bool NewParenthesisExpected = false;
  static uint NewTypedDeclarations = 0;

  inline bool IsWord( const zSTRING& word ) {
    if( word.Length() == 0 || word == "CONST" ) // For example CONST leaf is "this pattern"
      return false;

    return word[0u] == ':' || word[0u] == '_' || (word[0u] >= 'A' && word[0u] <= 'Z');
  }

  inline bool IsIndex( const zSTRING& word ) {
    return word[0u] == '[' || word[0u] == ']' || isdigit( word[0u] );
  }

  // Symbols can be repeated for example
  // after ReadWord-PrevWord construction
  inline bool IsUniqueWord( const zSTRING& word ) {
    static zSTRING lastWord;
    if( word == lastWord )
      return false;

    lastWord = word;
    return true;
  }

  void zCParser::CheckNamespace( zSTRING& word ) {
    if( word == "USING" ) {
      UsingNamespace( word );
      return;
    }

    if( word == "NAMESPACE" ) NamespaceBegin( word );
    if( word == "{" )         NamespaceAddEntry( word );
    if( word == "}" )         NamespaceRemoveEntry( word );
    if( word == "[" )         AddNamespaceAfter += 3;

    if( !NamespaceIsActive() )
      return;

    if( NewParenthesisExpected && word == "(" ) {
      DeclareNamespaceForNextWord( 2 );
      NewParenthesisExpected = false;
      return;
    }

    // if( !IsWord( word ) )
    // if( word[0u] != ':' && word[0u] != '_' && !(word[0u] >= 'A' && word[0u] <= 'Z') )
    // if( AddNamespaceAfter && word[0u] == '[' ) {
    //   AddNamespaceAfter += 3;
    // }
    if( !IsUniqueWord( word ) )
      return;

    // if( AddNamespaceAfter > 0 )
    //   cmd << "  ? " << AddNamespaceAfter << "  " << word << endl;


    if( AddNamespaceAfter && --AddNamespaceAfter == 0 ) {
      AddNamespace( word, NewSymbolExpected );
      NewSymbolExpected = false;
    }
    else if( NewTypedDeclarations ) {
      AddNamespace( word, false );
      NewTypedDeclarations--;
    }
  }


  zCPar_Symbol* zCParser::GetNearestVariable( zSTRING& word ) {
    zCPar_Symbol* symbol = GetLocalSymbol( word );
    if( symbol )
      return symbol;

    return GetSymbol( word );
  }


  zCPar_Symbol* zCParser::GetLocalSymbol( zSTRING& word ) {
    if( word.HasWord( "." ) ) {
      int index = FindInstanceVar( word );
      if( index != Invalid )
        return GetSymbol( index );
    }

    if( in_class ) {
      zSTRING symName = in_class->name + "." + word;
      zCPar_Symbol* sym = GetSymbol( symName );
      if( sym )
        return sym;
    }
    
    if( in_func ) {
      zSTRING symName = in_func->name + "." + word;
      zCPar_Symbol* sym = GetSymbol( symName );
      if( sym )
        return sym;
    }

    return Null;
  }


  void zCParser::AddNamespace( zSTRING& word, bool newSymbol ) {
    if( !IsWord( word ) )
      return;

    // Is a class var
    if( word.HasWord( "." ) ) {
      zSTRING instanceName = word.GetWord( ".", 1 );
      zSTRING propertyName = word.GetWord( ".", 2 );
      AddNamespace( instanceName );
      word = instanceName + "." + propertyName;
      return;
    }

    // At first check primary source of this symbol.
    // Colon char at 0 index of the word defines
    // a global source of this symbol.
    uint levelUp = 0;
    for( int i = 0; i < word.Length(); i++ ) {
      if( word[i] != ':' )
        break;
       
      levelUp++;
    }

    word.Cut( 0, levelUp );
    

    // Check this symbol in external func list.
    // If this simbol is external - it should
    // be loaded automatically.
    if( DynamicLoadExternal( word ) )
      return;

    // Define new symbol
    if( newSymbol ) {
      word = GetNamespacePrefix( levelUp ) + word;
      // cmd << "  " << word << endl;
      return;
    }
    
    // Check local variable
    if( GetLocalSymbol( word ) )
      return;

    // Check upper namespaces
    int startLevel = levelUp;
    int endLevel   = GetNamespaceLevel();
    for( int i = startLevel; i <= endLevel; i++ ) {
      zSTRING namespacePrefix = GetNamespacePrefix( i );
      zSTRING fullSymName = namespacePrefix + word;
      if( GetSymbol( fullSymName ) ) {
        word = fullSymName;
        // cmd << "  " << word << endl;
        return;
      }
    }
    
    // Check using namespaces
    if( !GetSymbol( word ) ) {
      for( uint i = 0; i < UsingNamespaces.GetNum(); i++ ) {
        zSTRING fullSymName = Z UsingNamespaces[i] + ":" + word;
        if( GetSymbol( fullSymName ) ) {
          word = fullSymName;
          // cmd << "  " << word << endl;
          return;
        }
      }
    }
  }



  void zCParser::DeclareNamespaceForNextWord( const uint& wordID, bool newSymbol ) {
    if( !NamespaceIsActive() )
      return;

    AddNamespaceAfter = wordID;
    NewSymbolExpected = newSymbol;
  }


  void zCParser::DeclareNamespaceForNextParenthesis() {
    if( !NamespaceIsActive() )
      return;

    NewParenthesisExpected = true;
  }


  bool zCParser::NamespaceIsActive() {
    return
      Namespaces.GetNum() > 0 ||
      zParserExtender.GetDefaultNamespace().Length() > 0;
  }


  zSTRING zCParser::GetNamespacePrefix( const uint& levelUp ) {
    zSTRING defaultNamespace = zParserExtender.GetDefaultNamespace();
    uint spacesNum           = Namespaces.GetNum();
    uint spacesNumTotal      = spacesNum;

    if( defaultNamespace.Length() ) {
      defaultNamespace += ":";
      spacesNumTotal++;
    }

    if( spacesNumTotal <= levelUp )
      return "";

    zSTRING namespaceName;
    spacesNum -= levelUp;
    for( uint i = 0; i < spacesNum; i++ )
      namespaceName += Z Namespaces[i].Name + ":";

    return defaultNamespace + namespaceName;
  }


  int zCParser::GetNamespaceLevel() {
    return (int)Namespaces.GetNum();
  }


  HOOK Hook_zCParser_Parse_Expression_Primary PATCH( &zCParser::Parse_Expression_Primary, &zCParser::Parse_Expression_Primary_Union );

  zCPar_TreeNode* zCParser::Parse_Expression_Primary_Union( int& token ) {
    AddNamespace( aword );
    return THISCALL( Hook_zCParser_Parse_Expression_Primary )(token);
  }


  HOOK Hook_zCParser_CreateStringLeaf PATCH( &zCParser::CreateStringLeaf, &zCParser::CreateStringLeaf_Union );

  zCPar_TreeNode* zCParser::CreateStringLeaf_Union() {
    DeclareNamespaceForNextWord( 1 );
    DeclareNamespaceForNextParenthesis();
    return THISCALL( Hook_zCParser_CreateStringLeaf )();
  }


  HOOK Hook_zCParser_CreateFloatLeaf PATCH( &zCParser::CreateFloatLeaf, &zCParser::CreateFloatLeaf_Union );

  zCPar_TreeNode* zCParser::CreateFloatLeaf_Union() {
    DeclareNamespaceForNextWord( 1 );
    return THISCALL( Hook_zCParser_CreateFloatLeaf )();
  }


  HOOK Hook_zCParser_DeclareVar PATCH( &zCParser::DeclareVar, &zCParser::DeclareVar_Union );

  void zCParser::DeclareVar_Union( int isConstant ) {
    if( !in_func && !in_class )
      DeclareNamespaceForNextWord( 2, true );
    
    NewTypedDeclarations = 1;
    THISCALL( Hook_zCParser_DeclareVar )(isConstant);
  }


  HOOK Hook_zCParser_DeclareClass PATCH( &zCParser::DeclareClass, &zCParser::DeclareClass_Union );

  void zCParser::DeclareClass_Union() {
    DeclareNamespaceForNextWord( 1, true );
    return THISCALL( Hook_zCParser_DeclareClass )();
  }


  HOOK Hook_zCParser_DeclarePrototype PATCH( &zCParser::DeclarePrototype, &zCParser::DeclarePrototype_Union );

  void zCParser::DeclarePrototype_Union() {
    DeclareNamespaceForNextWord( 1, true );
    return THISCALL( Hook_zCParser_DeclarePrototype )();
  }


  HOOK Hook_zCParser_DeclareAssignFunc PATCH( &zCParser::DeclareAssignFunc, &zCParser::DeclareAssignFunc_Union );

  void zCParser::DeclareAssignFunc_Union( zSTRING& name ) {
    // DeclareNamespaceForNextWord( 2, false );
    DeclareNamespaceForNextWord( 2, true );
    // cmd << name << endl;
    return THISCALL( Hook_zCParser_DeclareAssignFunc )(name);
  }


  void zCParser::CheckTreeNodeName( zCPar_TreeNode* node ) {
    int index = FindIndex( node->name );
    if( index != Invalid )
      return;

    zSTRING nameSpace = node->name.GetPattern( "", ":", -1 );
    if( nameSpace.IsEmpty() )
      return;

    zSTRING name = node->name.GetWord( ":", -1 );
    while( true ) {
      nameSpace = nameSpace.GetPattern( "", ":", -1 );
      if( nameSpace.IsEmpty() )
        break;

      zSTRING newSymName = nameSpace + ":" + name;
      index = FindIndex( newSymName );
      if( index != Invalid ) {
        node->name = newSymName;
        return;
      }
    }

    if( FindIndex( name ) != Invalid ) {
      node->name = name;
      return;
    }
  }


  HOOK Hook_zCParser_PushOnStack PATCH( &zCParser::PushOnStack, &zCParser::PushOnStack_Union );

  zCPar_TreeNode* zCParser::PushOnStack_Union( zCPar_TreeNode* node ) {
    if( node->token == zPAR_TOK_PUSHINDEX && zParserExtender.ExtendedParsingEnabled() )
      CheckTreeNodeName( node );

    return THISCALL( Hook_zCParser_PushOnStack )(node);
  }
}