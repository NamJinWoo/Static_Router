#pragma once


// TableAddDlg 대화 상자입니다.

class TableAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TableAddDlg)

public:
	TableAddDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TableAddDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_StaticRoutingTable_Add };
	

	void		getIp_dst(BYTE& ip1, BYTE& ip2, BYTE& ip3, BYTE& ip4);
	void		getIp_subnet(BYTE& sub1, BYTE& sub2, BYTE& sub3, BYTE& sub4);
	void		getIp_gateway(BYTE& gw1, BYTE& gw2, BYTE& gw3, BYTE& gw4);
	void		getInterfaceName(CString& interfaceName);
	void		getFalg(CString& flag);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	CComboBox		interfaceNames;
	CIPAddressCtrl	ip_dst;
	CIPAddressCtrl	ip_subnet;
	CIPAddressCtrl	ip_gateway;
	CButton			chkbox_up;
	CButton			chkbox_host;
	CButton			chkbox_gateway;


	BYTE	dst[4];
	BYTE	subnet[4];
	BYTE	gateway[4];
	CString interfaceName;
	CString flag;

public:
	afx_msg void OnBnClickedOk();

};
