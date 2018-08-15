/*
 * OMACConstants.h
 *
 *  Created on: Sep 4, 2012
 *      Author: Mukundan
 */

#ifndef OMACCONSTANTS_H_
#define OMACCONSTANTS_H_

#include <tinyhal.h>

#define PACK __attribute__ ((packed))

//types 1-5 are taken up by global types
//#define OMAC_DATA_BEACON_TYPE 7

// Define default primes
#ifndef P1
#define P1 97
#endif

#ifndef P2
#define P2 103
#endif

//How often should a node wakeup (in ms). This is roughly the receive duty cycle.
#ifndef WAKEUP_INTERVAL
#define WAKEUP_INTERVAL 2000
#endif
#ifndef DATA_INTERVAL
#define DATA_INTERVAL 43
#endif

#ifndef OPTIMUM_BEACON_PERIOD
#define OPTIMUM_BEACON_PERIOD 1024 * 32
#endif

#define TIMESTAMP_FOOTER_OFFSET -4
#define TIMESTAMP_SIZE 4



#define MAX_UINT16 	(0xFFFF)
#define MAX_UINT32 	(0xFFFFFFFF)
#define MAX_UINT64 	(0xFFFFFFFFFFFFFFFF)
#define MID_UINT64  (0x7FFFFFFFFFFFFFFF)

#define MAX_DATA_PCKT_SIZE 100
#define MAX_PCKT_SIZE 128




/*
 *
 */
enum OMACSchedulerState_t{
  I_IDLE,
  I_DATA_RCV_PENDING, //waiting to receive
  I_DATA_SEND_PENDING, //pending to send
  I_TIMESYNC_PENDING,
  I_DISCO_PENDING,
  I_DWELL_SEND, //BK: What is this?
  I_RADIO_STOP_RETRY,
  I_FAILSAFE_STOP,
  I_POST_EXECUTE
} ;

/*
 *
 */
typedef enum {
  NULL_HANDLER,
  CONTROL_BEACON_HANDLER,
  DATA_RX_HANDLER,
  DATA_TX_HANDLER,
  TIMESYNC_HANDLER
} HandlerType_t;

/*
 *
 */
typedef enum {
  OMAC_NORMAL_SEND,
  OMAC_SEND_PRELOAD,
  OMAC_FIRST_SEND
} OMacAction_t;

/*
 *
 */
typedef struct PACK DiscoveryMsg
{
	//UINT32 msg_identifier;
	//seed to generate the pseduo-random wakeup schedule
	UINT16 nextSeed;
	UINT16 mask;
	//use to compute the offset of neighbor's current slot w.r.t. the start of the next frame
	//UINT64 nextwakeupSlot;
	UINT32 nextwakeupSlot0;
	UINT32 nextwakeupSlot1;
	//the wakeup interval of the neighbor
	UINT32 seedUpdateIntervalinSlots;
	//fields below are just for convenience. not transmitted over the air
	//UINT16 nodeID;

	//UINT32 localTime0;
	//UINT32 localTime1;

	//UINT32 lastwakeupSlotUpdateTimeinTicks0;
	//UINT32 lastwakeupSlotUpdateTimeinTicks1;
} DiscoveryMsg_t;

typedef struct{
	UINT8 RSSI;
	UINT8 LinkQuality;
	UINT8 Delay;
}MsgLinkQualityMetrics_t;

#define DataMsgOverhead sizeof(UINT8)
typedef struct DataMsg_t
{
	//UINT32 msg_identifier;
	UINT8 size;
	UINT8 payload[MAX_DATA_PCKT_SIZE];
} DataMsg_t;

/*
 * After TEP 133, the message timestamp contains the difference between
 * event time and the time the message was actually sent out. TimeSyncP
 * sends the local time associated with this globalTime to the
 * TimeStamping mechanism, which then calculates the difference.
 *
 * On the receiving side, the difference is applied to the local
 * timestamp. The receiving timestamp thus represents the time on the
 * receiving clock when the remote globalTime was taken.
 */
struct PACK TimeSyncMsg
{

//  UINT32 globalTime0;
//  UINT32 globalTime1;

  //the time to startup the radio could be different for different nodes.
  //use this neighbor info along with local info to compute this difference
  //UINT16 radioStartDelay;
//  float skew;
  //UINT32 timesyncIdentifier;
  UINT32 localTime0;
  UINT32 localTime1;
  //bool request_TimeSync;
 // UINT16 nodeID;
  //UINT32 seqNo;

};

struct PACK TimeSyncRequestMsg
{
  //UINT32 timesyncIdentifier;
  bool request_TimeSync;
};




/*
 *
 */
typedef struct DataBeacon {
  UINT16 nodeID;
} DataBeacon_t;

/*
 *
 */
typedef struct OMacHeader {
  UINT8 flag;
} OMacHeader;

//Overflow provisioned class
template<class Base_T>
class OFProv:Base_T{
public:
	bool isThereOverflow(const Base_T& rhs){
		if(rhs>*this){
			if(rhs - *this <= MID_UINT64) return false;
			else return true;
		}
		else{
			if(*this - rhs <= MID_UINT64) return false;
			else return true;
		}
	}
	bool operator<(const Base_T& rhs){
		if (rhs == *this) return false;
		else if(isThereOverflow(rhs)){
			if (rhs<*this) return true;
			else return false;
		}
		else{
			if (rhs<*this) return true;
			else return false;
		}
	}
	bool operator>(const Base_T& rhs){
		if (rhs == *this) return false;
		else if(isThereOverflow(rhs)){
			if (rhs>*this) return true;
			else return false;
		}
		else{
			if (rhs>*this) return true;
			else return false;
		}
	};
	bool operator<=(const Base_T& rhs){
		if (*this == rhs) return true;
		return (*this < *rhs);
	}
	bool operator>=(const Base_T& rhs){
		if (*this == rhs) return true;
		return (*this > *rhs);
	}
};

typedef OFProv<UINT64> OMACMicroSeconds;
typedef OFProv<UINT64> OMACTicks;


/*
#if defined(CURRENT_RADIONAME) && CURRENT_RADIONAME==RADIONAME_SI4468

	#define RETRANS_DELAY_DUE_TO_MISSING_ACK	12.8*MILLISECINMICSEC	//A 64-byte packet takes 64*8*25 usec to be transmitted (12800 usec);
																		//At 40kbps, a bit takes 25 usec
	#define DELAY_IN_RECEIVING_ACK				3.6*MILLISECINMICSEC		//Delay in Rx generating an ack; SI4468 does not support H/w acks;
																		//A 3-byte s/w ack (along with additional 10 bytes added by radio firmware) takes 2600 usec
																		//Due to software delays (walking up and down the invocation call stack), an addition 1 ms is added

	#define MAX_PACKET_TX_DURATION_MICRO 		27.6*MILLISECINMICSEC	//128 byte packet takes 25600 usec, but in reality time taken is 27600 usec (138 bytes)
	#define ACK_RX_MAX_DURATION_MICRO 			8*MILLISECINMICSEC		//2x of ACK_DELAY. Indicates how long should tx wait to get back an ack from rx.
	#define ACK_TX_MAX_DURATION_MICRO 			8*MILLISECINMICSEC
	#define DISCO_BEACON_TX_MAX_DURATION_MICRO	10*MILLISECINMICSEC		//39 byte packet takes 7.8 ms; (16 bytes for DISCO + 13 bytes for header + 10 bytes added by radio)
	#define DISCO_PACKET_TX_TIME_MICRO 			10*MILLISECINMICSEC
	#define DISCO_SLOT_PERIOD_MICRO 			8*MILLISECINMICSEC
	#define HIGH_DISCO_PERIOD_IN_SLOTS 			9000

#elif defined(CURRENT_RADIONAME) && CURRENT_RADIONAME==RADIONAME_RF231

#if defined(RF231_HARDWARE_ACK)
	#define DELAY_IN_RECEIVING_HW_ACK				0.4*MILLISECINMICSEC		//(C)Delay in Rx generating a h/w ack
	#define DELAY_IN_RECEIVING_SW_ACK				0*MILLISECINMICSEC
	#define RETRANS_DELAY_DUE_TO_MISSING_HW_ACK		0*MILLISECINMICSEC
	#define RETRANS_DELAY_DUE_TO_MISSING_SW_ACK		0*MILLISECINMICSEC
	#define ACK_TX_MAX_DURATION_MICRO 				0*MILLISECINMICSEC
#else
	#define DELAY_IN_RECEIVING_HW_ACK				0*MILLISECINMICSEC
	#define DELAY_IN_RECEIVING_SW_ACK				0.6*MILLISECINMICSEC		//(C)Delay in Rx generating a s/w ack
	#define RETRANS_DELAY_DUE_TO_MISSING_HW_ACK		0*MILLISECINMICSEC
	#define RETRANS_DELAY_DUE_TO_MISSING_SW_ACK		0.2*MILLISECINMICSEC
	#define ACK_TX_MAX_DURATION_MICRO 				4*MILLISECINMICSEC
#endif

	#define DELAY_IN_RECEIVING_ACK				DELAY_IN_RECEIVING_HW_ACK+DELAY_IN_RECEIVING_SW_ACK
	#define RETRANS_DELAY_DUE_TO_MISSING_ACK	RETRANS_DELAY_DUE_TO_MISSING_HW_ACK+RETRANS_DELAY_DUE_TO_MISSING_SW_ACK
	#define MAX_PACKET_TX_DURATION_MICRO 		5*MILLISECINMICSEC		//At 256kbps, a bit takes 3.9 usec to be transmitted; A 128 byte packet takes 4000 usec;
	#define ACK_RX_MAX_DURATION_MICRO 			20*MILLISECINMICSEC
	#define DISCO_BEACON_TX_MAX_DURATION_MICRO	1.2*MILLISECINMICSEC		//35 byte packet takes 1 ms; (16 bytes for DISCO + 13 bytes for header + 6 bytes added by radio)
	#define DISCO_PACKET_TX_TIME_MICRO 			1*MILLISECINMICSEC
	#define DISCO_SLOT_PERIOD_MICRO 			8*MILLISECINMICSEC
	#define HIGH_DISCO_PERIOD_IN_SLOTS 			9000
#else
	#error "Radioname not defined"
#endif
*/


#define MAXUPDATESEEDITERS 2000

#define MILLISECINMICSEC 1000

#define MAX_SLEEP_RETRY_ATTEMPTS 10

#define FRAMERETRYMAXATTEMPT 2
#define FRAMERETRYMAXATTEMPTWARNINGLEVEL 10
#define SLOTRETRYMAXATTEMPT 2
#define CCA_PERIOD_FRAME_RETRY_MICRO 0 //BK: We need to double check this. Since 2 nodes will be off by this much. A node should CCA at least this much to make sure there was no other transmitter trying to reach the same destination.

#define HIGH_DISCO_PERIOD_IN_SLOTS_CONSTANT 50000 //This is setting the length of high disco period after start up. Note that the length of disco slot is different than the length of data slots. The ratio is determined by DISCOPERIODINSLOTS. Note that this is a convention and there is no direct relationship limiting the ratio between the data slots and the discovery slots.
//#define HIGH_DISCO_PERIOD_IN_SLOTS_CONSTANT 0
#ifdef PLATFORM_ARM_EmoteDotNow
#define HIGH_DISCO_PERIOD_ALWAYS_ON 1
#else
#define HIGH_DISCO_PERIOD_ALWAYS_ON 0
#endif

//#define RANDOM_BACKOFF_COUNT_MAX	4
//#define RANDOM_BACKOFF_COUNT_MIN	1
//#define DELAY_DUE_TO_CCA_MICRO	260
//#define RANDOM_BACKOFF_TOTAL_DELAY_MICRO	(RANDOM_BACKOFF_COUNT_MIN*DELAY_DUE_TO_CCA_MICRO)		//Random_backoff can happen atleast once. So, tx should wake up atleast this amount early.
																								// If it wakes up early by RANDOM_BACKOFF_COUNT_MAX amount, scheduler will not have a packet ready for tx.
//#define RETRY_RANDOM_BACKOFF_DELAY_MICRO	(RANDOM_BACKOFF_COUNT_MAX*DELAY_DUE_TO_CCA_MICRO)
//#define OMAC_TIME_ERROR	3*MILLISECINMICSEC	//pessimistic time error
// BK: Not used anymore #define EXTENDED_MODE_TX_DELAY_MICRO	0.8*MILLISECINMICSEC	//delay from start of tx to start of rx
//#define DELAY_FROM_OMAC_TX_TO_RADIO_DRIVER_TX	300	//(A)Delay from start of tx in OMAC to start of writing to SPI bus
//#define DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX	284	//(B)Delay between Node N1 starting TX to node N2 receiving
//#define RETRY_FUDGE_FACTOR	0.3*MILLISECINMICSEC			//(D)From observation, get avg,min,max for (A),(B). Min will go into (A),(B).
															//   Sum of (max-min) of (A),(B) will go into (D)


#define DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX_RF231	284
#define DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX_SI 1718


//#define PROCESSING_DELAY_BEFORE_TX_MICRO (581) //DELAY_FROM_OMAC_TX_TO_RF231_TX //581
#define DELAY_FROM_DTH_TX_TO_RADIO_DRIVER_TX_RF231 581
#define DELAY_FROM_DTH_TX_TO_RADIO_DRIVER_TX_SI 1440
#define RADIO_TURN_ON_DELAY_RX_MICRO_RF231 693
#define RADIO_TURN_ON_DELAY_TX_MICRO_RF231 693
#define RADIO_TURN_ON_DELAY_RX_MICRO_SI 163
#define RADIO_TURN_ON_DELAY_TX_MICRO_SI 143

#define RADIO_TURN_OFF_DELAY_MICRO_RF231 184 //453 //BK: This is not used but it is measured 184 micro secs (may not be very accurate)
#define TIMER_MODIFICATION_AND_START_DELAY_MICRO 269 // BK: This is a very rough number

//#define CCA_REACTION_TIME_MICRO 165 //BK: We need to double check this. This is the reaction time of the CCA module from the beginning of channel activity.
//#define CCA_PERIOD_MICRO GUARDTIME_MICRO //BK: We need to double check this. Since 2 nodes will be off by this much. A node should CCA at least this much to make sure there was no other transmitter trying to reach the same destination.
#define CCA_PERIOD_ERROR_SI 1565 //BK: It is observed that CCA is being done more than set by the protocol. This is the observed error on it. It is used in scheduling the tx side this much early
#define CCA_PERIOD_ERROR_RF231 165 //BK: It is observed that CCA is being done more than set by the protocol. This is the observed error on it. It is used in scheduling the tx side this much early
#define CCA_PERIOD_MICRO 200


#define ADDITIONAL_TIMEADVANCE_FOR_RECEPTION 0
#define TIME_RX_TIMESTAMP_OFFSET_MICRO_SI 1415 //TODO: BK: We need to revisit at this. This is a workaround for some unaccountant time stamping error. In rf231 this is due to the duration from the beginning of the first bit to the end of preamble (note that this is earlier than AMI) when the TS is taken.
#define TIME_RX_TIMESTAMP_OFFSET_MICRO_RF231 141 //Actual number is unknown. Have to go back into previous commits to find out for rf231 //TODO: BK: We need to revisit at this. This is a workaround for some unaccountant time stamping error. In rf231 this is due to the duration from the beginning of the first bit to the end of preamble (note that this is earlier than AMI) when the TS is taken.



#define END_OF_TX_TO_RECEPTION_OF_HW_ACK_MICRO	(1.2*MILLISECINMICSEC)
#define HW_ACK_TO_START_OF_TX_MICRO	(2*MILLISECINMICSEC)
#define EXTRA_DELAY_IN_WAITING_FOR_ACK (1.6*MILLISECINMICSEC)	//Difference between FAST_RECOVERY_WAIT_PERIOD_MICRO (or) MAX_PACKET_TX_DURATION_MICRO and 3.4ms. 3.4ms is the ideal round trip time.
#define TIME_BETWEEN_TX_RX_TS_TICKS (266*TICKS_PER_MICRO)

#define RADIO_STOP_RETRY_PERIOD_IN_MICS OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO
//Random_backoff is done before re-transmission
//GUARDTIME_MICRO+OMAC_TIME_ERROR - Pessimistic time error
//GUARDTIME_MICRO - optimistic time error (if there is a re-transmission, tx takes GUARDTIME_MICRO to do CCA

#define EXECUTE_WITH_CCA  0
#define  FAST_RECOVERY 0
#define  FAST_RECOVERY2 0

#define INITIAL_RETRY_BACKOFF_WINDOW_SIZE 3
#define MAX_RETRY_BACKOFF_WINDOW_SIZE 10


//How long should receiver be awake after sending a HW ack. BK: No it is not! see the following
// This is the maximum period to wait for the reception of a packet after receiving StartOfReception interrupt. Due to the change in RF231.cpp the interrupt is received after AMI. Hence it is the packet
#define PACKET_PERIOD_FOR_RECEPTION_HANDLER 16000
//#define PACKET_PERIOD_FOR_RECEPTION_HANDLER EXTENDED_MODE_TX_DELAY+END_OF_TX_TO_RECEPTION_OF_HW_ACK_MICRO+HW_ACK_TO_START_OF_TX_MICRO+CURRENTFRAMERETRYMAXATTEMPT*RANDOM_BACKOFF_TOTAL_DELAY_MICRO

#define RECEIVER_RADIO_STOP_RECHECK_INTERVAL_MICRO 1000
#define TIMER_EVENT_DELAY_OFFSET 0
#define MINEVENTTIME 50000				//minimum time (in micro seconds) required by scheduler to switch between modules
#define SEED_UPDATE_INTERVAL_IN_SLOTS 100 //The FRAME SIZE in slots


//Below 2 values are based on empirical observations made on a debug build
#define FAST_RECOVERY_WAIT_PERIOD_MICRO 5*MILLISECINMICSEC
#define RECV_HW_ACK_WAIT_PERIOD_MICRO	1.7*MILLISECINMICSEC
#define DATATX_POST_EXEC_DELAY	  10*MILLISECINMICSEC


//40000000 - 5 secs
//48000000 - 6 secs
//60000000 - 7.5 secs
//80000000 - 10 secs
//100000000 - 12.5 secs
//#define FORCE_REQUESTTIMESYNC_INTICKS 80000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
//#define FORCE_REQUESTTIMESYNC_INMICS 10000000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
////#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST  48000000		//Translates to 10 secs @8Mhz. Sender centric time threshold to send a TImeSync msg.
//#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST_INMICS  600000000		//Translates to 10 secs @8Mhz. Sender centric time threshold to send a TImeSync msg.


#define HFCLOCKID 1
//#define LFCLOCKID 4 // This is the RTC clock
#define LFCLOCKID LOW_DRIFT_TIMER
#define OMACClockSpecifier LOW_DRIFT_TIMER
#define OMACClockFreq 32
#define OMACClocktoSystemClockFreqRatio 244.140625
//#define OMACClockSpecifier HFCLOCKID
//#define OMACClockFreq 8000
//#define OMACClocktoSystemClockFreqRatio 1

#define INITIALIZATION_TIMESYNC_INTERVAL_INMICS 10000000   // Controls the interval during the neighbor intiialization period after a discovery. This interval is faster than the regular intervals in order to fill the neighbor's timesync table with the samples from the current node.

#if (OMACClockSpecifier==LFCLOCKID)
//#define FORCE_REQUESTTIMESYNC_INTICKS 80000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
#define FORCE_REQUESTTIMESYNC_INMICS 610000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
//#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST        10000000		//Translates to 10 secs @8Mhz. Sender centric time threshold to send a TImeSync msg.
#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST_INMICS  600000000		//z. Sender centric time threshold to send a TImeSync msg.
////GUARDTIME_MICRO should be calculated in conjuction with SLOT_PERIOD_MILLI
//// GUARDTIME_MICRO = (SLOT_PERIOD_MILLI - PacketTime)/2 - SWITCHING_DELAY_MICRO
////PacketTime = 125byte * 8 bits/byte / (250*10^3 bits/sec) = 4sec
#define GUARDTIME_MICRO 5000
#else
//#define FORCE_REQUESTTIMESYNC_INTICKS 80000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
#define FORCE_REQUESTTIMESYNC_INMICS 100000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
//#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST  48000000		//Translates to 10 secs @8Mhz. Sender centric time threshold to send a TImeSync msg.
#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST_INMICS  6000000		//Translates to 10 secs @8Mhz. Sender centric time threshold to send a TImeSync msg.
////GUARDTIME_MICRO should be calculated in conjuction with SLOT_PERIOD_MILLI
//// GUARDTIME_MICRO = (SLOT_PERIOD_MILLI - PacketTime)/2 - SWITCHING_DELAY_MICRO
////PacketTime = 125byte * 8 bits/byte / (250*10^3 bits/sec) = 4sec
#define GUARDTIME_MICRO 20000
#endif

#define OMAC_SCHEDULER_MIN_REACTION_TIME_IN_TICKS 4000
#define OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO 500

#define FAILSAFETIME_MICRO 6000000

#define WAKEUPPERIODINTICKS 8000000

#define TIMEITTAKES2TXDISCOPACKETINMICSEC 4096
#define DISCOPERIODINMILLI 15

//FCF table:
//15 14 13  12  11  10  9  8  7  6  5  4  3  2  1  0 (bits)
//1   0  0   0   1   0  0  0  0  1  1  0  0  0  0  1 (values)
//Frame type (bits 0,1,2)  			- Data
//Security enabled (bit 3) 			- No
//Frame pending (bit 4)				- No
//Ack request (bit 5)				- Yes
//Intra-pan (bit 6)					- Yes, source PAN-ID is omitted
//Bits 7,8,9						- Reserved
//Dest addressing mode (bits 10,11)	- Address field contains a 16-bit short address
//Frame version (bits 12,13)		- Frames are compatible with IEEE 802.15.4 2003
//Src addressing mode (bits 14,15)	- Address field contains a 16-bit short address
#define FCF_WORD_VALUE 0x8861 //34913
#define FCF_WORD_VALUE_DISCO FCF_WORD_VALUE//34913
#define SRC_PAN_ID	0xAAAA
#define DEST_PAN_ID	0x5555

enum {
  TICKS_PER_MILLI     = 8000,
  TICKS_PER_MICRO     = 8,
  BITS_PER_BYTE = 8,

#ifdef SHORT_SLOT
#warning *** USING 8ms SLOT ***
  SLOT_PERIOD_MILLI     = 8,    /*modify this along with SLOT_PERIOD_BITS*/
  SLOT_PERIOD_BITS    = 3 + 13,  /*13 = # of bits of TICKS_PER_MILLI, 4 = # of bits in SLOT_PERIOD_MILLI*/
#else
  SLOT_PERIOD_MILLI     = 16,     /*modify this along with SLOT_PERIOD_BITS*/
  SLOT_PERIOD_BITS    = 4 + 13,  /*13 = # of bits of TICKS_PER_MILLI, assuming its a 10Mhz clock, 4 = # of bits in SLOT_PERIOD_MILLI*/
#endif
  SLOT_PERIOD_TICKS   = SLOT_PERIOD_MILLI * TICKS_PER_MILLI,
  SLOT_PERIOD   = SLOT_PERIOD_MILLI,
  DWELL_TIME        = 10,
  /* sender margin compensates for time sync jitter, packet preload delay
   * and radio startup jitter*/
  SENDER_MARGIN_IN_MILLI = 3,
  SENDER_MARGIN     = SENDER_MARGIN_IN_MILLI * TICKS_PER_MILLI,
  TRANSITION_MARGIN   = SENDER_MARGIN_IN_MILLI * TICKS_PER_MILLI,
  MINIMUM_BACKOFF_WINDOW  = 64, /*in the unit of 1/32 ms*/
  RADIO_BACKOFF_WINDOW  = 64, /*in the unit of 1/32 ms*/
  RANDOM_SCHEDULE_WINDOW  = 0x0,
  /* receiver's wait time before the first packet arrives. It will not affect efficiency much
   * if the percentage of failed rendezvous is small because receivers go back to sleep upon
   * receiving the packet tagged as the last packet in the queue*/
  WAIT_TIME_AFTER_DATA_BEACON = (RADIO_BACKOFF_WINDOW + MINIMUM_BACKOFF_WINDOW) / TICKS_PER_MILLI + 12,
  WAIT_TIME_AFTER_PRELOAD = SLOT_PERIOD_MILLI,
  //8 minutes is the optimum time window for linear regression
  //the maximum number of times we tx a packet when previous
  //attempts fail due to congestion
  MAX_RETRY_CNT     = 10,
  // number of entries per neighbor
  MAX_ENTRIES             = 8,
  MAX_POOL_SIZE     = 32,
  MAX_CTRL_MSG_POOL_SIZE  = 8,
  //the number of consecutive messages sent during dwell time
  //DWELL_COUNT       = DEFAULT_DWELL_COUNT,
  DWELL_COUNT       = 3,
  DATA_ALARM_MAX_DURATION = 5,
  DELAY_AVG_FACTOR    = 9,
  //copy  from RIMAC implementation, should acknolwedge them
  //the number of cca detections needed to declare collision
  OMAC_COLLISION_CCA_THRESHOLD = 0,
  MAX_NON_SLEEP_STATE   = 10,
  MAX_NBR_SIZE      = 8,
  AM_DATA_BEACON      = 0x2E,
  AM_TIMESYNCMSG      = 0x3E,
  TIMESYNCMSG_LEN     = sizeof(TimeSyncMsg) - sizeof(UINT32) - sizeof(UINT16),
  INVALID_TIMESTAMP   = 0xFFFFFFFF,
  INVALID_ADDRESS     = 0xFFFF,
  INVALID_INDEX       = 0xFF,
  // time to declare itself the root if no msg was received (in sync periods)
  ROOT_TIMEOUT            = 5,
  // after becoming the root ignore other roots messages (in send period)
  IGNORE_ROOT_MSG         = 4,
  // of entries to become synchronized
  ENTRY_VALID_LIMIT       = 8,
  // if time sync error is bigger than this clear the table
  ENTRY_THROWOUT_LIMIT    = 200,
  // to detect whether my clock or my neighbor's clock has wrapped
  FLAG_TIMESTAMP_VALID  = (UINT8)1 << 3,
  FLAG_REQUEST_BEACON   = (UINT8)1 << 4,
  FLAG_DWELL_TIME     = (UINT8)1 << 6,
  OMAC_HEADER_LEN     = sizeof(OMacHeader),
  SEND_PIGGYBACK_BEACON = 1
    //TODO: needs random mechanism for DATA_INTERVAL
};

UINT16  CONTROL_BEACON_INTERVAL_SLOT = 7500;

UINT32 ArbiterP_Timing;

/*
 * Prime numbers used in determining DISCO period of a node
 */
#define DISCOVERY_SIZE_OF_PRIME_NUMBER_POOL 7

//UINT16 CONTROL_P1[] = {2131, 2099, 2129, 2111, 2153, 2113, 2137};
//UINT16 CONTROL_P2[] = {8429, 8419, 8623, 8443, 8627, 8447, 8467};
//UINT16 CONTROL_P3[] = {2131, 2099, 2129, 2111, 2153, 2113, 2137};
//UINT16 CONTROL_P4[] = {8429, 8419, 8623, 8443, 8627, 8447, 8467};

//UINT16 CONTROL_P1[] = {911, 727, 787, 769, 773, 853, 797};
//UINT16 CONTROL_P2[] = {2131, 2099, 2129, 2111, 2153, 2113, 2137};
//UINT16 CONTROL_P3[] = {911, 727, 787, 769, 773, 853, 797};
//UINT16 CONTROL_P4[] = {2131, 2099, 2129, 2111, 2153, 2113, 2137};

UINT16 CONTROL_P3[] = {2131, 2099, 2129, 2111, 2153, 2113, 2137};
UINT16 CONTROL_P4[] = {8429, 8419, 8623, 8443, 8627, 8447, 8467};
//UINT16 CONTROL_P1[] = {197, 157, 151, 163, 211, 113, 127};
//UINT16 CONTROL_P2[] = {911, 727, 787, 769, 773, 853, 797};

//UINT16 CONTROL_P3[] = {197, 157, 151, 163, 211, 113, 127};
//UINT16 CONTROL_P4[] = {911, 727, 787, 769, 773, 853, 797};
//UINT16 CONTROL_P1[] = {19, 17, 13, 37, 11, 5, 7};
//UINT16 CONTROL_P2[] = {67, 43, 53, 47, 61, 59};

//UINT16 CONTROL_P1[] = {47, 37, 43, 37, 53, 29, 31};
//UINT16 CONTROL_P2[] = {227, 181, 197, 191, 211, 199};

UINT16 CONTROL_P1[] = {67, 71, 79, 83, 89, 97};
UINT16 CONTROL_P2[] = {227, 229, 233, 239, 241, 251};


//Define total size of a Disco packet with piggybacking
#if OMAC_DEBUG_SEND_EXTENDEDMACINfo

#else

#endif



#endif /* OMACCONSTANTS_H_ */
