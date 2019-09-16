// Supported with union (c) 2018 Union team

#ifndef __OMENU__HELP_H__VER3__
#define __OMENU__HELP_H__VER3__

#include "zMenu.h"

namespace Gothic_II_Addon {

  struct oSMenuKey {
    zSTRING name;
    zSTRING internalName;
    zSTRING desc;
    unsigned short key;

    oSMenuKey() {}
  };

  class oCMenu_Help : public zCMenu {
  public:
    zCArray<oSMenuKey> keys;

    oCMenu_Help() {}
  };

  class oCHelpScreen {
  public:
    oCMenu_Help* help;

    oCHelpScreen() {}
  };

} // namespace Gothic_II_Addon

#endif // __OMENU__HELP_H__VER3__