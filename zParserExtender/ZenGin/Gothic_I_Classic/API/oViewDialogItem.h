// Supported with union (c) 2018 Union team

#ifndef __OVIEW_DIALOG_ITEM_H__VER0__
#define __OVIEW_DIALOG_ITEM_H__VER0__

namespace Gothic_I_Classic {

  class oCViewDialogItem : public zCViewDialog {
  public:
    zCLASS_DECLARATION( oCViewDialogItem )

    oCItem* Item;

    void oCViewDialogItem_OnInit()                                      zCall( 0x00727050 );
    oCViewDialogItem()                                                  zInit( oCViewDialogItem_OnInit() );
    void __fastcall SetItem( oCItem* )                                  zCall( 0x00727140 );
    static zCObject* _CreateNewInstance()                               zCall( 0x00726F80 );
    static void operator delete( void*, char const*, char const*, int ) zCall( 0x00727020 );
    /* for zCObject num : 15*/
    virtual zCClassDef* _GetClassDef() const                            zCall( 0x00727040 );
    virtual ~oCViewDialogItem()                                         zCall( 0x007270B0 );
    virtual void __fastcall Blit()                                      zCall( 0x00727180 );
    /* for zCViewBase num : 8*/
    /* for oCViewDialogItem num : 1*/

    // compatible with g2 operators style
    zOperatorDelete( oCViewDialogItem, zCViewDialog )
  };

} // namespace Gothic_I_Classic

#endif // __OVIEW_DIALOG_ITEM_H__VER0__