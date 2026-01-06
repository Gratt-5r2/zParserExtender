// Supported with union (c) 2020 Union team

// User API for zCPar_SymbolTable
// Add your methods here

void PostDefineExternal_Union( zCPar_Symbol* external );
void Load_Union( zFILE* f );
void RegisterEvent( const zSTRING& name );
int Insert_Union( zCPar_Symbol* sym );
int InsertAt_Union( zCPar_Symbol* sym, int alloc, bool checkHierarchy );
int GetIndex_Safe( zSTRING const& );
int __fastcall SearchNearest( const zSTRING& name, int low, int high );
int __fastcall Search_Union( const zSTRING& name, int low, int high );
void CheckNextSymbol( zCPar_Symbol* sym );
void RemoveSymbolSorted( zCPar_Symbol* sym );
void RestoreSymbolSorted( zCPar_Symbol* sym );
uint RenameSymbol( zCParser* scriptParser, zCPar_Symbol* sym, const zSTRING& newName, zCPar_Symbol* newSym );
bool ReplaceSymbol( zCPar_Symbol* sym, zCPar_Symbol* newSym );
static int Compare_Union( void const*, void const* );
zCPar_Symbol* zCPar_SymbolTable::GetSymbol_Union( const zSTRING& s );
void zCPar_SymbolTable::CheckSymbolsHierarchy();
void InsertEnd_Union( zCPar_Symbol* );
int GetIndex_Safe( zCPar_Symbol* );