// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  Col16 colWarn1 = Col16( CMD_RED );
  Col16 colWarn2 = Col16( CMD_RED | CMD_INT );
  Col16 colWarn3 = Col16();

  Col16 colParse1 = Col16( CMD_CYAN );
  Col16 colParse2 = Col16( CMD_CYAN | CMD_INT );
  Col16 colParse3 = Col16();

  Col16 colAtt1 = Col16( CMD_YELLOW );
  Col16 colAtt2 = Col16( CMD_YELLOW | CMD_INT );
  Col16 colAtt3 = Col16();


  zCParser* GetParserByParserName( string name ) {
    if( name == "Game"    || name == "parser"           ) return Gothic::Parsers::Game;
    if( name == "Auto"                                  ) return Gothic::Parsers::Game;
    if( name == "SFX"     || name == "parserSoundFX"    ) return Gothic::Parsers::SFX;
    if( name == "PFX"     || name == "parserParticleFX" ) return Gothic::Parsers::PFX;
    if( name == "VFX"     || name == "parserVisualFX"   ) return Gothic::Parsers::VFX;
    if( name == "Camera"  || name == "parserCamera"     ) return Gothic::Parsers::Camera;
    if( name == "Menu"    || name == "parserMenu"       ) return Gothic::Parsers::Menu;
    if( name == "Music"   || name == "parserMusic"      ) return Gothic::Parsers::Music;
    return Null;
  }


  string GetParserNameByParser( zCParser* par ) {
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


  static int EngineTagNameToInt( const string& tagName ) {
    if( tagName == "G1"  ) return Engine_G1;
    if( tagName == "G1A" ) return Engine_G1A;
    if( tagName == "G2"  ) return Engine_G2;
    if( tagName == "G2A" ) return Engine_G2A;

    if( zCParserExtender::MessagesLevel >= 1 )
      cmd << colWarn2 << "zParserExtender: "        <<
             colWarn1 << "unknown engine tag name " <<
             colWarn2 << tagName                    <<
             colWarn3 << endl;

    return Invalid;
  }


  static bool EngineVerToBool( const string& value ) {
    int engineVersion = Union.GetEngineVersion();

    Array<string> tags = value.Split( "," );
    for( uint i = 0; i < tags.GetNum(); i++ ) {
      string tagName = tags[i].Shrink();
      int tagVersion = EngineTagNameToInt( tagName );

      if( tagVersion == engineVersion )
        return true;
    }

    return false;
  }


  static Array<string> ParameterToArray( const string& value ) {
    Array<string> array = value.Split( "," );
    for( uint i = 0; i < array.GetNum(); i++ )
      array[i].Shrink();
    
    return array;
  }


  void zTScriptInfo::LoadDependency( string dependencyName ) {
    char buffer[1024];
    long filesCount = vdf_searchfile( dependencyName.ToChar(), buffer );

    if( filesCount == 0 )
      return;

    Array<string> files = (A buffer).Split( "|" );
    files.QuickSort();
    for( uint fileID = 0; fileID < files.GetNum(); fileID++ ) {
      string& file = files[fileID];
      uint index = file.Search( "AUTORUN", 0, true );
      if( index != Invalid ) {
        file.Cut( 0, index + 1 );
        zParserExtender.PushExternalScript( "Auto", file );
        break;
      }
    }
  }


  void zTScriptInfo::CheckDependencies() {
    for( uint i = 0; i < Dependencies.GetNum(); i++ ) {
      string& dependency = Dependencies[i];
      LoadDependency( dependency );
    }
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

        if( !line.EndWith( ";" ) ) {
          line += ";";
          if( zParserExtender.MessagesLevel > 1 )
             cmd << colAtt2 << "zParserExtender: "   <<
                    colAtt1 << "expected '"          <<
                    colAtt2 << ";"                   <<
                    colAtt1 << "' in META block in " <<
                    colAtt2 << Name                  <<
                    colAtt3 << endl;
        }

        string parameter = line.GetPattern( "",  "=" ).Shrink();
        string value     = line.GetPattern( "=", ";" ).Shrink();

        // Assign meta parameters
             if( parameter == "Parser" )      ParserName              = value;
        else if( parameter == "MergeMode" )   CompileInfo.MergeMode   = StringToBool( value );
        else if( parameter == "NativeWhile" ) CompileInfo.NativeWhile = StringToBool( value );
        else if( parameter == "Engine" )      CompileInfo.Compilable  = EngineVerToBool( value );
        else if( parameter == "After" )       Dependencies            = ParameterToArray( value );
        else if( parameter == "Namespace" )   CompileInfo.Namespace   = value.Upper();
        else if( parameter == "Using" )       CompileInfo.Using       = ParameterToArray( value.Upper() );
        else if( parameter == "Mod" )         Mods                    = ParameterToArray( value );
        else if( zCParserExtender::MessagesLevel >= 1 ) {
          // Wut ??
          cmd << colWarn2 << "zParserExtender: "        <<
                 colWarn1 << "unknown META parameter: " <<
                 colWarn2 << parameter                  <<
                 colWarn1 << " in "                     <<
                 colWarn2 << Name                       <<
                 colWarn3 << endl;
        }
        
        CheckDependencies();
      }
    }
  }


  bool zTScriptInfo::operator == ( const string& name ) const {
    return Name == name;
  }


  bool zTScriptInfo::operator == ( const zTScriptInfo& other ) const {
    return Name == other.Name;
  }


  bool zTScriptInfo::operator <  ( const string& name ) const {
    return Name < name;
  }


  bool zTScriptInfo::operator >  ( const string& name ) const {
    return Name > name;
  }


  zCParserExtender::zCParserExtender() {
    if( !CHECK_THIS_ENGINE )
      return;

    COption& option = &Union.GetGameOption() == &Union.GetDefaultOption() ?
      Union.GetUnionOption() :
      Union.GetGameOption();

    DefaultCompileInfo.Autorun = false;

    string FilesLine;
    option.Read( FilesLine,                      "ZPARSE_EXTENDER", "LoadScript",          "" );
    option.Read( DefaultCompileInfo.MergeMode,   "ZPARSE_EXTENDER", "MergeMode",           true );
    option.Read( DefaultCompileInfo.CompileDat,  "ZPARSE_EXTENDER", "CompileDat",          false );
    option.Read( DefaultCompileInfo.CompileOU,   "ZPARSE_EXTENDER", "CompileOU",           false );
    option.Read( DefaultCompileInfo.NativeWhile, "ZPARSE_EXTENDER", "NativeWhile",         false );
    option.Read( MessagesLevel,                  "ZPARSE_EXTENDER", "MessagesLevel",       MessagesLevel );
    option.Read( StringsIndexingMode,            "ZPARSE_EXTENDER", "StringsIndexingMode", zStringsIndexing_Default );
    option.Read( TestStack,                      "ZPARSE_EXTENDER", "TestStack",           false );
    option.Read( EarlyParsing,                   "ZPARSE_EXTENDER", "EarlyParsing",        true );
    DefaultCompileInfo.Compilable = true;

    Array<string> Files = FilesLine.Split( "," );
    for( uint i = 0; i < Files.GetNum(); i++ )
      LoadScriptList( Files[i].Shrink() );

    ParsingEnabled           = false;
    ParserExternals          = Null;
    ExtendedSymbolRangeStart = Invalid;
    ExtendedSymbolRangeEnd   = Invalid;
  }


  void zCAICamera::CreateParser() {
    zCAICamera* aiCam = new zCAICamera();
    aiCam->StartUp();
    aiCam->Release();
  }


  // void oCNpc::InitFightAI() {
  //   zoptions->ChangeDir( DIR_SCRIPTS );
  // 
  //   zCParser* p = zNEW( zCParser( 100 ) );
  //   p->Parse( filename + ".src" );
  //   p->CreatePCode();
  // }


  void zCParserExtender::StartUp() {
    zCMenu::CreateParser();
    zCMenu::Startup_Union();
    zCAICamera::CreateParser();
    
    zParserExtender.ParseBegin();
  }


  void zCParserExtender::LoadScriptList( string fileListName ) {
    string fileData;
    if( fileData.ReadFromVdf( fileListName, VDF_DEFAULT ) ) {
      if( zCParserExtender::MessagesLevel >= 1 )
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
      if( zCParserExtender::MessagesLevel >= 2 )
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
      if( zCParserExtender::MessagesLevel >= 1 )
        cmd << colWarn2 << "zParserExtender: " <<
               colWarn1 << "can not read "     <<
               colWarn2 << root + srcName      <<
               colWarn3 << endl;

      SrcFilesList.Remove( srcName );
      return;
    }

    // Create document file without comments
    fileData.Regex_Replace( "//.*", "" );
    fileData.Replace( "/", "\\" );
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
      if( CompileQueue[i]->FullName == fullName )
        return;

    // Create a default script info
    zTScriptInfo* scriptInfo          = new zTScriptInfo();
    scriptInfo->CompileInfo           = DefaultCompileInfo;
    scriptInfo->ParserName            = parserName;
    scriptInfo->FullName              = fullName;
    scriptInfo->Name                  = scriptName.GetWord( "\\", -1 );
    scriptInfo->CompileInfo.Autorun   = parserName == "Auto";
    scriptInfo->CompileInfo.Namespace = "";

    // Check directory of the current script
    string root = scriptInfo->CompileInfo.Autorun ?
      AutorunDirectory :
      ScriptsDirectory;

    // Load a script data
    string fileData;
    if( !fileData.ReadFromVdf( root + scriptName, VDF_DEFAULT ) ) {
      if( zCParserExtender::MessagesLevel >= 1 )
        cmd << colWarn2 << "zParserExtender: " <<
               colWarn1 << "can not read "     <<
               colWarn2 << root + scriptName   <<
               colWarn3 << endl;

      delete scriptInfo;
      return;
    }

    // Add completed file to queue
    scriptInfo->ParseMeta( fileData );
    if( scriptInfo->CompileInfo.Compilable )
      CompileQueue.Insert( scriptInfo );
    else
      delete scriptInfo;
  }


  static void InsertExternalScriptQueue( zTScriptInfo& info, Array<zTScriptInfo>& oldList, Array<zTScriptInfo>& newList ) {
    static Array<string> recursionProtect;
    if( recursionProtect.HasEqual( info.Name ) )
      return;

    if( newList.HasEqual( info ) )
      return;

    recursionProtect += info.Name;

    for( uint i = 0; i < info.Dependencies.GetNum(); i++ ) {
      uint index = oldList.SearchEqual( info.Dependencies[i] );
      cmd << index << info.Dependencies[i] << endl;
      if( index != Invalid )
        InsertExternalScriptQueue( oldList[index], oldList, newList );
    }

    newList += info;
    recursionProtect -= info.Name;
  }


  void zCParserExtender::GetCompilableScriptList( zTScriptInfo* root, Array<zTScriptInfo*>& queue ) {
    if( !root->CompileInfo.Compilable )
      return;

    if( queue.HasEqual( root ) )
      return;

    for( uint i = 0; i < root->Dependencies.GetNum(); i++ ) {
      string& dependecy = root->Dependencies[i];
      uint index = CompileQueue.SearchEqual( dependecy );
      if( index == Invalid ) // maybe ??
        continue;

      GetCompilableScriptList( CompileQueue[index], queue );
    }

    queue += root;
  }


  bool zCParserExtender::CheckValidityRecursive( zTScriptInfo* root ) {
    static Array<string> recursionProtect;

    bool& compilable = root->CompileInfo.Compilable;
    if( !compilable )
      return false;

    if( recursionProtect.HasEqual( root->Name ) ) {
        cmd << colWarn2 << "zParserExtender: "         <<
               colWarn1 << "recursive dependecies in " <<
               colWarn2 << root->Name                  <<
               colWarn3 << endl;
      return false;
    }

    recursionProtect += root->Name; // Queue IN

    for( uint i = 0; i < root->Dependencies.GetNum(); i++ ) {
      string& dependency = root->Dependencies[i];
      uint index = CompileQueue.SearchEqual( dependency );
      if( index == Invalid ) {
        cmd << colAtt2 << "zParserExtender: "        <<
               colAtt1 << "dependecy "               <<
               colAtt2 << dependency                 <<
               colAtt1 << " not found in "           <<
               colAtt2 << root->Name                 <<
               colAtt1 << ", script will be skipped" <<
               colAtt3 << endl;
        compilable = false;
        continue;
      }

      zTScriptInfo* info = CompileQueue[index];
      if( !CheckValidityRecursive( info ) )
        compilable = false;
    }

    for( uint i = 0; i < root->Mods.GetNum(); i++ ) {
      string& dependency = root->Mods[i];
      string fullVolumeName = Union.GetGameDirectory() + "Data\\" + dependency;
      if( _access( fullVolumeName, None ) == -1 ) {
        compilable = false;
        break;
      }
    }

    recursionProtect -= root->Name; // Queue OUT
    return compilable;
  }


  Array<zTScriptInfo*> zCParserExtender::GetCompilableScriptList() {
    for( uint i = 0; i < CompileQueue.GetNum(); i++ )
      CheckValidityRecursive( CompileQueue[i] );

    Array<zTScriptInfo*> queue;
    for( uint i = 0; i < CompileQueue.GetNum(); i++ )
      GetCompilableScriptList( CompileQueue[i], queue );

    return queue;
  }


  bool zCParserExtender::HasExternalScript( const string& scriptName ) {
    return CompileQueue.HasEqual( scriptName );
  }


  inline void SetParsersCompiled() {
    Gothic::Parsers::Game->compiled   = True;
    Gothic::Parsers::SFX->compiled    = True;
    Gothic::Parsers::PFX->compiled    = True;
    Gothic::Parsers::VFX->compiled    = True;
    Gothic::Parsers::Camera->compiled = True;
    Gothic::Parsers::Menu->compiled   = True;
    Gothic::Parsers::Music->compiled  = True;
  }


  void zCParserExtender::CheckExtendedSymbolsStart( zCParser* par ) {
    if( ExtendedSymbolRangeStart != Invalid || par != parser )
      return;

    ExtendedSymbolRangeStart = par->symtab.table.GetNum();
  }


  void zCParserExtender::CheckExtendedSymbolsEnd( zCParser* par ) {
    if( par != parser )
      return;

    ExtendedSymbolRangeEnd = par->symtab.table.GetNum() - 1;
  }


  bool zCParserExtender::IsExtendedSymbolID( const int& id ) {
    return id >= ExtendedSymbolRangeStart && id <= ExtendedSymbolRangeEnd;
  }


  bool zCParserExtender::IsExtendedSymbol( zCPar_Symbol* sym ) {
    int index = parser->symtab.GetIndex( sym );
    if( index == Invalid )
      return false;

    return IsExtendedSymbolID( index );
  }

#if ENGINE == Engine_G1A || ENGINE == Engine_G2
  static auto patch_physical_first = CreateSharedPatchSym<CPatchInteger>( "s_physical_first", 0 );
#endif

  int ParStackMaxLength = 0;
  void zCParserExtender::ParseBegin() {
    zCParser* savedParser = zCParser::cur_parser;
    Array<zTScriptInfo*> compilableList = GetCompilableScriptList();
    ParsingEnabled = true;
#if ENGINE == Engine_G1 || ENGINE == Engine_G2A
    int physicalFirst = zFILE_VDFS::s_physical_first;
    zFILE_VDFS::s_physical_first = True;
#else
    patch_physical_first->SetValue( 1 );
#endif

    {
      Array<zCParser*> activeParsers;
      for( uint i = 0; i < compilableList.GetNum(); i++ ) {
        zTScriptInfo& scriptInfo = *compilableList[i];
        CurrentCompileInfo       = scriptInfo.CompileInfo;

#if 0
        // Set root directory of
        // current external script
        zoptions->ChangeDir(
          CurrentCompileInfo.Autorun ?
          DIR_SYSTEM :
          DIR_SCRIPTS
          );
#else
        // Set root directory of
        // current external script
        static zFILE* fakeSystem = new zFILE_VDFS( "\\SYSTEM\\" );
        if( CurrentCompileInfo.Autorun )
          fakeSystem->ChangeDir( false );
        else
          zoptions->ChangeDir( DIR_SCRIPTS );
#endif

        // Search game parser by name
        zCParser* par = GetParserByParserName( scriptInfo.ParserName );

        if( par == Null ) {
          if( zCParserExtender::MessagesLevel >= 1 )
            cmd << colWarn2 << "zParserExtender: "    <<
                   colWarn1 << "invalid parser name " <<
                   colWarn2 << scriptInfo.ParserName  <<
                   colWarn1 << " in external script " <<
                   colWarn2 << scriptInfo.FullName    <<
                   colWarn3 << endl;
          continue;
        }

        zCParser::cur_parser = par;
        CurrentParser = par;
        CurrentParser->InitializeNamespace( GetDefaultNamespace() );

        ParStackMaxLength = parser->stack.GetDynSize() - 4;

        // Okay! begin parsing
        if( zCParserExtender::MessagesLevel >= 1 )
          cmd << colParse2 << "zParserExtender: " <<
                 colParse1 << "start parsing "    <<
                 colParse2 << scriptInfo.FullName <<
                 colParse3 << endl;

        // So, activate parser for parsing
        PrepareParserSymbols( par );
        par->ResetAdditionalInfo();
        par->SetEnableParsing_Union( True );

        CheckExtendedSymbolsStart( par );
        int parsed = par->ParseFile( Z scriptInfo.FullName );
        CheckExtendedSymbolsEnd( par );

        if( parsed != 0 ) {
          parsed = activeParsers & par;
          if( !parsed )
            par->SetEnableParsing_Union( False );

          if( zCParserExtender::MessagesLevel >= 1 )
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
      CurrentCompileInfo = DefaultCompileInfo;
      if( !activeParsers.HasEqual( parser ) ) {
        activeParsers.Insert( parser );
      }

      for( uint i = 0; i < activeParsers.GetNum(); i++ ) {
        CurrentParser = activeParsers[i];
        CurrentParser->CreatePCode();
        CurrentParser->SetEnableParsing_Union( False );

        if( CurrentParser == Gothic::Parsers::PFX )
          RegisterPFXSymbols();
      }

      activeParsers.Clear();
    }

#if ENGINE == Engine_G1 || ENGINE == Engine_G2A
    zFILE_VDFS::s_physical_first = physicalFirst;
#else
    patch_physical_first->SetValue( 0 );
#endif
    ParsingEnabled = false;
    zCParser::cur_parser = savedParser;
    zCPar_SymbolTable::cur_table = &savedParser->symtab;
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


  zCParser* zCParserExtender::GetParserExternals() {
    if( !ParserExternals )
      ParserExternals = new zCParser();

    return ParserExternals;
  }


  zCPar_Symbol* zCParserExtender::GetExternalFunction( const string& symName ) {
    zCParser* par = GetParserExternals();
    zCPar_Symbol* symbol = par->GetSymbol( symName );
    if( !symbol )
      if( par->DynamicLoadExternal( symName ) )
        return par->GetSymbol( symName );

    return symbol;
  }


  int zCParserExtender::GetParseID() {
    return zCParser::enableParsing;
  }


  bool zCParserExtender::MergeModeEnabled() {
    return CurrentCompileInfo.MergeMode && ParsingEnabled;
  }


  bool zCParserExtender::CompileDatEnabled() {
    return CurrentCompileInfo.CompileDat || !ParsingEnabled;
  }


  bool zCParserExtender::CompileOUEnabled() {
    return CurrentCompileInfo.CompileOU;
  }


  bool zCParserExtender::NativeWhileEnabled() {
    return CurrentCompileInfo.NativeWhile;
  }


  bool zCParserExtender::ExtendedParsingEnabled() {
    return ParsingEnabled;
  }


  bool zCParserExtender::ExternalScriptsListIsEmpty() {
    return CompileQueue.GetNum() == 0;
  }

  bool zCParserExtender::NeedToTestStack() {
    return TestStack;
  }

  bool zCParserExtender::NeedToEarlyParsing() {
    return EarlyParsing;
  }

  const string& zCParserExtender::GetDefaultNamespace() {
    return CurrentCompileInfo.Namespace;
  }


  const Array<string>& zCParserExtender::GetUsingNamespaces() {
    return CurrentCompileInfo.Using;
  }


  int zCParserExtender::GetStringsIndexingMode() {
    return StringsIndexingMode;
  }


  void ParseExternalScript( string parserName, string scriptName ) {
    zParserExtender.PushExternalScript( parserName, scriptName );
  }
}
