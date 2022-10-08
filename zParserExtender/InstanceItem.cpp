// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Hook_oCItem_InitByScript PATCH( &oCItem::InitByScript, &oCItem::InitByScript_Union );

  void* zCParser::GetInstance( const int& index ) {
    zCPar_Symbol* sym = GetSymbol( index );
    if( !sym )
      return Null;

    return (void*)sym->GetOffset();
  }


  void oCItem::InitByScript_Union( int a1, int a2 ) {
    int index = parser->GetIndex( "ITEM" );
    void* oldInstnace = parser->GetInstance( index );
    parser->SetInstance( index, this );
    THISCALL( Hook_oCItem_InitByScript )( a1, a2 );
    parser->SetInstance( index, oldInstnace );
  }


  HOOK Hook_oCNpc_AddItemEffects PATCH( &oCNpc::AddItemEffects, &oCNpc::AddItemEffects_Union );

  void oCNpc::AddItemEffects_Union( oCItem* item ) {
    int index = parser->GetIndex( "ITEM" );
    void* oldInstnace = parser->GetInstance( index );
    parser->SetInstance( index, this );
    THISCALL( Hook_oCNpc_AddItemEffects )(item);
    parser->SetInstance( index, oldInstnace );
  }


  HOOK Hook_oCNpc_RemoveItemEffects PATCH( &oCNpc::RemoveItemEffects, &oCNpc::RemoveItemEffects_Union );

  void oCNpc::RemoveItemEffects_Union( oCItem* item ) {
    int index = parser->GetIndex( "ITEM" );
    void* oldInstnace = parser->GetInstance( index );
    parser->SetInstance( index, this );
    THISCALL( Hook_oCNpc_RemoveItemEffects )(item);
    parser->SetInstance( index, oldInstnace );
  }
}