// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static Timer triggerTimer;
  const uint HeroUID = 0xFFFFFFFE;


  zTTriggerScript::zTTriggerScript() {
    Self   = Null;
    Other  = Null;
    Victim = Null;

    ZeroMemory( &Parser, sizeof( Parser ) );
    Parser.Enabled = True;

    TriggerScripts.InsertSorted( this );
    UpdateFirst();
  }


  bool zTTriggerScript::CallTrigger() {
    if( Function != Invalid && Parser.Enabled ) {
      parser->SetInstance( "SELF",   Self   );
      parser->SetInstance( "OTHER",  Other  );
      parser->SetInstance( "VICTIM", Victim );

      int Ok = *(int*)parser->CallFunc( Function );
      return Ok ? true : false;
    }

    // Delete after call ??
    return true;
  }


  bool zTTriggerScript::IsLocal() {
    return
      Self   != Null ||
      Other  != Null ||
      Victim != Null;
  }


  void zTTriggerScript::SetAsInstance( zSTRING instName ) {
    parser->SetInstance( instName, this );
  }


  uint NPC_2_UID( oCNpc* npc ) {
    if( npc == player )
      return HeroUID;

    if( npc )
      return ArchivedNpcUIDs.SearchEqual( npc );
    
    return Invalid;
  }


  oCNpc* UID_2_NPC( uint uid ) {
    if( uid == HeroUID )
      return player;

    if( uid != Invalid )
      return ArchivedNpcUIDs[uid];

    return Null;
  }


  void zTTriggerScript::Archive( zCArchiver& arc ) {
    arc.WriteString( "FunctionName", FunctionName );
    arc.WriteRaw   ( "Parser", &Parser, sizeof( Parser ) );
    arc.WriteInt   ( "Timer", triggerTimer[this].TimeHasPassed() );
    
    uint selfUID   = NPC_2_UID( Self   );
    uint OtherUID  = NPC_2_UID( Other  );
    uint VictimUID = NPC_2_UID( Victim );

    arc.WriteInt( "Self",   selfUID );
    arc.WriteInt( "Other",  OtherUID );
    arc.WriteInt( "Victim", VictimUID );
  }


  void zTTriggerScript::Unarchive( zCArchiver& arc ) {
    arc.ReadString( "FunctionName", FunctionName );
    Function = parser->GetIndex( FunctionName );

    arc.ReadRaw   ( "Parser", &Parser, sizeof( Parser ) );

    int_t passedTime;
    arc.ReadInt( "Timer", passedTime );
    triggerTimer[this].ResetTime().TimeAppend( passedTime );

    int selfUID   = 0;
    int OtherUID  = 0;
    int VictimUID = 0;

    arc.ReadInt( "Self",   selfUID );
    arc.ReadInt( "Other",  OtherUID );
    arc.ReadInt( "Victim", VictimUID );

    Self   = UID_2_NPC( selfUID   );
    Other  = UID_2_NPC( OtherUID  );
    Victim = UID_2_NPC( VictimUID );
  }


  zTTriggerScript* zTTriggerScript::Create( zSTRING funcName, int delay ) {
    zTTriggerScript* trigger = new zTTriggerScript();
    trigger->FunctionName    = funcName;
    trigger->Function        = parser->GetIndex( funcName );
    trigger->Parser.Delay    = delay;
    return trigger;
  }


  void zTTriggerScript::DoTriggers() {
    triggerTimer.ClearUnused();

    byte gamePause = ogame->IsOnPause() ? True : False;

    for( uint i = 0; i < TriggerScripts.GetNum(); i++ ) {
      auto& trigger = TriggerScripts[i];
      uint  delay   = trigger->Parser.Delay;

      // Stop timer processing on the game pause
      triggerTimer[trigger].Suspend( gamePause );
      if( gamePause )
        continue;

      if( triggerTimer[trigger].AwaitExact( delay ) ) {
        trigger->SetAsInstance( "SELFTRIGGER" );
        trigger->SetAsInstance( "ZPARSEREXTENDER:SELFTRIGGER" );
        if( trigger->CallTrigger() ) {
          delete trigger;
          i--;
        }
      }
    }
  }


  void zTTriggerScript::UpdateFirst() {
    zTTriggerScript* first = GetFirstTrigger();
    parser->SetInstance( "FIRSTTRIGGER", first );
    parser->SetInstance( "ZPARSEREXTENDER:FIRSTTRIGGER", first );
  }


  zTTriggerScript* zTTriggerScript::GetFirstTrigger() {
    return TriggerScripts.GetNum() ? TriggerScripts.GetFirst() : Null;
  }


  void zTTriggerScript::LoadTriggers( zCArchiver& arc, bool temp ) {
    int_t triggersNum;
    arc.ReadInt( "TriggersNum", triggersNum );

    for( int_t i = 0; i < triggersNum; i++ ) {
      zTTriggerScript* trigger = new zTTriggerScript();
      trigger->Unarchive( arc );
    }
  }


  void zTTriggerScript::SaveTriggers( zCArchiver& arc, bool temp ) {
    uint triggersNum = 0;
    if( temp ) {
      for( uint i = 0; i < TriggerScripts.GetNum(); i++ )
        if( TriggerScripts[i]->IsLocal() )
          triggersNum++;
    }
    else
      triggersNum = TriggerScripts.GetNum();

    arc.WriteInt( "TriggersNum", triggersNum );
    for( uint i = 0; i < triggersNum; i++ )
      if( !temp || TriggerScripts[i]->IsLocal() )
        TriggerScripts[i]->Archive( arc );
  }


  void zTTriggerScript::ClearTriggers( bool temp ) {
    for( uint i = 0; i < TriggerScripts.GetNum(); i++ )
      if( !temp || TriggerScripts[i]->IsLocal() )
        delete TriggerScripts[i--];
  }

  void zTTriggerScript::RegisterClassOffset() {
    zTTriggerScript* trigger = Null;
    parser->AddClassOffset( Z "C_TRIGGER", (int)&trigger->Parser );
  }


  zTTriggerScript::~zTTriggerScript() {
    TriggerScripts.RemoveSorted( this );
    UpdateFirst();
  }


  string GetSlotNameByID( int ID ) {
    if( ID > 0 )
      return "savegame" + A ID;

    if( ID == 0 )
      return "quicksave";

    return "current";
  }


  string GetTriggersArchivePath( string tempName = "" ) {
    if( !tempName.IsEmpty() )
      tempName.Put( ".", 0 );

    int slotID         = SaveLoadGameInfo.slotID;
    string savesDir    = zoptions->GetDirString( zTOptionPaths::DIR_SAVEGAMES );
    string slotDir     = GetSlotNameByID( slotID );
    string archivePath = string::Combine( "%s\\%s\\Triggers%s.sav", savesDir, slotDir, tempName );
    return archivePath;
  }


  // Load
  void LoadTriggers( string archiveName, bool temp ) {
    zTTriggerScript::ClearTriggers( temp );
    zCArchiver* ar = zarcFactory->CreateArchiverRead( archiveName, 0 );

    if( ar ) {
      zTTriggerScript::LoadTriggers( *ar, temp );
      ar->Close();
      ar->Release();
    }

    ArchivedNpcUIDs.Clear();
  }


  // Global
  void LoadTriggers_Global() {
    string archiveName = GetTriggersArchivePath();
    LoadTriggers( archiveName, false );
  }


  // Temporary
  void LoadTriggers_Temporary( string __worldName ) {
    string worldName = __worldName.Replace( "\\", "." ).GetPattern( "", ".", -1 );
    string archiveName = GetTriggersArchivePath( worldName );

    LoadTriggers( archiveName, true );
  }


  // Save
  void SaveTriggers( string archiveName, bool temp ) {
    zCArchiver* ar = zarcFactory->CreateArchiverWrite( archiveName, zARC_MODE_ASCII, 0, 0 );

    if( ar ) {
      zTTriggerScript::SaveTriggers( *ar, temp );
      ar->Close();
      ar->Release();
    }

    ArchivedNpcUIDs.Clear();
  }

  // Global
  void SaveTriggers_Global() {
    string archiveName = GetTriggersArchivePath();
    SaveTriggers( archiveName, false );
  }

  // Temporary
  void SaveTriggers_Temporary( string __worldName ) {
    string worldName   = __worldName.Replace( "\\", "." ).GetPattern( "", ".", -1 );
    string archiveName = GetTriggersArchivePath( worldName );

    SaveTriggers( archiveName, true );
  }


  HOOK Hook_oCNpc_Archive PATCH( &oCNpc::Archive, &oCNpc::Archive_Union );

  void oCNpc::Archive_Union( zCArchiver& ar ) {
    THISCALL( Hook_oCNpc_Archive )( ar );
    ArchivedNpcUIDs.Insert( this );
  }


  HOOK Hook_oCNpc_Unarchive PATCH( &oCNpc::Unarchive, &oCNpc::Unarchive_Union );

  void oCNpc::Unarchive_Union( zCArchiver& ar ) {
    THISCALL( Hook_oCNpc_Unarchive )(ar);
    ArchivedNpcUIDs.Insert( this );
  }
}