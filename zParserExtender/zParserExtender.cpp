// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  Col16 colWarn1 = Col16( CMD_RED );
  Col16 colWarn2 = Col16( CMD_RED | CMD_INT );
  Col16 colWarn3 = Col16();

  Col16 colParse1 = Col16( CMD_CYAN );
  Col16 colParse2 = Col16( CMD_CYAN | CMD_INT );
  Col16 colParse3 = Col16();





  zCParser* GetParserByName( string name ) {
    if( name.HasWordI( "Game"   ) || name.HasWordI( "parser"           ) ) return Gothic::Parsers::Game;
    if( name.HasWordI( "Auto"                                          ) ) return Gothic::Parsers::Game;
    if( name.HasWordI( "SFX"    ) || name.HasWordI( "parserSoundFX"    ) ) return Gothic::Parsers::SFX;
    if( name.HasWordI( "PFX"    ) || name.HasWordI( "parserParticleFX" ) ) return Gothic::Parsers::PFX;
    if( name.HasWordI( "VFX"    ) || name.HasWordI( "parserVisualFX"   ) ) return Gothic::Parsers::VFX;
    if( name.HasWordI( "Camera" ) || name.HasWordI( "parserCamera"     ) ) return Gothic::Parsers::Camera;
    if( name.HasWordI( "Menu"   ) || name.HasWordI( "parserMenu"       ) ) return Gothic::Parsers::Menu;
    if( name.HasWordI( "Music"  ) || name.HasWordI( "parserMusic"      ) ) return Gothic::Parsers::Music;
    return Null;
  }





  string GetNameByParser( zCParser* par ) {
    if( par == Gothic::Parsers::Game   ) return "Game";
    if( par == Gothic::Parsers::SFX    ) return "SFX";
    if( par == Gothic::Parsers::PFX    ) return "PFX";
    if( par == Gothic::Parsers::VFX    ) return "VFX";
    if( par == Gothic::Parsers::Camera ) return "Camera";
    if( par == Gothic::Parsers::Menu   ) return "Menu";
    if( par == Gothic::Parsers::Music  ) return "Music";
    return "Unknown";
  }






  static bool StringToBool( const string& value ) {
    if( value == "false" || value == "0" )
      return false;

    return true;
  }

  void zTScriptInfo::ParseMeta( string& buffer ) {
    // Check meta information
    if( buffer.GetWordSmart( 1 ) == "META" && buffer.GetWordSmart( 2 ) == "{" ) {
      CDocument metaData = buffer.GetPattern( "{", "}" );

      // Parse parameters
      for( uint i = 0; i < metaData.GetNum(); i++ ) {
        string line = metaData[i];
        if( line.Shrink().IsEmpty() )
          continue;

        string parameter = line.GetPattern( "",  "=" ).Shrink();
        string value     = line.GetPattern( "=", ";" ).Shrink();

        // Assign meta parameters
             if( parameter == "Parser" )      ParserName              = value;
        else if( parameter == "MergeMode" )   CompileInfo.MergeMode   = StringToBool( value );
        else if( parameter == "NativeWhile" ) CompileInfo.NativeWhile = StringToBool( value );
        else {
          // Wut ??
          cmd << colWarn2 << "zParserExtender: "       <<
                 colWarn1 << "unknown meta parameter " <<
                 colWarn2 << parameter                 <<
                 colWarn3 << endl;
        }
      }
    }
  }






  zCParserExtender::zCParserExtender() {
    COption& option = &Union.GetGameOption() == &Union.GetDefaultOption() ?
      Union.GetUnionOption() :
      Union.GetGameOption();

    DefaultCompileInfo.Autorun = false;

    string FilesLine;
    option.Read( FilesLine,                      "ZPARSE_EXTENDER", "LoadScript",  "" );
    option.Read( DefaultCompileInfo.MergeMode,   "ZPARSE_EXTENDER", "MergeMode",   true );
    option.Read( DefaultCompileInfo.CompileDat,  "ZPARSE_EXTENDER", "CompileDat",  false );
    option.Read( DefaultCompileInfo.NativeWhile, "ZPARSE_EXTENDER", "NativeWhile", false );

    Array<string> Files = FilesLine.Split( "," );
    for( uint i = 0; i < Files.GetNum(); i++ )
      LoadScriptList( Files[i].Shrink() );

    ParsingEnabled = false;
  }





  void zCParserExtender::LoadScriptList( string fileListName ) {
    string fileData;
    if( fileData.ReadFromVdf( fileListName, VDF_DEFAULT ) ) {
      cmd << colWarn2 << "zParserExtender: " <<
             colWarn1 << "can not read "     <<
             colWarn2 << fileListName        <<
             colWarn3 << endl;
      return;
    }

    CDocument fileDoc = fileData;
    for( uint i = 0; i < fileDoc.GetNum(); i++ ) {
      string& line = fileDoc[i];
      if( line.Shrink().IsEmpty() )
        continue;

      string parserName = line.GetWord( " \t", 1 );
      string scriptName = line.GetWord( " \t", 2 );

      PushExternalScript( parserName, scriptName );
    }
  }





  void zCParserExtender::PushExternalSrc( string parserName, string srcName ) {
    static Array<string> SrcFilesList;

    // Check src recursion
    if( SrcFilesList.HasEqual( srcName ) ) {
      cmd << colWarn2 << "zParserExtender: " <<
             colWarn1 << "recursion of "     <<
             colWarn2 << srcName             <<
             colWarn3 << endl;
      return;
    }

    // Add file to current parsing list
    SrcFilesList.Insert( srcName );

    // Check directory of current script
    string root = parserName == "Auto" ?
      AutorunDirectory :
      ScriptsDirectory;

    // Load script data
    string fileData;
    if( !fileData.ReadFromVdf( root + srcName, VDF_DEFAULT ) ) {
      cmd << colWarn2 << "zParserExtender: " <<
             colWarn1 << "can not read "     <<
             colWarn2 << root + srcName      <<
             colWarn3 << endl;
      return;
    }

    // Create document file without comments
    fileData.Regex_Replace( "//.*$", "" );
    CDocument fileDocument = fileData;

    // Parse file list
    for( uint i = 0; i < fileDocument.GetNum(); i++ ) {
      string& line = fileDocument[i];
      if( !line.Shrink().IsEmpty() )
        PushExternalScript( parserName, line.Upper() );
    }

    // Remove file from current parsing list
    SrcFilesList.Remove( srcName );
  }





  void zCParserExtender::PushExternalScript( string parserName, string scriptName ) {
    // Parse as src file
    if( scriptName.EndWith( ".SRC" ) )
      return PushExternalSrc( parserName, scriptName );

    string fullName = scriptName;
    if( parserName == "Auto" )
      fullName = "AUTORUN\\" + fullName;

    // Ignore exists
    for( uint i = 0; i < CompileQueue.GetNum(); i++ )
      if( CompileQueue[i].FullName == fullName )
        return;

    // Create a default script info
    zTScriptInfo scriptInfo;
    scriptInfo.CompileInfo         = DefaultCompileInfo;
    scriptInfo.ParserName          = parserName;
    scriptInfo.FullName            = fullName;
    scriptInfo.Name                = scriptName.GetWord( "\\", -1 );
    scriptInfo.CompileInfo.Autorun = parserName == "Auto";

    // Check directory of the current script
    string root = scriptInfo.CompileInfo.Autorun ?
      AutorunDirectory :
      ScriptsDirectory;

    // Load a script data
    string fileData;
    if( !fileData.ReadFromVdf( root + scriptName, VDF_DEFAULT ) ) {
      cmd << colWarn2 << "zParserExtender: " <<
             colWarn1 << "can not read "     <<
             colWarn2 << root + scriptName   <<
             colWarn3 << endl;
      return;
    }

    // Add completed file to queue
    scriptInfo.ParseMeta( fileData );
    CompileQueue.Insert( scriptInfo );
  }





  void zCParserExtender::ParseBegin() {
    ParsingEnabled = true;
    {
      Array<zCParser*> activeParsers;
      for( uint i = 0; i < CompileQueue.GetNum(); i++ ) {
        zTScriptInfo& scriptInfo = CompileQueue[i];
        CurrentCompileInfo       = scriptInfo.CompileInfo;

        // Set root directory of
        // current external script
        zoptions->ChangeDir(
          CurrentCompileInfo.Autorun ?
          DIR_SYSTEM :
          DIR_SCRIPTS
          );
        
        // Search game parser by name
        zCParser* par = GetParserByName( scriptInfo.ParserName );
        zCParser::cur_parser = par;
        CurrentParser = par;

        if( par == Null ) {
          cmd << colWarn2 << "zParserExtender: "    <<
                 colWarn1 << "invalid parser name " <<
                 colWarn2 << scriptInfo.ParserName  <<
                 colWarn1 << " in external script " <<
                 colWarn2 << scriptInfo.FullName    <<
                 colWarn3 << endl;
          continue;
        }

        // Okay! begin parsing
        cmd << colParse2 << "zParserExtender: " <<
               colParse1 << "start parsing "    <<
               colParse2 << scriptInfo.FullName <<
               colParse3 << endl;


        // So, activate parser for parsing and parse file
        par->ReserAdditionalInfo();
        par->SetEnableParsing_Union( True );
        if( par->ParseFile( Z scriptInfo.FullName ) != 0 ) {
          bool parsed = activeParsers & par;
          if( !parsed )
            par->SetEnableParsing_Union( False );

          cmd << colWarn2 << "zParserExtender: "   <<
                 colWarn1 << "Can not parse file " <<
                 colWarn2 << scriptInfo.FullName   <<
                 colWarn3 << endl;
          continue;
        }
        else
          // Add parser to active list
          activeParsers |= par;
      }

      // Compile parsed scripts...
      for( uint i = 0; i < activeParsers.GetNum(); i++ ) {
        CurrentParser = activeParsers[i];
        CurrentParser->CreatePCode();
        CurrentParser->SetEnableParsing_Union( False );

        if( CurrentParser == Gothic::Parsers::PFX )
          RegisterPFXSymbols();
      }

      activeParsers.Clear();
    }
    ParsingEnabled = false;
  }




  void zCParserExtender::RegisterPFXSymbols() {
    if( PFXSymbols.GetNum() == 0 )
      return;

    for( uint i = 0; i < PFXSymbols.GetNum(); i++ ) {
      zCPar_Symbol* sym          = PFXSymbols[i];
      zSTRING instanceName       = sym->name;
      zCParticleEmitter* emitter = new zCParticleEmitter();
      emitter->particleFXName    = instanceName;

      // Rename old pfx emitter
      int indexEqual = zCParticleFX::s_emitterPresetList.Search( emitter );
      if( indexEqual != Invalid )
        zCParticleFX::s_emitterPresetList[i]->particleFXName += "_OLD";
      
      Gothic::Parsers::PFX->CreateInstance( instanceName, emitter );
      zCParticleFX::s_emitterPresetList.Insert( emitter );
      emitter->UpdateInternals();
    }

    zCParticleFX::s_emitterPresetList.QuickSort();
    PFXSymbols.Clear();
  }




  void zCParserExtender::InsertPFXSymbol( zCPar_Symbol* sym ) {
    PFXSymbols += sym;
  }




  zCParser* zCParserExtender::GetParser() {
    return CurrentParser;
  }





  bool zCParserExtender::MergeModeEnabled() {
    return CurrentCompileInfo.MergeMode && ParsingEnabled;
  }





  bool zCParserExtender::CompileDatEnabled() {
    return CurrentCompileInfo.CompileDat || !ParsingEnabled;
  }





  bool zCParserExtender::NativeWhileEnabled() {
    return CurrentCompileInfo.NativeWhile;
  }





  bool zCParserExtender::ExtendedParsingEnabled() {
    return ParsingEnabled;
  }





  void ParseExternalScript( string parserName, string scriptName ) {
    zParserExtender.PushExternalScript( parserName, scriptName );
  }
}