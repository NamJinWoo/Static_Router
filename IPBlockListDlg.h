#pragma once


// IPBlockListDlg ��ȭ �����Դϴ�.

class IPBlockListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(IPBlockListDlg)

public:
	IPBlockListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~IPBlockListDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IPBlockList_Add };
	void	getIp_block(BYTE& ip1, BYTE& ip2, BYTE& ip3, BYTE& ip4);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnOK();
	
	CIPAddressCtrl	ip_block;

	BYTE	block[4];
	
	DECLARE_MESSAGE_MAP()
};
