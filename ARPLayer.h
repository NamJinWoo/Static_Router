// ARPLayer.h: interface for the CARPLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARPLayer_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)
#define AFX_ARPLayer_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CARPLayer 
: public CBaseLayer  
{
private:
		
	typedef struct _ARPLayer_HEADER {
		unsigned short hardType; //Protocol 타입
		unsigned short protType; //Protocol 타입
		unsigned char hardSize; //하드웨어 크기 2바이트
		unsigned char protSize; //Protocol 크기 2바이트
		unsigned short op; 
		unsigned char senderMac[6]; //송신측 Mac
		unsigned char senderIP[4]; //송신측 IP
		unsigned char targetMac[6]; //수신측 Mac
		unsigned char targetIP[4]; //수신측 IP
	} ARPLayer_HEADER, *LPARPLayer_HEADER ;
	
	unsigned char m_srcIP[4];
	unsigned char m_srcMAC[6];

	ARPLayer_HEADER	m_sHeader;
	inline void		ResetHeader( ) ;
	CObject* mp_Dlg ;
	int				SearchCacheTable( unsigned char ip[4] );
public:

	typedef struct CacheTableStruct{
		unsigned char macAddress[6]; //mac주소 6바이트
		unsigned char ipAddress[4]; //ip주소 4바이트
		CString interfaceName;
		CString state; //complete인지 아닌지
		int time;
	} CacheTable , *LPCacheTable;

	BOOL			Receive( unsigned char* ppayload );
	BOOL			Send( unsigned char* ppayload, int nlength );

	void			setSrcIPAddress(unsigned char* pAddress);
	void			setSrcMacAddress(unsigned char* pAddress);
	void			setDstIPAddress(unsigned char* pAddress);
	void			setDstMacAddress(unsigned char* pAddress);

	unsigned char * getMyMacAddress();
	void			setMyIPAddress(unsigned char* pAddress);
	void			setMyMacAddress(unsigned char* pAddress);

	CString			checkTableState( int index, int state );
	int				getTableCount();
	void			itemDelete( int index );
	void			allDelete();

	CARPLayer( char* pName );
	virtual ~CARPLayer();

protected:
	CList<CacheTable , CacheTable&> cache_table;

};

#endif // !defined(AFX_ARPLayer_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)

