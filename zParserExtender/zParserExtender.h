// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  struct zTCompileInfo {
    bool MergeMode;
    bool CompileDat;
    bool NativeWhile;
    bool Autorun;
    bool Compilable;
  };



  struct zTScriptInfo {
    zTCompileInfo CompileInfo;
    string ParserName;
    string FullName;
    string Name;

    void ParseMeta( string& buffer );
  };



  class zCParserExtender {
    zTCompileInfo DefaultCompileInfo;
    zTCompileInfo CurrentCompileInfo;
    Array<zTScriptInfo> CompileQueue;
    Array<zCPar_Symbol*> PFXSymbols;
    zCParser* CurrentParser;
    bool ParsingEnabled;
  public:

    zCParserExtender();
    void LoadScriptList( string fileListName );
    void PushExternalSrc( string parserName, string srcName );
    void PushExternalScript( string parserName, string scriptName );
    void ParseBegin();
    void RegisterPFXSymbols();
    void InsertPFXSymbol( zCPar_Symbol* sym );
    zCParser* GetParser();

    bool MergeModeEnabled();
    bool CompileDatEnabled();
    bool NativeWhileEnabled();
    bool ExtendedParsingEnabled();
    bool ExternalScriptsListIsEmpty();
  };

  zCParserExtender zParserExtender;



  extern string DataDirectory;
  extern string ScriptsDirectory;
  extern string SystemDirectory;
  extern string AutorunDirectory;
}