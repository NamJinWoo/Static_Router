// TableAddDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARP.h"
#include "TableAddDlg.h"
#include "afxdialogex.h"

#include "ARPappDlg.h"

// TableAddDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(TableAddDlg, CDialogEx)

TableAddDlg::TableAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(TableAddDlg::IDD, pParent)
{

}

TableAddDlg::~TableAddDlg()
{
}

void TableAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_Table_Dest, ip_dst);
	DDX_Control(pDX, IDC_Table_SubNetMask, ip_subnet);
	DDX_Control(pDX, IDC_Table_Gateway, ip_gateway);
	DDX_Control(pDX, IDC_Interface_Combo, interfaceNames);
	DDX_Control(pDX, IDC_Flag_UP, chkbox_up);
	DDX_Control(pDX, IDC_Flag_Gateway, chkbox_gateway);
	DDX_Control(pDX, IDC_Flag_Host, chkbox_host);

	CString NIC_description;

	CNILayer* m_NI = ((CARPAppDlg*)GetParent())->Get_NI();

	for(int i = 0 ; i< NI_COUNT_NIC ; i ++ )
	{
		if( !m_NI->GetAdapter( i ) )
			break;
		NIC_description = m_NI->GetAdapter( i )->description ;
		NIC_description.Trim();
		interfaceNames.AddString( NIC_description ) ;
			
	}
	interfaceNames.SetCurSel( 0 ) ;
}

void TableAddDlg::OnOK(){

	bool isUP = IsDlgButtonChecked(IDC_Flag_UP);
	bool isGateWay = IsDlgButtonChecked(IDC_Flag_Gateway);
	bool isHost = IsDlgButtonChecked(IDC_Flag_Host);

	ip_dst.GetAddress(this->dst[3],this->dst[2],this->dst[1],this->dst[0]); // 도착 어드레스 (ip)
	ip_subnet.GetAddress(this->subnet[3],this->subnet[2],this->subnet[1],this->subnet[0]); // subnet (ip)
	ip_gateway.GetAddress(this->gateway[3],this->gateway[2],this->gateway[1],this->gateway[0]); // gateway (ip)
	
	flag = L"";
	interfaceName = L"";

	if( isUP ) flag += "U";
	if( isGateWay ) flag += "G";
	if( isHost ) flag += "H";

	interfaceNames.GetLBText( interfaceNames.GetCurSel(), interfaceName );

	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(TableAddDlg, CDialogEx)
END_MESSAGE_MAP()

void TableAddDlg::getIp_dst(BYTE& ip1, BYTE& ip2, BYTE& ip3, BYTE& ip4){
	ip1 = dst[0];
	ip2 = dst[1];
	ip3 = dst[2];
	ip4 = dst[3];
}
void TableAddDlg::getIp_subnet(BYTE& sub1, BYTE& sub2, BYTE& sub3, BYTE& sub4){
	sub1 = subnet[0];
	sub2 = subnet[1];
	sub3 = subnet[2];
	sub4 = subnet[3];
}
void TableAddDlg::getIp_gateway(BYTE& gw1, BYTE& gw2, BYTE& gw3, BYTE& gw4){
	gw1 = gateway[0];
	gw2 = gateway[1];
	gw3 = gateway[2];
	gw4 = gateway[3];
}
void TableAddDlg::getInterfaceName(CString& interfaceName){
	interfaceName = this->interfaceName;
}
void TableAddDlg::getFalg(CString& flag){
	flag = this->flag;
}

// TableAddDlg 메시지 처리기입니다.
