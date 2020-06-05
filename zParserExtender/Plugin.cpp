// This file added in headers queue
// File: "Sources.h"

namespace GOTHIC_ENGINE {

  string DataDirectory    = "_work\\data\\";
  string ScriptsDirectory = "_Work\\Data\\Scripts\\";
  string SystemDirectory  = "System\\";
  string AutorunDirectory = "System\\Autorun\\";



  void DefinePaths() {
    CDocument Paths;
    Paths.ReadFromFile( Union.GetSystemDirectory() + "Paths.d" );
    
    for( uint i = 0; i < Paths.GetNum(); i++ ) {
      string& line = Paths[i];

      if( line.HasWord( "DIR_DATA" ) ) {
        DataDirectory = line.GetPattern( "\"", "\"" );
        if( DataDirectory.First() == '\\' )
          DataDirectory.Cut( 0, 1 );
      }
      else if( line.HasWord( "DIR_SCRIPTS" ) ) {
        ScriptsDirectory = line.GetPattern( "\"", "\"" );
        if( ScriptsDirectory.First() == '\\' )
          ScriptsDirectory.Cut( 0, 1 );

        ScriptsDirectory.Replace( "$DATA$\\", DataDirectory );
      }
      else if( line.HasWord( "DIR_SYSTEM" ) ) {
        SystemDirectory = line.GetPattern( "\"", "\"" );
        if( SystemDirectory.First() == '\\' )
          SystemDirectory.Cut( 0, 1 );

        SystemDirectory.Replace( "$DATA$", DataDirectory );
        SystemDirectory += "Autorun\\";
        AutorunDirectory = SystemDirectory + "Autorun\\";
      }
    }
  }



  void Game_Entry() {
    DefinePaths();
  }

  void Game_Init() {
    zParserExtender.ParseBegin();
    parser->CallGameInit_Union();
  }

  void Game_Exit() {
  }

  void Game_Loop() {
    parser->CallGameLoop_Union();
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

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
    DefineExternals();
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals
  );
}