#pragma once
#include "stdafx.h"

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

//�ܺ� ��ƿ
#include "./Util/tinyxml/tinyxml.h"




//��ƿ
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



//��Ʈ��ũ
#include "./Network/Server.h"
#include "./Network/ServerIOCP.h"
#include "./Network/Session.h"

#include "./Network/IOCPSession.h"
#include "./Network/SessionManager.h"
#include "./Network/HeartBeat.h"

#include "./Network/TerminalSession.h"
#include "./Network/Terminal.h"
#include "./Network/TerminalManager.h"


//��Ŷ ó��
#include "Network/Packet/StreamBuffer.h"

#include "./Network/Packet/Stream.h" //delete

#include "./Network/Packet/PacketHeader.h"
#include "./Network/Packet/PacketClass.h"
#include "./Network/Packet/PacketAnalyer.h"

#include "./Network/Packet/PacketFactory.h" //delete

#include "./Network/Packet/Package.h"
#include "./Network/Protocol.h"
#include "./Network/Packet/GpType.h"

//��Ŷ ����
#include "Network/Packet/Operation.h"


//#include "./Network/Packet/Protocol.h"



//������ ���̽�

#include "Database/ADODatabase.h"
#include "Database/Query.h"
#include "Database/DBManager.h"




#include "Network/Shutdown.h"

