META
{
    Parser    = Game;
    MergeMode = true;
    Engine    = G2A;
};

func void b_enter_newworld()
{
    Hlp_PrintConsole("HookTest 1");
    
    var int x;
    var string y;
    x = 5000;
    y = IntToString(x);
    Hlp_PrintConsole(y);
    
    b_enter_newworld_Old();
};


