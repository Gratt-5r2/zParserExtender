// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <intrin.h>

namespace GOTHIC_ENGINE {
#if 0
  HOOK Hook_oCNpcFocus_IsValid PATCH( &oCNpcFocus::HasLowerPriority, &oCNpc::IsValidFocusVob );

  int oCNpc::IsValidFocusVob( int  ptr, int lastPrio ) {
    //if( _ReturnAddress() != (void*)0x00733FAE )
    //  return 1;// THISCALL( Hook_oCNpcFocus_IsValid )(ptr);

    static int G_CanTakeInFocus = parser->GetIndex( "G_CANTAKEINFOCUS" );
    static int Shared           = parser->GetIndex( "shared" );

    zCVob* vob = (zCVob*)ptr;
    if( G_CanTakeInFocus != Invalid && Shared != Invalid ) {
			parser->SetInstance( Shared, vob );
      int Ok = *(int*)parser->CallFunc( G_CanTakeInFocus );
      if( Ok != -1 )
        return Ok;
    }

    bool_t overrided = False;
    if( zDYNAMIC_CAST<oCMOB>( vob ) ) {
      oCMOB* mob = (oCMOB*)vob;
      overrided = mob->focusOverride;
    }

    if( !overrided )
      //if( !THISCALL( Hook_oCNpcFocus_IsValid )(vob->type) ) // oCNpcFocus::focus->IsValid( vob->type );
      if( !oCNpcFocus::focus->IsValid( vob->type ) || oCNpcFocus::focus->HasLowerPriority( vob->type, lastPrio ) )
        return False;

    return True;
  }




  FASTHOOK( oCSpell, IsValidTarget );

  int oCSpell::IsValidTarget_Union( zCVob* vob ) {
    return True;
  }
#endif
}