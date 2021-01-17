// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	static Array<zCPar_Symbol*> HookedNpcInstances;
	static Array<oCNpc*> HookedNpcObjects;

	inline bool IsNpcSymbol( zCPar_Symbol* sym ) {
		if( sym->type != zPAR_TYPE_INSTANCE )
			return false;

		while( sym ) {
			if( sym->name == "C_NPC" )
				return true;

			sym = sym->parent;
		}

		return false;
	}



	static void CheckHookedNpcInstance( zCPar_Symbol* sym ) {
		if( IsNpcSymbol( sym ) )
			HookedNpcInstances.InsertSorted( sym );
	}



	HOOK Hook_oCNpc_InitByScript PATCH_IF( &oCNpc::InitByScript, &oCNpc::InitByScript_Union, NinjaInjected() );

	void oCNpc::InitByScript_Union( int instance, int saveGame ) {
		THISCALL( Hook_oCNpc_InitByScript )(instance, saveGame);

		zCPar_Symbol* sym = parser->GetSymbol( instance );
		if( HookedNpcInstances.HasEqualSorted( sym ) )
			HookedNpcObjects += this;
	}



	void AdjustEditedNpcObjects() {
		for( uint i = 0; i < HookedNpcObjects.GetNum(); i++ ) {
			cmd << "מעלוםא פכאדאא " << HookedNpcObjects[i]->name[0] << endl;
			HookedNpcObjects[i]->dontWriteIntoArchive = False;
		}

		HookedNpcObjects.Clear();
	}
}