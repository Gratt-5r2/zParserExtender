// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  struct zTTriggerScript {
    zTTriggerScript* Prev;
    zTTriggerScript* Next;

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
    void SetAsInstance( zSTRING instName );
    void Archive( zCArchiver& arc );
    void Unarchive( zCArchiver& arc );
   ~zTTriggerScript();

    static zTTriggerScript* Create( zSTRING funcName, int delay );
    static void DoTriggers();
    static void UpdateFirst();
    static zTTriggerScript* GetFirstTrigger();
    static void LoadTriggers( zCArchiver& arc );
    static void SaveTriggers( zCArchiver& arc );
  protected:
    static Array<zTTriggerScript*> TriggerScripts;
  };



  Array<zTTriggerScript*> zTTriggerScript::TriggerScripts;

  Array<oCNpc*> ArchivedNpcUIDs;
}