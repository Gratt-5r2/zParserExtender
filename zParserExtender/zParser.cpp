// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  bool zCParser::OverrideNextSymbol = false;

  static bool NeedToReparse( string s ) {
    //if( !zParserExtender.ExtendedParsingEnabled() )
    //  return false;

    string parName = s.GetPattern( "\\", "." );
    if( parName == "GOTHIC"     && zoptions->Parm( "ZREPARSE_GAME"   ) ) return true;
    if( parName == "SFX"        && zoptions->Parm( "ZREPARSE_SFX"    ) ) return true;
    if( parName == "PARTICLEFX" && zoptions->Parm( "ZREPARSE_PFX"    ) ) return true;
    if( parName == "VISUALFX"   && zoptions->Parm( "ZREPARSE_VFX"    ) ) return true;
    if( parName == "CAMERA"     && zoptions->Parm( "ZREPARSE_CAMERA" ) ) return true;
    if( parName == "MENU"       && zoptions->Parm( "ZREPARSE_MENU"   ) ) return true;
    if( parName == "MUSIC"      && zoptions->Parm( "ZREPARSE_MUSIC"  ) ) return true;
    if( parName == "FIGHT"      && zoptions->Parm( "ZREPARSE_FIGHT"  ) ) return true;
    return false;
  }


  static string GetParserNameByDAT( string s ) {
    string parName = s.GetPattern( "\\", "." );
    if( parName == "GOTHIC"     ) return "Game";
    if( parName == "SFX"        ) return "SFX";
    if( parName == "PARTICLEFX" ) return "PFX";
    if( parName == "VISUALFX"   ) return "VFX";
    if( parName == "CAMERA"     ) return "Camera";
    if( parName == "MENU"       ) return "Menu";
    if( parName == "MUSIC"      ) return "Music";
    if( parName == "FIGHT"      ) return "Fight";
    return s;
  }


  static string GetDATNameByParser( zCParser* par ) {
    if( par == Gothic::Parsers::Game   ) return "GOTHIC.DAT";
    if( par == Gothic::Parsers::SFX    ) return "SFX.DAT";
    if( par == Gothic::Parsers::PFX    ) return "PARTICLEFX.DAT";
    if( par == Gothic::Parsers::VFX    ) return "VISUALFX.DAT";
    if( par == Gothic::Parsers::Camera ) return "CAMERA.DAT";
    if( par == Gothic::Parsers::Menu   ) return "MENU.DAT";
    if( par == Gothic::Parsers::Music  ) return "MUSIC.DAT";
    return "UNKNOWN.DAT";
  }


  static bool NeedToReparseOU() {
    return
       zoptions->Parm( "ZREPARSE" ) ||
      (zoptions->Parm( "ZREPARSE_GAME" ) && zoptions->Parm( "ZREPARSE_OU" ));
  }


  HOOK Hook_zCParser_SaveDat PATCH( &zCParser::SaveDat, &zCParser::SaveDat_Union );

  int zCParser::SaveDat_Union( zSTRING& name ) {
    if( zParserExtender.GetParseID() == NinjaParseID )
      return THISCALL( Hook_zCParser_SaveDat )(name);

    // Apply all hooks
    if( zParserExtender.ExtendedParsingEnabled() ) {
      PostCompileCallReplace();

      // Save edited DAT
      if( zParserExtender.CompileDatEnabled() )
        SaveDatCopy();

      // Save edited OU
      if( this == parser && zParserExtender.CompileOUEnabled() ) {
        ogame->GetCutsceneManager()->LibSortion();
        ogame->GetCutsceneManager()->LibStoreCopy( zLIB_STORE_ASCII | zLIB_STORE_BIN );
      }

      return True;
    }

    int Ok = THISCALL( Hook_zCParser_SaveDat )(name);

    if( this == parser && NeedToReparseOU() ) {
      ogame->GetCutsceneManager()->LibSortion();
      ogame->GetCutsceneManager()->LibStore( zLIB_STORE_ASCII | zLIB_STORE_BIN );
      ogame->GetCutsceneManager()->LibLoad( zLIB_STORE_BIN ); // Force update library
    }

    return Ok;
  }


  int zCParser::SaveDatCopy() {
    zSTRING datName   = GetDATNameByParser( this );
    zSTRING clearName = datName.GetPattern( "", ".", -1 );
    zSTRING extension = datName.GetWord( ".", -1 );
            datName   = clearName + ".EDITED." + extension;

    int Ok = THISCALL( Hook_zCParser_SaveDat )( datName );

    if( zCParserExtender::MessagesLevel >= 2 )
      cmd << colParse2 << "zParserExtender: " <<
             colParse1 << "file "             <<
             colParse2 << datName             <<
             colParse1 << " saved"            <<
             colParse3 << endl;

    return Ok;
  }


  void zCParser::SetEnableParsing_Union( bool_t enabled ) {
    mergemode = enabled && zParserExtender.MergeModeEnabled();
    compiled = !enabled;
  }


  static int op_loop_level       = 0;
  static int op_while_enumerator = 0x10000;

#define WHILE_BEGIN op_loop_level++, op_while_enumerator += 2
#define WHILE_END   op_loop_level--

  HOOK Hook_zCParser_ParseBlock PATCH( &zCParser::ParseBlock, &zCParser::ParseBlock_Union );

  void zCParser::ParseBlock_Union() {
    if( zParserExtender.GetParseID() == NinjaParseID )
      return THISCALL( Hook_zCParser_ParseBlock )();

    if( !zParserExtender.ExtendedParsingEnabled() )
      ParseBlock_OU_Union();

    bool whileEnabled = zParserExtender.NativeWhileEnabled();

    zSTRING word;
    Match( Z "{" );
    while( pc < pc_stop ) {
      ReadWord( word );
      if( word == "VAR" )
        DeclareVar( False );
      else if( word == "CONST" )
        DeclareVar( True );
      else if( word == "RETURN" )
        DeclareReturn();
      else if( word == "IF" )
        DeclareIf();
      else if( zParserExtender.ExtendedParsingEnabled() && word == "TEST" )
        DeclareTest();
      else if( whileEnabled && word == "WHILE" )
        DeclareWhile_Union();
      else if( whileEnabled && word == "BREAK" )
        DeclareBreak_Union();
      else if( whileEnabled && word == "CONTINUE" )
        DeclareContinue_Union();
      else if( word == "}" )
        return;
      else
        DeclareAssign_Union( word );

      Match( Z ";" );
    }
  }



  void zCParser::ParseOperatorLine() {
    if( !zParserExtender.ExtendedParsingEnabled() )
      ParseBlock_OU_Union();

    bool whileEnabled = zParserExtender.NativeWhileEnabled();

    zSTRING word;
    while( pc < pc_stop ) {
      ReadWord_Union( word );
      if( word == "VAR" )
        DeclareVar( False );
      else if( word == "CONST" )
        DeclareVar( True );
      else if( word == "RETURN" )
        DeclareReturn();
      else if( word == "IF" )
        DeclareIf();
      else if( word == "TEST" )
        DeclareTest();
      else if( whileEnabled && word == "WHILE" )
        DeclareWhile_Union();
      else if( whileEnabled && word == "BREAK" )
        DeclareBreak_Union();
      else if( whileEnabled && word == "CONTINUE" )
        DeclareContinue_Union();
      else if( word == "FUNC" )
        DeclareFunc();
      else if( word == "CLASS" )
        DeclareClass();
      else if( word == "INSTANCE" )
        DeclareInstance();
      else if( word == "PROTOTYPE" )
        DeclarePrototype();
      else if( word == ";" )
        return;
      else
        DeclareAssign_Union( word );
    }
  }



  static auto patch_ParseFile = CreateSharedPatchSym<CPatchInteger>( "ParseFileWord", (int)TInstance( &zCParser::ParseFileWord ).data );
  int zCParser::ParseFileWord( const zSTRING& word ) {
    if( word == "TEST" ) {
      DeclareTest();
      return True;
    }
    else if( word == "EXTERN" ) {
      DeclareExtern();
      return True;
    }

    return False;
  }


  bool zCParser::ParseBlockOrOperatorLine() {
    zSTRING word;
    ReadWordBase( word );
    if( word == "{" ) {
      int entries = 1;
      while( true ) {
        ParseOperatorLine();
        ReadWordBase( word );
        if( word == "}" ) {
          if( --entries <= 0 )
            break;
        }
        else if( word.IsEmpty() ) {
          Error();
          return false;
        }
        else
          PrevWord();
      }

      return false;
    }
    else {
      PrevWord();
      ParseOperatorLine();
      return true;
    }
  }


  HOOK Ivk_zCParser_ReadWord PATCH( &zCParser::ReadWord, &zCParser::ReadWord_Union );

  void zCParser::ReadWord_Union( zSTRING& word ) {
    int _prevword_nr = (prevword_nr + 1) & 15;
    ReadWordBase(word);
    prevword_nr = _prevword_nr;

    if( prevword_index[prevword_nr] == pc_start && word == "META" ) {
      SkipMeta_Union();
      ReadWordBase( word );
    }

    // TODO
    // if( word == "ONCE" ) {
    //   OverrideNextSymbol = true;
    //   return THISCALL( Ivk_zCParser_ReadWord )(word);
    // }

    CheckNamespace( word );
    ReadMacro( word );
  }


  void zCParser::ReadWordBase( zSTRING& word ) {
    THISCALL( Ivk_zCParser_ReadWord )(word);
  }


  void zCParser::DeclareTest() {
    zSTRING word;
    bool test = TestSymbolExpresstion();

    if( test ) {
      bool opline = ParseBlockOrOperatorLine();
      ReadWordBase( word );
      if( word == "ELSE" ) {
        if( SkipBlock() )
          PrevWord();
      }
      else {
        PrevWord();
        if( opline )
          PrevWord();
      }
    }
    else {
      bool opline = SkipBlock();
      ReadWordBase( word );
      if( word == "ELSE" ) {
        bool opline = ParseBlockOrOperatorLine();
        if( opline )
          PrevWord();
      }
      else {
        PrevWord();
        if( opline )
          PrevWord();
      }
    }
  }

  void zCParser::DeclareExtern() {
    zSTRING word;
    ReadWordBase( word );
    if( word == "CONST" || word == "VAR" || word == "FUNC" ) {
      ReadWordBase( word );
      ReadWord( word );
      PrevWord();
      PrevWord();
    }
    else {
      ReadWord( word );
      PrevWord();
    }

    PrevWord();

    int index = symtab.GetIndex_Safe( word );
    if( index != Invalid ) {
      SkipOperatorsLine();
      PrevWord();
    }
    else {
      ParseOperatorLine();
      PrevWord();
    }
  }

  bool CheckSteamEnabled() {
    return
      GetModuleHandle( "GameOverlayRenderer.dll" ) ||
      GetModuleHandle( "GameOverlayRenderer.dll.tmp" );
  }


  bool zCParser::TestSymbol() {
    zSTRING word;
    ReadWordBase( word );
    if( word == "!" ) {
      return !TestSymbol();
    }
    else if( word == "(" ) {
      int ok = TestSymbolExpresstion();
      Match( Z ")" );
      return ok;
    }

         if( word == "STEAM" && CheckSteamEnabled() ) return true;
    else if( word == "G1"  && ENGINE == Engine_G1   ) return true;
    else if( word == "G1A" && ENGINE == Engine_G1A  ) return true;
    else if( word == "G2"  && ENGINE == Engine_G2   ) return true;
    else if( word == "G2A" && ENGINE == Engine_G2A  ) return true;
    else                                              return symtab.GetIndex_Safe( word.Upper() ) != Invalid;
    return false;
  }

  inline zSTRING GetNextTestOperator( zCParser* par ) {
    zSTRING word;
    par->ReadWordBase(word);
    if( word == "&" ) {
      par->ReadWordBase(word);
      if( word == "&" )
        return "&&";
      else {
        par->PrevWord();
        return "&";
      }
    }
    else if( word == "|" ) {
      par->ReadWordBase(word);
      if( word == "|" )
        return "||";
      else {
        par->PrevWord();
        return "|";
      }
    }
    else
      return word;
  }

  bool zCParser::TestSymbolExpresstion() {
    bool ok = TestSymbol();

    zSTRING word;
    while( true )
    {
      word = GetNextTestOperator( this );
      if( word == "!" ) {
        ok = !TestSymbol();
      }
      else if( word == "&&" ) {
        ok = TestSymbol() && ok;
      }
      else if( word == "||" ) {
        ok = TestSymbol() || ok;
      }
      else if( word == ")" ) {
        PrevWord();
        return ok;
      }
      else {
        PrevWord();
        break;
      }
    }

    return ok;
  }

  void zCParser::SkipOperatorsLine() {
    zSTRING word;
    int entries = 0;
    while( true ) {
      ReadWordBase( word );
      if( word == "\"" ) {
        PrevWord();
        SkipString();
      }
      else if( word == "{" ) {
        entries++;
      }
      else if( word == "}" ) {
        entries--;
      }
      else if( entries <= 0 && word == ";" )
        break;
    }
  }

  bool zCParser::SkipBlock() {
    zSTRING word;
    ReadWordBase( word );
    if( word != "{" ) {
      PrevWord();
      SkipOperatorsLine();
      return true;
    }

    int entries = 1;
    while( true ) {
      ReadWordBase( word );
      if( word == "{" )
        entries++;
      else if( word == "}" ) {
        if( --entries <= 0 )
          break;
      }
      else if( word == "\"" ) {
        PrevWord();
        SkipString();
      }
      else if( word.IsEmpty() )
        Error();
    }

    return false;
  }

  void zCParser::SkipString() {
    Match( Z "\"" );
    FindNext( "\"" );
    pc++;
  }


  void zCParser::ReadMacro( zSTRING& word ) {
    string& adigit = (string&)aword;
    if( adigit.StartWith( "KEY_" ) || adigit.StartWith( "MOUSE_" ) ) {
      int value = zInput_GetKeyIndex( adigit );
      if( value != Invalid )
        aword = value;
    }
    else if( adigit.StartWith( "GAME_" ) ) {
      int value = zInput_GetLogicalKeyIndex( adigit );
      if( value != Invalid )
        aword = value;
    }
  }


  void zCParser::DeclareWhile_Union() {
    WHILE_BEGIN;

    int op_while_index_begin = op_while_enumerator + 1;
    int op_while_index_end   = op_while_enumerator;

    do {
      // create label of the 'begin' of loop
      treenode = CreateLeaf( zPAR_TOK_LABEL, treenode );
      treenode->value = op_while_index_begin;

      // create expression of the condition
      treenode->SetNext( ParseExpression() );
      while( treenode->next )
        treenode = treenode->next;

      // break from the loop if the expression is 'false'
      treenode = CreateLeaf( zPAR_TOK_JUMPF, treenode );
      treenode->value = op_while_index_end;

      // parse body of the loop
      PrevWord();
      ParseBlock_Union();
      ReadWord( aword );

    } while( aword != ";" );

    // return to the 'begin' of loop
    treenode = CreateLeaf( zPAR_TOK_JUMP, treenode );
    treenode->value = op_while_index_begin;

    // create 'end' label of the loop
    treenode = CreateLeaf( zPAR_TOK_LABEL, treenode );
    treenode->value = op_while_index_end;

    PrevWord();

    WHILE_END;
  }


  void zCParser::DeclareBreak_Union() {
    if( op_loop_level <= 0 )
      Error( Z "Found empty break operator!", 0 );

    do {
      // break from top loop
      treenode = CreateLeaf( zPAR_TOK_JUMP, treenode );
      treenode->value = op_while_enumerator;

      ReadWord( aword );
    } while( aword != ";" );

    PrevWord();
  }


  void zCParser::DeclareContinue_Union() {
    if( op_loop_level <= 0 )
      Error( Z "Found empty continue operator!", 0 );

    do {
      // break from top loop
      treenode = CreateLeaf( zPAR_TOK_JUMP, treenode );
      treenode->value = op_while_enumerator + 1;

      ReadWord( aword );
    } while( aword != ";" );

    PrevWord();
  }


  void zCParser::SkipMeta_Union() {
    do {
      THISCALL( Ivk_zCParser_ReadWord )(aword);
    } while( !aword.IsEmpty() && aword != "}" );
    Match( Z ";" );
  }


  void zCParser::CallGameInit_Union() {
    static zSTRING eventName = "GAMEINIT";
    DoEvent( eventName );
  }


  void zCParser::DoEvent( const zSTRING& eventName ) {
    const Array<int>& indexes = zTEventFuncCollection::GetCollection( this ).GetIndexes( eventName );
    for( uint i = 0; i < indexes.GetNum(); i++ )
      CallFunc( indexes[i] );
  }


  void zCParser::CallGameLoop_Union() {
    static zSTRING eventName = "GAMELOOP";
    DoEvent( eventName );
  }


  void zCParser::ResetAdditionalInfo() {
    add_funclist = Null;
  }


  HOOK Hook_zCParser_Parse PATCH( &zCParser::Parse, &zCParser::Parse_Union );

  int zCParser::Parse_Union( zSTRING s ) {
    cur_parser = this;
    bool needToReparce = NeedToReparse( s ) && enableParsing != NinjaParseID;
    if( !zParserExtender.ExtendedParsingEnabled() && !needToReparce )
      return THISCALL( Hook_zCParser_Parse )(s);

    int enableParsing_tmp = enableParsing;
    enableParsing = enableParsing_tmp || needToReparce ? PluginParseID : False;

    if( zCParserExtender::MessagesLevel >= 2 ) {
      cmd << s                          << endl;
      cmd << "enableParsing: "          << enableParsing << endl;
      cmd << "ExtendedParsingEnabled: " << zParserExtender.ExtendedParsingEnabled() << endl;
      cmd << "NeedToReparse: "          << needToReparce << endl;
    }

    if( enableParsing && zCParserExtender::MessagesLevel >= 1 ) {
      cmd << colParse2 << "zParserExtender: "     <<
             colParse1 << "building "             <<
             colParse2 << GetParserNameByDAT( s ) <<
             colParse1 << " parser"               <<
             colParse3 << endl;
    }

    int Ok = THISCALL( Hook_zCParser_Parse )(s);

    enableParsing = enableParsing_tmp;
    return Ok;
  }


  HOOK Ivk_zCOption_Parm PATCH( &zCOption::Parm, &zCOption::Parm_Union );

  int zCOption::Parm_Union( const zSTRING& parmname ) {
    string cmd = commandline;
    string par = parmname;

    int index = cmd.Search( parmname );
    if( index != Invalid ) {
      string key = cmd.GetWordSmartEx( 1, true, index );
      if( key == par )
        return True;
    }
    
    return False;
  }


  void zCParser::RenameTreeNode( zCPar_Symbol* sym, zSTRING newName ) {
    for( int i = 0; i < file.GetNumInList(); i++ ) {
      if( file[i]->tree ) {
        zCPar_TreeNode* node = file[i]->tree;

        while( node ) {
          if( node->name.GetWord( "." ) == sym->GetName() )
            node->name.Replace( sym->GetName(), newName );

          node = node->next;
        }
      }
    }
  }


  zCPar_TreeNode* zCParser::CreateLeafCallInstance( const zSTRING& instName, zCPar_TreeNode* leafBase ) {
    if( in_func && in_func->name == instName )
      Error( Z string::Combine( "Instance call recursion (%z)", instName ), 0 );

    zCPar_Symbol* sym = GetSymbol( instName );

    if( sym->type == zPAR_TYPE_INSTANCE ) {
      // Push [this] instance owner
      treenode = CreateLeaf( zPAR_TOK_PUSHINST, treenode );
      treenode->name = in_func->name;
      treenode->info = zPAR_TYPE_INSTANCE;

      // Push [this] instance member
      treenode = CreateLeaf( zPAR_TOK_PUSHINST, treenode );
      treenode->name = instName;
      treenode->info = zPAR_TYPE_INSTANCE;

      // Move [this] from owner to member
      treenode = CreateLeaf( zPAR_TOK_ASSIGNINST, treenode );
    }

    // Call instance or prototype as function
    treenode = CreateLeaf( zPAR_TOK_CALL, treenode );
    treenode->name = instName;
    return treenode;
  }


  HOOK Hook_zCParser_DeclareFuncCall PATCH( &zCParser::DeclareFuncCall, &zCParser::DeclareFuncCall_Union );

  inline zCPar_Symbol* GetClassObject( zCPar_Symbol* object ) {
    zCPar_Symbol* classObject = object;
    while( classObject && classObject->type != zPAR_TYPE_CLASS )
      classObject = classObject->GetParent();
    
    return classObject;
  }


  extern bool ActivateDynamicExternal( const string& funcName );

  bool zCParser::DynamicLoadExternal( const zSTRING& symName, const bool& forceReplace ) {
    if( symName == "" )
      return false;

    zCPar_Symbol* sym = GetSymbol( symName );
    if( sym && !forceReplace )
      return false;

    return ActivateDynamicExternal( symName );
  }


  bool zCParser::IsVaExternal( const zSTRING& name ) {
     zCPar_Symbol* sym = GetSymbol( name );
     if( !sym || !sym->HasFlag( zPAR_FLAG_EXTERNAL ) )
       return false;

     return sym->next && sym->next->type == zPAR_TYPE_VA;
  }


  void zCParser::DeclareFuncCall_Union( zSTRING& name, int typematch ) {
    if( in_func && in_func->name == name ) {
      string message = string::Combine( "Function call recursion (%z)", name );
      zerr->Warning( message );
      cmd << Col16( CMD_YELLOW | CMD_INT ) << message << Col16() << endl;
    }

    string functionName = in_func->name;
    DynamicLoadExternal( name );

    if( IsVaExternal( name ) )
      return DeclareVaFuncCall( name );

    AddNamespace( name );
    THISCALL( Hook_zCParser_DeclareFuncCall )(name, typematch);
  }


#if ENGINE >= Engine_G2
  HOOK Ivk_zCParser_LoadGlobalVars AS_IF( &zCParser::LoadGlobalVars, &zCParser::LoadGlobalVars_Union, !NinjaInjected() );

  inline bool IsValidIntegerSymbol( zCPar_Symbol* sym ) {
    return sym && sym->type == zPAR_TYPE_INT && sym->flags == 0;
  }


  // Parser bugfix
  int zCParser::LoadGlobalVars_Union( zCArchiver& arc ) {
    string symRowName = NullStr;
    int symNum        = 0;
    int arrayNum      = 0;
    int tmpValue      = 0;

    arc.ReadInt( "numSymbols", symNum );
    for( int i = 0; i < symNum; i++ ) {
      symRowName = string::Combine( "symName%i", i );

      zSTRING symName;
      arc.ReadString( symRowName, symName );
      arc.ReadInt( symRowName + "cnt", arrayNum );

      zCPar_Symbol* sym  = GetSymbol( symName );
      bool isValidSymbol = IsValidIntegerSymbol( sym );

      for( int j = 0; j < arrayNum; j++ ) {

        // Read null-size symbol at first!
        // Otherwise the data reading may be incorrect
        if( sym && sym->ele == 0 ) {
          symRowName = "symValue" + A i;
          arc.ReadInt( symRowName.ToChar(), tmpValue );
        }
        else {
          symRowName = string::Combine( "symValue%i_%i", i, j );
          arc.ReadInt( symRowName, tmpValue );

          // Check symbol AFTER data reading!
          // For else the engine will loop (or maybe crash)
          if( isValidSymbol )
            sym->SetValue( tmpValue, j );
        }
      }
    }

    return True;
  }
#endif


  HOOK Hook_zCMenu_Startup PATCH( &zCMenu::Startup, &zCMenu::Startup_Union );

  void zCMenu::Startup_Union() {
    static bool initialized = false;
    if( !initialized ) {
      Hook_zCMenu_Startup();
      initialized = true;
    }
  }


  HOOK Hook_zCParser_ReadFuncType PATCH( &zCParser::ReadFuncType, &zCParser::ReadFuncType_Union );

  static bool s_RenameTreeNode = false;

  int zCParser::ReadFuncType_Union() {
    ReadWord( aword );
    if( aword == "EVENT" ) {
      s_DeclareEvent = true;
      s_RenameTreeNode = true;
      DeclareNamespaceForNextWord( 0, false );
      return zPAR_TYPE_VOID;
    }

    PrevWord();

    THISCALL( Hook_zCParser_ReadFuncType )();
  }


  HOOK Hook_zCParser_DeclareFunc PATCH( &zCParser::DeclareFunc, &zCParser::DeclareFunc_Union );

  extern uint NewTypedDeclarations;

  void zCParser::DeclareFunc_Union() {
    if( !s_RenameTreeNode ) {
      // DeclareNamespaceForNextWord( 3, true ); // FIXME
      DeclareNamespaceForNextWord( 2, true );
    }

    auto treenode_old = treenode;
    NewTypedDeclarations = 2;
    THISCALL( Hook_zCParser_DeclareFunc )();

    if( s_RenameTreeNode ) {
      auto treenode_func = treenode_old->next;
      treenode_func->name = Z string::Combine( "EVENT.%z.%z", treenode_func->name, fname.GetPattern( "\\", "." ) );
      s_RenameTreeNode = false;
    }
  }


  HOOK Hook_zCParser_LoadDat PATCH( &zCParser::LoadDat, &zCParser::LoadDat_Union );

  int zCParser::LoadDat_Union( zSTRING& datName ) {
    cur_parser = this;
    return THISCALL( Hook_zCParser_LoadDat )(datName);
  }


  inline string StringSymbolToString( zCPar_Symbol* sym ) {
    int ele = sym->ele;
    string symName = sym->name;

    if( ele == 1 ) {
      string value = *sym->stringdata;
      return string::Combine( "const string %s = \"%s\";", symName, value );
    }

    string values;
    for( int i = 0; i < ele; i++ ) {
      values += string::Combine( "\n\t\"%z\"", sym->stringdata[i] );
      if( i < ele - 1 )
        values += ",";
    }

    return string::Combine( "const string %s[%i] =\n{%s\n};", symName, ele, values );
  }


  void zCParser::ExportStringList() {
    rowString stringList;
    for( int i = 0; i < symtab.GetNumInList(); i++ ) {
      zCPar_Symbol* sym = symtab.table[i];
      if( sym->type == zPAR_TYPE_STRING )
        if( sym->HasFlag( zPAR_FLAG_CONST ) )
          stringList.InsertLines( StringSymbolToString( sym ) );
    }

    stringList.WriteToFile( "_work\\Data\\Scripts\\Exports\\StringList.d" );
  }


  HOOK Hook_zCParser_ReadString AS_IF( &zCParser::ReadString, &zCParser::ReadString_Union, 0 );

  void zCParser::ReadString_Union( zSTRING& s ) {
    ReadWord( aword );
    zCPar_Symbol* sym = GetSymbol( aword );
    if( sym ) {
      ReadWord( aword );
      if( aword[0] == '[' ) {
        int index = ReadInt();
        Match( Z "]" );
        s = sym->stringdata[index];
      }
      else {
        PrevWord();
        s = sym->stringdata[0];
      }

      return;
    }

    PrevWord();
    THISCALL( Hook_zCParser_ReadString )( s );
  }



  HOOK Hook_zCParser_CreateStringLeaf_Array PATCH( &zCParser::CreateStringLeaf, &zCParser::CreateStringLeafArray );

  zCPar_TreeNode* zCParser::CreateStringLeafArray() {
    auto node = THISCALL( Hook_zCParser_CreateStringLeaf_Array )(); // Call-order bugfix

    if( node->token == zPAR_TOK_VAR ) {
      ReadWord( aword );
      if( aword[0] == '[' ) {
        zCPar_Symbol* sym = GetSymbol( node->name );
        if( sym ) {
          int index = ReadInt();
          Match( Z "]" );

          node->token = zPAR_TOK_NEWSTRING;
          node->name = sym->stringdata[index];
          node->value = 0;
          node->info = zPAR_TYPE_STRING;
          return node;
        }
      }

      PrevWord();
    }

    return node;
  }


  HOOK Hook_zCParser_DeclareReturn PATCH( &zCParser::DeclareReturn, &zCParser::DeclareReturn_Union );

  void zCParser::DeclareReturn_Union() {
    if( !zParserExtender.ExtendedParsingEnabled() || parser->enableParsing == NinjaParseID )
      return THISCALL( Hook_zCParser_DeclareReturn )();

    if( !in_func || !in_func->HasFlag( zPAR_FLAG_RETURN ) )
      return THISCALL( Hook_zCParser_DeclareReturn )();

    if( in_func->GetOffset() == zPAR_TYPE_INSTANCE ) {
      ReadWord( aword );
      treenode = CreateLeaf( zPAR_TOK_INSTANCE, treenode );
      auto symbol = GetLocalSymbol( aword );
      if( symbol ) {
        treenode->name = symbol->name;
        treenode->info = zPAR_TYPE_INSTANCE;
        treenode->token = zPAR_TOK_PUSHINST;
        treenode = CreateLeaf( zPAR_TOK_RET, treenode );
        return;
      }
    }

    THISCALL( Hook_zCParser_DeclareReturn )();
  }
}