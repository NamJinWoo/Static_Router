// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arp.h"
#include "EthernetLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernetLayer::CEthernetLayer( char* pName )
: CBaseLayer( pName )
{
	ResetHeader( ) ;
}

CEthernetLayer::~CEthernetLayer()
{
}

// Ether ���� ����
void CEthernetLayer::ResetHeader()
{
	memset( m_sHeader.enet_dstaddr.addrs, 0, 6 ) ;
	memset( m_sHeader.enet_srcaddr.addrs, 0, 6 ) ;
	memset( m_sHeader.enet_data, 0, ETHER_MAX_DATA_SIZE) ;
	m_sHeader.enet_type = 0x0000;
}

unsigned char* CEthernetLayer::GetSourceAddress()
{
	return m_sHeader.enet_srcaddr.addrs ;
}

unsigned char* CEthernetLayer::GetDestinAddress()
{
	return m_sHeader.enet_dstaddr.addrs ;
}

void CEthernetLayer::SetSourceAddress(unsigned char *pAddress)
{
	memcpy(m_sHeader.enet_srcaddr.addrs, pAddress, 6);
}

void CEthernetLayer::SetDestinAddress(unsigned char *pAddress)
{
	memcpy( m_sHeader.enet_dstaddr.addrs, pAddress, 6 ) ;
}

void CEthernetLayer::SetEnetType( unsigned short enet_type ){

	this->m_sHeader.enet_type = htons( enet_type );

}

// �Ұ� ����. type 0x800���� ����, ����Ʈ ����
BOOL CEthernetLayer::Send(unsigned char *ppayload, int nlength)
{
	memcpy( m_sHeader.enet_data, ppayload, nlength ) ;

	BOOL bSuccess = FALSE ;
	bSuccess = this->mp_UnderLayer->Send(( unsigned char* )( &m_sHeader ), nlength + ETHER_HEADER_SIZE ); //UnderLayer�� NILayer

	return bSuccess ;
}
// ���ּ� Ȯ��, ���� ����Ʈ������ ���� ��ġ�� ����̴� ����Ʈ ������ �Ű� ���� �ʾƵ� ��
BOOL CEthernetLayer::Receive( unsigned char* ppayload )
{
	LPETHERNET lpFrame = (LPETHERNET) ppayload ;
	
	unsigned char broadcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

	BOOL bSuccess = FALSE ;

	// �̴��� �ּ� Ȯ�� // 
	if( ( memcmp( m_sHeader.enet_srcaddr.addrs, lpFrame->enet_dstaddr.addrs, 6 ) == 0 || 	// ������ ���°ų� broadcast �ε�, ���� ������ �ƴ� �� //
		  memcmp( lpFrame->enet_dstaddr.addrs, broadcast, 6 ) == 0 ) &&		
		  memcmp( m_sHeader.enet_srcaddr.addrs, lpFrame->enet_srcaddr.addrs, 6 ) != 0 ){	
		if( ntohs(lpFrame->enet_type) == 0x800) { // IP ���������� ��� ó�� //
			bSuccess = this->mp_aUpperLayer[0]->Receive(lpFrame->enet_data); //IPLayer�� 0�� !IPLayer ���� set�ؾ���
		}
		else if (ntohs(lpFrame->enet_type) == 0x806) { // ARP ���������� ��� ó�� //
			bSuccess = this->mp_aUpperLayer[1]->Receive(lpFrame->enet_data); //ARPLayer�� 1��
		}
		else {
			bSuccess = FALSE;
		}
	}
	return bSuccess ;
}
