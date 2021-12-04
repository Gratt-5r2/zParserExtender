// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
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


  static int SwitchToWaitFocusReturn = 0;

  int Open_Link() {
    zCParser* par = zCParser::GetParser();
    zSTRING link;
    par->GetParameter(link);

    if( zrenderer->Vid_GetScreenMode() == zRND_SCRMODE_FULLSCREEN ) {
      zrenderer->Vid_SetScreenMode( zRND_SCRMODE_WINDOWED );
      SwitchToWaitFocusReturn = 2;
    }

    system( "start " + link );
    return 0;
  }

  void WaitFocusReturn() {
    if( SwitchToWaitFocusReturn == 2 ) {
      if( GetForegroundWindow() != hWndApp )
        SwitchToWaitFocusReturn = 1;
    }
    else if( SwitchToWaitFocusReturn == 1 ) {
      if( GetForegroundWindow() == hWndApp ) {
        SwitchToWaitFocusReturn = 0;
        zrenderer->Vid_SetScreenMode( zRND_SCRMODE_FULLSCREEN );
      }
    }
  }


  bool ActivateDynamicExternal_Menu( const zSTRING& funcName, bool createFuncList ) {
    zCParser* par = Null;
    if( !createFuncList ) {
      par = zCParser::GetParser();
      if( par->GetIndex( funcName ) != Invalid )
        return true;

      if( funcName.IsEmpty() )
        return false;
    }

    EXTERNAL_DEFINE_BEGIN( Menu_SearchItems ) zPAR_TYPE_VOID, zPAR_TYPE_STRING EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Open_Link        ) zPAR_TYPE_VOID, zPAR_TYPE_STRING EXTERNAL_DEFINE_END

    return false;
  }
}