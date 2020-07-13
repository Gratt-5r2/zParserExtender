// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Hook_oCItem_InitByScript AS( &oCItem::InitByScript, &oCItem::InitByScript_Union );

  void oCItem::InitByScript_Union( int a1, int a2 ) {
    int index = parser->GetIndex( "ITEM" );
    parser->SetInstance( index, this );
    THISCALL( Hook_oCItem_InitByScript )( a1, a2 );
    parser->SetInstance( index, Null );
  }
}