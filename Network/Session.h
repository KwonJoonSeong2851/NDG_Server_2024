#pragma once
#include "../stdafx.h"




struct SOCKET_DATA
{
    SOCKET        socket_;
    SOCKADDR_IN   addrInfo_;
};

enum
{
    SESSION_TYPE_TERMINAL,
    SESSION_TYPE_CLIENT,
};

class Package;
class Packet;
class User;
class Peer;


/// <summary>
/// Accept�� Client�鿡 ���� �ۼ��� �������̽��� �����ϰ� 
/// Client�� ������ �����ϴ� �߻�Ŭ�����Դϴ�.
/// </summary>
class Session
{
protected:
    SOCKET_DATA      m_socketData;
    oid_t            m_id;
    int8_t           m_type;
    tick_t           m_lastHeartBeat;
    int              m_sessionLocalTime;
    Protocol*        m_protocol;
    Peer*             m_peer;
    bool             SetSocketOpt();
  

public:
    Session();
    virtual ~Session();
    virtual bool OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);

    virtual void OnSend(size_t transferSize) = 0;
    virtual void SendPacket(Packet* packet)  = 0;

    virtual Package* OnRecv(size_t transferSize) = 0;
    virtual void RecvStandBy() {};
    virtual void OnClose();

    virtual void ReadPing(char* buf) = 0;
    virtual void SendPing() = 0;

    SOCKET& GetSocket();
    str_t GetClientAddress();

    oid_t GetId();
    void SetId(oid_t id); 

    int8_t GetType();
    void SetType(int8_t type);

    tick_t HeartBeat();
    void UpdateHeartBeat();

    Protocol* GetProtocol();
    void SetPeer(Peer* peer);
    Peer* GetPeer();




};