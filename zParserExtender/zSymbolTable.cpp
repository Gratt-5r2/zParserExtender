// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Ivk_zCPar_SymbolTable_Load_Union AS( &zCPar_SymbolTable::Load,   &zCPar_SymbolTable::Load_Union );
  HOOK Ivk_zCPar_SymbolTable_Insert     AS( &zCPar_SymbolTable::Insert, &zCPar_SymbolTable::Insert_Union );



  struct zTCallReplaceInfo {
    int_t ReplaceLength;
    zCParser* Parser;
    zCPar_Symbol* OldSymbol;
    zCPar_Symbol* NewSymbol;
  };

  Array<zTCallReplaceInfo> CallReplaceInfos;



  // Find and replace call address to other function
  void ReplaceStackCallAddress( zCPar_Stack& stack, zCPar_Symbol* symLeft, zCPar_Symbol* symRight, int_t length ) {
    int oldPos = symLeft->single_intdata;
    int newPos = symRight->single_intdata;

    for( int_t i = 0; i < length; i++ ) {
      byte& command = stack.stack[i];
      if( command == zPAR_TOK_CALL ) {
        int& address = (int&)stack.stack[i + 1];
        if( address == oldPos ) {

          cmd << colParse2 << "zParserExtender: " <<
                 colParse1 << "func call "        <<
                 colParse2 << AHEX32( oldPos )    <<
                 colParse1 << " -> "              <<
                 colParse2 << AHEX32( newPos )    <<
                 colParse1 << " replaced."        <<
                 colParse3 << endl;

          (int&)stack.stack[i + 1] = newPos;
        }
      }
    }
  }



  void PostCompileCallReplace() {
    for( uint i = 0; i < CallReplaceInfos.GetNum(); i++ ) {
      if( CallReplaceInfos[i].Parser != zParserExtender.GetParser() )
        continue;

      ReplaceStackCallAddress(
        CallReplaceInfos[i].Parser->stack,
        CallReplaceInfos[i].OldSymbol,
        CallReplaceInfos[i].NewSymbol,
        CallReplaceInfos[i].ReplaceLength );

      CallReplaceInfos.RemoveAt( i-- );
    }
  }




  void zCPar_SymbolTable::PostDefineExternal_Union( zCPar_Symbol* external ) {
    // Restore function & arguments of function
    zCPar_Symbol* Symbol = external;
    while( Symbol ) {
      Insert_Union( Symbol );
      Symbol = Symbol->next;

      if( Symbol == external )
        break;
    }
  }





  void zCPar_SymbolTable::Load_Union( zFILE* f ) {
    zCPar_Symbol* Symbol;
    Array<zCPar_Symbol*> Externals;

    if( !zParserExtender.MergeModeEnabled() ) {
      for( int i = 0; i < table.GetNumInList(); i++ ) {
        Symbol = table[i];
        if( Symbol->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
          Externals += Symbol;
          table.RemoveIndex( i-- );
        }
      }
    }

    if( preAllocatedSymbols ) {
      delete[] preAllocatedSymbols;
      preAllocatedSymbols = Null;
      nextPreAllocated = Invalid;
    }

    // empty lists
    if( !zParserExtender.MergeModeEnabled() ) {
      table.EmptyList();
      tablesort.EmptyList();
    }

    int SymCount = 0;
    f->Read( &SymCount, sizeof( SymCount ) );

    // pre alloc memory
    table.AllocAbs( table.GetNum() + SymCount );
    tablesort.AllocAbs( tablesort.GetNum() + SymCount );
    tablesort.MarkNumAllocUsed();

    // read symbols on sorted table
    f->Read( tablesort.GetArray(), sizeof( tablesort[0] ) * SymCount );
    PreAlloc( SymCount );

    zSTRING ParentFuncName;
    zCPar_Symbol* ParentFuncSymbol = Null;
    zCPar_Symbol* PreviousSymbol = Null;

    for( int i = 0; i < SymCount; i++ ) {
      Symbol = preAllocatedSymbols + (nextPreAllocated--);
      Symbol->LoadFull( f );

      // is a symbol - function ??
      if( Symbol->type == zPAR_TYPE_FUNC || Symbol->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
        if( !Symbol->name.HasWordI( ParentFuncName ) ) {
          ParentFuncSymbol = Symbol;
          PreviousSymbol = Symbol;
          ParentFuncName = Symbol->name;
        }
      }

      // check arguments and define `next` pointers
      if( ParentFuncSymbol ) {
        if ( Symbol->name.GetWord( "." ) == ParentFuncName ) {
          PreviousSymbol->next = Symbol;
          PreviousSymbol = Symbol;
        }
        else
          ParentFuncSymbol = Null;
      }

      // load parent symbol
      bool32 ParentIndex = Invalid;
      f->Read( &ParentIndex, sizeof( ParentIndex ) );
      if( ParentIndex != Invalid )
        Symbol->SetParent( GetSymbol( ParentIndex ) );

      table.InsertEnd( Symbol );
    }

    // restore external functions
    if( !zParserExtender.MergeModeEnabled() ) {
      for( uint i = 0; i < Externals.GetNum(); i++ ) {
        zCPar_Symbol*& OldExternal = Externals[i];
        zCPar_Symbol* NewExternal = GetSymbol( OldExternal->name );

        if( NewExternal ) {
          int StackAddress;
          OldExternal->GetStackPos( StackAddress, 0 );
          NewExternal->SetStackPos( StackAddress, 0 );

          // copy arguments from old to new
          NewExternal->next = OldExternal->next;
          delete OldExternal;
        }
        else
          PostDefineExternal_Union( OldExternal );
      }
    }
  }





  int zCPar_SymbolTable::Insert_Union( zCPar_Symbol* sym ) {
    return InsertAt_Union( sym, True );
  }





  int zCPar_SymbolTable::InsertAt_Union( zCPar_Symbol* sym, int alloc ) {
    if( !sym )
      return False;

    if( zParserExtender.ExtendedParsingEnabled() && zParserExtender.GetParser() == Gothic::Parsers::PFX )
      zParserExtender.InsertPFXSymbol( sym );

    int index = GetIndex( sym->name ); // TO DO Safe ??
    if( index != Invalid ) {
      zCPar_Symbol* oldSym = table[index];

      // Replace external func to internal
      bool externalFunc = !sym->HasFlag( zPAR_FLAG_EXTERNAL ) && oldSym->HasFlag( zPAR_FLAG_EXTERNAL );

      if( externalFunc || zParserExtender.MergeModeEnabled() ) {

        table[index] = sym;
        sym->AllocSpace();

        oldSym->Rename( oldSym->GetName() + "_OLD" );
        InsertAt_Union( oldSym, False );

        cmd << colParse2 << "zParserExtender: " <<
               colParse1 << "symbol "           <<
               colParse2 << sym->name           <<
               colParse1 << " replaced."        <<
               colParse3 << endl;

        // Add function to the post compile queue.
        // This symbol will be replace the original
        // call address for new call address. For
        // calling origin func - use '_old' suffix.
        if( sym->type == zPAR_TYPE_FUNC && !oldSym->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
          zTCallReplaceInfo& callReplace = CallReplaceInfos.Create();
          callReplace.ReplaceLength      = zParserExtender.GetParser()->stack.stacksize - 4;
          callReplace.Parser             = zParserExtender.GetParser();
          callReplace.OldSymbol          = oldSym;
          callReplace.NewSymbol          = sym;
        }

        cur_table = this;
        return True;
      }
      else
        return False;
    }

    cur_table = this;

    table.InsertEnd( sym );
    tablesort.InsertSort( table.GetNumInList() - 1 );

    if( alloc )
      sym->AllocSpace();

    if( !firstsym ) firstsym = sym;
    if( lastsym )   lastsym->next = sym;
    lastsym = sym;

    return True;
  }
}