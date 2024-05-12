#pragma once

enum class PeerStatusCode : unsigned char
{
    PeerCreate,
    Initialized,
    JoinedLobby,
    JoinedRoom,
    //�� �߰�����

};

enum class OperationCode : unsigned char
{
    Join = 255,
    JoinLobby = 229,
    LeaveLobby = 228,
    CreateRoom = 227,
    JoinRoom = 226,
    JoinRandomGame = 225,
    Leave = 254,
    RaiseEvent = 253,
    SetProperties = 252,
    GetProperties = 251,
    ChangeGroups = 248,
    GetLobbyStats = 221,
    GetRegions = 220,
    ServerSettings = 218,
    GetRoomList = 217,
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

enum class ParameterCode : unsigned char
{
    SuppressRoomEvents = 0xED,
    EmptyRoomTTL = 236,
    PlayerTTL = 235,
    EventForward = 234,
    IsInactive = 233,
    CheckUserOnJoin = 232,
    ExpectedValues = 231,
    Address = 230,
    PeerCount = 229,
    GameCount = 228,
    MasterPeerCount = 227,
    UserId = 225,
    ApplicationId = 224,
    Position = 223,
    MatchMakingType = 223, // �� ��� �ߺ��� ���̹Ƿ� ���ǰ� �ʿ��մϴ�.
    GameList = 222,
    Secret = 221,
    AppVersion = 220,
    RoomName = 255,
    Broadcast = 250,
    ActorList = 252,
    ActorNr = 254,
    PlayerProperties = 249,
    CustomEventContent = 245,
    Data = 245, // �� ��� �ߺ��� ���̹Ƿ� ���ǰ� �ʿ��մϴ�.
    Code = 244,
    RoomProperties = 248,
    Properties = 251,
    TargetActorNr = 253,
    ReceiverGroup = 246,
    Cache = 247,
    CleanupCacheOnLeave = 241,
    Group = 240,
    Remove = 239,
    PublishUserId = 239, // �� ��� �ߺ��� ���̹Ƿ� ���ǰ� �ʿ��մϴ�.
    Add = 238,
    Info = 218,
    ClientAuthenticationType = 217,
    ClientAuthenticationParams = 216,
    JoinMode = 215,
    ClientAuthenticationData = 214,
    MasterClientId = 203,
    LobbyName = 213,
    LobbyType = 212,
    LobbyStats = 211,
    Region = 210,
    CacheSliceIndex = 205,
    Plugins = 204,
    NickName = 202,
    PluginName = 201,
    PluginVersion = 200,
    Cluster = 196,
    ExpectedProtocol = 195,
    CustomInitData = 194,
    EncryptionMode = 193,
    EncryptionData = 192,
    RoomOptionFlags = 191
};

enum class RoomPropertyKey : unsigned char {
    MaxPlayers = 255, ///< �ִ� �÷��̾� ��
    IsVisible = 254, ///< �κ񿡼� ���� ������ ��Ÿ���� �����մϴ�.
    IsOpen = 253, ///< ���� ���� �ִ��� ���θ� ��Ÿ���ϴ�.
    PlayerCount = 252, ///< ���� �÷��̾� ��
    Removed = 251, ///< �����ǰų� ������ ���� ��� true
    PropsListedInLobby = 250, ///< �κ񿡼� �������� Custom Properties ���
    CleanupCacheOnLeave = 249, ///< �÷��̾ ���� �� ĳ�� ���� ����
    MasterClientId = 248, ///< ������ Ŭ���̾�Ʈ ID
    ExpectedUsers = 247, ///< ����Ǵ� ����� ��
    PlayerTtl = 246, ///< �÷��̾��� TTL (Time To Live)
    EmptyRoomTtl = 245 ///< ������ �÷��̾ ������ �� ���� �����Ǵ� �ð�
};

enum class ServerConnection : unsigned char
{
    MasterServer,
    GameServer
};

class EventCode {
public:
    static const unsigned char GameList = 230;
    static const unsigned char GameListUpdate = 229;
    static const unsigned char AppStats = 226;
    static const unsigned char LobbyStats = 224;
    static const unsigned char Join = 255;
    static const unsigned char Leave = 254;
    static const unsigned char PropertiesChanged = 253;
    static const unsigned char ErrorInfo = 251;
    static const unsigned char CacheSliceChanged = 250;
    static const unsigned char AuthEvent = 223;
};

class GameEventCode {
public:
    static constexpr byte RPC = 200;
    static constexpr byte SendSerialize = 201;
    static constexpr byte Instantiation = 202;
    static constexpr byte CloseConnection = 203;
    static constexpr byte Destroy = 204;
    static constexpr byte RemoveCachedRPCs = 205;
    static constexpr byte SendSerializeReliable = 206; 
    static constexpr byte DestroyPlayer = 207; 
    static constexpr byte OwnershipRequest = 209;
    static constexpr byte OwnershipTransfer = 210;
    static constexpr byte VacantViewIds = 211;
    static constexpr byte OwnershipUpdate = 212;
};

class RpcTarget
{
public:
    static constexpr byte All = 0;
    static constexpr byte Others = 1;
    static constexpr byte MasterClient = 2;
    static constexpr byte AllBuffered = 3;
    static constexpr byte OthersBuffered = 4;
};