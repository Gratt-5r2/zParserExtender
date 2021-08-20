// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static bool IsUnnamedStringSymbol( zCPar_Symbol* symbol ) {
    zSTRING& name = symbol->name;
    if( name[0u] != char( 255 ) )
      return false;

    int length = name.Length();
    if( length != 6 )
      return false;

    for( int i = 1; i < length; i++ ) {
      byte& letter = (byte&)name[i];
      if( letter < '0' || letter > '9' )
        return false;
    }

    return true;
  }


  static void DefineStringsTableSize( zCParser* par ) {
    zCPar_SymbolTable& table = par->symtab;
    for( int i = table.table.GetNum() - 1; i >= 0; i-- ) {
      zCPar_Symbol* sym = table.table[i];
      if( IsUnnamedStringSymbol( sym ) ) {
        string index = sym->GetName().ToChar() + 1;
        par->stringcount = index.ToInt32() + 1;
        return;
      }
    }
  }


  inline void inc_string_numbers( zSTRING& value, const uint& start, const uint& end ) {
    uint index = end;
    do {
      char& dec = value[index--];
      if( ++dec > '9' )
        dec = '0';
      else
        break;

    } while( index > start );
  }


  static void RepairStringsTable( zCParser* par ) {
    zSTRING str_stringcount = zSTRING( char( 255 ) ) + "10000";

    zCPar_SymbolTable& table = par->symtab;
    for( int i = 0; i < table.table.GetNum(); i++ ) {
      zCPar_Symbol* sym = table.table[i];
      if( IsUnnamedStringSymbol( sym ) ) {
        if( sym->name != str_stringcount )
          sym->name = str_stringcount;

        inc_string_numbers( str_stringcount, 1, str_stringcount.Length() - 1 );
      }
    }

    par->stringcount = (int)zSTRING( str_stringcount.ToChar() + 1 ).ToInt32();
  }


  // @N1kX request: on recompilation DAT file in symbol table
  // appear string symbols dublicates (�10000 and higher).
  // Thats not critical for Vanilla game with Plugins, but
  // fix maybe need for Ninja patches or Translators.
  static void CheckStringsIndexing( zCParser* par ) {
    int indexingMode = zParserExtender.GetStringsIndexingMode();
    bool indexingEnabled = !NinjaInjected() || indexingMode > zStringsIndexing_None;

    // Works if Ninja not injected (it have same fix)
    // or mode is not Default
    if( indexingEnabled ) {

      // Repair mode checks all string symbols in table
      // and repairs all wrong names. Process works ~15ms
      if( indexingMode == zStringsIndexing_Default || indexingMode == zStringsIndexing_Repair )
        RepairStringsTable( par );

      // This mode find the top string symbol and writes the
      // index from its name to the parser.stringcount property
      else if( indexingMode == zStringsIndexing_FromTop )
        DefineStringsTableSize( par );
    }
  }


  static void PrepareParserSymbols( zCParser* par ) {
    Array<zCParser*> parsers;
    if( parsers & par )
      return;

    parsers += par;
    auto& symbols = par->symtab.table;
    for( int i = 0; i < symbols.GetNum(); i++ )
      par->symtab.CheckNextSymbol( symbols[i] );

    CheckStringsIndexing( par );
  }
}