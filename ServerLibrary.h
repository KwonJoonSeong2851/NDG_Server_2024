#pragma once
//#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"Winmm.lib")


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <mmsystem.h>
#include <WS2spi.h>
#include <mstcpip.h>


#include <Windows.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <stdint.h>
#include <ctime>




#include <assert.h>
#include <fcntl.h>
#include <algorithm>
#include <functional>
#include <array>
#include <unordered_map>


#include<thread>
#include<mutex>
#include<memory>


#include <string>

#include <ctime>
#include <random>
#include <typeinfo>



#if _DEBUG
#define CONTEXT_SWITCH Sleep(1)
#else
#define CONTEXT_SWITCH ::  SwitchToThread()
#endif

typedef void(*Funcation)(void*);

//외부 유틸
#include "./Util/tinyxml/tinyxml.h"


//유틸
#include "./Util/Util.h"
#include "./Util/Type.h"

#include "./Util/Singleton.h"
#include "./Util/Config.h"
#include "./Util/Lock.h"
#include "./Util/Thread.h"
#include "./Util/Clock.h"
#include "./Util/Logger.h"
#include "./Util/Task.h"
#include "./Util/ThreadJobQueue.h"


//Packet 처리
#include "./Network/Enums.h"
#include "./Network/StreamBuffer.h"
#include "./Network/Packet/PacketHeader.h"

#include "./Network/Packet/GpType.h"

#include "./Network/Packet/CustomType/Object.h"
#include "./Network/Packet/CustomType/generic_.h"
#include "./Network/Packet/CustomType/wstring_.h"
#include "./Network/Packet/CustomType/vector_.h"
#include "./Network/Packet/CustomType/Dictionary.h"
#include "./Network/Packet/CustomType/Hashtable.h"
#include "./Network/Packet//CustomType/GameCustomType/Vector3.h"
#include "./Network/Packet//CustomType/GameCustomType/Quaternion.h"

#include "./Network/Packet/Package.h"
#include "./Network/Packet/PacketClass.h"

#include "./Network/Protocol.h"
#include "./Network/Packet/Protocol18.h"
#include "./Network/Packet/PacketAnalyer.h"


#include "Network/Packet/ContentsProcess.h"




#include "./Network/Peer.h"


//#ifndef C_UNITYPEER_H
//#include "./Network/C_UnityPeer.h"
//#endif // !C_UNITYPEER_H



//네트워크

#include "./Network/Session.h"
#include "./Network/SessionManager.h"

#include "./Network/Server.h"
#include "./Network/ServerIOCP.h"

#include "./Network/IOCPSession.h"
#include "./Network/HeartBeat.h"


///Game /// 


#include "Game/Room.h"
#include "Game/Lobby.h"
#include "Game/LobbyManager.h"





//데이터 베이스

//#include "Database/ADODatabase.h"
//#include "Database/Query.h"
//#include "Database/DBManager.h"
//
//
//

#include "Network/Shutdown.h"

