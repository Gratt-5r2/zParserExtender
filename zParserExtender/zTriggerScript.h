// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  struct zTTriggerScript {
    zSTRING FunctionName;
    int Function;

    oCNpc* Self;
    oCNpc* Other;
    oCNpc* Victim;

    struct {
      int Delay;
      int Enabled;
      int AIVariables[16];
    }
    Parser;

    zTTriggerScript();
    bool CallTrigger();
    bool IsLocal();
    void SetAsInstance( zSTRING instName );
    void Archive( zCArchiver& arc );
    void Unarchive( zCArchiver& arc );
   ~zTTriggerScript();

    static zTTriggerScript* Create( zSTRING funcName, int delay );
    static void DoTriggers();
    static void UpdateFirst();
    static zTTriggerScript* GetFirstTrigger();
    static void LoadTriggers( zCArchiver& arc, bool temp );
    static void SaveTriggers( zCArchiver& arc, bool temp );
    static void ClearTriggers( bool temp );
  protected:

    friend int AI_GetTriggerByID();
    friend int AI_GetTriggersNum();
    friend int AI_GetNextTriggerByFunc();
    friend int AI_GetNextTriggerByFuncName();
    friend int AI_GetNextTriggerBySelf();
    friend int AI_GetNextTriggerByOther();
    friend int AI_GetNextTriggerByVictim();
    friend int AI_GetNextTriggerByNPCs();

    static Array<zTTriggerScript*> TriggerScripts;
  };


  Array<zTTriggerScript*> zTTriggerScript::TriggerScripts;
  Array<oCNpc*> ArchivedNpcUIDs;
}