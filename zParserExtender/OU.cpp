// Supported with union (c) 2020 Union team
// Union SOURCE file

//
// Thanks @Saturas for help.
//

#define zFATAL( msg ) zerr->Report( zERR_TYPE_FATAL, 0, msg, -1, 0, __LINE__, __FILE__, 0 )

namespace GOTHIC_ENGINE {
  bool_t ForceOUSave    = False;
  bool_t SVM            = False;
  zCPar_Symbol* LastSym = Null;

  HOOK Ivk_oCGame_LoadParserFile PATCH( &oCGame::LoadParserFile, &oCGame::LoadParserFile_Union );

  zCList<zSTRING>& GetFuncListSVM() {
    static zCList<zSTRING> funclist = [] {
      zCList<zSTRING> list;
      list.Insert(new zSTRING("AI_OUTPUT"));
      return list;
    }();
    return funclist;
  }

  int oCGame::LoadParserFile_Union( zSTRING const& parserfile ) {
    parser->SetInfoFile( &GetFuncListSVM(), "OuInfo.inf");
    int ok = THISCALL( Ivk_oCGame_LoadParserFile )(parserfile);
    if( zParserExtender.NeedToEarlyParsing() )
      zParserExtender.StartUp();

    return ok;
  };


  HOOK Ivk_zCParser_DeclareInstance PATCH( &zCParser::DeclareInstance, &zCParser::DeclareInstance_Union );

  void zCParser::DeclareInstance_Union() {
    // Checkpoint to track new symbols after DeclareInstance
    LastSym = symtab.lastsym;
    DeclareNamespaceForNextWord( 1, true );
    DeclareNamespaceForNextParenthesis();
    return THISCALL( Ivk_zCParser_DeclareInstance )();
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
  

  HOOK Ivk_zCParser_DeclareAssign PATCH( &zCParser::DeclareAssign, &zCParser::DeclareAssign_Union );

  inline bool IsEOL( const char& sym ) {
    return
      sym == '\r' ||
      sym == '\n' ||
      sym == '\0';
  }

  void zCParser::DeclareAssign_Union( zSTRING& symName ) {
    DynamicLoadExternal( symName );
    AddNamespace( symName );

    if( SVM ) {
      int index = FindIndexInst_Union( symName );
      if( index != Invalid ) {

        char* fileData = pc;
        int lineLength = 0;

        for( ; true; lineLength++ )
          if( IsEOL( fileData[lineLength] ) )
            break;

        zSTRING lineText = string( fileData, lineLength );
        WriteAdditionalInfo( lineText, linec, line_start );
      }
    }
    else if( parser->enableParsing != NinjaParseID ) {
      zCPar_Symbol* sym = GetSymbol( symName );
      if( sym && (sym->type == zPAR_TYPE_INSTANCE || sym->type == zPAR_TYPE_PROTOTYPE) ) {
        zSTRING wordLeft, wordRight;
        ReadWord( wordLeft );
        ReadWord( wordRight );
        if( wordLeft == "(" && wordRight == ")" ) {
          CreateLeafCallInstance( symName, treenode );
          return;
        }
        else {
          PrevWord();
          PrevWord();
        }
      }
    }

    THISCALL( Ivk_zCParser_DeclareAssign )(symName);
  }

  
  HOOK Hook_zCParser_IsInAdditionalInfo PATCH( &zCParser::IsInAdditionalInfo, &zCParser::IsInAdditionalInfo_Union );

  int zCParser::IsInAdditionalInfo_Union( const zSTRING& symName ) {
    return symName == "AI_Output";
  }


  void zDeleteOU();

  HOOK Hook_zInitOptions PATCH( &zInitOptions, &zDeleteOU );

  void zDeleteOU() {
    Hook_zInitOptions();
    if( !NeedToReparseOU() )
      return;

    auto& optDef  = Union.GetDefaultOption();
    auto& optGame = Union.GetGameOption();

    string ouFileName = "OU";
    if( &optDef != &optGame )
      optGame.Read( ouFileName, "FILES", "OUTPUTUNITS", "OU" );

#if ENGINE >= Engine_G2
    string ouFullFileName = Union.GetGameDirectory() + A zoptions->GetDirString( DIR_OUTPUTUNITS ) + ouFileName;
#else
    string ouFullFileName = Union.GetGameDirectory() + A zoptions->GetDirString( DIR_SCRIPTS ) + "content\\cutscene\\" + ouFileName;
#endif

    string ouBin = ouFullFileName + ".bin";
    string ouCsl = ouFullFileName + ".csl";

    DeleteFile( ouBin );
    DeleteFile( ouCsl );
  }


  HOOK Ivk_zCParser_CreatePCode PATCH( &zCParser::CreatePCode, &zCParser::CreatePCode_Union );

  void zCParser::CreatePCode_Union() {
    if( parser && this == parser )
      zTTriggerScript::RegisterClassOffset();

    THISCALL( Ivk_zCParser_CreatePCode )();

    if( !parser || this != parser )
      return;

    zCCSManager* csman = ogame->GetCutsceneManager();
    csman->LibSortion();
  }


  void zCCSManager::LibSortion() {
    if( library )
      library->ouList.QuickSort();
  }


  HOOK Ivk_zCParser_Error_Union PATCH( &zCParser::Error, &zCParser::Error_Union );

  // More details report
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


  HOOK Ivk_zCParser_WriteAdditionalInfo PATCH( &zCParser::WriteAdditionalInfo, &zCParser::WriteAdditionalInfo_Union );
  
  int zCParser::WriteAdditionalInfo_Union( zSTRING& str, int line, int index ) {
    string name = str.GetPattern( "\"", "\"" ); // Extract SVM sound name
    string text = str.GetWord( "//", 2 );       // Extract SVM text
    text.Shrink();

    if( text.IsEmpty() )
      return 0;

    zCCSManager* csMan = ogame->GetCutsceneManager();
    int ouValidateIndex = csMan->LibValidateOU( Z name );
    if( ouValidateIndex != Invalid ) {
      // Delete equal cutscene block from OU
      csMan->LibDelOU( ouValidateIndex );
      if( zCParserExtender::MessagesLevel >= 3 )
        cmd << colAtt2 << "zParserExtender: " <<
               colAtt1 << "Cutscene block "   <<
               colAtt2 << name                <<
               colAtt1 << " replaced ("       <<
               colAtt2 << ouValidateIndex     <<
               colAtt1 << ")"                 <<
               colAtt3 << endl;
    }

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


  static string s_LastLibStoreName;
  static bool   s_SaveLibCopy = false;

  void zCCSManager::LibStoreCopy( int flags ) {
    s_SaveLibCopy = true;
    LibStore( flags );
    s_SaveLibCopy = false;

    Col16  statusColor = CMD_GREEN | CMD_INT;
    if( zCParserExtender::MessagesLevel >= 2 )
      cmd << colParse2   << "zParserExtender: " <<
             colParse1   << "file "             <<
             colParse2   << s_LastLibStoreName  <<
             colParse1   << " saved"            <<
             colParse3   << endl;
  }


  HOOK Hook_zCArchiverFactory_CreateArchiverWrite PATCH( &zCArchiverFactory::CreateArchiverWrite, &zCArchiverFactory::CreateArchiverWrite_Union );

  zCArchiver* zCArchiverFactory::CreateArchiverWrite_Union( const zSTRING& __fileName, zTArchiveMode mode, int saveGame, int flags ) {
    string fileName = __fileName;
    if( s_SaveLibCopy ) {
      string name      = fileName.GetPattern( "", ".", -1 );
      string extension = fileName.GetPattern( ".", "", -1 );
             fileName  = name + ".EDITED." + extension;
    }

    s_LastLibStoreName = fileName.GetWord( "\\", -1 );
    return THISCALL( Hook_zCArchiverFactory_CreateArchiverWrite )(fileName, mode, saveGame, flags );
  };
}