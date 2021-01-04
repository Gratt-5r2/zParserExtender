META
{
    Parser    = Game;
    MergeMode = true;
    Engine    = G2A;
};

func void b_enter_newworld()
{
    Hlp_PrintConsole("HookTest 2");
    
    var string a;
    var string b;
    var string y;
    a = "text 1/3";
    b = "text 2/3";
    y = "text 3/3";
    Hlp_PrintConsole(a);
    Hlp_PrintConsole(b);
    Hlp_PrintConsole(y);
    
    b_enter_newworld_Old();
};