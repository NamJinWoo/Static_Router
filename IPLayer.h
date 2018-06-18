// IPLayer.h: interface for the CIPLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_IPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CIPLayer 
: public CBaseLayer  
{
private:
   inline void      ResetHeader( );
   int            SearchRouterTable( unsigned char ip[4], unsigned char netmast[4] );
   unsigned char* CIPLayer::IPNetmask(unsigned char *dstIP, unsigned char *nestmask);
   // 수신한 패킷의 Src와 Dst 주소
public:
   BOOL         Receive( unsigned char* ppayload ) ;
   BOOL         Send( unsigned char* ppayload, int nlength );

   void         SetDestinAddress( unsigned char* pAddress );
   void         SetSourceAddress( unsigned char* pAddress );

   CIPLayer( char* pName );
   virtual ~CIPLayer();

   typedef struct _IPLayer_HEADER {
      unsigned char ip_verlen;   // ip version & header length (4bits*2)
      unsigned char ip_tos;      // type of service   (1byte)
      unsigned short ip_len;      // total packet length   (2byte)
      unsigned short ip_id;      // datagram id         (2byte)
      unsigned short ip_fragoff;   // fragment offset      (2byte)
      unsigned char ip_ttl;      // time to live in gateway hops (1byte)
      unsigned char ip_proto;      // IP protocol         (1byte)
      unsigned short ip_cksum;   // header checksum      (2byte)
      unsigned char ip_src[4];   // IP address of source   (4byte)
      unsigned char ip_dst[4];   // IP address of destination   (4byte)
      unsigned char ip_data[IP_DATA_SIZE]; // variable length data
      
   } IPLayer_HEADER, *LPIPLayer_HEADER ;

protected:
   typedef struct RouterTableStruct{
      unsigned char ipAddress[4]; //ip주소 4바이트
      unsigned char NetMask[4];
      unsigned char gateway[4];
      CString Flag;
      CString interfaceName; //complete인지 아닌지
      int metric;
   } RouterTable , *LPCacheTable;

   CList<RouterTable , RouterTable&> router_table;

   unsigned char m_srcIP[4];
   unsigned char m_srcMAC[6];

   IPLayer_HEADER   m_sHeader ;
};

#endif // !defined(AFX_IPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)