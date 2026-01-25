
namespace GOTHIC_ENGINE {


#if 0
#if ENGINE == G2A
  HOOK Hook_zCParser_DefineExternal AS( 0x007A0190, &zCParser::DefineExternal_Union );
#elif ENGINE == G2
  HOOK Hook_zCParser_DefineExternal AS( 0x00740750, &zCParser::DefineExternal_Union );
#elif ENGINE == G1A
  HOOK Hook_zCParser_DefineExternal AS( 0x00730820, &zCParser::DefineExternal_Union );
#else
  HOOK Hook_zCParser_DefineExternal = InvokeAuto( 0x006F6840, &zCParser::DefineExternal_Union );
#endif
#endif
  // HOOK Hook_zCParser_DefineExternal AS( &zCParser::DefineExternal, &zCParser::DefineExternal_Union);

#if 1

  // HOOK Hook_zCParser_DefineExternal = InvokeAuto( ZenDef( 0x006F6840, 0x00730820, 0x00740750, 0x007A0190), &zCParser::DefineExternal_Union);

  DWORD SwitchAddress( DWORD g1, DWORD g1a, DWORD g2, DWORD g2a ) {
    switch( Union.GetEngineVersion() ) {
#if ENGINE == Engine_G2A
    case Engine_G2A: return g2a;
#elif ENGINE == Engine_G2
    case Engine_G2: return g2;
#elif ENGINE == Engine_G1A
    case Engine_G1A: return g1a;
#elif ENGINE == Engine_G1
    case Engine_G1: return g1;
#else
    default: return 0;
#endif
    }
  }

  HOOK Hook_zCParser_DefineExternal = InvokeAuto( SwitchAddress( 0x006F6840, 0x00730820, 0x00740750, 0x007A0190 ), &zCParser::DefineExternal_Union);

  void zCParser::DefineExternal_Union( zSTRING const& name, int( __cdecl* proc )(), int return_type, int argument_type_first ... ) {
    zCPar_Symbol* sym = new zCPar_Symbol();

    sym->name = name;
    sym->name.Upper();
    sym->type = zPAR_TYPE_FUNC;
    sym->ele = 0;
    sym->SetFlag( zPAR_FLAG_CONST | zPAR_FLAG_EXTERNAL );

    if( return_type != zPAR_TYPE_VOID )
      sym->SetFlag( zPAR_FLAG_RETURN );

    sym->SetOffset( return_type );

    if( !symtab.Insert( sym ) ) {
      if( !compiled ) {
        auto origin = symtab.GetSymbol( sym->name );
        if( origin->type != return_type )
          Error( zSTRING( "Redefined External . " + name + ": origin type: " + zSTRING( (int)origin->type ) + " new type: " + zSTRING( (int)return_type ) ), 0 );

        int argument_type = argument_type_first;
        int argument_index = 0;
        va_list marker;
        va_start( marker, argument_type_first );
        while( argument_type != zPAR_TYPE_VOID ) {
          auto argument_name = sym->name + ".PAR" + zSTRING( argument_index );
          auto argument_symbol = symtab.GetSymbol( argument_name );

          if( !argument_symbol )
            Error( zSTRING( "Redefined External . " + name + " parameter " + zSTRING( argument_index ) + ": not found" ), 0 );

          if( argument_symbol->type != argument_type )
            Error( zSTRING( "Redefined External . " + name + " parameter " + zSTRING( argument_index ) + ": origin type: " + zSTRING( (int)argument_symbol->type ) + " new type: " + zSTRING( (int)argument_type ) ), 0 );

          argument_type = va_arg( marker, int );
          ++argument_index;
        }
        va_end( marker );

        if( zCParserExtender::MessagesLevel >= 0 ) {
          Col16 cl1;
          Col16 cl2( CMD_YELLOW );
          Col16 cl3( CMD_YELLOW | CMD_INT );

          cmd << cl3 << "zParserExtender: "
            << cl2 << "same existing external reused - "
            << cl3 << name
            << cl1 << endl;
        }
      }

      zSTRING s = sym->name;
      delete(sym);
      sym = GetSymbol( s );
    }

    sym->SetStackPos( (int)proc, 0 );

    if( !compiled ) {
      zCPar_Symbol* par;
      int	argument_type = argument_type_first;
      va_list marker;
      va_start( marker, argument_type_first );
      while( argument_type != zPAR_TYPE_VOID ) {
        par = zNEW( zCPar_Symbol() );
        par->name = sym->name + ".PAR" + zSTRING( (int)sym->ele );
        par->type = argument_type;
        par->SetParent( NULL );
        symtab.Insert( par );
        sym->ele++;
        argument_type = va_arg( marker, int );
      }
      va_end( marker );
    }
  }
#endif
}