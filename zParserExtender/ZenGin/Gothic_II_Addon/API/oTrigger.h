// Supported with union (c) 2018 Union team

#ifndef __OTRIGGER_H__VER3__
#define __OTRIGGER_H__VER3__

#include "zVob.h"
#include "zVobMisc.h"
#include "zArchiver.h"

namespace Gothic_II_Addon {

  class oCTriggerChangeLevel : public zCTrigger {
  public:
    zCLASS_DECLARATION( oCTriggerChangeLevel )

    zSTRING levelName;
    zSTRING startVob;

    void oCTriggerChangeLevel_OnInit()                  zCall( 0x0043BCF0 );
    oCTriggerChangeLevel()                              zInit( oCTriggerChangeLevel_OnInit() );
    void SetLevelName( zSTRING const&, zSTRING const& ) zCall( 0x0043C140 );
    static zCObject* _CreateNewInstance()               zCall( 0x0043B940 );
    static void operator delete( void* )                zCall( 0x0043BA00 );
    static void* operator new( unsigned int )           zCall( 0x006D0050 );
    virtual zCClassDef* _GetClassDef() const            zCall( 0x0043BA20 );
    virtual void Archive( zCArchiver& )                 zCall( 0x0043C370 );
    virtual void Unarchive( zCArchiver& )               zCall( 0x0043C3B0 );
    virtual ~oCTriggerChangeLevel()                     zCall( 0x0043BD80 );
    virtual void TriggerTarget( zCVob* )                zCall( 0x0043BE20 );
    virtual void UntriggerTarget( zCVob* )              zCall( 0x0043C130 );
  };

  class oCTriggerScript : public zCTrigger {
  public:
    zCLASS_DECLARATION( oCTriggerScript )

    zSTRING scriptFunc;

    void oCTriggerScript_OnInit()             zCall( 0x0043C3F0 );
    oCTriggerScript()                         zInit( oCTriggerScript_OnInit() );
    void SetScriptFunc( zSTRING const&, int ) zCall( 0x0043CAB0 );
    static zCObject* _CreateNewInstance()     zCall( 0x0043BC20 );
    static void operator delete( void* )      zCall( 0x0043BCC0 );
    virtual zCClassDef* _GetClassDef() const  zCall( 0x0043BCE0 );
    virtual void Archive( zCArchiver& )       zCall( 0x0043CC00 );
    virtual void Unarchive( zCArchiver& )     zCall( 0x0043CC30 );
    virtual ~oCTriggerScript()                zCall( 0x0043C460 );
    virtual void TriggerTarget( zCVob* )      zCall( 0x0043C4C0 );
    virtual void UntriggerTarget( zCVob* )    zCall( 0x0043C820 );
  };

  class oCObjectGenerator : public zCVob {
  public:
    float speed;
    zSTRING objectName;

    void oCObjectGenerator_OnInit()          zCall( 0x0043CC60 );
    oCObjectGenerator()                      zInit( oCObjectGenerator_OnInit() );
    void SetObjectName( zSTRING const& )     zCall( 0x0043CD70 );
    void SetObjectSpeed( float )             zCall( 0x0043CEB0 );
    virtual void Archive( zCArchiver& )      zCall( 0x0043D1A0 );
    virtual void Unarchive( zCArchiver& )    zCall( 0x0043D1E0 );
    virtual ~oCObjectGenerator()             zCall( 0x0043CD10 );
    virtual void OnTrigger( zCVob*, zCVob* ) zCall( 0x0043CEC0 );
  };

} // namespace Gothic_II_Addon

#endif // __OTRIGGER_H__VER3__