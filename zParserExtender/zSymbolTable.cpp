// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  struct zTCallReplaceInfo {
    static Array<zTCallReplaceInfo> CallReplaceInfos;

    int_t ReplaceLength;
    zCParser* Parser;
    zCPar_Symbol* OldSymbol;
    zCPar_Symbol* NewSymbol;
    int_t OldIndex;

    static zTCallReplaceInfo& Create( int_t replaceLength, zCParser* scriptParser, zCPar_Symbol* oldSymbol, zCPar_Symbol* newSymbol, int_t oldIndex ) {
      for( uint i = 0; i < CallReplaceInfos.GetNum(); i++ ) {
        if( CallReplaceInfos[i].NewSymbol == oldSymbol ) {
          CallReplaceInfos[i].NewSymbol = newSymbol;
          CallReplaceInfos[i].OldIndex  = oldIndex;
          return CallReplaceInfos[i];
        }
      }

      zTCallReplaceInfo& callReplace = CallReplaceInfos.Create();
      callReplace.ReplaceLength      = replaceLength;
      callReplace.Parser             = scriptParser;
      callReplace.OldSymbol          = oldSymbol;
      callReplace.NewSymbol          = newSymbol;
      callReplace.OldIndex           = oldIndex;
      return callReplace;
    }
  };

  Array<zTCallReplaceInfo> zTCallReplaceInfo::CallReplaceInfos;

  inline string IntToIndex( const int& id ) {
    string ID = A "ID " + id;
    for( uint i = ID.Length(); i < 10; i++ )
      ID += " ";
    
    return ID;
  }

  // Find and replace call address to other function
  void ReplaceStackCallAddress( TReferralTokenList& referalTokens, zCParser& scriptParser, zCPar_Symbol* symLeft, zCPar_Symbol* symRight, int_t oldIndex, int_t length ) {
    int oldPos       = symLeft->single_intdata;
    int newPos       = symRight->single_intdata;
    int newIndex     = scriptParser.symtab.GetIndex_Safe( symRight );
    int calls        = 0;
    int refs         = 0;
    int symType      = symLeft->type;
    bool isClass     = symType == zPAR_TYPE_CLASS;
    bool isFunc      = symType == zPAR_TYPE_FUNC;
    bool isInstance  = symType == zPAR_TYPE_PROTOTYPE || symType == zPAR_TYPE_INSTANCE;
    bool isVariable  = symType == zPAR_TYPE_FLOAT || symType == zPAR_TYPE_INT || symType == zPAR_TYPE_STRING;


    if( isFunc || isInstance )            calls += referalTokens.ReplaceJumps( oldPos, newPos, length );
    if( isInstance )                      refs  += referalTokens.ReplaceIndexes( oldIndex, newIndex, zREF_TYPE_INST,     length );
    if( isFunc || isInstance || isClass ) refs  += referalTokens.ReplaceIndexes( oldIndex, newIndex, zREF_TYPE_INT,      length );
    if( isVariable )                      refs  += referalTokens.ReplaceIndexes( oldIndex, newIndex, zREF_TYPE_VAR,      length );
    if( isVariable )                      refs  += referalTokens.ReplaceIndexes( oldIndex, newIndex, zREF_TYPE_VARARRAY, length );


    if( calls && zCParserExtender::MessagesLevel >= 3 )
      cmd << colParse2 << "zParserExtender: "                  <<
             colParse1 << "func calls "                        <<
             colParse2 << AHEX32( oldPos )                     <<
             colParse1 << " -> "                               <<
             colParse2 << AHEX32( newPos )                     <<
             colParse1 << " replaced ("                        <<
             colParse2 << calls                                <<
             colParse1 << ")\t"                                <<
             colParse2 << SymbolTypeToString( symRight->type ) <<
             colParse1 << "  "                                 <<
             colParse2 << symRight->name                       <<
             colParse3 << endl;

    if( refs && zCParserExtender::MessagesLevel >= 3 )
      cmd << colParse2 << "zParserExtender: "                  <<
             colParse1 << "references "                        <<
             colParse2 << AHEX32( oldIndex )                   <<
             colParse1 << " -> "                               <<
             colParse2 << AHEX32( newIndex )                   <<
             colParse1 << " replaced ("                        <<
             colParse2 << refs                                 <<
             colParse1 << ")\t"                                <<
             colParse2 << SymbolTypeToString( symRight->type ) <<
             colParse1 << "  "                                 <<
             colParse2 << symRight->name                       <<
             colParse3 << endl;
  }
  

  void PostCompileCallReplace() {
    zCParser* extParser = zParserExtender.GetParser();
    if ( !extParser ) {
      return;
    }
    TReferralTokenList referalTokens;
    referalTokens.Init( extParser->stack );

    for( uint i = 0; i < zTCallReplaceInfo::CallReplaceInfos.GetNum(); i++ ) {
      zTCallReplaceInfo& info = zTCallReplaceInfo::CallReplaceInfos[i];
      if( info.Parser != extParser )
        continue;

      ReplaceStackCallAddress(
        referalTokens,
        *info.Parser,
        info.OldSymbol,
        info.NewSymbol,
        info.OldIndex,
        min( ParStackMaxLength, info.ReplaceLength ) );

      zTCallReplaceInfo::CallReplaceInfos.RemoveAt( i-- );
    }
  }


  void zCPar_SymbolTable::PostDefineExternal_Union( zCPar_Symbol* external ) {
    // Restore the function & arguments of the function
    int ele = external->ele;
    zCPar_Symbol* Symbol = external;
    while( Symbol && ele-- >= 0 ) {
      Insert_Union( Symbol );
      Symbol = Symbol->next;

      if( Symbol == external )
        break;
    }
  }


  HOOK Hook_zCPar_SymbolTable_Load AS( &zCPar_SymbolTable::Load, &zCPar_SymbolTable::Load_Union );

  void zCPar_SymbolTable::Load_Union( zFILE* f ) {
    zCParser* par = zCParser::GetParser();
    zCPar_Symbol* Symbol;
    Array<zCPar_Symbol*> StartupExternals;
    Array<zCPar_Symbol*> LoadedExternals;

    if( !zParserExtender.MergeModeEnabled() ) {
      for( int i = 0; i < table.GetNumInList(); i++ ) {
        Symbol = table[i];

        if( Symbol->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
          StartupExternals += Symbol;
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
    bool SkipNextFunction = false;

    for( int i = 0; i < SymCount; i++ ) {
      Symbol = preAllocatedSymbols + (nextPreAllocated--);
      Symbol->LoadFull( f );

      // is a symbol - function ??
      if( Symbol->type == zPAR_TYPE_FUNC || Symbol->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
        if( !Symbol->name.HasWordI( ParentFuncName ) ) {
          ParentFuncSymbol = Symbol;
          PreviousSymbol = Symbol;
          ParentFuncName = Symbol->name;

          if( Symbol->HasFlag( zPAR_FLAG_EXTERNAL ) )
            LoadedExternals.Insert( Symbol );
        }
      }

      // check arguments and define `next` pointers
      if( ParentFuncSymbol ) {
        if( Symbol->name.GetWord( "." ) == ParentFuncName ) {
          PreviousSymbol->next = Symbol;
          PreviousSymbol = Symbol;
        }
        else {
          ParentFuncSymbol = Null;
          SkipNextFunction = false;
        }
      }

      // load parent symbol
      bool32 ParentIndex = Invalid;
      f->Read( &ParentIndex, sizeof( ParentIndex ) );

      if( !SkipNextFunction ) {
        if( ParentIndex != Invalid )
          Symbol->SetParent( GetSymbol( ParentIndex ) );

        table.InsertEnd( Symbol );
      }
    }


    for( uint i = 0; i < LoadedExternals.GetNum(); i++ ) {
      zCPar_Symbol* OldExternal = LoadedExternals[i];
      zCPar_Symbol* NewExternal = zParserExtender.GetExternalFunction( OldExternal->name );
      if( NewExternal ) {
        int StackAddress;
        OldExternal->GetStackPos( StackAddress, 0 );
        NewExternal->SetStackPos( StackAddress, 0 );
      }
    }

    // restore external functions
    if( !zParserExtender.MergeModeEnabled() ) {
      for( uint i = 0; i < StartupExternals.GetNum(); i++ ) {
        zCPar_Symbol*& OldExternal = StartupExternals[i];
        zCPar_Symbol* NewExternal = GetSymbol( OldExternal->name );
        
        if( NewExternal ) {
          int StackAddress;
          OldExternal->GetStackPos( StackAddress, 0 );
          NewExternal->SetStackPos( StackAddress, 0 );

          // copy arguments from old to new
          NewExternal->next = OldExternal->next;
          // delete OldExternal;
        }
        else {
          PostDefineExternal_Union( OldExternal );
        }
      }
    }
  }


  int zCPar_SymbolTable::GetIndex_Safe( const zSTRING& name ) {
    for( int i = 0; i < table.GetNum(); i++ )
      if( table[i] && table[i]->name == name )
        return i;
      
    return Invalid;
  }

  int zCPar_SymbolTable::GetIndex_Safe( zCPar_Symbol* symbol ) {
    int index = GetIndex( symbol );
    if( index == Invalid )
      return GetIndex_Safe( symbol->name );

    return index;
  }


  void zCPar_SymbolTable::RegisterEvent( const zSTRING& name ) {
    zSTRING eventName = Z string::Combine( "EVENT.%z.START", name );
    if( GetIndex( eventName ) != Invalid )
      return;

    zCPar_Symbol* sym = new zCPar_Symbol();
    sym->name = eventName;
    sym->type = zPAR_TYPE_VOID;
    InsertAt_Union( sym, 0, false );
  }


  HOOK Hook_zCPar_SymbolTable_Insert AS( &zCPar_SymbolTable::Insert, &zCPar_SymbolTable::Insert_Union );

  static bool s_DeclareEvent = false;

  int zCPar_SymbolTable::Insert_Union( zCPar_Symbol* sym ) {
    if( s_DeclareEvent && zCParser::cur_parser ) {
      s_DeclareEvent = false;
      // RegisterEvent( sym->name );

      zSTRING& fileName = zCParser::cur_parser->fname.GetPattern( "\\", "." );
      string keyName    = sym->name;
      sym->name         = Z string::Combine( "EVENT.%z.%z", sym->name, fileName );
      int ok            = InsertAt_Union( sym, True, true );
      int symIndex      = table.Search( sym );

      zTEventFuncCollection::GetCollection( zCParser::cur_parser ).PushIndex( keyName, symIndex );
      return ok;
    }

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


  uint zCPar_SymbolTable::RenameSymbol( zCParser* scriptParser, zCPar_Symbol* sym, const zSTRING& newName, zCPar_Symbol* newSym ) {
    uint collisions = 1;

    int nextIndex = GetIndex( newName );
    if( nextIndex != Invalid ) {
      zCPar_Symbol* nextSym = table[nextIndex];
      collisions += RenameSymbol( scriptParser, nextSym, nextSym->GetName() + "_OLD", sym );
    }

    cur_table          = this;
    zSTRING oldName    = sym->GetName();
    zCPar_Symbol* list = sym;

    while( list ) {
      int index = GetIndex( list );
      scriptParser->RenameTreeNode( list, newName );
      tablesort.RemoveOrder( index );
      if( !zCParser::OverrideNextSymbol ) {
        list->name.Replace( oldName, newName );
        tablesort.InsertSort( index );
      }
      // TODO fix mem leaks

      // Get next local variable
      // of the function symbol
      list = list->next;
      if( list == sym )
        break;

      // Bad hierarchy ??
      if( list && !Par_IsBaseSymbol( oldName, list ) )
        break;
    }

    // if( zCParser::OverrideNextSymbol )
    //   zCParser::OverrideNextSymbol = false;

    return collisions;
  }



  int zCPar_SymbolTable::InsertAt_Union( zCPar_Symbol* sym, int alloc, bool checkHierarchy ) {
    if( !sym )
      return False;

    zCParser* extParser = zParserExtender.GetParser();

    if ( !extParser  || &extParser->symtab != this ) {
        return (this->*Hook_zCPar_SymbolTable_Insert)(sym);
    }

    // Search new PFX symbols for creation new emitters
    if( zParserExtender.ExtendedParsingEnabled() && extParser == Gothic::Parsers::PFX )
      zParserExtender.InsertPFXSymbol( sym );

    int index = GetIndex( sym->name );

    if( index != Invalid ) {
      zCPar_Symbol* oldSym = table[index];

      // Replace 'declaration to definition' or 'external func to internal'
      bool internalToExternal = !sym->HasFlag( zPAR_FLAG_EXTERNAL ) && oldSym->HasFlag( zPAR_FLAG_EXTERNAL );

      if( internalToExternal || zParserExtender.MergeModeEnabled() ) {
        RenameSymbol( extParser, oldSym, oldSym->GetName() + "_OLD", sym );

        if( zCParserExtender::MessagesLevel >= 3 )
          cmd << colParse2 << "zParserExtender: " <<
            colParse1 << "symbol "                <<
            colParse2 << sym->name                <<
            colParse1 << " replaced."             <<
            colParse3 << endl;

        // Add function to the post compile queue.
        // This symbol will be replace the original
        // call address for new call address. For
        // calling origin func - use '_old' suffix.
        if( !oldSym->HasFlag( zPAR_FLAG_EXTERNAL ) ) {
          if( oldSym->type != sym->type ) {
            string symName     = sym->name;
            string oldTypeName = SymbolTypeToString( oldSym->type );
            string newTypeName = SymbolTypeToString( sym->type );
            string errorText   = string::Combine( "The replacement symbol '%z' has incorrect type! Src '%z' != new '%z'", sym->name, oldTypeName, newTypeName );
            extParser->Error( Z errorText, 0 );
          }

          zTCallReplaceInfo::Create(
            extParser->stack.GetDynSize() - 4,
            extParser,
            oldSym,
            sym,
            index
          );
        }
      }
      else
        return False;
    }

    if( zCParser::OverrideNextSymbol )
      zCParser::OverrideNextSymbol = false;

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


  HOOK Hook_zCPar_SymbolTable_GetSymbol PATCH( &zCPar_SymbolTable::GetSymbol, &zCPar_SymbolTable::GetSymbol_Union );

  zCPar_Symbol* zCPar_SymbolTable::GetSymbol_Union( const zSTRING& s ) {
    if( s.IsEmpty() || s[0u] == ',' )
      return Null;

    return GetSymbol( GetIndex( s ) );
  }
}