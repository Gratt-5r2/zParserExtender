META
{
    Parser    = Game;
    MergeMode = true;
    Engine    = G2A;
};

func void b_enter_newworld()
{
    Hlp_PrintConsole("HookTest 3");
    b_enter_newworld_Old();
};