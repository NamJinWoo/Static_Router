// ARPLayer.cpp: implementation of the CARPLayer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "arp.h"
#include "ARPLayer.h"
#include "EthernetLayer.h"
#include <queue>
using namespace std;
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define UM_ARPTABLEUPDATE WM_USER+2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

queue<unsigned char*> payload_Q;
// boolean isData = false �� ���� ������ ��Ŷ���� �ƴϸ� �׳� arp ��û���� Ȯ���Ѵ�.


CARPLayer::CARPLayer( char* pName ) 
: CBaseLayer( pName ), 
  mp_Dlg( NULL )
{
   ResetHeader( ) ;
}

CARPLayer::~CARPLayer()
{
}


// ��� ����
void CARPLayer::ResetHeader( )
{
   memset(m_sHeader.senderMac, 0, 6);
   memset(m_sHeader.targetMac, 0, 6);
}
// host ip, mac �ּ� ����
void CARPLayer::setMyIPAddress(unsigned char* pAddress)
{
   memcpy(m_srcIP, pAddress, 4);
}
void CARPLayer::setMyMacAddress(unsigned char* pAddress)
{
   memcpy(m_srcMAC, pAddress, 6);
}

unsigned char * CARPLayer::getMyMacAddress()
{
   return m_srcMAC;
}
///////���� ARP Packet�� �����غ��ô� ���� /////////////

//�۽����� IP,MAC �ּҸ� �����Ѵ�.
//Source IPAddress�� ���� �մϴ�^^
void CARPLayer::setSrcIPAddress(unsigned char* pAddress)
{
   memcpy(m_sHeader.senderIP, pAddress, 4);
}
void CARPLayer::setSrcMacAddress(unsigned char* pAddress)
{
   memcpy(m_sHeader.senderMac, pAddress, 6);
}

//�������� IP,MAC�ּҸ� �����Ѵ�.
//Destination IPAddress�� ���� �մϴ�^^
void CARPLayer::setDstIPAddress(unsigned char* pAddress)
{
   memcpy(m_sHeader.targetIP, pAddress, 4);
}
void CARPLayer::setDstMacAddress(unsigned char* pAddress)
{
   memcpy(m_sHeader.targetMac, pAddress, 6);
}

BOOL CARPLayer::Send(unsigned char *ppayload, int nlength)
{
   payload_Q.push(ppayload);
   //������ arp��Ŷ ���� ����� �ִ� ���� ��������...
   m_sHeader.protType = htons(0x0800); //request�ϱ� 0800�� �����Ѵ�.
   m_sHeader.hardType = htons(0x0001);
   m_sHeader.protSize = 4;
   m_sHeader.hardSize = 6;
   m_sHeader.op = htons(0x0001); //request �ϱ� 1�� ����.
   
   int ARPindex = SearchCacheTable( m_sHeader.targetIP );
      
   unsigned char broadcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
      
   // GARP ���� ���ƿ� ���ּҸ� ���� ���� NIC ���ּҸ� ���� ������.
   setDstMacAddress( m_srcMAC );

   //ARP Table�ȿ� �ִ��� �˻��Ѵ�.�Լ��� �����µ�
   if( ARPindex == -1 ){
      if( memcmp( m_sHeader.targetIP, m_srcIP, 4 ) != 0 ){
         // arp �ΰ�츸
         CacheTable data;
         memcpy( data.ipAddress, m_sHeader.targetIP, 4 );
         memcpy( data.macAddress, broadcast, 6 );
         data.state.Format("%s","imcomplete");
         data.time = TIMEOFIMCOMPLETE;
         // arp table�� �߰�
         cache_table.AddTail( data );
      }
   }else {
      if( memcmp( m_sHeader.targetIP, m_srcIP, 4 ) != 0 ){
         // arp �ΰ�츸
         cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).state.Format("%s","complete");
         cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).time = TIMEOFCOMPLETE;
         // ������ ����
         //���Ⱑ ������ �ͼ� �ٽ� ��Ŷ�� �����ٴ� �κ�.
            //��Ŷ�� ���� ��.
         mp_UnderLayer->Send(payload_Q.front() , sizeof(m_sHeader));
      }else{
         //garp �� ���
         AfxMessageBox("IP �浹");
         
         AfxGetApp()->m_pMainWnd->PostMessageA(UM_ARPTABLEUPDATE);
         return false;
      }
   }
         
      //������ FF-FF-FF...���� �ٺ���...
   //�̴��� �ּҸ� ��ε�ĳ��Ʈ�� ����.
   AfxGetApp()->m_pMainWnd->PostMessageA(UM_ARPTABLEUPDATE);
   ((CEthernetLayer * )mp_UnderLayer)->SetEnetType( 0x806 );
   ((CEthernetLayer * )mp_UnderLayer)->SetDestinAddress(broadcast);

    return mp_UnderLayer->Send((unsigned char*)&m_sHeader , sizeof(m_sHeader));//���⼭ ����� ������ ���� ���̾�� ����
      //ARP Request ����
   //�� index�� ������ ����.
}

// �ޱ�
BOOL CARPLayer::Receive( unsigned char* ppayload )
{
   
   LPARPLayer_HEADER pFrame = (LPARPLayer_HEADER)ppayload;

   if( ntohs(pFrame->op) == 0x0001 ){ // arp request
      m_sHeader.protType = htons(0x0800); 
      m_sHeader.hardType = htons(0x0001);
      m_sHeader.protSize = 4;
      m_sHeader.hardSize = 6;
      m_sHeader.op = htons(0x0002); //reply �ϱ� 1�� ����.

      setSrcIPAddress( m_srcIP );
      setSrcMacAddress( m_srcMAC );
      setDstIPAddress( pFrame->senderIP );
      setDstMacAddress( pFrame->senderMac );
      
      // �۽��� ������׸� �����ϱ�����
      ((CEthernetLayer * )mp_UnderLayer)->SetEnetType( 0x806 );
      ((CEthernetLayer * )mp_UnderLayer)->SetDestinAddress( pFrame->targetMac );

      if( memcmp( pFrame->targetIP, pFrame->senderIP, 4 ) == 0 ){
         // garp ��û�� ���
         if( memcmp( pFrame->targetIP, m_srcIP, 4 ) == 0 ){
            // ���� IP �̸� �浹, ������
            return mp_UnderLayer->Send((unsigned char*)&m_sHeader , sizeof(m_sHeader));
         }else{
            // ���� IP �ƴϸ� ARP Table update, �Ⱥ�����
            int ARPindex = SearchCacheTable( pFrame->senderIP );

            if( ARPindex == -1 ){
               CacheTable data;
               memcpy( data.ipAddress, pFrame->senderIP, 4 );
               memcpy( data.macAddress, pFrame->senderMac, 6 );
               data.state.Format("%s","complete");
               data.time = TIMEOFCOMPLETE;
               cache_table.AddTail( data );
            }else{
               memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).ipAddress, pFrame->senderIP, 4);
               memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).macAddress, pFrame->senderMac, 6);
               cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).state.Format("%s","complete");
               cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).time = TIMEOFCOMPLETE;
            }

            AfxGetApp()->m_pMainWnd->PostMessageA(UM_ARPTABLEUPDATE);
         }
      }
      else{
         // arp ��û�� ���, ������Ʈ �� ������
         int ARPindex = SearchCacheTable( pFrame->senderIP );

         if( ARPindex == -1 ){
            CacheTable data;
            memcpy( data.ipAddress, pFrame->senderIP, 4 );
            memcpy( data.macAddress, pFrame->senderMac, 6 );
            data.state.Format("%s","complete");
            data.time = TIMEOFCOMPLETE;
            cache_table.AddTail( data );
         }else{
            memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).ipAddress, pFrame->senderIP, 4);
            memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).macAddress, pFrame->senderMac, 6);
            cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).state.Format("%s","complete");
            cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).time = TIMEOFCOMPLETE;
         }
         
         AfxGetApp()->m_pMainWnd->PostMessageA(UM_ARPTABLEUPDATE);

         if( memcmp( pFrame->targetIP, m_srcIP, 4 ) == 0 ){
            return mp_UnderLayer->Send((unsigned char*)&m_sHeader , sizeof(m_sHeader));
         }
      }
   }
   else if( ntohs(pFrame->op) == 0x0002 ){
      //reply
      if( memcmp( pFrame->targetIP, pFrame->senderIP, 4 ) == 0 ){
         int ARPindex = SearchCacheTable( pFrame->senderIP );

         if( ARPindex == -1 ){
            CacheTable data;
            memcpy( data.ipAddress, pFrame->senderIP, 4 );
            memcpy( data.macAddress, pFrame->senderMac, 6 );
            data.state.Format("%s","complete");
            data.time = TIMEOFCOMPLETE;
            cache_table.AddTail( data );
            
         }else{
            memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).ipAddress, pFrame->senderIP, 4);
            memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).macAddress, pFrame->senderMac, 6);
            cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).state.Format("%s","complete");
            cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).time = TIMEOFCOMPLETE;
         }

         AfxGetApp()->m_pMainWnd->PostMessageA(UM_ARPTABLEUPDATE);
         AfxMessageBox("IP �浹");
      }
      else{
         // arp ������ ���, ������Ʈ
         int ARPindex = SearchCacheTable( pFrame->senderIP );

         if( ARPindex == -1 ){
            CacheTable data;
            memcpy( data.ipAddress, pFrame->senderIP, 4 );
            memcpy( data.macAddress, pFrame->senderMac, 6 );
            data.state.Format("%s","complete");
            data.time = TIMEOFCOMPLETE;
            cache_table.AddTail( data );
         }else{
            memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).ipAddress, pFrame->senderIP, 4);
            memcpy( cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).macAddress, pFrame->senderMac, 6);
            cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).state.Format("%s","complete");
            cache_table.GetAt( cache_table.FindIndex( ARPindex ) ).time = TIMEOFCOMPLETE;
            //���Ⱑ ������ �ͼ� �ٽ� ��Ŷ�� �����ٴ� �κ�.
            //��Ŷ�� ���� ��.
            //isData()�� Ʈ���϶�
            mp_UnderLayer->Send(payload_Q.front() , sizeof(m_sHeader));
            /*
               isData()�� false�̸� �ƹ� �۾� ���� ������.
            */
         }
         
         AfxGetApp()->m_pMainWnd->PostMessageA(UM_ARPTABLEUPDATE);
      }
   }
   return TRUE; //op�� 1,2�� �ƴҶ�.
}

int CARPLayer::SearchCacheTable( unsigned char ip[4] ){
   if(cache_table.GetCount() == 0){ //CacheTable �ȿ� �ƹ��͵� ������
      return -1;
   }
   else{ //CacheTable �ȿ� ���𰡰� ������.
      for(int i = 0; i< cache_table.GetCount(); i++){ //getCount�� ����Ͽ� �� ���ڸ�ŭ for���� �����ش�.
         if(memcmp(cache_table.GetAt(cache_table.FindIndex(i)).ipAddress, ip, 4) == 0 ){
            //memcmp�� ����Ͽ� targetIP�� ���̺�ȿ��ִ°͵� ipAddress�� Ȯ���Ͽ� ������� ( 0�϶� )
            return i; //�� �ε����� �����Ѵ�.
         }
      }
   }
   return -1; //cacheTable�ȿ� ������ -1�� �����Ѵ�.
}

int      CARPLayer::getTableCount(){
   return cache_table.GetCount();
}

CString CARPLayer::checkTableState( int index, int state ){
	CString index_info;
	CacheTable& check = cache_table.GetAt(cache_table.FindIndex(index));
	int time = check.time;

	if( state == 0 ) // Ÿ�̸��ΰ��
		check.time -= 1;

	if( time == TIMEOFDELETE ){		// �ش� ���� ����
		CString deleted("deleted");
		cache_table.RemoveAt(cache_table.FindIndex(index));
		return deleted;
	}else if ( time == TIMEOFIMCOMPLETE ){		// ���� �� ���� 
		check.state.Format("%s","imcomplete");
	}

	index_info.Format("%d.%d.%d.%d/%02x:%02x:%02x:%02x:%02x:%02x/%s/%s",
			check.ipAddress[0],
			check.ipAddress[1],
			check.ipAddress[2],
			check.ipAddress[3],
			check.macAddress[0],
			check.macAddress[1],
			check.macAddress[2],
			check.macAddress[3],
			check.macAddress[4],
			check.macAddress[5],
			check.interfaceName,
			check.state);

	return index_info;
}

void	CARPLayer::itemDelete( int index ){
	cache_table.RemoveAt( cache_table.FindIndex(index) );
}

void	CARPLayer::allDelete(){
	if( !cache_table.IsEmpty() ){
		cache_table.RemoveAll();
	}
}