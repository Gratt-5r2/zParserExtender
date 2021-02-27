// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Hook_zCPar_SymbolTable_Load_Union AS( &zCPar_SymbolTable::Load,   &zCPar_SymbolTable::Load_Union   );
  HOOK Hook_zCPar_SymbolTable_Insert     AS( &zCPar_SymbolTable::Insert, &zCPar_SymbolTable::Insert_Union );



  struct zTCallReplaceInfo {
    static Array<zTCallReplaceInfo> CallReplaceInfos;

    int_t ReplaceLength;
    zCParser* Parser;
    zCPar_Symbol* OldSymbol;
    zCPar_Symbol* NewSymbol;
    int_t OldIndex;

    static zTCallReplaceInfo& Create( int_t replaceLength, zCParser* parser, zCPar_Symbol* oldSymbol, zCPar_Symbol* newSymbol, int_t oldIndex ) {
      for( uint i = 0; i < CallReplaceInfos.GetNum(); i++ ) {
        if( CallReplaceInfos[i].NewSymbol == oldSymbol ) {
          CallReplaceInfos[i].NewSymbol = newSymbol;
          CallReplaceInfos[i].OldIndex  = oldIndex;
          return CallReplaceInfos[i];
        }
      }

      zTCallReplaceInfo& callReplace = CallReplaceInfos.Create();
      callReplace.ReplaceLength      = zParserExtender.GetParser()->stack.GetDynSize() - 4;
      callReplace.Parser             = zParserExtender.GetParser();
      callReplace.OldSymbol          = oldSymbol;
      callReplace.NewSymbol          = newSymbol;
      callReplace.OldIndex           = oldIndex;
      return callReplace;
    }
  };

  Array<zTCallReplaceInfo> zTCallReplaceInfo::CallReplaceInfos;




  // Find and replace call address to other function
  void ReplaceStackCallAddress( zCPar_Stack& stack, zCPar_Symbol* symLeft, zCPar_Symbol* symRight, int_t oldIndex, int_t length ) {
    int oldPos = symLeft->single_intdata;
    int newPos = symRight->single_intdata;
    int calls  = 0;
    int refs   = 0;

    bool isFunction =
      symLeft->type == zPAR_TYPE_FUNC ||
      symLeft->type == zPAR_TYPE_PROTOTYPE ||
      symLeft->type == zPAR_TYPE_INSTANCE;

    if( isFunction ) {
      for( int_t i = 0; i < length; i++ ) { // TEST: start from first symbol position
        byte& command = stack.stack[i];
        if( command == zPAR_TOK_CALL ) {
          int& address = (int&)stack.stack[i + 1];
          if( address == oldPos ) {
            (int&)stack.stack[i + 1] = newPos;
            calls++;
          }
        }
      }
    }

    int newIndex = parser->symtab.GetIndex( symRight );
    for( int_t i = 0; i < length; i++ ) {
      byte& command = stack.stack[i];
      if( command >= zPAR_TOK_PUSHINT && command <= zPAR_TOK_PUSHSTR ) {
        int& address = (int&)stack.stack[i + 1];
        if( address == oldIndex ) {
          (int&)stack.stack[i + 1] = newIndex;
          refs++;
        }
      }
    }

    if( calls && zCParserExtender::MessagesLevel >= 3 )
      cmd << colParse2 << "zParserExtender: " <<
             colParse1 << "func calls " <<
             colParse2 << AHEX32( oldPos ) <<
             colParse1 << " -> " <<
             colParse2 << AHEX32( newPos ) <<
             colParse1 << " replaced (" <<
             colParse2 << calls <<
             colParse1 << ")\t" <<
             colParse2 << symRight->name <<
             colParse3 << endl;

    if( refs && zCParserExtender::MessagesLevel >= 3 )
      cmd << colParse2 << "zParserExtender: " <<
             colParse1 << "references " <<
             colParse2 << AHEX32( oldIndex ) <<
             colParse1 << " -> " <<
             colParse2 << AHEX32( newIndex ) <<
             colParse1 << " replaced (" <<
             colParse2 << refs <<
             colParse1 << ")\t" <<
             colParse2 << symRight->name <<
             colParse3 << endl;
  }

  

  void PostCompileCallReplace() {
    for( uint i = 0; i < zTCallReplaceInfo::CallReplaceInfos.GetNum(); i++ ) {
      zTCallReplaceInfo& info = zTCallReplaceInfo::CallReplaceInfos[i];
      if( info.Parser != zParserExtender.GetParser() )
        continue;

      ReplaceStackCallAddress(
        info.Parser->stack,
        info.OldSymbol,
        info.NewSymbol,
        info.OldIndex,
        min( ParStackMaxLength, info.ReplaceLength ) );

      zTCallReplaceInfo::CallReplaceInfos.RemoveAt( i-- );
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
        if( Symbol->name.GetWord( "." ) == ParentFuncName ) {
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
    return InsertAt_Union( sym, True, true );
  }




  void zCPar_SymbolTable::CheckNextSymbol( zCPar_Symbol* sym ) {
    static zCPar_Symbol* parent = Null;
    static string baseName = NullStr;
    string name = A sym->name;

    // Is a function ??
    if( !sym->name.HasWord( "." ) ) {
      parent = sym;
      baseName = name;
    }
    else if( parent ) {
      // Is a local variable ??
      if( name.StartWith( baseName + "." ) ) {
        parent->next = sym;
        parent = sym;
      }
    }
  }


  inline bool_t Par_IsBaseSymbol( zSTRING base, zCPar_Symbol* child ) {
    if( base.HasWord( "." ) || !child->name.HasWord( "." ) )
      return False;

    return child->name.GetWord( "." ) == base;
  }



  uint zCPar_SymbolTable::RenameSymbol( zCPar_Symbol* sym, const zSTRING& newName, zCPar_Symbol* newSym ) {
    uint collisions = 1;

    int nextIndex = GetIndex( newName );
    if( nextIndex != Invalid ) {
      zCPar_Symbol* nextSym = table[nextIndex];
      collisions += RenameSymbol( nextSym, nextSym->GetName() + "_OLD", sym );
    }

    cur_table          = this;
    zSTRING oldName    = sym->GetName();
    zCPar_Symbol* list = sym;

    while( list ) {
      int index = GetIndex( list );
      zParserExtender.GetParser()->RenameTreeNode( list, newName );
      tablesort.RemoveOrder( index );
      list->name.Replace( oldName, newName );
      tablesort.InsertSort( index );

      // Get next local variable
      // of the function symbol
      list = list->next;
      if( list == sym )
        break;

      // Bad hierarchy ??
      if( list && !Par_IsBaseSymbol( oldName, list ) )
        break;
    }

    return collisions;
  }







  int zCPar_SymbolTable::InsertAt_Union( zCPar_Symbol* sym, int alloc, bool checkHierarchy ) {
    if( !sym )
      return False;

    // Search new PFX symbols for creation new emitters
    if( zParserExtender.ExtendedParsingEnabled() && zParserExtender.GetParser() == Gothic::Parsers::PFX )
      zParserExtender.InsertPFXSymbol( sym );

    int index = GetIndex( sym->name );
    if( index != Invalid ) {
      zCPar_Symbol* oldSym = table[index];

      // Replace 'declaration to definition' or 'external func to internal'
      bool externalFunc = !sym->HasFlag( zPAR_FLAG_EXTERNAL ) && oldSym->HasFlag( zPAR_FLAG_EXTERNAL );

      if( externalFunc || zParserExtender.MergeModeEnabled() ) {
        RenameSymbol( oldSym, oldSym->GetName() + "_OLD", sym );

        if( zCParserExtender::MessagesLevel >= 3 )
          cmd << colParse2 << "zParserExtender: " <<
            colParse1 << "symbol " <<
            colParse2 << sym->name <<
            colParse1 << " replaced." <<
            colParse3 << endl;

        // Add function to the post compile queue.
        // This symbol will be replace the original
        // call address for new call address. For
        // calling origin func - use '_old' suffix.
        if( !oldSym->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
          zTCallReplaceInfo::Create(
            zParserExtender.GetParser()->stack.GetDynSize() - 4,
            zParserExtender.GetParser(),
            oldSym,
            sym,
            index
          );
        }
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




  HOOK Hook_zCPar_SymbolTable_Compare AS( &zCPar_SymbolTable::Compare, &zCPar_SymbolTable::Compare_Union );

  template<class T>
  inline T CompareLerp(T a, T v, T b) {
    if( v < a || v > b ) {
      cmd << "CompareLerp: " << a << " " << v << " " << b << endl;
      return 0;
    }

    return v;
  }

  int zCPar_SymbolTable::Compare_Union( void const* a, void const* b ) {
    int s0 = cur_table->GetNumInList() - 1;
    int i1 = CompareLerp( 0, *(int*)a, s0 );
    int i2 = CompareLerp( 0, *(int*)b, s0 );

    char* sa = cur_table->table[i1]->name.ToChar();
    char* sb = cur_table->table[i2]->name.ToChar();

    return strcmp( sa, sb );
  }







  FASTHOOK( zCPar_SymbolTable, GetSymbol );

  zCPar_Symbol* zCPar_SymbolTable::GetSymbol_Union( const zSTRING& s ) {
    if( s.IsEmpty() )
      return Null;

    return GetSymbol( GetIndex( s ) );
  }
}