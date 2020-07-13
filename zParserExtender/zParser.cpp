// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Ivk_zCParser_SaveDat        AS( &zCParser::SaveDat,        &zCParser::SaveDat_Union );
  HOOK Ivk_zCParser_ParseBlock     AS( &zCParser::ParseBlock,     &zCParser::ParseBlock_Union );
  HOOK Ivk_zCParser_ReadWord       AS( &zCParser::ReadWord,       &zCParser::ReadWord_Union );
  HOOK Ivk_zCParser_LoadGlobalVars AS( &zCParser::LoadGlobalVars, &zCParser::LoadGlobalVars_Union );





  int zCParser::SaveDat_Union( zSTRING& name ) {
    PostCompileCallReplace();

    if( zParserExtender.CompileDatEnabled() ) {
      if( zParserExtender.ExtendedParsingEnabled() ) {
        zSTRING clearName = name.GetPattern("", ".", -1);
        zSTRING extension = name.GetWord( ".", -1 );
        name = clearName + ".EDITED." + extension;
      }
      
      return THISCALL( Ivk_zCParser_SaveDat )( name );
    }
  }





  void zCParser::SetEnableParsing_Union( bool_t enabled ) {
    mergemode = enabled && zParserExtender.MergeModeEnabled();
    compiled = !enabled;
  }





  static int op_loop_level       = 0;
  static int op_while_enumerator = 0x10000;

#define WHILE_BEGIN op_loop_level++, op_while_enumerator += 2
#define WHILE_END   op_loop_level--

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





  void zCParser::Reset_Union() {
    add_funclist = Null;
  }




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

      for( uint j = 0; j < arrayNum; j++ ) {

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
}