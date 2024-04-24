#pragma once

enum class PeerStatusCode : int
{
    Connect = 1024,
    Disconnect = 1025,
    //더 추가예정

};

enum class EgMessageType : unsigned char
{
    Init = (unsigned char)0,
    InitResponse = 1,
    Operation = 2,
    OperationResponse = 3,
    Event = 4,
    InternalOperationRequest = 6,
    InternalOperationResponse = 7,
    Message = 8,
    RawMessage = 9,
};