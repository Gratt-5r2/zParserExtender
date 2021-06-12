// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  // int __cdecl DefineMenuScriptFunctions(void)
  int DefineMenuScriptFunctions_Union();


  int Menu_SearchItems() {
    zCParser* par     = zCParser::GetParser();
    zCMenu* empty     = (zCMenu*)Null;
    int offset        = (int)&empty->itemID[0] - (int)&empty->backPic;
    zCPar_Symbol* sym = (zCPar_Symbol*)zCPar_Symbol::GetUseInstance();
    zSTRING* itemID   = (zSTRING*)(sym->GetOffset() + offset);
    auto& symTable    = par->symtab.table;
    zSTRING mask      = "";

    par->GetParameter( mask );
    for( int i = 0, id = 1; i < symTable.GetNum(); i++ ) {
      zCPar_Symbol* sym = symTable[i];
      string& symName   = (string&)sym->name;
      if( symName.CompareMasked( mask ) )
        itemID[id++] = sym->name;
    }

    return True;
  }


  int Open_Link() {
    zCParser* par = zCParser::GetParser();
    zSTRING link;
    par->GetParameter(link);

    if( zrenderer->Vid_GetScreenMode() == zRND_SCRMODE_FULLSCREEN )
      zrenderer->Vid_SetScreenMode( zRND_SCRMODE_WINDOWED );

    system( "start " + link );
    return 0;
  }



  HOOK Hook_DefineMenuScriptFunctions PATCH( &DefineMenuScriptFunctions, &DefineMenuScriptFunctions_Union );

  int DefineMenuScriptFunctions_Union() {
    Hook_DefineMenuScriptFunctions();
    zCParser* parser = zCMenu::GetParser();
    parser->DefineExternal( "Str_GetLocalizedString", Str_GetLocalizedString, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Str_UTF8_to_ANSI",       Str_UTF8_to_ANSI,       zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Str_GetCurrentCP",       Str_GetCurrentCP,       zPAR_TYPE_INT,    0 );
    parser->DefineExternal( "Menu_SearchItems",       Menu_SearchItems,       zPAR_TYPE_VOID,   zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Hlp_MessageBox",         Hlp_MessageBox,         zPAR_TYPE_VOID,   zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Hlp_PrintConsole",       Hlp_PrintConsole,       zPAR_TYPE_VOID,   zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Str_Format",             Str_Format,             zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Open_Link",              Open_Link,              zPAR_TYPE_VOID,   zPAR_TYPE_STRING, 0 );
    return True;
  }
}