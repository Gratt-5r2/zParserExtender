// Supported with union (c) 2020 Union team

// User API for zCPar_SymbolTable
// Add your methods here

void PostDefineExternal_Union( zCPar_Symbol* external );
void Load_Union( zFILE* f );
int Insert_Union( zCPar_Symbol* sym );
int InsertAt_Union( zCPar_Symbol* sym, int alloc );