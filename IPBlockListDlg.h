#pragma once


// IPBlockListDlg 대화 상자입니다.

class IPBlockListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(IPBlockListDlg)

public:
	IPBlockListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~IPBlockListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IPBlockList_Add };
	void	getIp_block(BYTE& ip1, BYTE& ip2, BYTE& ip3, BYTE& ip4);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnOK();
	
	CIPAddressCtrl	ip_block;

	BYTE	block[4];
	
	DECLARE_MESSAGE_MAP()
};
