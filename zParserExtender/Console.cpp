// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static int SaveDatEval( const zSTRING& __command, zSTRING& message ) {
    string command = __command;

    if( command.GetWordSmart( 1 ) == "PARSER" ) {
      string secondWord = command.GetWordSmart( 2 );

      if( secondWord == "SAVEDAT" ) {
        string datName = command.GetWordSmart( 3 );

        // Save OU bin & csl
        if( datName == "OU" ) {
          ogame->GetCutsceneManager()->LibStoreCopy( zLIB_STORE_ASCII | zLIB_STORE_BIN );
          return True;
        }

        // Save DAT file
        zCParser* par = GetParserByParserName( datName );
        if( par ) {
          par->SaveDatCopy();
          return True;
        }

        message = "Parser name error. Use: Game, SFX, PFX, VFX, Camera, Menu, Music.";
        return True;
      }

      if( secondWord == "EXPORT" ) {
        string what = command.GetWordSmart( 3 );

        if( what == "STRINGLIST" ) {
          string datName = command.GetWordSmart( 4 );
          zCParser* par = GetParserByParserName( datName );
          if( par ) {
            par->ExportStringList();
            return True;
          }

          message = "Parser name error. Use: Game, SFX, PFX, VFX, Camera, Menu, Music.";
          return False;
        }
      }

      message = "What ???";
      return True;
    }

    // Command is not 'parser'
    return False;
  }


  static void Game_InitConsole_Union() {
    zcon->AddEvalFunc( SaveDatEval );
    zcon->Register( "parser savedat game", "Save 'Game' dat file" );
    zcon->Register( "parser savedat sfx", "Save 'SFX' dat file" );
    zcon->Register( "parser savedat pfx", "Save 'PFX' dat file" );
    zcon->Register( "parser savedat vfx", "Save 'VFX' dat file" );
    zcon->Register( "parser savedat camera", "Save 'Camera' dat file" );
    zcon->Register( "parser savedat menu", "Save 'Menu' dat file" );
    zcon->Register( "parser savedat music", "Save 'Music' dat file" );
    zcon->Register( "parser savedat ou", "Save 'Music' dat file" );
    zcon->Register( "parser export stringlist", "Export string symbols" );
  }
}