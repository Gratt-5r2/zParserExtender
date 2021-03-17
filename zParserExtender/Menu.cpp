// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  // int __cdecl DefineMenuScriptFunctions(void)
  int DefineMenuScriptFunctions_Union();


  int Search_Items_Automatically() {
    zCParser* par     = zCParser::GetParser();
    zCMenu* empty     = (zCMenu*)Null;
    int offset        = (int)&empty->itemID[0] - (int)&empty->backPic;
    zCPar_Symbol* sym = (zCPar_Symbol*)zCPar_Symbol::GetUseInstance();
    zSTRING* itemID   = (zSTRING*)(sym->GetOffset() + offset);
    auto& symTable    = par->symtab.table;
    zSTRING mask      = "";

    par->GetParameter( mask );
    for( uint i = 0, id = 0; i < symTable.GetNum(); i++ ) {
      zCPar_Symbol* sym = symTable[i];
      string& symName   = (string&)sym->name;
      if( symName.CompareMasked( mask ) ) {
        if( symName.EndWith( "BACK" ) )
          itemID[100] = sym->name;
        else
          itemID[id++] = sym->name;
      }
    }

    return True;
  }



  HOOK Hook_DefineMenuScriptFunctions PATCH( &DefineMenuScriptFunctions, &DefineMenuScriptFunctions_Union );

  int DefineMenuScriptFunctions_Union() {
    Hook_DefineMenuScriptFunctions();
    zCParser* parser = zCMenu::GetParser();
    parser->DefineExternal( "Search_Items_Automatically", Search_Items_Automatically, zPAR_TYPE_VOID, zPAR_TYPE_STRING, 0 );
    return True;
  }
}