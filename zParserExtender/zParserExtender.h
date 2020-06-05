// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
#if 0
  struct zTCompilationDescriptor {
    bool Merge;
    bool SaveDAT;
    bool NativeWhile;

    zTCompilationDescriptor();
  };





  class zCParserExtender {
    zTCompilationDescriptor GlobalDescriptor;
    zTCompilationDescriptor FileDescriptor;
  public:

    zCParserExtender();
    zTCompilationDescriptor& GetNextFileDescriptor();
    bool MergeEnabled();
    bool SaveDATEnabled();
    bool NativeWhileEnabled();
  };

  zCParserExtender zParserExtender;





  struct zTExternalScript {
    zTCompilationDescriptor LocalDescriptor;
    zCParser** Parser;
    string ParserName;
    string ScriptName;

    void ParseMeta( string& fileData );
    void Init( string parser, string scriptName );
  };





  class zCExternalQueue {
    Array<zTExternalScript> Scripts;
  public:

    zTExternalScript ParseMeta( string& fileData );
    void PushScript( string parser, string scriptName );
  };

  zCExternalQueue zExternalQueue;
#endif
  
  struct zTCompileInfo {
    bool MergeMode;
    bool CompileDat;
    bool NativeWhile;
    bool Autorun;
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
    bool ParsingEnabled;
  public:

    zCParserExtender();
    void LoadScriptList( string fileListName );
    void PushExternalSrc( string parserName, string srcName );
    void PushExternalScript( string parserName, string scriptName );
    void ParseBegin();

    bool MergeModeEnabled();
    bool CompileDatEnabled();
    bool NativeWhileEnabled();
    bool ExtendedParsingEnabled();
  };

  zCParserExtender zParserExtender;




  extern string DataDirectory;
  extern string ScriptsDirectory;
  extern string SystemDirectory;
  extern string AutorunDirectory;
}