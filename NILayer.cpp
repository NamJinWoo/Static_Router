// NILayer.cpp: implementation of the CNILayer class.
//
//////////////////////////////////////////////////////////////////////

/*
 * Test ����
 */

#include "stdafx.h"
#include "arp.h"
#include "NILayer.h"
#include <stdio.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace std;

CNILayer::CNILayer( char* pName, LPADAPTER *lpAdapterm, int iAdapterNumToUse )
: CBaseLayer( pName )
{
	m_pAdapter = NULL;
	m_AdapterNumToUse = iAdapterNumToUse;
	m_thrdState = TRUE;
	SetAdapterList();
}

CNILayer::~CNILayer()
{
}
// ���õ� ��ī�带 ���� ��Ŷ ����
BOOL CNILayer::Send(unsigned char *ppayload, int nlength)
{
	if(pcap_sendpacket(this->m_pAdapter, ppayload, nlength))
	{
		AfxMessageBox("��Ŷ ���� ����");	
		return FALSE;
	}

	return TRUE ;
}
// ��Ŷ �۽Ű� ������ �����ϱ� ���� ���� �����忡 ���Ǵ� �Լ� //
UINT CNILayer::ReadingThread(LPVOID lpParam){
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	int result;
	
	CNILayer *pNI = ( CNILayer *) lpParam;

	while( pNI->m_thrdState ){
		// ���� ���� ���¿� ���� ��, �� ���� ��ư�� ������ �� //
		result = pcap_next_ex( pNI->m_pAdapter, &header, &pkt_data );
		// �������� ĸó �Ǵ� �������̽��� ���� ��Ŷ�� ���� //
		// 1 : ����, 0 : timeout, -1 : ����, -2 : EOF //
		if( result == 0 ) {}
		// ���� �� ��� ���� ���̾�� ���� //
		else if ( result == 1 ){ pNI->Receive((u_char *)pkt_data);} 
		else if ( result < 0 ) {}
	}

	return 0;
}
BOOL CNILayer::Receive( unsigned char* ppayload )
{	
	return  this->mp_aUpperLayer[0]->Receive(ppayload);
}

// ������ ��ī��, ����� �� ���� ���� �����ؼ� ��� �غ� �Ϸ�!
void CNILayer::OpenSocket()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if( ( m_pAdapter = pcap_open_live(m_pAdapterList[m_AdapterNumToUse]->name, 1514, 1, -1, errbuf ) ) == NULL )
	{// ����̽� open , m_pAdapterList���� m_AdapterNumToUse��°�� ����� ���
	// pcap_open_live( ��ġ�� �̸�, ĸ���� ũ��( �̴����� �ִ� ����), Promiscuous mode�� ����� ��, ��Ŷ�� �������� �� ��Ŷ�� 
	// ��� ��ٸ��� �ð�, errbuf ������ �߻����� �� ���� ������ ����/ �����ϸ� Null�� ����)

		AfxMessageBox(errbuf);
		return ;
	}
	
	AfxBeginThread(ReadingThread, this);
}

void CNILayer::SetThreadState( BOOL state )
{
	m_thrdState = state ;
}

// ��ī�� ��� �����
void CNILayer::SetAdapterList()
{
	pcap_if_t *alldevs, *d;
	int i = 0; // for iter
	char errbuf[PCAP_ERRBUF_SIZE];

	for(int j=0 ; j < NI_COUNT_NIC ; j++)
		m_pAdapterList[j] = NULL;

	if( pcap_findalldevs( &alldevs, errbuf ) == -1 ){
		AfxMessageBox("NIC ����");
		return;
	}

	if( !alldevs ){
		AfxMessageBox("NIC ����");
		return;
	}

	for( d = alldevs ; d ; d = d->next ){
		m_pAdapterList[i++] = d ;
	}
}

pcap_if_t* CNILayer::GetAdapter(int index){
	return m_pAdapterList[index];
}

// ����� ��ī�� ����
void CNILayer::SetAdapterNum(int num)
{
	m_AdapterNumToUse = num ;
}