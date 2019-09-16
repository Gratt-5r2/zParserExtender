#include "UnionAfx.h"

void Game_Entry() {
}

void Game_Init() {
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