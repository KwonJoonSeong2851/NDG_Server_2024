#pragma once


enum class GpType : byte
{
    /// <summary>Unkown. GpType: 0.</summary>
    Unknown = (byte)0,
    /// <summary>Boolean. GpType: 2. See: BooleanFalse, BooleanTrue.</summary>
    Boolean = (byte)2,
    /// <summary>Byte. GpType: 3.</summary>
    Byte = (byte)3,
    /// <summary>Short. GpType: 4.</summary>
    Short = (byte)4,
    /// <summary>32-bit floating-point value. GpType: 5.</summary>
    Float = (byte)5,
    /// <summary>64-bit floating-point value. GpType: 6.</summary>
    Double = (byte)6,
    /// <summary>String. GpType: 7.</summary>
    String = (byte)7,
    /// <summary>Null value don't have types. GpType: 8.</summary>
    Null = (byte)8,
    /// <summary>CompressedInt. GpType: 9.</summary>
    CompressedInt = (byte)9,
    /// <summary>CompressedLong. GpType: 10.</summary>
    CompressedLong = (byte)10, // 0x0A
    /// <summary>Int1. GpType: 11.</summary>
    Int1 = (byte)11, // 0x0B
    /// <summary>Int1_. GpType: 12.</summary>
    Int1_ = (byte)12, // 0x0C
    /// <summary>Int2. GpType: 13.</summary>
    Int2 = (byte)13, // 0x0D
    /// <summary>Int2_. GpType: 14.</summary>
    Int2_ = (byte)14, // 0x0E
    /// <summary>L1. GpType: 15.</summary>
    L1 = (byte)15, // 0x0F
    /// <summary>L1_. GpType: 16.</summary>
    L1_ = (byte)16, // 0x10
    /// <summary>L2. GpType: 17.</summary>
    L2 = (byte)17, // 0x11
    /// <summary>L2_. GpType: 18.</summary>
    L2_ = (byte)18, // 0x12
    /// <summary>Custom Type. GpType: 19.</summary>
    Custom = (byte)19, // 0x13
    /// <summary>Dictionary. GpType: 20.</summary>
    Dictionary = (byte)20, // 0x14
    /// <summary>Hashtable. GpType: 21.</summary>
    Hashtable = (byte)21, // 0x15
    /// <summary>ObjectArray. GpType: 23.</summary>
    ObjectArray = (byte)23, // 0x17
    /// <summary>OperationRequest. GpType: 24.</summary>
    OperationRequest = (byte)24, // 0x18
    /// <summary>OperationResponse. GpType: 25.</summary>
    OperationResponse = (byte)25, // 0x19
    /// <summary>EventData. GpType: 26.</summary>
    EventData = (byte)26, // 0x1A
    /// <summary>Boolean False. GpType: 27.</summary>
    BooleanFalse = (byte)27, // 0x1B
    /// <summary>Boolean True. GpType: 28.</summary>
    BooleanTrue = (byte)28, // 0x1C
    /// <summary>ShortZero. GpType: 29.</summary>
    ShortZero = (byte)29, // 0x1D
    /// <summary>IntZero. GpType: 30.</summary>
    IntZero = (byte)30, // 0x1E
    /// <summary>LongZero. GpType: 3.</summary>
    LongZero = (byte)31, // 0x1F
    /// <summary>FloatZero. GpType: 32.</summary>
    FloatZero = (byte)32, // 0x20
    /// <summary>DoubleZero. GpType: 33.</summary>
    DoubleZero = (byte)33, // 0x21
    /// <summary>ByteZero. GpType: 34.</summary>
    ByteZero = (byte)34, // 0x22
    /// <summary>Array for nested Arrays. GpType: 64 (0x40). Element count and type follows.</summary>
    Array = (byte)64, // 0x40
    BooleanArray = (byte)66, // 0x42
    ByteArray = (byte)67, // 0x43
    ShortArray = (byte)68, // 0x44
    FloatArray = (byte)69, // 0x45
    DoubleArray = (byte)70, // 0x46
    StringArray = (byte)71, // 0x47
    CompressedIntArray = (byte)73, // 0x49
    CompressedLongArray = (byte)74, // 0x4A
    CustomTypeArray = (byte)83, // 0x53
    DictionaryArray = (byte)84, // 0x54
    HashtableArray = (byte)85, // 0x55
    /// <summary>Custom Type Slim. GpType: 128 (0x80) and up.</summary>
    CustomTypeSlim = (byte)128, // 0x80

    Quaternion = (byte)209,
    Vector3 = (byte)214,
};