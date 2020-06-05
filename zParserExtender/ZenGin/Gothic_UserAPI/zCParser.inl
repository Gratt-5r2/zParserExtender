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
void Reset_Union();