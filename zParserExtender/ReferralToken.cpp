// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  void TReferralTokenList::Init( zCPar_Stack& stack ) {
    Init( stack.stack, stack.GetDynSize() );
  }


  bool TReferralTokenList::IsIllegalToken( const byte& token ) {
    switch( token ) {
      // 1 byte instructions
      case zPAR_OP_IS:
      case zPAR_OP_ISPLUS:
      case zPAR_OP_ISMINUS:
      case zPAR_OP_ISMUL:
      case zPAR_OP_ISDIV:
      case zPAR_OP_PLUS:
      case zPAR_OP_MINUS:
      case zPAR_OP_MUL:
      case zPAR_OP_DIV:
      case zPAR_OP_MOD:
      case zPAR_OP_OR:
      case zPAR_OP_AND:
      case zPAR_OP_LOWER:
      case zPAR_OP_HIGHER:
      case zPAR_OP_LOG_OR:
      case zPAR_OP_LOG_AND:
      case zPAR_OP_SHIFTL:
      case zPAR_OP_SHIFTR:
      case zPAR_OP_LOWER_EQ:
      case zPAR_OP_EQUAL:
      case zPAR_OP_NOTEQUAL:
      case zPAR_OP_HIGHER_EQ:
      case zPAR_OP_UN_MINUS:
      case zPAR_OP_UN_PLUS:
      case zPAR_OP_UN_NOT:
      case zPAR_OP_UN_NEG:
      case zPAR_TOK_RET:
      case zPAR_TOK_ASSIGNSTR:
      case zPAR_TOK_ASSIGNSTRP:
      case zPAR_TOK_ASSIGNFUNC:
      case zPAR_TOK_ASSIGNFLOAT:
      case zPAR_TOK_ASSIGNINST:
        break;

      // Invalid instructions
      default:
        return false;
    }

    return true;
  }


  void TReferralTokenList::Init( byte* bytecode, uint max ) {
    static byte prevToken = Invalid;
    Bytecode = bytecode;
    Clear();

    uint index = 0;
    while( index < max - 1 ) {
      byte& token = bytecode[index++];

      switch( token ) {
        case zPAR_TOK_PUSHINT:
          ReferralTokens[zREF_TYPE_INT] += &token;
          index += 4;
          break;

        case zPAR_TOK_PUSHVAR:
          ReferralTokens[zREF_TYPE_VAR] += &token;
          index += 4;
          break;

        case zPAR_TOK_PUSHVAR + zPAR_TOK_FLAGARRAY:
          ReferralTokens[zREF_TYPE_VARARRAY] += &token;
          index += 5;
          break;

        case zPAR_TOK_PUSHINST:
          ReferralTokens[zREF_TYPE_INST] += &token;
          index += 4;
          break;

        case zPAR_TOK_CALL:
          ReferralTokens[zREF_TYPE_JUMP] += &token;
          index += 4;
          break;

        case zPAR_TOK_CALLEXTERN:
        case zPAR_TOK_JUMP:
        case zPAR_TOK_JUMPF:
        case zPAR_TOK_SETINSTANCE:
          index += 4;
          break;

        default:
          if( !zParserExtender.NeedToTestStack() || IsIllegalToken( token ) )
            break;

          string tokSym = string( token, 16 ) + "h";
          if( tokSym.Length() < 3 )
            tokSym = "0" + tokSym;

          string prevTokSym = string( prevToken, 16 ) + "h";
          if( prevTokSym.Length() < 3 )
            prevTokSym = "0" + prevTokSym;

          cmd << colWarn1 << "zParserExtender: "
              << colWarn2 << "unknown token "
              << colWarn1 << "at "
              << colWarn2 << AHEX32( GetIndex( bytecode ) + index )
              << colWarn1 << " with code "
              << colWarn2 << tokSym
              << colWarn1 << " previous token "
              << colWarn2 << prevTokSym
              << colWarn3 << endl;
      }

      prevToken = token;
    }
  }


  int TReferralTokenList::ReplaceIndexes( int_t oldIndex, int_t newIndex, zEReferralType refID, int_t length ) {
    int refs = 0;
    Array<byte*>& bytecodes = ReferralTokens[refID];
    for( uint i = 0; i < bytecodes.GetNum(); i++ ) {
      byte* bytecode = bytecodes[i];
      if( GetIndex( bytecode ) >= length )
        continue;

      byte& token = bytecode[0];
      int& address = (int&)bytecode[1];

      if( address == oldIndex ) {
        address = newIndex;
        bytecodes.FastRemoveAt( i-- );
        refs++;
      }
    }

    return refs;
  }


  int TReferralTokenList::ReplaceJumps( int_t oldPos, int_t newPos, int_t length ) {
    int calls = 0;
    Array<byte*>& bytecodes = ReferralTokens[zREF_TYPE_JUMP];
    for( uint i = 0; i < bytecodes.GetNum(); i++ ) {
      byte* bytecode = bytecodes[i];
      if( GetIndex( bytecode ) >= length )
        continue;

      byte& command = bytecode[0];
      int& address = (int&)bytecode[1];

      if( address == oldPos ) {
        address = newPos;
        bytecodes.FastRemoveAt( i-- );
        calls++;
      }
    }

    return calls;
  }


  void TReferralTokenList::Clear() {
    for( uint i = 0; i < zREF_TYPE_MAX; i++ )
      ReferralTokens[i].Clear();
  }


  int_t TReferralTokenList::GetIndex( byte* bytecode ) {
    return (int_t)bytecode - (int_t)Bytecode;
  }
}