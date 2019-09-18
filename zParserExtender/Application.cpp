#include "UnionAfx.h"
#include "ParserExtender.h"

extern bool ParseExternals = false;

void Game_Entry() {
}

void Game_Init() {
  ParseExternals = true;
#ifdef __G1
  if( Union.GetEngineVersion() == Engine_G1 )
    Gothic_I_Classic::ParseExternalScripts();
#endif
#ifdef __G1A
  if( Union.GetEngineVersion() == Engine_G1A )
    Gothic_I_Addon::ParseExternalScripts();
#endif
#ifdef __G2
  if( Union.GetEngineVersion() == Engine_G2 )
    Gothic_II_Classic::ParseExternalScripts();
#endif
#ifdef __G2A
  if( Union.GetEngineVersion() == Engine_G2A )
    Gothic_II_Addon::ParseExternalScripts();
#endif
  ParseExternals = false;
}

void Game_Exit() {
}

void Game_Loop() {
}

void Game_SaveBegin() {
}

void Game_SaveEnd() {
}

void LoadBegin() {
}

void LoadEnd() {
}

void Game_LoadBegin_NewGame() {
  LoadBegin();
}

void Game_LoadEnd_NewGame() {
 LoadEnd();
}

void Game_LoadBegin_SaveGame() {
 LoadBegin();
}

void Game_LoadEnd_SaveGame() {
 LoadEnd();
}

void Game_LoadBegin_ChangeLevel() {
 LoadBegin();
}

void Game_LoadEnd_ChangeLevel() {
 LoadEnd();
}

void Game_LoadBegin_Trigger() {
}

void Game_LoadEnd_Trigger() {
}

void Game_Pause() {
}

void Game_Unpause() {
}

void Game_DefineExternals() {
#ifdef __G1
  if( Union.GetEngineVersion() == Engine_G1 )
    Gothic_I_Classic::DefineExternals();
#endif
#ifdef __G1A
  if( Union.GetEngineVersion() == Engine_G1A )
    Gothic_I_Addon::DefineExternals();
#endif
#ifdef __G2
  if( Union.GetEngineVersion() == Engine_G2 )
    Gothic_II_Classic::DefineExternals();
#endif
#ifdef __G2A
  if( Union.GetEngineVersion() == Engine_G2A )
    Gothic_II_Addon::DefineExternals();
#endif
}

CApplication* lpApplication = CApplication::CreateRefApplication (
  Game_Entry,
  Game_Init,
  Game_Exit,
  Game_Loop,
  Game_SaveBegin,
  Game_SaveEnd,
  Game_LoadBegin_NewGame,
  Game_LoadEnd_NewGame,
  Game_LoadBegin_SaveGame,
  Game_LoadEnd_SaveGame,
  Game_LoadBegin_ChangeLevel,
  Game_LoadEnd_ChangeLevel,
  Game_LoadBegin_Trigger,
  Game_LoadEnd_Trigger,
  Game_Pause,
  Game_Unpause,
  Game_DefineExternals
  );