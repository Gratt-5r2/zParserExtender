// Interface queue
#include "UnionAfx.h"
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;






namespace Gothic_I_Classic  { extern void ParseExternalScript( string parser, string script ); }
namespace Gothic_I_Addon    { extern void ParseExternalScript( string parser, string script ); }
namespace Gothic_II_Classic { extern void ParseExternalScript( string parser, string script ); }
namespace Gothic_II_Addon   { extern void ParseExternalScript( string parser, string script ); }

extern "C" __declspec(dllexport) void ParseExternalScript( string parser, string script ) {
#ifdef __G1
  if( Union.GetEngineVersion() == Engine_G1 )
    Gothic_I_Classic::ParseExternalScript( parser, script );
#endif
#ifdef __G1A
  if( Union.GetEngineVersion() == Engine_G1A )
    Gothic_I_Addon::ParseExternalScript( parser, script );
#endif
#ifdef __G2
  if( Union.GetEngineVersion() == Engine_G2 )
    Gothic_II_Classic::ParseExternalScript( parser, script );
#endif
#ifdef __G2A
  if( Union.GetEngineVersion() == Engine_G2A )
    Gothic_II_Addon::ParseExternalScript( parser, script );
#endif
}







// Check executed engine with current source code
#define CHECK_THIS_ENGINE (Union.GetEngineVersion() == ENGINE)
#define Engine_G1  1
#define Engine_G1A 2
#define Engine_G2  3
#define Engine_G2A 4



// Include headers
#ifdef __G1
#define GOTHIC_ENGINE Gothic_I_Classic
#define ENGINE Engine_G1
#include "Headers.h"
#endif
#ifdef __G1A
#define GOTHIC_ENGINE Gothic_I_Addon
#define ENGINE Engine_G1A
#include "Headers.h"
#endif
#ifdef __G2
#define GOTHIC_ENGINE Gothic_II_Classic
#define ENGINE Engine_G2
#include "Headers.h"
#endif
#ifdef __G2A
#define GOTHIC_ENGINE Gothic_II_Addon
#define ENGINE Engine_G2A
#include "Headers.h"
#endif



// Include source files (with same as header parameters)
#ifdef __G1
#define GOTHIC_ENGINE Gothic_I_Classic
#define ENGINE Engine_G1
#include "Sources.h"
#endif
#ifdef __G1A
#define GOTHIC_ENGINE Gothic_I_Addon
#define ENGINE Engine_G1A
#include "Sources.h"
#endif
#ifdef __G2
#define GOTHIC_ENGINE Gothic_II_Classic
#define ENGINE Engine_G2
#include "Sources.h"
#endif
#ifdef __G2A
#define GOTHIC_ENGINE Gothic_II_Addon
#define ENGINE Engine_G2A
#include "Sources.h"
#endif