// Supported with union (c) 2018 Union team

#ifndef __ZAI_H__VER1__
#define __ZAI_H__VER1__

namespace Gothic_I_Addon {

  class zCAIBase : public zCObject {
  public:
    zCLASS_DECLARATION( zCAIBase )

    zCAIBase() {}
    static void operator delete( void*, char const*, char const*, int ) zCall( 0x0048F1E0 );
    virtual zCClassDef* _GetClassDef() const                            zCall( 0x0048F200 );
    virtual ~zCAIBase()                                                 zCall( 0x0048F310 );
    virtual void DoAI( zCVob*, int& )                                   zPureCall;
    virtual int CanThisCollideWith( zCVob* )                            zCall( 0x0048F210 );
    virtual void ReportCollisionToAI( zCCollisionReport const& )        zCall( 0x0048F2C0 );
    virtual int HasAIDetectedCollision()                                zCall( 0x0048F220 );
    virtual void AICollisionResponseSelfDetected( zVEC3 const&, int& )  zCall( 0x0048F230 );
    virtual void HostVobRemovedFromWorld( zCVob*, zCWorld* )            zCall( 0x0048F240 );
    virtual void HostVobAddedToWorld( zCVob*, zCWorld* )                zCall( 0x0048F2D0 );

    // compatible with g2 operators style
    zOperatorDelete( zCAIBase, zCObject )
  };

  class zCAIBaseSound : public zCAIBase {
  public:
    zCLASS_DECLARATION( zCAIBaseSound )

    int slideSoundHandle;
    char slideSoundOn;

    zCAIBaseSound() {}
    void RemoveSlideSound()                                             zCall( 0x0050F7F0 );
    void CheckSlideSound( zCVob* )                                      zCall( 0x0050F810 );
    void StartDefaultCollisionSound( zCVob*, zCCollisionReport const& ) zCall( 0x0050F8A0 );
  };

} // namespace Gothic_I_Addon

#endif // __ZAI_H__VER1__