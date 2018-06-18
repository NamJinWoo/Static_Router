// IPBlockListDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARP.h"
#include "IPBlockListDlg.h"
#include "afxdialogex.h"


// IPBlockListDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(IPBlockListDlg, CDialogEx)

IPBlockListDlg::IPBlockListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IPBlockListDlg::IDD, pParent)
{

}

IPBlockListDlg::~IPBlockListDlg()
{
}

void IPBlockListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	
	DDX_Control(pDX, IDC_IPBlockList_IP, ip_block);
}

void IPBlockListDlg::OnOK(){
	ip_block.GetAddress(this->block[3],this->block[2],this->block[1],this->block[0]); // ���(����)��! (ip)
	
	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(IPBlockListDlg, CDialogEx)
END_MESSAGE_MAP()


// IPBlockListDlg �޽��� ó�����Դϴ�.

void IPBlockListDlg::getIp_block(BYTE& ip1, BYTE& ip2, BYTE& ip3, BYTE& ip4){
	ip1 = block[0];
	ip2 = block[1];
	ip3 = block[2];
	ip4 = block[3];
}
