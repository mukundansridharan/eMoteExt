/*
 * Copyright The Samraksh Company
 *
 * Name : 		 message.h
 *
 * Author : 	 Mukundan.Sridharan@samraksh.com, nived.sivadas@samraksh.com
 *
 * Description : defines the interface for message, the theory is that all protocols would be required to
 *				 implement this interface thereby ensuring a common template across multiple higher layers
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <tinyhal.h>
#include <Samraksh\Radio_decl.h>

#define PACK __attribute__ ((packed))



// template for message header
//template<class Header_T,UINT16 PayLoadSize_T, class Footer_T>
template<class Header_T,UINT16 PayLoadSize_T, class Footer_T, class Metadata_T>
class Message
{
	Header_T header;
	UINT8 payload[PayLoadSize_T];
	Footer_T footer;
	Metadata_T metadata;

public:


	// Returns a pointer to the message
	Message* GetMessage()
	{
		return this;
	}

	// returns the size of the message packet
	UINT16 GetMessageSize()
	{
		return sizeof(Header_T) + sizeof(Footer_T) + sizeof(Metadata_T) + PayLoadSize_T;
		//return sizeof(Header_T) + sizeof(Footer_T) + PayLoadSize_T;
	}

	// returns a pointer to the header
	Header_T* GetHeader()
	{
		return &header;
	}

	// returns a pointer to the header
	const Header_T* GetHeaderConst() const
	{
		return &header;
	}

	// Returns the size of the header
	// ensure that the mac header class has a size function
	UINT16 GetHeaderSize()
	{
		return sizeof(Header_T);
	}

	// returns a pointer to the payload
	UINT8* GetPayload()
	{
		return payload;
	}

	// returns the payload size
	UINT16 GetPayloadSize()
	{
		return  PayLoadSize_T;
	}

	// returns a pointer to the footer
	Footer_T* GetFooter()
	{
		return &footer;
	}

	// returns the size of the footer
	UINT16 GetFooterSize()
	{
		return sizeof(Footer_T);
	}

	// returns a pointer to the metadata
	Metadata_T* GetMetaData()
	{
		return &metadata;
	}

	// returns a pointer to the metadata
	const Metadata_T* GetMetaDataConst() const
	{
		return &metadata;
	}

	// returns the metadata size
	UINT16 GetMetaDataSize()
	{
		return sizeof(Metadata_T);
	}

};


#define OMAC_TYPE_ID 139

#define APPLICATION_PAYLOADTYPE_MASK	0xE0
#define MF_PAYLOADTYPE_MASK		0x1F
//Well known packet payload types: MFM stands for MicroFramework Message.
//To be kept in sync with that in IMAC.cs in Samraksh_eMote_Net.dll
//Values 0 to 31 (lower 5 bits - 0x00 to 0x1F) and possibly 0xFF (255) are for MF.
//Rest are for applications.
enum PayloadTypeUser{
	TYPE00 = 0x00,
	TYPE01 = 0x01,
	TYPE02 = 0x02,
	TYPE03 = 0x03,
	TYPE04 = 0x04,
	TYPE05 = 0x05,
	TYPE06 = 0x06,
	TYPE07 = 0x07,
	TYPE08 = 0x08,
	TYPE09 = 0x09,
	TYPE10 = 0x0A,
	TYPE11 = 0x0B,
	TYPE12 = 0x0C,
	TYPE13 = 0x0D,
	TYPE14 = 0x0E,
	TYPE15 = 0x0F,
	TYPE16 = 0x10,
	TYPE17 = 0x11,
	TYPE18 = 0x12,
	TYPE19 = 0x13,
	TYPE20 = 0x14,
	TYPE21 = 0x15,
	TYPE22 = 0x16,
	TYPE23 = 0x17,
	TYPE24 = 0x18,
	TYPE25 = 0x19,
	TYPE26 = 0x1A,
	TYPE27 = 0x1B,
	TYPE28 = 0x1C,
	TYPE29 = 0x1D,
	TYPE30 = 0x1E,
	TYPE31 = 0x1F,
	MFM_DATA = 0x20
};

enum PayloadTypeMAC{
	MFM_CSMA_TIMESYNCREQ = 0x21,
	MFM_CSMA_NEIGHBORHOOD = 0x22,
	MFM_CSMA_ROUTING = 0x23,
	MFM_CSMA_DISCOVERY = 0x24,
	MFM_TIMESYNC = 0x25,
	MFM_CSMA_DATA_ACK = 0x26,
	MFM_OMAC_TIMESYNCREQ = 0x31,
	MFM_OMAC_NEIGHBORHOOD = 0x32,
	MFM_OMAC_ROUTING = 0x33,
	MFM_OMAC_DISCOVERY = 0x34,
	MFM_OMAC_DATA_ACK = 0x36,
	MFM_OMAC_DATA_BEACON_TYPE = 0x37
};

#define NATIVE_APP_ID_OFFSET 0x80

enum PayloadTypeNative{
	MFM_UPDATE = NATIVE_APP_ID_OFFSET + 0x0,
};

#define IsValidNativeAppIdOffset(x) ((x)>= NATIVE_APP_ID_OFFSET && (x) < (NATIVE_APP_ID_OFFSET + MAX_NATIVE_APPS))

//#define MFM_DATA (1 << 0)
//#define MFM_TIMESYNC (1 << 1)
//#define MFM_NEIGHBORHOOD (1 << 2)
//#define MFM_ROUTING (1 << 3)
//#define MFM_DISCOVERY (1 << 4)
#define TIMESTAMPED_FLAG (1 << 0)
#define MFM_TIMESYNC_FLAG (1 << 1)
#define MFM_DISCOVERY_FLAG (1 << 2)
#define MFM_EXTENDED_MAC_INFO_FLAG (1 << 4)
//#define MFM_TIMESYNCREQ (1 << 6)
//#define MFM_DATA_ACK (1 << 7)


//IEEE802.15.4 Message structure
#define IEEE802_15_4_FRAME_LENGTH 128

//All fields up to 'network' are 802.15.4 specification fields, network is a option field for 6LowPAN compatibility
//mac_id is Samraksh's Radio API to demultiplex radio packets

#define FRAME_TYPE_BEACON 	000
#define FRAME_TYPE_DATA 	001
#define FRAME_TYPE_ACK 		010
#define FRAME_TYPE_MAC 		011

struct PACK softwareACKHeader{
	UINT8 payloadType;
	UINT16 src;
	UINT16 dest;
};

//Please refer page 80 in RF231 datasheet
typedef union{
	//Below struct (though has bit-fields) occupies more memory than the word value which stops msgs from being received.
	//It worked when I tested, but does not work now.
	//Have either the bit-field (with reserved commented out) or have the word value.
	/*struct IEEE802_15_4_Header_FCF_BitValue{
		UINT8 frameType : 3;		//Can be beacon (000), data (001), ack (010), MAC (011)
		UINT8 securityEnabled : 1;	//True if security processing applies to this frame
		UINT8 framePending : 1;		//Indicates that node which transmitted the ACK has more data to send
		UINT8 ackRequired : 1;		//If set within data or MAC command that is not broadcast, recipient
									// acks a frame within time specified by IEEE 802.15.4
		UINT8 intraPAN : 1;			//If src and dest addresses are present, PAN-ID of src address field is omitted.
		//UINT8 reserved : 3;			//Reserved
		UINT8 destAddrMode : 2;		//Format of destination address of frame
		UINT8 frameVersion : 2;		//Specifies version number corresponding to frame. Set to 1 to indicate an
									// IEEE 802.15.4-2006 frame.
		UINT8 srcAddrMode : 2;		//Format of source address of frame
	}IEEE802_15_4_Header_FCF_BitValue;*/
	UINT16 fcfWordValue;
}IEEE802_15_4_Header_FCF_t;

//This structure is as per IEEE 802.15.4 standard.
//MAC Protocol Layer Data Unit (MPDU) in page 80 of RF231 datasheet.
//Do not modify as extended mode will not work
typedef struct PACK IEEE802_15_4_Header {
  //UINT16 fcf;
  IEEE802_15_4_Header_FCF_t fcf;
  UINT8 dsn;
  UINT16 destpan;
  UINT16 dest;
  //UINT16 srcpan;
  UINT16 src;

  UINT8 length;
  UINT8 macName;
  UINT8 payloadType;
  UINT8 flags;
  //UINT8 retryAttempt;

//  IEEE802_15_4_Header() : dsn(0), destpan(0), dest(0), src(0), length(0), macName(0), payloadType(0), flags(0) {
//	  fcf.fcfWordValue = 0;
//  }

  IEEE802_15_4_Header() {
	  Clear();
  }

  UINT8 GetDestConst() const{
	  return dest;
  }
  UINT8 GetPayloadTypeConst() const{
	  return payloadType;
  }
  UINT8 GetFlagsConst() const{
	  return flags;
  }

  void Clear(){
	  fcf.fcfWordValue = 0;
	  dsn = 0;
	  destpan = 0;
	  dest = 0;
	  src = 0;
	  length = 0;
	  macName = 0;
	  payloadType = 0;
	  flags = 0;

  }

/*public:
	UINT8 GetLength(){
		return this->length;
	}
	void SetLength(UINT8 _length){
		length = _length;
	}
	UINT8 GetMACId(){
		return this->mac_id;
	}
	void SetMACId(UINT8 mac_id){
		this->mac_id = mac_id;
	}
	UINT8 GetType(){
		return type;
	}
	void SetType(UINT8 type){
		this->type = type;
	}
	// Timestamp has been changed to flags
	UINT8 GetFlags() {
		return flags;
	}
	void SetFlags(UINT8 flags) {
		this->flags = flags;
	}*/
} IEEE802_15_4_Header_t;

//Transmitting footer in a MAC frame is not required, though for FCS to succeed, footer is needed.
//It has been tested that footer need not be transmitted along with header and payload.
typedef struct PACK IEEE802_15_4_Footer{
	UINT16 FCS;
//	IEEE802_15_4_Footer() : FCS(0) {};
	IEEE802_15_4_Footer(){
		Clear();
	}
	void Clear(){
		FCS = 0;
	}
}IEEE802_15_4_Footer_t;

typedef struct PACK IEEE802_15_4_Metadata{
	/*UINT8 length;
	UINT8 mac_id;
	UINT8 type;
	UINT8 flags;*/
	UINT8 network;  // optionally included with 6LowPAN layer
	UINT8 Rssi;
	UINT8 Lqi;
	UINT32 ReceiveTimeStamp0;
	UINT32 ReceiveTimeStamp1;
	UINT8 RetryAttempts;

	UINT8 PacketID;

//  public:
	IEEE802_15_4_Metadata(){
		ClearData();
	}
	void ClearData(){
		RetryAttempts = 0;	//Retry attempt per packet
		ReceiveTimeStamp1 = 0;
		ReceiveTimeStamp0 = 0;
		Lqi = 0;
		Rssi = 0;
		network = 0;
		PacketID = 0;
	}
	/*UINT8 GetLength(){
		return this->length;
	}
	void SetLength(UINT8 _length){
		length = _length;
	}
	UINT8 GetMACId(){
		return this->mac_id;
	}
	void SetMACId(UINT8 mac_id){
		this->mac_id = mac_id;
	}
	UINT8 GetType(){
		return type;
	}
	void SetType(UINT8 type){
		this->type = type;
	}
	// Timestamp has been changed to flags
	UINT8 GetFlags() {
		return flags;
	}
	void SetFlags(UINT8 flags) {
		this->flags = flags;
	}*/
	UINT8 GetNetwork() const{
		return this->network;
	}
	void SetNetwork(UINT8 network){
		this->network = network;
	}
	UINT8 GetRssi() const{
		return Rssi;
	}
	void SetRssi(UINT8 Rssi){
		this->Rssi = Rssi;
	}
	UINT8 GetLqi() const{
		return Lqi;
	}
	void SetLqi(UINT8 Lqi){
		this->Lqi = Lqi;
	}
	UINT64 GetReceiveTimeStamp() const{
		UINT64 rtn;
		rtn=((UINT64)ReceiveTimeStamp1)<<32;
		rtn+=ReceiveTimeStamp0;
		return rtn;
	}
	void SetReceiveTimeStamp(UINT64 timestamp){
		this->ReceiveTimeStamp0 = (UINT32)timestamp;
		this->ReceiveTimeStamp1= (UINT32)(timestamp>>32);
	}
	void SetReceiveTimeStamp(INT64 timestamp){
		this->ReceiveTimeStamp0 = (UINT32)timestamp;
		this->ReceiveTimeStamp1= (UINT32)(timestamp>>32);
	}
	void SetReceiveTimeStamp(UINT32 timestamp){
		this->ReceiveTimeStamp0 = timestamp;
	}
	UINT8 GetRetryAttempts() const{
		return RetryAttempts;
	}
	UINT8 SetRetryAttempts(UINT8 r){
		RetryAttempts = r;
		return RetryAttempts;
	}

	UINT8 GetPacketID() const{
		return PacketID;
	}
	UINT8 SetPacketID(UINT8 r){
		PacketID = r;
		return PacketID;
	}


}IEEE802_15_4_Metadata_t;

const int crc_size = 2;			//used in Radio driver's RF231Radio::Send_TimeStamped
const int timestamp_size = 4;	//used in Radio driver's RF231Radio::Send_TimeStamped
//IEEE802_15_4_MAX_PAYLOAD cannot go beyond 106 (14 for IEEE802_15_4_Header_t added with CRC (2 bytes) and timestamp (4 bytes) in radio driver makes it 126 which is IEEE802_15_4_FRAME_LENGTH).
//IEEE802_15_4_MAX_PAYLOAD below is 101 bytes.
#define IEEE802_15_4_MAX_PAYLOAD (IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)-sizeof(IEEE802_15_4_Footer_t)-sizeof(IEEE802_15_4_Metadata_t))
//#define IEEE802_15_4_MAX_PAYLOAD (IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)-sizeof(IEEE802_15_4_Footer_t)-sizeof(IEEE802_15_4_Metadata_t)-crc_size-timestamp_size)

typedef Message<IEEE802_15_4_Header_t,IEEE802_15_4_MAX_PAYLOAD,IEEE802_15_4_Footer_t,IEEE802_15_4_Metadata_t> IEEE802_15_4_Message_t;
//typedef Message<IEEE802_15_4_Header_t,IEEE802_15_4_MAX_PAYLOAD,IEEE802_15_4_Footer_t> IEEE802_15_4_Message_t;

class IEEE802_15_4_Message_t2 : public IEEE802_15_4_Message_t{
public:
	IEEE802_15_4_Message_t2(){

	}
	IEEE802_15_4_Message_t2& operator=(const IEEE802_15_4_Message_t2& other){
//		this->GetHeader()->dest = other.GetHeaderConst()->dest;
		memcpy(this, &other, sizeof(IEEE802_15_4_Message_t2));
		return *this;
	}
};

#define Message_15_4_t IEEE802_15_4_Message_t2


void ClearMsgContents(Message_15_4_t* msg){
	memset(msg,0,sizeof(Message_15_4_t));
}


#endif /* MESSAGE_H_ */
