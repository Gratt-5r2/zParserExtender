#include "UnionAfx.h"
#include <crtversion.h>
#include "ParserExtender.h"

string LoadScript;
bool   MergeMode  = true;
bool   CompileDat = false;

extern "C"
int __stdcall DllMain( HPLUGIN hModule, DWORD fdwReason, LPVOID lpvReserved )
{
  if( fdwReason == DLL_PROCESS_ATTACH )
  {
    Union.DefineCRTVersion( _VC_CRT_MAJOR_VERSION, _VC_CRT_MINOR_VERSION, _VC_CRT_BUILD_VERSION, _VC_CRT_RBUILD_VERSION );
    COption& option = Union.GetGameOption();
    option.Read( LoadScript, "zParse_Extender", "LoadScript" );
    option.Read( MergeMode,  "zParse_Extender", "MergeMode",  false );
    option.Read( CompileDat, "zParse_Extender", "CompileDat", false );
    option.SaveAll();
  }
  if( fdwReason == DLL_PROCESS_DETACH )
  {

  }
  return True;
}