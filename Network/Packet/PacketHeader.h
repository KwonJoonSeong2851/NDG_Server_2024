#pragma once
#include "../../stdafx.h"

enum PacketType : Int64
{
	E_InitRequest = 0,
	E_InitResponse = 1,
	E_OperationRequest = 3,
	E_OperationResponse = 4,
	E_EventData = 5,

	//채팅 패킷
	E_C_REQ_CHATTING = 1000,
	E_S_ANS_CHATTING = 1001,

	//데이터 베이스 패킷
	E_I_DB_REQ_ID_PW = 3001,
	E_I_DB_ANS_ID_PW = 3002,
	E_I_DB_REQ_LOAD_DATA = 3003,

	//로그인 패킷
	E_I_LOGIN_NOTIFY_ID_LOADED = 4001,
	E_C_REQ_ID_PW = 4002,
	E_I_ANS_ID_PW = 4003,
	E_C_REQ_SIGNIN = 4004,
	E_I_ANS_SIGNIN = 4005,


	//Server REQ Function
	E_C_REQ_USERINFO = 5001,
	E_I_ANS_USERINFO = 5002,
	E_C_REQ_GAMERESULT = 5003,
	E_I_NOTIFY_TERMINAL = 1002,

	E_C_REQ_GET_TUTORIAL = 5004,
	E_S_ANS_GET_TUTORIAL = 5005,
	E_C_REQ_SET_TUTORIAL = 5006,



	E_C_REQ_ROOM_LIST = 6001,
	E_S_ANS_ROOM_LIST = 6002,
	E_C_REQ_ROOM_CREATE = 6003,
	E_S_ANS_ROOM_CREATE = 6004,

	E_C_REQ_TEST = 9998,  
	E_S_ANS_TEST = 9999,

};