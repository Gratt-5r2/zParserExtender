// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Ivk_zCPar_SymbolTable_Load_Union AS( &zCPar_SymbolTable::Load,   &zCPar_SymbolTable::Load_Union );
  HOOK Ivk_zCPar_SymbolTable_Insert     AS( &zCPar_SymbolTable::Insert, &zCPar_SymbolTable::Insert_Union );





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
      if( Symbol->type == zPAR_TYPE_FUNC ) {
        if( !Symbol->name.HasWordI( ParentFuncName ) ) {
          ParentFuncSymbol = Symbol;
          PreviousSymbol = Symbol;
          ParentFuncName = Symbol->name;
        }
      }

      // check arguments and define `next` pointers
      if( ParentFuncSymbol ) {
        if( Symbol->name.HasWordI( ParentFuncName ) ) {
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

    int index = GetIndex( sym->name );
    if( index != Invalid ) {
      zCPar_Symbol* oldSym = table[index];

      // Replace external func to internal
      bool externalFunc = !sym->HasFlag( zPAR_FLAG_EXTERNAL ) && oldSym->HasFlag( zPAR_FLAG_EXTERNAL );

      if( externalFunc || zParserExtender.MergeModeEnabled() ) {

        table[index] = sym;
        sym->AllocSpace();

        oldSym->Rename( oldSym->GetName() + "_OLD" );
        InsertAt_Union( oldSym, False );

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