// Supported with union (c) 2020 Union team

// User API for zCParser
// Add your methods here

int SaveDat_Union( zSTRING& );
void ParseBlock_Union();
void DeclareWhile_Union();
void DeclareBreak_Union();
void DeclareContinue_Union();
void SkipMeta_Union();
void DeclareInstance_Union();
void DeclarePrototype_Union();
void DeclareAssign_Union( zSTRING& );
void CreatePCode_Union();
void Error_Union( zSTRING&, int );
int WriteAdditionalInfo_Union( zSTRING&, int, int );
int FindIndexInst_Union( zSTRING& );
void ParseBlock_OU_Union();
void SetEnableParsing_Union( bool_t );
void CallGameInit_Union();
void CallGameLoop_Union();
void ReadWord_Union( zSTRING& );
int LoadGlobalVars_Union( zCArchiver& );
int IsInAdditionalInfo_Union( zSTRING const& );
void RemoveAdditionalInfo_Union( zSTRING& );
void Reset_Union();
void ResetAdditionalInfo();
int Parse_Union( zSTRING s );
void DeclareVaFuncCall( zSTRING& s );
void DeclareFuncCall_Union( zSTRING& s, int n );
void DeclareVar_Union( int isConstant );
zCPar_TreeNode* Parse_Expression_Primary_Union( int& token );
zCPar_TreeNode* CreateStringLeaf_Union();
zCPar_TreeNode* CreateFloatLeaf_Union();
void DeclareClass_Union();
bool DynamicLoadExternal( const zSTRING&, const bool& = false );
void DeclareAssign_Union_2( zSTRING& );
void SetInfoFile_Union( zCList<zSTRING>*, zSTRING const& );
int SaveDatCopy();
void* CallFunc_Union( int index, ... );
void DeclareFunc_Union();
int ReadFuncType_Union();
void DoEvent( const zSTRING& eventName );
int GetEventIndex( const zSTRING& eventName );
int ReadInt_Union();
int GetNextToken_Union();
float ReadFloat_Union();
void ReadMacro( zSTRING& );
int LoadDat_Union( zSTRING& datName );
bool NamespaceIsActive();
bool UsingNamespace( zSTRING& word );
void DeclareAssignFunc_Union( zSTRING& name );
void* GetInstance( const int& index );
bool ParseBlockOrOperatorLine();
void ParseOperatorLine();
void DeclareTest();
void DeclareExtern();
bool TestSymbol();
void SkipOperatorsLine();
bool SkipBlock();
void SkipString();
int ParseFileWord( const zSTRING& word );

void PreCompile();
void RenameTreeNode( zCPar_Symbol* sym, zSTRING newName );

zCPar_TreeNode* PushOnStack_Union( zCPar_TreeNode* node );
int ParseSource_Union( zSTRING& s );
zCPar_TreeNode* CreateLeafCallInstance( const zSTRING& instName, zCPar_TreeNode* );
zCPar_Symbol* GetNearestVariable( zSTRING& word );
zCPar_Symbol* GetLocalSymbol( zSTRING& word );
bool IsVaExternal( const zSTRING& funcName );

void InitializeNamespace( const zSTRING& defaultNamespace );
void NamespaceBegin( zSTRING& );
void NamespaceAddEntry( zSTRING& );
void NamespaceRemoveEntry( zSTRING& );
void CheckNamespace( zSTRING& );
void DeclareNamespaceForNextWord( const uint& wordID, bool newSymbol = false );
void DeclareNamespaceForNextParenthesis();
void AddNamespace( zSTRING&, bool newSymbol = false );
zSTRING GetNamespacePrefix(const uint& levelUp);
int GetNamespaceLevel();
void ExportStringList();

void __cdecl DefineExternal_Union( zSTRING const&, int( __cdecl* )(), int, int, ... );