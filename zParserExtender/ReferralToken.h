// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zEReferralType {
    zREF_TYPE_INT,
    zREF_TYPE_VAR,
    zREF_TYPE_VARARRAY,
    zREF_TYPE_INST,
    zREF_TYPE_JUMP,
    zREF_TYPE_MAX
  };


  struct TReferralTokenList {
    byte* Bytecode;
    Array<byte*> ReferralTokens[zREF_TYPE_MAX];

    void Init( zCPar_Stack& stack );
    void Init( byte* bytecode, uint max );

    int ReplaceIndexes( 
      int_t oldIndex,
      int_t newIndex,
      zEReferralType refID,
      int_t length );

    int ReplaceJumps(
      int_t oldPos,
      int_t newPos,
      int_t length );

    bool IsIllegalToken( const byte& token );
    void Clear();
    int_t GetIndex( byte* bytecode );
  };
}