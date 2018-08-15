////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <tinyhal_releaseinfo.h>
#include <BlockStorage_decl.h>

#ifndef _DRIVERS_MFUPDATE_DECL_H_
#define _DRIVERS_MFUPDATE_DECL_H_ 1

#define MFUPDATE_UPDATETYPE_FIRMWARE 0x0000
#define MFUPDATE_UPDATETYPE_ASSEMBLY 0x0001
#define MFUPDATE_UPDATETYPE_KEY      0x0002
#define MFUPDATE_UPDATETYPE_BACKUP   0x4000
#define MFUPDATE_UPDATETYPE_USERDEF  0x8000
#define MFUPDATE_UPDATETYPE_ANY      0xFFFF

#define MFUPDATE_UPDATESUBTYPE_FIRMWARE_ALL  0x0000
#define MFUPDATE_UPDATESUBTYPE_FIRMWARE_CLR  0x0001
#define MFUPDATE_UPDATESUBTYPE_FIRMWARE_DAT  0x0002
#define MFUPDATE_UPDATESUBTYPE_FIRMWARE_CFG  0x0003
#define MFUPDATE_UPDATESUBTYPE_USER_DEFIND   0x8000
#define MFUPDATE_UPDATESUBTYPE_ANY           0xFFFF

#define MFUPDATE_UPDATESUBTYPE_ASSEMBLY_ADD             0x0001
#define MFUPDATE_UPDATESUBTYPE_ASSEMBLY_REPLACE_DEPLOY  0x0002


#define MFUPDATE_UPDATEID_ANY        0xFFFFFFFF

// TinyCLR_Debugging.h used UINT32 for m_updateId but MFUpdate_decl.h originally
// used INT32 for UpdateID... led to conflicts when needing to save/send/request
// UpdateID among devices, so switch to standardized UINT32 and make handles match IDs
typedef UINT32 UpdateID_t;

struct MFUpdateHeader
{
    UpdateID_t UpdateID;
    MFVersion  Version;
    UINT16     UpdateType;
    UINT16     UpdateSubType;
    UINT32     UpdateSize;
    UINT32     PacketSize;
};

struct MFUpdate;

struct IUpdateStorageProvider
{
    BOOL   (*Initialize  )( );
    INT32  (*Create      )( MFUpdateHeader& storageHeader, UINT32 flags );
    INT32  (*Open        )( INT32  storageID    , UINT16 storageType, UINT16 storageSubType );
    void   (*Close       )( INT32  handleStorage );
    BOOL   (*Delete      )( INT32  storageID    , UINT16 storageType, UINT16 storageSubType  );
    BOOL   (*GetFiles    )( UINT16 storageType  , INT32* storageIDs, INT32* storageCount     );
    BOOL   (*IsErased    )( INT32  handleStorage, INT32 fileOffset  , INT32  len              );
    INT32  (*Write       )( INT32  handleStorage, INT32 fileOffset  , UINT8* pData, INT32 len );
    INT32  (*Read        )( INT32  handleStorage, INT32 fileOffset  , UINT8* pData, INT32 len );
    BOOL   (*GetHeader   )( INT32  handleStorage, MFUpdateHeader* pHeader );
    UINT32 (*GetEraseSize)( INT32  handleStorage );

    void* Extension;
};

#define MFUPDATE_VALIDATION_COMMAND__GET_AUTH_TYPE 1
#define MFUPDATE_AUTHENTICATION_TYPE__SSL  1
#define MFUPDATE_AUTHENTICATION_TYPE__CRC  2
#define MFUPDATE_AUTHENTICATION_TYPE__X509 3

struct IUpdateValidationProvider
{
    BOOL (*AuthCommand   )( MFUpdate* pUpdate, UINT32 cmd        , UINT8* pArgs        , INT32  argsLen    , UINT8* pResponse   , INT32& responseLen );
    BOOL (*Authenticate  )( MFUpdate* pUpdate, UINT8* pAuth      , INT32  authLen                                                                    );    
    BOOL (*ValidatePacket)( MFUpdate* pUpdate, UINT8* pPacket    , INT32  packetLen    , UINT8* pValidation, INT32 validationLen                     );
    BOOL (*ValidateUpdate)( MFUpdate* pUpdate, UINT8* pValidation, INT32  validationLen                                                              );    

    void* Extension;
};

struct IUpdateBackupProvider
{
    BOOL (*CreateBackup )( UINT16 updateType, UINT16 updateSubType );
    BOOL (*RestoreBackup)( UINT16 updateType, UINT16 updateSubType );
    BOOL (*DeleteBackup )( UINT16 updateType, UINT16 updateSubType );

    void* Extension;
};

#define MFUPDATE_FLAGS__INUSE           0x00000001
#define MFUPDATE_FLAGS__BACKUP_REQIURED 0x00000002
#define MFUPDATE_FLAGS__AUTHENTICATED   0x00000004
#define MFUPDATE_FLAGS__VALIDATED       0x00000008
// TODO: formalize state machine and neighbor state tracking
#define STATE_OFFSET 16
#define NEIGHBOR_OFFSET 24
#define STATE_MASK                      (0xFF << STATE_OFFSET)
#define NEIGHBOR_MASK                   (0xFF << NEIGHBOR_OFFSET)
#define RECOVER_REBOOT_FLAG             (0x1 << 0)
#define START_FLAG                      (0x1 << 1)
#define AUTHCMD_FLAG                    (0x1 << 2)
#define AUTHENTICATED_FLAG              (0x1 << 3)
#define GETMISSINGPKTS_FLAG             (0x1 << 4)
#define ADDPACKET_FLAG                  (0x1 << 5)
#define VALIDATED_FLAG                  (0x1 << 6)
#define INSTALL_FLAG                    (0x1 << 7)
#define STATE_FLAGS__RECOVER_REBOOT     (RECOVER_REBOOT_FLAG << STATE_OFFSET)
#define STATE_FLAGS__START              (START_FLAG          << STATE_OFFSET)
#define STATE_FLAGS__AUTHCMD            (AUTHCMD_FLAG        << STATE_OFFSET)
#define STATE_FLAGS__AUTHENTICATED      (AUTHENTICATED_FLAG  << STATE_OFFSET)
#define STATE_FLAGS__GETMISSINGPKTS     (GETMISSINGPKTS_FLAG << STATE_OFFSET)
#define STATE_FLAGS__ADDPACKET          (ADDPACKET_FLAG      << STATE_OFFSET)
#define STATE_FLAGS__VALIDATED          (VALIDATED_FLAG      << STATE_OFFSET)
#define STATE_FLAGS__INSTALL            (INSTALL_FLAG        << STATE_OFFSET)
#define NEIGHBOR_FLAGS__RECOVER_REBOOT  (RECOVER_REBOOT_FLAG << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__START           (START_FLAG          << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__AUTHCMD         (AUTHCMD_FLAG        << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__AUTHENTICATED   (AUTHENTICATED_FLAG  << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__GETMISSINGPKTS  (GETMISSINGPKTS_FLAG << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__ADDPACKET       (ADDPACKET_FLAG      << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__VALIDATED       (VALIDATED_FLAG      << NEIGHBOR_OFFSET)
#define NEIGHBOR_FLAGS__INSTALL         (INSTALL_FLAG        << NEIGHBOR_OFFSET)

struct IUpdateProvider
{
    BOOL   (*InitializeUpdate)( MFUpdate* pUpdate );
    BOOL   (*GetProperty     )( MFUpdate* pUpdate, LPCSTR szPropName , UINT8* pPropValue, INT32* pPropValueSize );
    BOOL   (*SetProperty     )( MFUpdate* pUpdate, LPCSTR szPropName , UINT8* pPropValue, INT32 pPropValueSize  );
    BOOL   (*InstallUpdate   )( MFUpdate* pUpdate, UINT8* pValidation, INT32 validationLen );

    void* Extension;
};

struct IUpdatePackage
{
    LPCSTR                           ProviderName;
    const IUpdateProvider*           Update;
    const IUpdateValidationProvider* Validation;
    const IUpdateStorageProvider*    Storage;
    const IUpdateBackupProvider*     Backup;

    void* Extension;
};

struct MFUpdate
{
    MFUpdateHeader Header;
    INT32          StorageHandle;
    UINT32         Flags;

    const IUpdatePackage *Providers;
    void* Extension;

    UINT32 m_finalPacketIdx;
    UINT32 m_missingPktsWordfieldSize;
    static const unsigned int MAX_MISSING_WORDFIELD_SIZE = 25;
    UINT32 m_missingPkts[MAX_MISSING_WORDFIELD_SIZE];

    static const UpdateID_t badHandle = 0;  //!< Original version used INT32 and -1 for bad handles, but we need to use UINT32 for updateID for new protocol.
    static MFUpdate* GetUpdate(UpdateID_t handle);

    BOOL IsAuthenticated() { return 0 != (Flags & MFUPDATE_FLAGS__AUTHENTICATED); }
    BOOL IsValidated()     { return 0 != (Flags & MFUPDATE_FLAGS__VALIDATED    ); }
};


void  MFUpdate_Initialize       ();
void  MFUpdate_EnumerateUpdates (UpdateID_t updateIDs[], int len_updateIDs );
UpdateID_t MFUpdate_InitUpdate  ( LPCSTR szProvider, MFUpdateHeader& update );
BOOL  MFUpdate_AuthCommand      ( UpdateID_t updateHandle, UINT32 cmd, UINT8* pArgs, INT32 argsLen, UINT8* pResponse, INT32& responseLen );
BOOL  MFUpdate_Authenticate     ( UpdateID_t updateHandle, UINT8* pAuthData, INT32 authLen );
BOOL  MFUpdate_Open             ( UpdateID_t updateHandle );
BOOL  MFUpdate_Create           ( UpdateID_t updateHandle );
BOOL  MFUpdate_GetProperty      ( UpdateID_t updateHandle, LPCSTR szPropName, UINT8* pPropValue, INT32* pPropValueSize);
BOOL  MFUpdate_SetProperty      ( UpdateID_t updateHandle, LPCSTR szPropName, UINT8* pPropValue, INT32  pPropValueSize);
BOOL  MFUpdate_GetMissingPackets( UpdateID_t updateHandle, UINT32* pPackets , INT32* pCount );
BOOL  MFUpdate_AddPacket        ( UpdateID_t updateHandle, INT32 packetIndex, UINT8* packetData, INT32 packetLen, UINT8* pValidationData, INT32 validationLen );
BOOL  MFUpdate_Validate         ( UpdateID_t updateHandle, UINT8* pValidationData, INT32 validationLen );
BOOL  MFUpdate_Install          ( UpdateID_t updateHandle, UINT8* pValidationData, INT32 validationLen );
BOOL  MFUpdate_Delete           ( UpdateID_t updateHandle );
void  MFUpdate_Clear            ( MFUpdate* update );

//--//

extern const IUpdatePackage* g_UpdatePackages;
extern const INT32           g_UpdatePackageCount;


#define MAX_UPDATE_COUNT 2       //!< FIXME: breaks one definition rule. update storage provider needs to agree with this.
extern UINT32       g_UpdateCount;
extern MFUpdate     g_Updates[MAX_UPDATE_COUNT];


#endif // _DRIVERS_MFUPDATE_DECL_H_
