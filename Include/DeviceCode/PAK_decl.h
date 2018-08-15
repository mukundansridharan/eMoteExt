/**
 * PAK_decl.h
 *
 *  Created on: Apr 1, 2013
 *  Updated on: March 31, 2014
 *      Author: Michael Andrew McGrath
 */

#ifndef PAK_DECL_H_
#define PAK_DECL_H_


#include <WireProtocol.h>
#include <Samraksh\Mac_decl.h>
#include <Samraksh\Message.h>
#include <MFUpdate_decl.h>


/**
 * UpdateMessage in UpdateState.cs
 * must fit inside 4 bits for packing to CLR via in NativeToManagedUpdaterProgressHandler to NativeEventQueue
 */
enum UpdateMessage
{
        NOMSG = 0,
        START = 1, START_ACK = 2,
        AUTHCMD = 3, AUTHCMD_ACK = 4,
        AUTHENTICATE = 5, AUTHENTICATE_ACK = 6,
        GETMISSINGPKTS = 7, GETMISSINGPKTS_ACK = 8,
        ADDPACKET = 9, ADDPACKET_ACK = 10,
        INSTALL = 11, INSTALL_ACK = 12,
};

/// used for g_UpdateManagerContext for Interop callback.
// #include <TinyCLR_Runtime.h>
//extern CLR_RT_HeapBlock_NativeEventDispatcher *g_UpdateManagerContext;
//extern UINT64 g_UpdateManagerUserData;
//extern BOOL g_UpdateManagerInterruptEnabled;

typedef void (*UPDATER_PROGRESS_HANDLER)(UINT32 updateID, UINT16 destAddr, UINT8 u4_cmd, UINT16 u12_data);

//TODO: specify packed attribute. do this better.
/**
 * simple payload contains only necessary pieces of WP_Message
 */
typedef struct {
    UINT32 cmd;
    UINT32 flags;
    UINT8 payload; //todo: make payload the field size since this struct can't be forward-declared anyway.
} simple_payload_t;

enum Emote_Update_ErrorFlags {
    UPDATE_INSTALL_NO_MSG          = 0x1 << 0,
    UPDATE_INSTALL_FAIL_RADIO      = 0x1 << 1,
    UPDATE_INSTALL_FAIL_TIME       = 0x1 << 2,
    UPDATE_INSTALL_FAIL_VALIDATION = 0x1 << 3,
    UPDATE_INSTALL_MSG_OUT_OF_SYNC = 0x1 << 4,
    UPDATE_SEND_BADHANDLE          = 0x1 << 5,
    UPDATE_INSTALL_FAIL_INSTALL    = 0x1 << 6,
};

inline Emote_Update_ErrorFlags operator|(Emote_Update_ErrorFlags a, Emote_Update_ErrorFlags b) {
    return static_cast<Emote_Update_ErrorFlags>(static_cast<int>(a) | static_cast<int>(b));
}

struct Samraksh_Emote_Update
{
protected:
    static HAL_COMPLETION s_UpdateCompletion;              //!< used for scheduling installation. only one completion allowed at a time.
    static BOOL           s_UpdateCompletionInitialized;   //!< prevent allocating multiple copies of pending install message on heap.
    static void*          s_UpdateCompletionArg;           //!< save ptr to arg on heap in case completion is aborted.

private:
    bool              m_fInitialized;    //!< singleton pattern.
    bool              m_fMACInitialized;
    bool              m_fMACHandlerInitialized;
    bool              m_fMACInitializedByNativeBeforeInstall;
    WP_Controller     m_controller;      //!< use the WireProtocol engine.
    WP_Message        m_outboundMessage; //!< used for echo and manager

    static WP_Message s_lastUsbMessage;  //!< when basestation, use for constructing correct sequence number in reply to PC.
    static void*      s_lastUsbOwner;    //!< when basestation, use for constructing correct transport comm  in reply to PC.
    UINT8 m_outboundMessagePayload[128];

    static const UINT32 c_Monitor_UpdateInit                       = 0x0000000F; //!< WireProtocol packet type.  Must match duplicate definition in TinyCLR_Debugging.h' CLR_DBG_Commands::
    static const UINT32 c_Monitor_UpdateDeInit                     = 0x00000010; //
    static const UINT32 c_Debugging_MFUpdate_Start                 = 0x00020056; //
    static const UINT32 c_Debugging_MFUpdate_AddPacket             = 0x00020057; //
    static const UINT32 c_Debugging_MFUpdate_Install               = 0x00020058; //
    static const UINT32 c_Debugging_MFUpdate_AuthCommand           = 0x00020059; //
    static const UINT32 c_Debugging_MFUpdate_Authenticate          = 0x00020060; //
    static const UINT32 c_Debugging_MFUpdate_GetMissingPkts        = 0x00020061; //

    Emote_Update_ErrorFlags last_error;


public:
    static const INT32 COMM_BUFFER_SIZE = (IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t));
    //TODO: static_assert(COMM_BUFFER_SIZE == MAC.h's template instantiation size minus the header).

    MACEventHandler PAK_EventHandler;  //!< handle incoming wireless events.
    UINT8 PAK_MacName;                 //!< really enum MacName, aka MacId in other parts of the API.
    UINT8 PAK_routingAppID;
    UINT8 PAK_radioName;
    MACConfig PAK_MacConfig;
    UINT8 PAK_channel;

    UINT8 SendBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];          //!< local buffer for storing outgoing packets.      Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.
    UINT8 ReceiveBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];       //!< local buffer for manipulating incoming packets. Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.
#if defined(LEGACY_RECEIVE_HANDLER)
    UINT8 MacReceiveBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE +16]; //!< extra buffer because MAC inserts size into first two bytes instead of passing as a parameter, so passing the ReceiveBuffer to MAC would make ReceiveBuffer incompatible with its use as a regular payload buffer elsewhere.  Also I don't know why the design decision was made that MAC_GetNextPacket writes metadata to the end of the buffer instead of passing them as parameters like the old API.
#endif
    INT32 m_ReceiveState;


    static UINT8  s_RadioID;            //!< MAC API change causes storage of Radio ID outside of MacConfig.
    static BOOL   s_fRadioOn;           //!< indicates whether MAC is initialized.
    static BOOL   s_fUseWpPacket;       //!< determines whether wireless interface will send entire WireProtocol packet.  idea is that short format fits inside single wireless packet payload.
    static BOOL   s_fBaseStationMode;   //!< turn on/off basestation mode.
    static UINT16 s_destAddr;           //!< wireless destination in basestation mode;
    static UINT32 s_destMissingPkts[MFUpdate::MAX_MISSING_WORDFIELD_SIZE];
    static BOOL   s_fPublishUpdateMode; //!< turn on/off update publisher mode
    static UPDATER_PROGRESS_HANDLER s_UpdaterProgressHandler; //!< notify (managed library) something happened.


    static void CreateInstance();
    static void Initialize();
    static bool InitializeMac();
    static bool InitializeMacHandler();
    static bool SetMacHandler();
    static bool InitializeDriversAfterInstall();
    static bool UnInitializeDriversBeforeInstall();
    static void Cleanup();
    static void DeleteInstance();

    static void Receive_IEEE_802_15_4(void* buffer, UINT16 payloadType);
    static void Receive(void* msg, UINT16 size);
    static void SendAck(void* msg, UINT16 size, NetOpStatus status);
    static void NeighborChange(INT16 numberOfNeighbors);

    static bool Wireless_Phy_ReadReceiveBytes( void* state, UINT8*& ptr, UINT32 & size );
    static bool Wireless_Phy_TransmitMessage ( void* state, const WP_Message* msg      );

    static bool App_ProcessHeader ( void* state,  WP_Message* msg );
    static bool App_ProcessPayload( void* state,  WP_Message* msg );
    static bool App_Release       ( void* state,  WP_Message* msg );

    static void SendStart         ( UpdateID_t id, UINT16 destAddr );
    static void SendAuthCommand   ( UpdateID_t id, UINT16 destAddr );
    static void SendAuthenticate  ( UpdateID_t id, UINT16 destAddr );
    static void SendGetMissingPkts( UpdateID_t id, UINT16 destAddr );
    static void SendAddPacket     ( UpdateID_t id, UINT16 destAddr, UINT32 packetIndex );
    static void SendInstall       ( UpdateID_t id, UINT16 destAddr );
    static void HandleTimeout();

    // Pull MFUpdate functions from Debugger.cpp into this library to handle Wired + Wireless
    static bool UpdateInit           ( WP_Message* msg, void* owner );
    static bool UpdateDeInit         ( WP_Message* msg, void* owner );
    static bool Start                ( WP_Message* msg, void* owner );
    static bool AuthCommand          ( WP_Message* msg, void* owner );
    static bool Authenticate         ( WP_Message* msg, void* owner );
    static bool GetMissingPkts       ( WP_Message* msg, void* owner );
    static bool AddPacket            ( WP_Message* msg, void* owner );
    static bool Install              ( WP_Message* msg, void* owner );

    static void SetUpdateProgressCallback(UPDATER_PROGRESS_HANDLER*);

protected:
    static void UpdateCompletion(void *arg);
};



extern Samraksh_Emote_Update    g_Samraksh_Emote_Update;

struct NeighborUpdateRecord
{
	static const unsigned int MAX_MISSING_WORDFIELD_SIZE = 25;
	UINT16 destAddr;
    UINT32 m_neighborMissingPkts[MAX_MISSING_WORDFIELD_SIZE];
    UINT32 UpdateID;
};



//TODO: API for new WirelessUpdate is below.  Implement and augment this after the baseline update works.
#if 0

// need pak protocol support added to wire protocol, with each sub-command calling one of these functions.

// need config struct here for storing additional info?  how will we tell what is not finished?
// how to store both configs and updates? use config area?  or use special update type?

enum PAKConfigExtra {
    SINGLEUSER     = 0x01, /*!< config should be the only config active. */
};

enum PAKState {
    INIT     = 0x00,
    READY    = 0x01,
    DEPLOYED = 0x02,
    LOADED   = 0x04,
    UNDEFINED= 0x08
};

/**
 * Represents a plan for what should be deployed.
 * DeploymentConfiguration is a regular file with a DeploymentConfigHeader followed by the assembly list in the payload.
 */
struct PAKConfigHeader {
    UINT32 MAGIC;    /*!< CONFIG file type identifier. */
    UINT32 ID;         /*!< unique identity for the config. */
    UINT32 VERSION;  /*!< this config ID's version.  Higher versions will need replaced. */
    UINT8  CONFIGVERSION; /*!< config file version. *//*NOT NEEDED*/
    UINT32 PAYLOADLEN; /*!< length of config payload. */
    UINT8  EXTRA;     /*!< extra config options like singleuser. */
    UINT8  STATELOCAL; /*!< local state of the config. */
    UINT8  STATEGROUP; /*!< state of the config in the group. */
    UINT8  PERMISSIONSTABLELENGTH; /*!< how long the permissions table is in bytes. */
    UINT16 ASSEMBLYLISTOFFSET; /*!< Offset of where the list assemblies is in this file.*/
    UINT16 ASSEMBLYLISTLENGTH;
};

struct PAKConfigAssemblyStringTable {

};

// TODO: implement PAKSchedOpts
enum PAKSchedOpts {
    ABSOLUTESTART = 0x01,
    NEXTAVAILABLE = 0x02,
    FORCESTART    = 0x04,
    FORCELENGTH   = 0x08,
    FORCESTOP     = 0x10,
    PERIODIC      = 0X20
};

// TODO: implement PAKConfigPermissionsTableENTRY
struct PAKConfigPermissionsTableENTRY {
    UINT32  PERMISSIONTYPE;    /* node, group, all*/
    UINT32  TARGETGROUP;
    UINT32  TARGETID;          /* node id or group id*/
    UINT32  SCHEDOPTS;         /* absolute start, next available start, force start, guarantee length, guarantee stop, periodic */
    UINT32  STARTTIME;         /*do not start before */
    UINT32  LENGTH;            /* time to run experiment */
    UINT32  EXPIRETIME;        /*do not start after */
};


/**
 * Re-read assemblies and configs.
 */
void Initialize();

//////////////////////////////////////////////////
/**
 * Place configuration file in PAK storage.
 */
void StoreConfig();

/**
 * Delete configuration file from PAK storage.
 * OPTION: delete referenced assemblies too.
 */
void DeleteConfig();



/**
 * Determines whether all assemblies are available (done being downloaded)
 */
void IsConfigReady();

/**
 * Determines whether a config is in the staging area.
 */
void IsConfigStaged();

/**
 * Determines whether the config's assemblies are written in the deployment area.
 */
void IsConfigDeployed();

/**
 * Determines whether a config's assemblies are loaded in the CLR.
 */
void IsConfigLoaded();



//////////////////////////////////////////////////
/**
 * Process an incoming app storage request.
 */
void StoreAssembly();

/**
 * Process an incoming delete assembly request.
 */
void DeleteAssembly();

void IsAssemblyReady();
void IsAssemblyStaged();
void IsAssemblyDeployed();
void IsAssemblyLoaded();


//////////////////////////////////////////////////
void StorePatch();
void ApplyPatchToStoredAssembly();
void ApplyPatchToDeployedAssembly();
void DeletePatch();

//////////////////////////////////////////////////

/**
 * Adds a config's assemblies to the list of assemblies to be deployed.
 * Fails if no more space in deployment region for this config's assemblies.
 * Fails if incompatible with current staged configs.
 */
void StageConfig();
/**
 * Removes a config's assemblies from the deployment region.
 * Leaves assemblies in the deployment region if referenced in another config.
 */
void UnStageConfig();

/**
 * copies an assembly to the deployment region.
 */
void DeployAssembly();

/**
 * removes an assembly from the deployment region.
 */
void UnDeployAssembly();

/**
 * Soft-reboots the CLR and replaces deployment before interpreter loads assemblies.
 */
void FinalizeConfig();


void RefreshStorage();

/**
 * provide a list of assemblies
 */
void ListStorage(char*);

/**
 * provide a list of assemblies in the deployment region.
 */
void ListDeployment(char* );

/**
 * List assemblies that are loaded in the CLR.
 */
void ListLoaded(char* );

/////////////////////////////////////////////////////////////////////////////
// PAK COMMUNICATION
// TODO: not sure about network-layer like groups and IDs vs app-layer groups and IDs.

#endif


#endif /* PAK_DECL_H_ */
