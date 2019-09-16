// Supported with union (c) 2018 Union team

#ifndef __OCS_PLAYER_H__VER0__
#define __OCS_PLAYER_H__VER0__

#include "zCCsPlayer.h"

namespace Gothic_I_Classic {

  class oCCSPlayer : public zCCSPlayer {
  public:
    zCLASS_DECLARATION( oCCSPlayer )

    void oCCSPlayer_OnInit()                                            zCall( 0x00403750 );
    oCCSPlayer()                                                        zInit( oCCSPlayer_OnInit() );
    static void operator delete( void*, char const*, char const*, int ) zCall( 0x004022C0 );
    static zCObject* _CreateNewInstance()                               zCall( 0x004036C0 );
    /* for zCObject num : 21*/
    virtual zCClassDef* _GetClassDef() const                            zCall( 0x00403740 );
    virtual ~oCCSPlayer()                                               zCall( 0x004037A0 );
    virtual void ProcessCutscene( zCCSCutsceneContext*, zVEC3 const& )  zCall( 0x004037C0 );
    /* for zCCSDebugger num : 7*/

    // compatible with g2 operators style
    zOperatorDelete( oCCSPlayer, zCCSPlayer )
  };

} // namespace Gothic_I_Classic

#endif // __OCS_PLAYER_H__VER0__