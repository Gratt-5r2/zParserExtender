// Supported with union (c) 2020 Union team
// Union SOURCE file

//
// Thanks Saturas for help.
//

#define zFATAL( msg ) zerr->Report( zERR_TYPE_FATAL, 0, msg, -1, 0, __LINE__, __FILE__, 0 )

namespace GOTHIC_ENGINE {
  bool_t ForceOUSave    = False;
  bool_t SVM            = False;
  zCPar_Symbol* LastSym = Null;

  HOOK Ivk_oCGame_LoadParserFile AS( &oCGame::LoadParserFile, &oCGame::LoadParserFile_Union );

  int oCGame::LoadParserFile_Union( zSTRING const& parserfile ) {
    zCList<zSTRING> funclist;
    funclist.Insert( new zSTRING( "AI_OUTPUT" ) );
    parser->SetInfoFile( &funclist, "OuInfo.inf" );

    return THISCALL( Ivk_oCGame_LoadParserFile )(parserfile);
  };




  HOOK Ivk_zCParser_DeclareInstance AS( &zCParser::DeclareInstance, &zCParser::DeclareInstance_Union );

  void zCParser::DeclareInstance_Union() {
    // Checkpoint to track new symbols after DeclareInstance
    LastSym = symtab.lastsym;
    THISCALL( Ivk_zCParser_DeclareInstance )();
  }




  /*
    Called from zCParser::ParseBlock_Union
    --------------------------------------
    HOOK Ivk_zCParser_ParseBlock AS( &zCParser::ParseBlock, &zCParser::ParseBlock_OU_Union );
  */

  void zCParser::ParseBlock_OU_Union() {
    SVM = False;

    if( LastSym != Null ) {

      // Find among new symbols SVM keys
      zCPar_Symbol* sym = LastSym->next;
      while( sym ) {
        if( sym->name.HasWordI( "SVM_" ) ) {

          // Find base class of the symbol
          zCPar_Symbol* base_class = sym->parent;
          if( base_class->type == zPAR_TYPE_PROTOTYPE )
            base_class = base_class->GetParent();

          // If class is SVM - allow add the symbol to the library
          if( base_class->name == "C_SVM" ) {
            SVM = True;
            break;
          }
        }

        sym = sym->next;
      }

      LastSym = Null;
    }

    /*
      Called from zCParser::ParseBlock_Union
      --------------------------------------
      THISCALL( Ivk_zCParser_ParseBlock )();
    */
  }





  // CRASH FIX after double-use FindIndex
  int zCParser::FindIndexInst_Union( zSTRING& symName ) {
    int index = Invalid;

    if( in_func )
      index = symtab.GetIndex( in_func->name + "." + symName );

    if( index == Invalid && in_class )
      index = symtab.GetIndex( in_class->name + "." + symName );

    if( index == Invalid )
      index = symtab.GetIndex( symName );

    return index;
  }





  HOOK Ivk_zCParser_DeclareAssign AS( &zCParser::DeclareAssign, &zCParser::DeclareAssign_Union );

  void zCParser::DeclareAssign_Union( zSTRING& symName ) {
    if( SVM ) {
      int index = FindIndexInst_Union( symName );
      if( index != Invalid ) {
        zCPar_Symbol* sym = symtab.GetSymbol( index );

        if( sym->type == zPAR_TYPE_STRING ) {
          char* fileData = pc_start;
          int pc_end     = pc_stop - pc_start;

          // Collect current row symbols to the string
          zSTRING text;
          for( int i = line_start; i < pc_end; i++ ) {
            if( fileData[i] == '\n' )
              break;

            if( (byte)fileData[i] >= (byte)' ' || (byte)fileData[i] == (byte)'\t' )
              text += fileData[i];
          }

          // Write AI_Output information to library
          WriteAdditionalInfo( text, linec, line_start );
        }
      }
    }

    THISCALL( Ivk_zCParser_DeclareAssign )(symName);
  }





  HOOK Ivk_zCParser_CreatePCode AS( &zCParser::CreatePCode, &zCParser::CreatePCode_Union );

  void zCParser::CreatePCode_Union() {
    bool_t compiled_tmp = compiled;

    THISCALL( Ivk_zCParser_CreatePCode )();

    if( compiled_tmp || !parse_changed )
      return;

    if( !add_funclist || add_funclist->GetNum() == 0 )
      return;

    ogame->GetCutsceneManager()->LibStore( zLIB_STORE_ASCII | zLIB_STORE_BIN );

    ForceOUSave = True;
    ogame->GetCutsceneManager()->LibLoad_Union( 2 );
    ForceOUSave = False;
  }





  HOOK Ivk_zCCSManager_LibLoad AS( &zCCSManager::LibLoad, &zCCSManager::LibLoad_Union );

  void zCCSManager::LibLoad_Union( int flags ) {
    if( !library )
      return;

    if( ForceOUSave | !zoptions->Parm( "ZREPARSE" ) ) {
      THISCALL( Ivk_zCCSManager_LibLoad )(flags);
    }
    else {
      library->DeleteLib();
      library->loaded = True;
    }
  }





  HOOK Ivk_zCParser_Error_Union AS( &zCParser::Error, &zCParser::Error_Union );

  void zCParser::Error_Union( zSTRING& reason, int line ) {

    zSTRING message;
    zSTRING upar = "U:PAR: ";
    if( linkingnr < 0 || line < 0 ) {
      message = fname + ": " + reason + " ( line " + Z linec + " )";

      if( msgfunc != NULL )
        msgfunc( message );

      if( stop_on_error ) zFATAL( upar + message );
      else                zerr->Message( upar + message );
    }
    else
      THISCALL( Ivk_zCParser_Error_Union )(reason, line);
  }





  void oCMsgConversation::MD_SetInfo_Union( zSTRING const& inf ) {
    zSTRING s = inf;
    switch( subType ) {
      case oCMsgConversation::EV_PLAYANISOUND:
        name = s.GetWord( "#", 1 ) + ".WAV"; // sound
        text = s.GetWord( "#", 2 );          // text
        break;

      case oCMsgConversation::EV_PLAYANI:   name = s; break;
      case oCMsgConversation::EV_PLAYSOUND: text = s; break;
      case oCMsgConversation::EV_LOOKAT:    targetVobName = s; break;
    }
  }





  HOOK Ivk_zCParser_WriteAdditionalInfo AS( &zCParser::WriteAdditionalInfo, &zCParser::WriteAdditionalInfo_Union );
  
  int zCParser::WriteAdditionalInfo_Union( zSTRING& str, int line, int index ) {
    string name = str.GetPattern( "\"", "\"" ); // Extract SVM sound name
    string text = str.GetWord( "//", 2 );       // Extract SVM text
    text.Shrink();

    if( text.IsEmpty() )
      return 0;

    zCCSManager* csMan            = ogame->GetCutsceneManager();
    zCCSBlock* pBlock             = new zCCSBlock;
    zCCSAtomicBlock* pAtomicBlock = new zCCSAtomicBlock;
    zCEventMessage* newmsg        = csMan->CreateOuMessage();

    dynamic_cast<oCMsgConversation*>(newmsg)->MD_SetInfo_Union( name + "#" + text );

    pAtomicBlock->command = newmsg;
    pBlock->InsertBlock( pAtomicBlock, 0 );
    pBlock->SetRoleName( Z name );
    csMan->LibAddOU( pBlock );
    zRELEASE( pBlock );

    return True;
  }
}