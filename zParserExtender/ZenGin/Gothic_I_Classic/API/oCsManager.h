// Supported with union (c) 2018 Union team

#ifndef __OCS_MANAGER_H__VER0__
#define __OCS_MANAGER_H__VER0__

#include "zCCsManager.h"

namespace Gothic_I_Classic {

  class oCCSManager : public zCCSManager {
  public:
    zCLASS_DECLARATION( oCCSManager )

    void oCCSManager_OnInit()                                           zCall( 0x00401430 );
    oCCSManager()                                                       zInit( oCCSManager_OnInit() );
    static zCObject* _CreateNewInstance()                               zCall( 0x004012F0 );
    static void operator delete( void*, char const*, char const*, int ) zCall( 0x00401360 );
    virtual zCClassDef* _GetClassDef() const                            zCall( 0x00402170 );
    virtual ~oCCSManager()                                              zCall( 0x00402210 );
    virtual zCEventMessage* CreateMessage( int )                        zCall( 0x004022F0 );
    virtual zCEventMessage* CreateOuMessage()                           zCall( 0x004022E0 );
    virtual zCCSPlayer* CreateCutscenePlayer( zCWorld* )                zCall( 0x00402220 );
    virtual zCCSProps* CreateProperties()                               zCall( 0x00402B50 );

    // compatible with g2 operators style
    zOperatorDelete( oCCSManager, zCCSManager )
  };

} // namespace Gothic_I_Classic

#endif // __OCS_MANAGER_H__VER0__