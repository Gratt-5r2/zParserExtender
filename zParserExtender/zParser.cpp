// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static bool NeedToReparse( string s ) {
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
    return s + ".DAT";
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



  HOOK Ivk_zCParser_SaveDat AS( &zCParser::SaveDat, &zCParser::SaveDat_Union );

  int zCParser::SaveDat_Union( zSTRING& name ) {
    PostCompileCallReplace();

    if( zParserExtender.CompileDatEnabled() ) {
      // Modified save process
      if( zParserExtender.ExtendedParsingEnabled() )
        return SaveDatCopy();
      
      // Original save process
      int Ok = THISCALL( Ivk_zCParser_SaveDat )( name );

      // Compile new OU library
      if( this == parser && NeedToReparseOU() ) {
        ogame->GetCutsceneManager()->LibSortion();
        ogame->GetCutsceneManager()->LibStore( zLIB_STORE_ASCII | zLIB_STORE_BIN );
        ogame->GetCutsceneManager()->LibLoad( zLIB_STORE_BIN );
      }

      // Reload a symbol table after clear compilation
      // for safely symbols loading
      if( !zParserExtender.ExternalScriptsListIsEmpty() )
        LoadDat( name );

      return Ok;
    }

    return False;
  }



  int zCParser::SaveDatCopy() {
    zSTRING datName   = GetDATNameByParser( this );
    zSTRING clearName = datName.GetPattern( "", ".", -1 );
    zSTRING extension = datName.GetWord( ".", -1 );
            datName   = clearName + ".EDITED." + extension;

    int Ok = THISCALL( Ivk_zCParser_SaveDat )( datName );

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

  HOOK Ivk_zCParser_ParseBlock AS( &zCParser::ParseBlock, &zCParser::ParseBlock_Union );

  void zCParser::ParseBlock_Union() {
    if( !zParserExtender.ExtendedParsingEnabled() )
      zCParser::ParseBlock_OU_Union();

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
        DeclareAssign( word );

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
      ParseBlock();
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



  HOOK Ivk_zCParser_Parse AS( &zCParser::Parse, &zCParser::Parse_Union );

  int zCParser::Parse_Union( zSTRING s ) {
    int enableParsing_tmp = enableParsing;
    enableParsing = enableParsing_tmp || NeedToReparse( s ) ? True : False;

    if( enableParsing ) {
      cmd << colParse2 << "zParserExtender: "     <<
             colParse1 << "building "             <<
             colParse2 << GetParserNameByDAT( s ) <<
             colParse1 << " parser"               <<
             colParse3 << endl;
    }

    int Ok = THISCALL( Ivk_zCParser_Parse )(s);

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



#if ENGINE >= Engine_G2
  HOOK Ivk_zCParser_LoadGlobalVars AS( &zCParser::LoadGlobalVars, &zCParser::LoadGlobalVars_Union );

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
}