// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zEStringsIndexingMode {
    zStringsIndexing_Default = -1, // In this version default is Repair
    zStringsIndexing_None    =  0, // Do nothing
    zStringsIndexing_FromTop =  1, // Set a parser.stringcount by the last string symbol
    zStringsIndexing_Repair  =  2  // Check and repair all string symbol names
  };


  enum {
    zPAR_TYPE_VA = zPAR_TYPE_INSTANCE + 1,
  };


  struct zTCompileInfo {
    bool MergeMode;
    bool CompileDat;
    bool CompileOU;
    bool NativeWhile;
    bool Autorun;
    bool Compilable;
    string Namespace;
    Array<string> Using;
  };


  struct zTScriptInfo {
    zTCompileInfo CompileInfo;
    string ParserName;
    string FullName;
    string Name;
    Array<string> Dependencies;
    Array<string> Mods;

    void LoadDependency( string dependencyName );
    void CheckDependencies();
    void ParseMeta( string& buffer );
    bool operator == ( const string& name ) const;
    bool operator == ( const zTScriptInfo& other ) const;
    bool operator <  ( const string& name ) const;
    bool operator >  ( const string& name ) const;
  };


  static bool operator == ( zTScriptInfo* info, const string& name ) { return *info == name; }
  static bool operator <  ( zTScriptInfo* info, const string& name ) { return *info < name; }
  static bool operator >  ( zTScriptInfo* info, const string& name ) { return *info > name; }


  class zCParserExtender {
    zTCompileInfo DefaultCompileInfo;
    zTCompileInfo CurrentCompileInfo;
    Array<zTScriptInfo*> CompileQueue;
    Array<zCPar_Symbol*> PFXSymbols;
    zCParser* CurrentParser;
    zCParser* ParserExternals;
    bool ParsingEnabled;
    bool InjectUnionMenu;
    bool EarlyParsing;
    int StringsIndexingMode;
    int TestStack;
    int ExtendedSymbolRangeStart;
    int ExtendedSymbolRangeEnd;

    void CheckExtendedSymbolsStart( zCParser* parser );
    void CheckExtendedSymbolsEnd( zCParser* parser );
  public:
    zCParserExtender();
    void StartUp();
    void LoadScriptList( string fileListName );
    void PushExternalSrc( string parserName, string srcName );
    void PushExternalScript( string parserName, string scriptName );
    void CheckSymbolsHierarchy( zCParser* parser );
    void ParseBegin();
    void RegisterPFXSymbols();
    void InsertPFXSymbol( zCPar_Symbol* sym );
    zCParser* GetParser();
    zCParser* GetParserExternals();
    zCPar_Symbol* GetExternalFunction( const string& symName );
    int GetParseID();

    bool MergeModeEnabled();
    bool CompileDatEnabled();
    bool CompileOUEnabled();
    bool NativeWhileEnabled();
    bool ExtendedParsingEnabled();
    bool ExternalScriptsListIsEmpty();
    bool NeedToTestStack();
    bool NeedToEarlyParsing();
    bool IsExtendedSymbol( zCPar_Symbol* sym );
    bool IsExtendedSymbolID( const int& id );
    const string& GetDefaultNamespace();
    const Array<string>& GetUsingNamespaces();

    void GetCompilableScriptList( zTScriptInfo* root, Array<zTScriptInfo*>& queue );
    Array<zTScriptInfo*> GetCompilableScriptList();
    bool HasExternalScript( const string& scriptName );
    bool CheckValidityRecursive( zTScriptInfo* root );
    int GetStringsIndexingMode();

    static int MessagesLevel;
  };


  int zCParserExtender::MessagesLevel = 1;
  zCParserExtender zParserExtender;
  const int PluginParseID = 1;
  const int NinjaParseID  = 42;


  extern string DataDirectory;
  extern string ScriptsDirectory;
  extern string SystemDirectory;
  extern string AutorunDirectory;


  static bool NinjaInjected() {
    return GetModuleHandle( "Ninja.dll" ) != Null;
  }
}