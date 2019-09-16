
namespace NAMESPACE {
  
  // not used
  bool32 zCParser::parserMergeMode = False;





  HOOK Ivk_zCPar_SymbolTable_Load_Union AS( ZCPAR_SYMBOLTABLE_LOAD, &zCPar_SymbolTable::Load_Union );

  struct zTParserExternals {
    zSTRING ExternalName;
    int StackAddress;

    zTParserExternals( zCPar_Symbol* sym ) {
      ExternalName = sym->name;
      if( sym->type == zPAR_TYPE_FUNC )
        sym->GetStackPos( StackAddress, 0 );
      else
        StackAddress = (int)sym->GetDataAdr( 0 );
    }

    zTParserExternals() { }

    void RestoreSymbol( zCPar_Symbol* sym ) {
      if( sym->type == zPAR_TYPE_FUNC )
        sym->SetStackPos( StackAddress, 0 );
      else
        sym->SetDataAdr( (void*)StackAddress );
    }
  };






  void zCPar_SymbolTable::Load_Union( zFILE *f ) {

    zCPar_Symbol* Symbol;
    Array<zTParserExternals> Externals;

    if( !IsMergeMode() ) {
      for( int i = 0; i < table.GetNumInList(); i++ ) {
        Symbol = table[i];
        if( Symbol->HasFlag( zPAR_FLAG_EXTERNAL ) )
          Externals += Symbol;
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
        Symbol = GetSymbol( Externals[i].ExternalName );
        if( Symbol ) {
          Externals[i].RestoreSymbol( Symbol );
        }
        else
          Message::Warning( "U: PAR: External " + Externals[i].ExternalName + " not found." );
      }
    }
  }






  HOOK Ivk_zCPar_SymbolTable_Insert AS( ZCPAR_SYMBOLTABLE_INSERT, &zCPar_SymbolTable::Insert_Union );

  bool32 zCPar_SymbolTable::Insert_Union( zCPar_Symbol* sym ) {
    if( !sym )
      return False;

    int index = GetIndex( sym->name );
    if( index != Invalid ) {
      if( IsMergeMode() ) {
        table[index] = sym;
        sym->AllocSpace();
        return True;
      }
      else
        return False;
    }

    cur_table = this;

    table.InsertEnd( sym );
    tablesort.InsertSort( table.GetNumInList() - 1 );
    sym->AllocSpace();

    if( !firstsym ) firstsym      = sym;
    if( lastsym )   lastsym->next = sym;
    lastsym = sym;

    return True;
  }






  HOOK Ivk_zCParser_SaveDat AS( ZCPARSER_SAVEDAT, &zCParser::SaveDat_Union );

  int zCParser::SaveDat_Union( zSTRING &s ) {
    if( IsCompileDat() )
      return( this->*Ivk_zCParser_SaveDat )( s );
  }





  // disabled
  HOOK Ivk_zCParser_DeclareFuncCall AS( ZCPARSER_DECLAREFUNCCALL, &zCParser::DeclareFuncCall_Union, IVK_DISABLED );

  void zCParser::DeclareFuncCall_Union( zSTRING &name, int typematch ) {
    zCPar_Symbol* Symbol = GetSymbol( name );
    if( Symbol != Null ) {
      zCPar_Symbol* SymbolCopied = new zCPar_Symbol( *Symbol );
      SymbolCopied->name += "_old";
      symtab.table.Insert( SymbolCopied );
    }
    ( this->*Ivk_zCParser_DeclareFuncCall )( name, typematch );
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

  void AdjustParsed() {
    for( uint i = 0; i < GetParsed().GetNum(); i++ ) {
      GetParsed()[i]->CreatePCode();
      parser->mergemode = False;
      parser->compiled  = True;
    }

    GetParsed().Clear();
  }

  void ParseExternalScripts() {

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
      
      par->mergemode = True;
      par->compiled  = False;
      
      string fileName = line.Copy(parName.Length(), line.Length() - parName.Length()).Shrink();
      if( par->ParseFile( Z fileName ) != 0 ) {
        Message::Error( "U: PAR: Can not parse file: `" + fileName + "`" );
        continue;
      }
      
      GetParsed() += par;
    }

    AdjustParsed();
  }
}