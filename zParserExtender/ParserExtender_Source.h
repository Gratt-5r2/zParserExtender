
namespace NAMESPACE {

  HOOK Ivk_zCPar_SymbolTable_Load_Union AS( ZCPAR_SYMBOLTABLE_LOAD, &zCPar_SymbolTable::Load_Union, zCURRENT_GAME );

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

  void zCPar_SymbolTable::Load_Union( zFILE *f ) {

    zCPar_Symbol* Symbol;
    Array<zCPar_Symbol*> Externals;

    if( !IsMergeMode() ) {
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
    if( !IsMergeMode() ) {
      table.EmptyList();
	  tablesort.EmptyList();
    }

    int SymCount = 0;
    f->Read( &SymCount, sizeof( SymCount ) );
  
    // pre alloc memory
    table    .AllocAbs( table.GetNum() + SymCount );
    tablesort.AllocAbs( tablesort.GetNum() + SymCount );
    tablesort.MarkNumAllocUsed();

    // read symbols on sorted table
    f->Read( tablesort.GetArray(), sizeof( tablesort[0] ) * SymCount );
    PreAlloc( SymCount );

    zSTRING ParentFuncName;
    zCPar_Symbol* ParentFuncSymbol = Null;
    zCPar_Symbol* PreviousSymbol   = Null;

    for( int i = 0; i < SymCount; i++ ) {
      Symbol = preAllocatedSymbols + ( nextPreAllocated-- );
      Symbol->LoadFull( f );

      // is a symbol - function ??
      if( Symbol->type == zPAR_TYPE_FUNC ) {
        if( !Symbol->name.HasWordI( ParentFuncName ) ) {
          ParentFuncSymbol = Symbol;
          PreviousSymbol   = Symbol;
          ParentFuncName   = Symbol->name;
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
    if( !IsMergeMode() ) {
      for( uint i = 0; i < Externals.GetNum(); i++ ) {
        zCPar_Symbol*& OldExternal = Externals[i];
        zCPar_Symbol*  NewExternal = GetSymbol( OldExternal->name );

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






  HOOK Ivk_zCPar_SymbolTable_Insert AS( ZCPAR_SYMBOLTABLE_INSERT, &zCPar_SymbolTable::Insert_Union, zCURRENT_GAME );

  bool32 zCPar_SymbolTable::Insert_Union( zCPar_Symbol* sym ) {
    return InsertAt_Union( sym, True );
  }

  bool32 zCPar_SymbolTable::InsertAt_Union( zCPar_Symbol* sym, int alloc ) {
    if( !sym )
      return False;

    int index = GetIndex( sym->name );
    if( index != Invalid ) {
      if( IsMergeMode() ) {
        zCPar_Symbol* oldSym = table[index];
        
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

  HOOK Ivk_zCParser_SaveDat AS( ZCPARSER_SAVEDAT, &zCParser::SaveDat_Union, zCURRENT_GAME );

  int zCParser::SaveDat_Union( zSTRING &s ) {
    if( IsCompileDat() )
      return( this->*Ivk_zCParser_SaveDat )( s );
  }

  zSTRING zCPar_Symbol::GetName() {
    return (Z name).GetWord( "." );
  }

  void zCPar_Symbol::Rename( const zSTRING& newName ) {
    zSTRING oldName = GetName();
    zCPar_Symbol* list = this;
    while( list ) {
      list->name.Replace( oldName, newName );
      list = list->next;
      if( list == this )
        break;
    }
  }






  // zCParser*& Game   = parser;
  // zCParser*& SFX    = parserSoundFX;
  // zCParser*& PFX    = parserParticleFX;
  // zCParser*& VFX    = parserVisualFX;
  // zCParser*& Camera = parserCamera;
  // zCParser*& Menu   = parserMenu;
  // zCParser*& Music  = parserMusic;

  static Array<zCParser*>& GetParsed() {
    static Array<zCParser*> arrParsed;
    return arrParsed;
  }

  zCParser* GetParserByName(string name) {
    if( name.HasWordI( "Game" ) || name.HasWordI( "parser" ) )
      return Gothic::Parsers::Game;

    if( name.HasWordI( "SFX" ) || name.HasWordI( "parserSoundFX" ) )
      return Gothic::Parsers::SFX;

    if( name.HasWordI( "PFX" ) || name.HasWordI( "parserParticleFX" ) )
      return Gothic::Parsers::PFX;

    if( name.HasWordI( "VFX" ) || name.HasWordI( "parserVisualFX" ) )
      return Gothic::Parsers::VFX;

    if( name.HasWordI( "Camera" ) || name.HasWordI( "parserCamera" ) )
      return Gothic::Parsers::Camera;

    if( name.HasWordI( "Menu" ) || name.HasWordI( "parserMenu" ) )
      return Gothic::Parsers::Menu;

    if( name.HasWordI( "Music" ) || name.HasWordI( "parserMusic" ) )
      return Gothic::Parsers::Music;

    return Null;
  }
  

  void SetEnableParsing(zCParser* par, bool32 enable) {
    par->mergemode =  enable && IsMergeMode();
    par->compiled  = !enable;
  }

  void AdjustParsed() {
    for( uint i = 0; i < GetParsed().GetNum(); i++ ) {
      zCParser* par = GetParsed()[i];
      par->CreatePCode();
      SetEnableParsing( par, False );
    }

    GetParsed().Clear();
  }

  void ParseExternalScripts() {
    if( LoadScript.IsEmpty() )
      return;

    zoptions->ChangeDir( DIR_SCRIPTS );

    string fileData;
    if( !fileData.ReadFromVdf( LoadScript, VDF_DEFAULT ) )
      return Message::Error( "U: PAR: " + LoadScript + " not exists." );

    CDocument doc = fileData;

    for( uint i = 0; i < doc.GetNum(); i++ ) {
      string& line = doc[i];
      if( line.Shrink().IsEmpty() )
        continue;
      
      string parName = line.GetWord( " \t", 1 );
      zCParser* par = GetParserByName( parName );
      if( !par ) {
        Message::Error( "U: PAR: Undefined parser name: " + parName );
        continue;
      }
      
      SetEnableParsing( par, True );
      string fileName = line.Copy(parName.Length(), line.Length() - parName.Length()).Shrink();
      if( par->ParseFile( Z fileName ) != 0 ) {
        if( !(GetParsed() & par) )
          SetEnableParsing( par, False );

        Message::Error( "U: PAR: Can not parse file: `" + fileName + "`" );
        continue;
      }
      
      GetParsed() |= par;
    }

    AdjustParsed();
  }












  HOOK Ivk_zCParser_ParseBlock AS( ZCPARSER_PARSEBLOCK, &zCParser::ParseBlock_Union, zCURRENT_GAME );

  static int op_break_level = 0;
  static int op_while_enumerator = 0x10000;

#define WHILE_BEGIN op_break_level++, op_while_enumerator += 2
#define WHILE_END   op_break_level--

  void zCParser::ParseBlock_Union() {
    zSTRING word;
	Match( Z "{" );
    while( pc < pc_stop ) {
      ReadWord( word );
      if( word == "VAR" )
        DeclareVar( False );
      else if( word == "CONST" )
        DeclareVar( True );
      else if( word == "RETURN" )
        DeclareReturn();
      else if( word == "IF" )
        DeclareIf();
      else if( word == "WHILE" )
        DeclareWhile_Union();
      else if( word == "BREAK" )
        DeclareBreak_Union();
      else if( word == "}" )
        return;
      else
        DeclareAssign( word );
      Match( Z ";" );
    }
  }

  void zCParser::DeclareWhile_Union() {
    WHILE_BEGIN;

    int op_while_index_begin = op_while_enumerator + 1;
    int op_while_index_end   = op_while_enumerator;

    do {
      // create label of the 'begin' of loop
      treenode = CreateLeaf( zPAR_TOK_LABEL, treenode );
      treenode->value = op_while_index_begin;

      // create expression of the condition
      treenode->SetNext( ParseExpression() );
      while( treenode->next )
        treenode = treenode->next;
      
      // break from the loop if the expression is 'false'
      treenode = CreateLeaf( zPAR_TOK_JUMPF, treenode );
      treenode->value = op_while_index_end;

      // parse body of the loop
      PrevWord();
      ParseBlock();
      ReadWord( aword );

    } while( aword != ";" );

    // return to the 'begin' of loop
    treenode = CreateLeaf( zPAR_TOK_JUMP, treenode );
    treenode->value = op_while_index_begin;

    // create 'end' label of the loop
    treenode = CreateLeaf( zPAR_TOK_LABEL, treenode );
    treenode->value = op_while_index_end;

    PrevWord();

    WHILE_END;
  }

  void zCParser::DeclareBreak_Union() {
    if( op_break_level <= 0 )
      Error( Z "found empty break operator!", 0 );

    do {
      // break from top loop
      treenode = CreateLeaf( zPAR_TOK_JUMP, treenode );
      treenode->value = op_while_enumerator;

      ReadWord( aword );
    } while( aword != ";" );

    PrevWord();
  }
}