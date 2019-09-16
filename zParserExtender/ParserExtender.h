#pragma once

#ifdef __G1
#define NAMESPACE Gothic_I_Classic
#include "ParserExtender_Header.h"
#endif

#ifdef __G1A
#define NAMESPACE Gothic_I_Addon
#include "ParserExtender_Header.h"
#endif

#ifdef __G2
#define NAMESPACE Gothic_II_Classic
#include "ParserExtender_Header.h"
#endif

#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#include "ParserExtender_Header.h"
#endif

extern string LoadScript;
extern bool   MergeMode;
extern bool   CompileDat;
extern bool   ParseExternals;

inline bool IsMergeMode() {
  return MergeMode && ParseExternals;
}

inline bool IsCompileDat() {
  return CompileDat || !ParseExternals;
}