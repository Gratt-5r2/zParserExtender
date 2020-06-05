// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Ivk_zCParser_SaveDat    AS( &zCParser::SaveDat,    &zCParser::SaveDat_Union );
  HOOK Ivk_zCParser_ParseBlock AS( &zCParser::ParseBlock, &zCParser::ParseBlock_Union );
  HOOK Ivk_zCParser_ReadWord   AS( &zCParser::ReadWord,   &zCParser::ReadWord_Union );





  int zCParser::SaveDat_Union( zSTRING& name ) {
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
}