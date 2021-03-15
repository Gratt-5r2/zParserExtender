// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
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
    if( zParserExtender.GetParser()->enableParsing == NinjaParseID )
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
    if( zParserExtender.GetParser()->enableParsing == NinjaParseID )
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


  HOOK Ivk_zCParser_ReadWord AS( &zCParser::ReadWord, &zCParser::ReadWord_Union );

  void zCParser::ReadWord_Union( zSTRING& word ) {
    THISCALL( Ivk_zCParser_ReadWord )(word);
    if( prevword_index[prevword_nr] == pc_start && word == "META" ) {
      SkipMeta_Union();
      THISCALL( Ivk_zCParser_ReadWord )(word);
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
      Error( Z "found empty break operator!", 0 );

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
      Error( Z "found empty continue operator!", 0 );

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
      ReadWord( aword );
    } while( !aword.IsEmpty() && aword != "}" );
    Match( Z ";" );
  }



  void zCParser::CallGameInit_Union() {
    static int index = GetIndex( "GAMEINIT" );
    if( index == Invalid )
      return;

    CallFunc( index );
  }



  void zCParser::CallGameLoop_Union() {
    static int index = GetIndex( "GAMELOOP" );
    if( index == Invalid )
      return;

    CallFunc( index );
  }



  void zCParser::ResetAdditionalInfo() {
    add_funclist = Null;
  }



  HOOK Hook_zCParser_Parse PATCH( &zCParser::Parse, &zCParser::Parse_Union );

  int zCParser::Parse_Union( zSTRING s ) {
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
      cmd << colParse2 << "zParserExtender: "      <<
              colParse1 << "building "             <<
              colParse2 << GetParserNameByDAT( s ) <<
              colParse1 << " parser"               <<
              colParse3 << endl;
    }

    int Ok = THISCALL( Hook_zCParser_Parse )(s);

    enableParsing = enableParsing_tmp;
    return Ok;
  }



  HOOK Ivk_zCOption_Parm AS( &zCOption::Parm, &zCOption::Parm_Union );

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
    
    cmd << classObject->name << endl;
    return classObject;
  }

  zCPar_Symbol* zCParser::GetNearestVariable( const zSTRING& varName ) {
    zCPar_Symbol* symbol = Null;
    if( in_func )
      symbol = parser->GetSymbol( in_func->name + "." + varName );

    if( !symbol )
      symbol = parser->GetSymbol( varName );

    return symbol;
  }

  bool WordIsFloat( const string& word ) {
    return word.IsNumber() && word.Search( "." ) != Invalid;
  }

  bool WordIsInteger( const string& word ) {
    return word.IsNumber() && !WordIsFloat( word );
  }

  void zCParser::DeclareFuncCall_Union( zSTRING& name, int typematch ) {
    string functionName = in_func->name;
    if( name != "STR_FORMAT" )
      return THISCALL( Hook_zCParser_DeclareFuncCall )(name, typematch);

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
      zCPar_Symbol* sym = GetNearestVariable( word );
      cmd << word << "  " << AHEX32( sym ) << endl;
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
    treenode->name = "STR_FORMAT";
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
}