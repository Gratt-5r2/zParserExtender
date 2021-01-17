// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

void Archive_Union( zCArchiver& ar );
void Unarchive_Union( zCArchiver& ar );
int EV_CallScript_Union( oCMsgManipulate* );
int IsValidFocusVob( int ptr, int lastPrio );
int ApplyOverlay_AtFirst( zSTRING const& );
void Archive2( zCArchiver& );
void Unarchive2( zCArchiver& );
void InitByScript_Union( int, int );