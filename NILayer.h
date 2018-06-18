// NILayer.h: interface for the CNILayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NILAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_)
#define AFX_NILAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include <pcap.h>
#include <Packet32.h>

// ���� ��Ŷ �ۼ���
// ������ Ȱ���Ͽ� �׻� ���� ����

class CNILayer 
: public CBaseLayer  
{
protected:
	pcap_t*			m_pAdapter;
	
	pcap_if_t*		m_pAdapterList[NI_COUNT_NIC];
	int				m_AdapterNumToUse;

	BOOL			m_thrdState;
	unsigned char*	m_ppayload;
	
public:
	void			OpenSocket();

	void			SetAdapterNum(int num);
	pcap_if_t*		GetAdapter(int index);

	void			SetThreadState( BOOL state ) ;
	static UINT		ReadingThread(LPVOID lpParam);
	BOOL			Receive(unsigned char* m_ppayload); // ���Ͽ��� �о ���� �������� ���� ����;
	BOOL			Send( unsigned char* ppayload, int nlength );

	CNILayer( char* pName, LPADAPTER *lpAdapter = NULL, int iAdapterNumToUse = 0 );
	virtual ~CNILayer();

private:
	void SetAdapterList();
};

#endif // !defined(AFX_NILAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_)
