// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	int Cast_PointerToInstance() {
    zCParser* par = zCParser::GetParser();
    int address;
    par->GetParameter(address);
    par->SetReturn((void*)address);
    return 0;
  }

  int Cast_InstanceToPointer() {
    zCParser* par = zCParser::GetParser();
    void* instance = par->GetInstance();
    par->SetReturn( (int)instance );
    return 0;
  }

  int Cast_PointerToNpc() {
    zCParser* par = zCParser::GetParser();
    void* instance = par->GetInstance();
    par->SetReturn( (int)instance );
    return 0;
  }

  int Cast_InstanceIsNpc() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    par->SetReturn( (int)instance->CastTo<oCNpc>() );
    return 0;
  }

  int Cast_InstanceIsItem() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    par->SetReturn( (int)instance->CastTo<oCItem>() );
    return 0;
  }

  int Cast_InstanceIsMob() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    par->SetReturn( (int)instance->CastTo<oCMOB>() );
    return 0;
  }

  int Cast_GetInstanceIndex() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    oCVob* vob = instance->CastTo<oCVob>();
    par->SetReturn( vob ? vob->GetInstance() : -1 );
    return 0;
  }

  int Hlp_HasFocusVob() {
    zCParser* par = zCParser::GetParser();
    oCNpc* instance = (oCNpc*)par->GetInstance();
    par->SetReturn( (int)instance->GetFocusVob() );
    return 0;
  }

  int Hlp_GetFocusVob() {
    zCParser* par = zCParser::GetParser();
    oCNpc* instance = (oCNpc*)par->GetInstance();
    par->SetReturn( instance->GetFocusVob() );
    return 0;
  }

  int Hlp_GetFocusVobName() {
    zCParser* par = zCParser::GetParser();
    oCNpc* instance = (oCNpc*)par->GetInstance();
    par->SetReturn( (zSTRING&)instance->GetFocusVob()->GetObjectName() );
    return 0;
  }

  int Hlp_GetStringLength() {
    zCParser* par = zCParser::GetParser();
    zSTRING str;
    par->GetParameter( str );
    par->SetReturn( str.Length() );
    return 0;
  }

  int Hlp_IsNAN() {
    zCParser* par = zCParser::GetParser();
    float value;
    par->GetParameter( value );
    par->SetReturn( ISNAN( value ) );
    return 0;
  }

  int Hlp_KeyToggled() {
    zCParser* par = zCParser::GetParser();
    int key;
    par->GetParameter( key );
    par->SetReturn( zKeyToggled( key ) );
    return 0;
  }

  int Hlp_KeyPressed() {
    zCParser* par = zCParser::GetParser();
    int key;
    par->GetParameter( key );
    par->SetReturn( zKeyPressed( key ) );
    return 0;
  }

  int Hlp_LogicalKeyToggled() {
    zCParser* par = zCParser::GetParser();
    int logical;
    par->GetParameter( logical );
    par->SetReturn( zinput->GetToggled( logical ) );
    return 0;
  }

  int Hlp_GameOnPause() {
    zCParser* par = zCParser::GetParser();
    par->SetReturn( ogame->IsOnPause() );
    return 0;
  }

  int Wld_ChangeLevel() {
    zCParser* par = zCParser::GetParser();
    zSTRING worldName;
    zSTRING waypointName;
    par->GetParameter( waypointName );
    par->GetParameter( worldName );
    ogame->ChangeLevel( worldName, waypointName );
    return 0;
  }

  int Wld_FindVob() {
    zCParser* par = zCParser::GetParser();
    zSTRING vobName;
    par->GetParameter( vobName );
    oCWorld* world = ogame->GetGameWorld();
    zCVob* vob = world->SearchVobByName( vobName );
    par->SetReturn( vob );
    return 0;
  }

  int Wld_PlayEffectVob() {
    zCParser* par = zCParser::GetParser();
    zSTRING effect;
    zCVob* pvob;
    int level;
    int damage;
    int damage_type;
    int damage_speed;

    par->GetParameter( damage_speed );
    par->GetParameter( damage_type );
    par->GetParameter( damage );
    par->GetParameter( level );
    pvob = (zCVob*)par->GetInstance();
    par->GetParameter( effect );

    oCVisualFX::CreateAndPlay( effect, pvob, Null, level, (float)damage, damage_type, damage_speed );
    return 0;
  }

  int Wld_PlayEffectAt() {
    zCParser* par = zCParser::GetParser();
    zSTRING effect;
    zVEC3 ppos;
    int level;
    int damage;
    int damage_type;
    int damage_speed;

    par->GetParameter( damage_speed );
    par->GetParameter( damage_type );
    par->GetParameter( damage );
    par->GetParameter( level );
    ppos = *(zVEC3*)par->GetInstance();
    par->GetParameter( effect );

    oCVisualFX::CreateAndPlay( effect, ppos, Null, level, damage, damage_type, damage_speed );
    return 0;
  }

  int Wld_ToggleRain() {
    zCParser* par = zCParser::GetParser();
    oCWorld* world = ogame->GetGameWorld();
    float weight;
    float time;
    par->GetParameter( time );
    par->GetParameter( weight );
    zCSkyControler_Outdoor* controller = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    if( controller )
      controller->SetRainFXWeight( weight, time );

    return 0;
  }

  int Wld_SetWeatherType() {
#if ENGINE >= Engine_G2
    zCParser* par = zCParser::GetParser();
    oCWorld* world = ogame->GetGameWorld();
    int weather;
    par->GetParameter( weather );
    zCSkyControler_Outdoor* controller = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    if( controller )
      controller->SetWeatherType( (zTWeather&)weather );
#endif
    return 0;
  }

  int Wld_GetWeatherType() {
    zCParser* par = zCParser::GetParser();
#if ENGINE >= Engine_G2
    oCWorld* world = ogame->GetGameWorld();
    int weather;
    par->GetParameter( weather );
    zCSkyControler_Outdoor* controller = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    par->SetReturn( controller ? controller->GetWeatherType() : -1 );
#else
    par->SetReturn( -1 );
#endif
    return 0;
  }

  int Mdl_GetAnimationIndex() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    zSTRING aniName;
    par->GetParameter( aniName );
    npc = (oCNpc*)par->GetInstance();
    npc->GetModel()->GetAniIDFromAniName( aniName );
    return 0;
  }

  int Mdl_GetAnimationName() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModelAni* ani = npc->GetModel()->GetAniFromAniID( index );
    par->SetReturn( ani ? ani->aniName : "" );
    return 0;
  }

  int Mdl_AnimationIsExists() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModelAni* ani = npc->GetModel()->GetAniFromAniID( index );
    par->SetReturn( ani ? True : False );
    return 0;
  }

  int Mdl_AnimationIsActive() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();
    zCModelAni* ani = model->GetAniFromAniID( index );
    par->SetReturn( ani ? model->IsAniActive( ani ) : False );
    return 0;
  }

  int Mdl_SetAllAnimationsFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    float fps;
    par->GetParameter( fps );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    auto& protos = model->modelProtoList;
    for( int i = 0; i < protos.GetNum(); i++ ) {
      auto& anis = protos[i]->protoAnis;
      for( int j = 0; j < anis.GetNum(); j++ ) {
        anis[j]->fpsRate = fps;
      }
    } 

    return 0;
  }

  int Mdl_ResetAllAnimationsFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    auto& protos = model->modelProtoList;
    for( int i = 0; i < protos.GetNum(); i++ ) {
      auto& anis = protos[i]->protoAnis;
      for( int j = 0; j < anis.GetNum(); j++ ) {
        anis[j]->fpsRate = anis[j]->fpsRateSource;
      }
    }

    return 0;
  }

  int Mdl_SetAnimationFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    float fps;
    par->GetParameter( fps );
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    zCModelAni* ani = model->GetAniFromAniID( index );
    if( ani )
      ani->fpsRate = fps;

    return 0;
  }

  int Mdl_ResetAnimationFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    zCModelAni* ani = model->GetAniFromAniID( index );
    if( ani )
      ani->fpsRate = ani->fpsRateSource;

    return 0;
  }

  int Mdl_SetVisible() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int visible;
    par->GetParameter( visible );
    npc = (oCNpc*)par->GetInstance();
    npc->GetModel()->isVisible = visible;
    return 0;
  }

  int Npc_SetAsHero() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->SetAsPlayer();
    return 0;
  }

#if ENGINE >= Engine_G2
#define INV_MODE INV_MODE_DEFAULT
#else
#define INV_MODE
#endif

  int Npc_OpenInventory() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->OpenInventory( INV_MODE );
    return 0;
  }

  int Npc_OpenInventorySteal() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->OpenSteal();
    return 0;
  }

  int Npc_OpenInventoryTrade() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->OpenTradeOffer( player );
    return 0;
  }

  int Mob_Destroy() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();
    mob->Destroy();
    return 0;
  }

  int Mob_RemoveItem() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();
    
    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      zCListSort<oCItem>* list = mob_container->items->contents->next;
      while( list ) {
        oCItem* item = list->GetData();
        if( item->instanz == index ) {
          mob_container->Remove( item );
          break;
        }

        list = list->next;
      }
    }

    return 0;
  }

  int Mob_RemoveItems() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    int count;
    par->GetParameter( count );
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();
    
    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      zCListSort<oCItem>* list = mob_container->items->contents->next;
      while( list ) {
        oCItem* item = list->GetData();
        if( item->instanz == index ) {
          if( item->amount > count )
            item->amount -= count;
          else
            mob_container->Remove( item );
          break;
        }

        list = list->next;
      }
    }

    return 0;
  }

  int Mob_InsertItem() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();

    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      oCItem* item = new oCItem();
      item->InitByScript( index, 1 );
      mob_container->Insert( item );
    }

    return 0;
  }
  
  int Mob_InsertItems() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    int count;
    par->GetParameter( count );
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();

    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      oCItem* item = new oCItem();
      item->InitByScript( index, count );
      mob_container->Insert( item );
    }

    return 0;
  }

  int Mob_GetLockCombination() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      par->SetReturn( mob_lockable->pickLockStr );

    return 0;
  }

  int Mob_SetLockCombination() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    zSTRING combination;
    par->GetParameter( combination );
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      mob_lockable->pickLockStr = combination;

    return 0;
  }

  int Mob_IsLocked() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      par->SetReturn( mob_lockable->locked );

    return 0;
  }

  int Mob_SetLocked() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int locked;
    par->GetParameter( locked );
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      mob_lockable->locked = locked;

    return 0;
  }

  int Mob_GetKeyInstance() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable ) {
      int index = par->GetIndex( mob_lockable->keyInstance );
      par->SetReturn( index );
    }

    return 0;
  }

  int Mob_SetKeyInstance() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable ) {
      zCPar_Symbol* sym = par->GetSymbol( index );
      if( sym )
        mob_lockable->keyInstance = sym->name;
    }

    return 0;
  }

  int Hlp_MessageBox() {
    zCParser* par = zCParser::GetParser();
    zSTRING message;
    par->GetParameter( message );
    Message::Box( message, "Parser" );
    return 0;
  }

  void DefineExternals() {
    // CAST
    parser->DefineExternal( "Cast_PointerToInstance",    Cast_PointerToInstance,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Cast_PointerToNpc",         Cast_PointerToInstance,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Cast_PointerToItem",        Cast_PointerToInstance,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Cast_InstanceToPointer",    Cast_InstanceToPointer,    zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_InstanceIsItem",       Cast_InstanceIsItem,       zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_InstanceIsMob",        Cast_InstanceIsMob,        zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_GetInstanceIndex",     Cast_GetInstanceIndex,     zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );

    // HLP
    parser->DefineExternal( "Hlp_HasFocusVob",           Hlp_HasFocusVob,           zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_GetFocusVob",           Hlp_GetFocusVob,           zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_GetFocusVobName",       Hlp_GetFocusVobName,       zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_GetStringLength",       Hlp_GetStringLength,       zPAR_TYPE_INT,      zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Hlp_IsNAN",                 Hlp_IsNAN,                 zPAR_TYPE_INT,      zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Hlp_KeyToggled",            Hlp_KeyToggled,            zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_KeyPressed",            Hlp_KeyPressed,            zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_LogicalKeyToggled",     Hlp_LogicalKeyToggled,     zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_GameOnPause",           Hlp_GameOnPause,           zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_MessageBox",            Hlp_MessageBox,            zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   0 );

    // WLD
    parser->DefineExternal( "Wld_ChangeLevel",           Wld_ChangeLevel,           zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Wld_FindVob",               Wld_FindVob,               zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Wld_PlayEffectVob",         Wld_PlayEffectVob,         zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Wld_PlayEffectAt",          Wld_PlayEffectAt,          zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Wld_ToggleRain",            Wld_ToggleRain,            zPAR_TYPE_VOID,     zPAR_TYPE_FLOAT,    zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Wld_SetWeatherType",        Wld_SetWeatherType,        zPAR_TYPE_VOID,     zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Wld_GetWeatherType",        Wld_GetWeatherType,        zPAR_TYPE_INT,      0 );

    // MDL
    parser->DefineExternal( "Mdl_GetAnimationIndex",     Mdl_GetAnimationIndex,     zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Mdl_GetAnimationName",      Mdl_GetAnimationName,      zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_AnimationIsExists",     Mdl_AnimationIsExists,     zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_AnimationIsActive",     Mdl_AnimationIsActive,     zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_SetAllAnimationsFPS",   Mdl_SetAllAnimationsFPS,   zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Mdl_ResetAllAnimationsFPS", Mdl_ResetAllAnimationsFPS, zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mdl_SetAnimationFPS",       Mdl_SetAnimationFPS,       zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Mdl_ResetAnimationFPS",     Mdl_ResetAnimationFPS,     zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_SetVisible",            Mdl_SetVisible,            zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );

    // NPC
    parser->DefineExternal( "Npc_SetAsHero",             Npc_SetAsHero,             zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_OpenInventory",         Npc_OpenInventory,         zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_OpenInventorySteal",    Npc_OpenInventorySteal,    zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_OpenInventoryTrade",    Npc_OpenInventoryTrade,    zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );

    // MOB
    parser->DefineExternal( "Mob_Destroy",               Mob_Destroy,               zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_RemoveItem",            Mob_RemoveItem,            zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mob_RemoveItems",           Mob_RemoveItems,           zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mob_InsertItem",            Mob_InsertItem,            zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mob_InsertItems",           Mob_InsertItems,           zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mob_GetLockCombination",    Mob_GetLockCombination,    zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_SetLockCombination",    Mob_SetLockCombination,    zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Mob_IsLocked",              Mob_IsLocked,              zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_SetLocked",             Mob_SetLocked,             zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mob_GetKeyInstance",        Mob_GetKeyInstance,        zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_SetKeyInstance",        Mob_SetKeyInstance,        zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );

    // OTHER
    static float fNan  = NAN;
    static void* pNull = Null;
    parser->DefineExternalVar( "NAN",  &fNan,  zPAR_TYPE_FLOAT,    1 );
    parser->DefineExternalVar( "NULL", &pNull, zPAR_TYPE_INSTANCE, 1 );
  }
}