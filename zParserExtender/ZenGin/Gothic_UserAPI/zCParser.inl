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
void DeclareFuncCall_Union( zSTRING& s, int n );
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
zCPar_TreeNode* CreateFloatLeaf_Union();
float ReadFloat_Union();
void ReadMacro( zSTRING& );
int LoadDat_Union( zSTRING& datName );

void PreCompile();
void RenameTreeNode( zCPar_Symbol* sym, zSTRING newName );

zCPar_TreeNode* PushOnStack_Union( zCPar_TreeNode* node );
int ParseSource_Union( zSTRING& s );
zCPar_TreeNode* CreateLeafCallInstance( const zSTRING& instName, zCPar_TreeNode* );
zCPar_Symbol* GetNearestVariable( const zSTRING& varName );

void __cdecl DefineExternal_Union( zSTRING const&, int( __cdecl* )(), int, int, ... );