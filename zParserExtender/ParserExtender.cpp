#include "UnionAfx.h"
#include "ParserExtender.h"

#ifdef __G1
#define NAMESPACE Gothic_I_Classic
#define ZCPAR_SYMBOLTABLE_LOAD   0x006F9970
#define ZCPAR_SYMBOLTABLE_INSERT 0x006FA430
#define ZCPARSER_SAVEDAT         0x006E54C0
#define ZCPARSER_DECLAREFUNCCALL 0x006F4F80
#define ZCPARSER_PARSEBLOCK      0x006E6C00
#define zCURRENT_GAME            (Union.GetEngineVersion() == Engine_G1 ? IVK_AUTO : IVK_DISABLED)
#include "ParserExtender_Source.h"
#include "ParserExtender_Externals.h"
#endif

#ifdef __G1A
#define NAMESPACE Gothic_I_Addon
#define ZCPAR_SYMBOLTABLE_LOAD   0x00733CF0
#define ZCPAR_SYMBOLTABLE_INSERT 0x007347C0
#define ZCPARSER_SAVEDAT         0x0071DF50
#define ZCPARSER_DECLAREFUNCCALL 0x0072ECA0
#define ZCPARSER_PARSEBLOCK      0x0071F8E0
#define zCURRENT_GAME            (Union.GetEngineVersion() == Engine_G1A ? IVK_AUTO : IVK_DISABLED)
#include "ParserExtender_Source.h"
#include "ParserExtender_Externals.h"
#endif

#ifdef __G2
#define NAMESPACE Gothic_II_Classic
#define ZCPAR_SYMBOLTABLE_LOAD   0x00743A20
#define ZCPAR_SYMBOLTABLE_INSERT 0x007444C0
#define ZCPARSER_SAVEDAT         0x0072ED00
#define ZCPARSER_DECLAREFUNCCALL 0x0073ED70
#define ZCPARSER_PARSEBLOCK      0x007303F0
#define zCURRENT_GAME            (Union.GetEngineVersion() == Engine_G2 ? IVK_AUTO : IVK_DISABLED)
#include "ParserExtender_Source.h"
#include "ParserExtender_Externals.h"
#endif

#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#define ZCPAR_SYMBOLTABLE_LOAD   0x007A3460
#define ZCPAR_SYMBOLTABLE_INSERT 0x007A3F00
#define ZCPARSER_SAVEDAT         0x0078E740
#define ZCPARSER_DECLAREFUNCCALL 0x0079E7B0
#define ZCPARSER_PARSEBLOCK      0x0078FE30
#define zCURRENT_GAME            (Union.GetEngineVersion() == Engine_G2A ? IVK_AUTO : IVK_DISABLED)
#include "ParserExtender_Source.h"
#include "ParserExtender_Externals.h"
#endif