// Supported with union (c) 2018 Union team

#ifndef __ZNET__WIN32_H__VER0__
#define __ZNET__WIN32_H__VER0__

namespace Gothic_I_Classic {

  struct zTNetAddress {
    unsigned short Protocol;
    union {
      struct zTNetAddressDummy0 {
        unsigned short Port;
        unsigned long IP;
      };

      struct zTNetAddressDummy1 {
        unsigned char Net[4];
        unsigned char Node[6];
        unsigned short Socket;
      };

      zTNetAddressDummy0 INET;
      zTNetAddressDummy1 IPX;
    };

    zTNetAddress() {}
  };

  struct zTNetBufferInfo {
    int inPackets;
    int inBytes;

    zTNetBufferInfo() {}
  };

  struct zTNetPacket {
    zTNetAddress From;
    unsigned long timestamp;
    unsigned long size;
    unsigned char Data[];

    zTNetPacket() {}
  };

  struct zTNetTimeout {
    unsigned long getHostAddress;
    unsigned long getHostName;
    unsigned long getService;
    unsigned long Connect;

    zTNetTimeout() {}
  };

} // namespace Gothic_I_Classic

#endif // __ZNET__WIN32_H__VER0__