// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static Timer triggerTimer;


  zTTriggerScript::zTTriggerScript() {
    Prev = Null;
    Next = Null;
    ZeroMemory( &Parser, sizeof( Parser ) );
    Parser.Enabled = True;

    Prev = TriggerScripts.GetNum() ? TriggerScripts.GetLast() : Null;
    if( Prev )
      Prev->Next = this;

    TriggerScripts += this;
    UpdateFirst();
  }



  bool zTTriggerScript::CallTrigger() {
    if( Function != Invalid && Parser.Enabled ) {
      parser->SetInstance( "SELF", Self );
      parser->SetInstance( "OTHER", Other );
      parser->SetInstance( "VICTIM", Victim );

      int Ok = *(int*)parser->CallFunc( Function );
      return Ok ? true : false;
    }

    // Delete after call ??
    return true;
  }



  void zTTriggerScript::SetAsInstance( zSTRING instName ) {
    parser->SetInstance( "SelfTrigger", &Parser );
  }



  void zTTriggerScript::Archive( zCArchiver& arc ) {
    arc.WriteString( "FunctionName", FunctionName );
    arc.WriteRaw( "Parser", &Parser, sizeof( Parser ) );
    arc.WriteInt( "Timer", triggerTimer[this].TimeHasPassed() );

    uint selfUID   = Self   ? ArchivedNpcUIDs.SearchEqual( Self )   : Invalid;
    uint OtherUID  = Other  ? ArchivedNpcUIDs.SearchEqual( Other )  : Invalid;
    uint VictimUID = Victim ? ArchivedNpcUIDs.SearchEqual( Victim ) : Invalid;

    arc.WriteInt( "Self", selfUID );
    arc.WriteInt( "Other", OtherUID );
    arc.WriteInt( "Victim", VictimUID );
  }



  void zTTriggerScript::Unarchive( zCArchiver& arc ) {
    arc.ReadString( "FunctionName", FunctionName );
    Function = parser->GetIndex( FunctionName );

    arc.ReadRaw( "Parser", &Parser, sizeof( Parser ) );

    int_t passedTime;
    arc.ReadInt( "Timer", passedTime );
    triggerTimer[this].ResetTime().TimeAppend( passedTime );

    int selfUID = 0;
    int OtherUID = 0;
    int VictimUID = 0;

    arc.ReadInt( "Self", selfUID );
    arc.ReadInt( "Other", OtherUID );
    arc.ReadInt( "Victim", VictimUID );

    Self   = selfUID   != Invalid ? ArchivedNpcUIDs[selfUID]   : Null;
    Other  = OtherUID  != Invalid ? ArchivedNpcUIDs[OtherUID]  : Null;
    Victim = VictimUID != Invalid ? ArchivedNpcUIDs[VictimUID] : Null;
  }



  zTTriggerScript* zTTriggerScript::Create( zSTRING funcName, int delay ) {
    zTTriggerScript* trigger = new zTTriggerScript();
    trigger->FunctionName = funcName;
    trigger->Function = parser->GetIndex( funcName );
    trigger->Parser.Delay = delay;
    return trigger;
  }



  void zTTriggerScript::DoTriggers() {
    triggerTimer.ClearUnused();

    byte gamePause = ogame->IsOnPause() ? True : False;



    for( uint i = 0; i < TriggerScripts.GetNum(); i++ ) {
      auto trigger = TriggerScripts[i];
      uint delay = trigger->Parser.Delay;

      // Stop timer processing on the game pause
      triggerTimer[trigger].Suspend( gamePause );
      if( gamePause )
        continue;

      if( triggerTimer[trigger].Await( delay ) ) {
        trigger->SetAsInstance( "SelfTrigger" );
        if( trigger->CallTrigger() ) {
          delete trigger;
          i--;
        }
      }
    }
  }



  void zTTriggerScript::UpdateFirst() {
    zTTriggerScript* first = GetFirstTrigger();
    parser->SetInstance( "FirstTrigger", &first->Parser );
  }



  zTTriggerScript* zTTriggerScript::GetFirstTrigger() {
    return TriggerScripts.GetNum() ? TriggerScripts.GetFirst() : Null;
  }



  void zTTriggerScript::LoadTriggers( zCArchiver& arc ) {
    TriggerScripts.Clear();
    int_t triggersNum;

    arc.ReadInt( "TriggersNum", triggersNum );
    for( int_t i = 0; i < triggersNum; i++ ) {
      zTTriggerScript* trigger = new zTTriggerScript();
      trigger->Unarchive( arc );
    }
  }



  void zTTriggerScript::SaveTriggers( zCArchiver& arc ) {
    uint triggersNum = TriggerScripts.GetNum();

    arc.WriteInt( "TriggersNum", triggersNum );
    for( uint i = 0; i < triggersNum; i++ )
      TriggerScripts[i]->Archive( arc );
  }



  zTTriggerScript::~zTTriggerScript() {
    if( Prev )
      Prev->Next = Next;

    if( Next )
      Next->Prev = Prev;

    TriggerScripts -= this;
    UpdateFirst();
  }





  string GetTriggersArchivePath() {
    string savesDir = zoptions->GetDirString( zTOptionPaths::DIR_SAVEGAMES );
    string slotDir = "savegame" + A SaveLoadGameInfo.slotID;
    string archivePath = string::Combine( "%s\\%s\\Triggers.sav", savesDir, slotDir );
    return archivePath;
  }

  void LoadTriggers() {
    zCArchiver* ar = zarcFactory->CreateArchiverRead( GetTriggersArchivePath(), 0 );
    cmd << GetTriggersArchivePath() << endl;

    if( ar ) {
      zTTriggerScript::LoadTriggers( *ar );
      ar->Close();
      ar->Release();
    }

    ArchivedNpcUIDs.Clear();
  }

  void SaveTriggers() {
    zCArchiver* ar = zarcFactory->CreateArchiverWrite( GetTriggersArchivePath(), zARC_MODE_ASCII, 0, 0 );
    cmd << GetTriggersArchivePath() << " " << (int)ar << endl;

    if( ar ) {
      zTTriggerScript::SaveTriggers( *ar );
      ar->Close();
      ar->Release();
    }

    ArchivedNpcUIDs.Clear();
  }



  HOOK Hook_oCNpc_Archive PATCH( &oCNpc::Archive, &oCNpc::Archive_Union );

  void oCNpc::Archive_Union( zCArchiver& ar ) {
    THISCALL( Hook_oCNpc_Archive )(ar);
    ArchivedNpcUIDs.Insert( this );
  }



  HOOK Hook_oCNpc_Unarchive PATCH( &oCNpc::Unarchive, &oCNpc::Unarchive_Union );

  void oCNpc::Unarchive_Union( zCArchiver& ar ) {
    THISCALL( Hook_oCNpc_Unarchive )(ar);
    ArchivedNpcUIDs.Insert( this );
  }
}