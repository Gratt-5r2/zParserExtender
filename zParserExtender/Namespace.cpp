// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  struct NamespaceInfo {
    string Name;
    uint Entries;
    NamespaceInfo( const string& name ) : Entries( 1 ) {
      Name = name;
    }
  };

  Array<NamespaceInfo> Namespaces;
  static char* prevNamespacePos = (char*)Invalid;



  void zCParser::InitializeNamespace( const zSTRING& defaultNamespace ) {
    Namespaces.Clear();
  }



  void zCParser::NamespaceBegin( zSTRING& word ) {
    ReadWord( word );
    string namespaceName = word;
    ReadWord( word );
    ReadWord( word );

    Namespaces.Create( namespaceName );
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
      ReadWord( word );
      ReadWord( word );
    }
  }



  static uint AddNamespaceAfter = 0;
  static bool NewSymbolExpected = false;

  inline bool IsWord( const zSTRING& word ) {
    if( word.Length() == 0 || word == "CONST" ) // For example CONST leaf is "this pattern"
      return false;

    return word[0u] == ':' || word[0u] == '_' || (word[0u] >= 'A' && word[0u] <= 'Z');
  }

  void zCParser::CheckNamespace( zSTRING& word ) {
         if( word == "NAMESPACE" ) NamespaceBegin( word );
    else if( word == "{" )         NamespaceAddEntry( word );
    else if( word == "}" )         NamespaceRemoveEntry( word );

    if( !NamespaceIsActive() )
      return;

    if( AddNamespaceAfter && --AddNamespaceAfter == 0 ) {
      AddNamespace( word, NewSymbolExpected );
      NewSymbolExpected = false;
    }
  }


  zCPar_Symbol* zCParser::GetLocalSymbol( zSTRING& word ) {
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

    return GetSymbol( word );
  }



  void zCParser::AddNamespace( zSTRING& word, bool newSymbol ) {
    if( !IsWord( word ) )
      return;

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


    // If this symbol is not global, try
    // to find symbol in the current namespace.
    zSTRING fullSymName = GetNamespacePrefix( levelUp ) + word;
    if( newSymbol || GetSymbol( fullSymName ) || !GetLocalSymbol( word ) )
      word = fullSymName;
  }



  void zCParser::DeclareNamespaceForNextWord( const uint& wordID, bool newSymbol ) {
    AddNamespaceAfter = wordID;
    NewSymbolExpected = newSymbol;
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









  HOOK Hook_zCParser_Parse_Expression_Primary PATCH( &zCParser::Parse_Expression_Primary, &zCParser::Parse_Expression_Primary_Union );

  zCPar_TreeNode* zCParser::Parse_Expression_Primary_Union( int& token ) {
    AddNamespace( aword );
    return THISCALL( Hook_zCParser_Parse_Expression_Primary )(token);
  }


  HOOK Hook_zCParser_CreateStringLeaf PATCH( &zCParser::CreateStringLeaf, &zCParser::CreateStringLeaf_Union );

  zCPar_TreeNode* zCParser::CreateStringLeaf_Union() {
    DeclareNamespaceForNextWord( 1 );
    zCPar_TreeNode* node = THISCALL( Hook_zCParser_CreateStringLeaf )();
    DeclareNamespaceForNextWord( 0 );
    return node;
  }


  HOOK Hook_zCParser_CreateFloatLeaf PATCH( &zCParser::CreateFloatLeaf, &zCParser::CreateFloatLeaf_Union );

  zCPar_TreeNode* zCParser::CreateFloatLeaf_Union() {
    DeclareNamespaceForNextWord( 1 );
    zCPar_TreeNode* node = THISCALL( Hook_zCParser_CreateFloatLeaf )();
    DeclareNamespaceForNextWord( 0 );
    return node;
  }


  HOOK Hook_zCParser_DeclareVar PATCH( &zCParser::DeclareVar, &zCParser::DeclareVar_Union );

  void zCParser::DeclareVar_Union( int isConstant ) {
    if( !in_func && !in_class )
      DeclareNamespaceForNextWord( 2, true );

    return THISCALL( Hook_zCParser_DeclareVar )(isConstant);
  }


  HOOK Hook_zCParser_DeclareClass PATCH( &zCParser::DeclareClass, &zCParser::DeclareClass_Union );

  void zCParser::DeclareClass_Union() {
    DeclareNamespaceForNextWord( 3, true );
    return THISCALL( Hook_zCParser_DeclareClass )();
  }


  HOOK Hook_zCParser_DeclarePrototype PATCH( &zCParser::DeclarePrototype, &zCParser::DeclarePrototype_Union );

  void zCParser::DeclarePrototype_Union() {
    DeclareNamespaceForNextWord( 3, true );
    return THISCALL( Hook_zCParser_DeclarePrototype )();
  }
}