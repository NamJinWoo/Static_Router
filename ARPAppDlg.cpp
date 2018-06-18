// ARPAppDlg.cpp : implementation file
//
#include "stdafx.h"
#include "arp.h"
#include "ARPAppDlg.h"

#define UM_UPDATEDATA WM_USER+1
#define UM_ARPTABLEUPDATE WM_USER+2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CARPAppDlg dialog

// 수정 필요
CARPAppDlg::CARPAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CARPAppDlg::IDD, pParent), 
	  CBaseLayer( "ARPDlg" ),
	  m_bSendReady( FALSE )
{
	//{{AFX_DATA_INIT(CARPAppDlg)

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	// 아이콘 설정
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// -------------------------- 계층 생성 -------------------------- //
	m_LayerMgr.AddLayer( new CARPLayer( "ARP" ) ) ; //  
	m_LayerMgr.AddLayer( new CIPLayer( "IP" ) ) ; 
	m_LayerMgr.AddLayer( new CEthernetLayer( "Ethernet" ) ) ; // 
	m_LayerMgr.AddLayer( new CNILayer( "NI" ) ) ; // 
	m_LayerMgr.AddLayer( this ) ; //  ChatDlg

	// -------------------------- 계층 연결 -------------------------- //
	m_LayerMgr.ConnectLayers("NI ( *Ethernet ( +IP ( -ARP *ARPDlg ) *ARP ) )"); // 계층 순서대로 연결

	m_ARP = (CARPLayer*) m_LayerMgr.GetLayer( "ARP" ) ;
	m_IP = (CIPLayer*) m_LayerMgr.GetLayer( "IP" ) ;
	m_ETH = (CEthernetLayer*) m_LayerMgr.GetLayer( "Ethernet" ) ;
	m_NI = (CNILayer*) m_LayerMgr.GetLayer( "NI" ) ;
}	

void CARPAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CARPAppDlg)
	DDX_Control(pDX, IDC_ARRCacheTable_List, m_ListARP);
	DDX_Control(pDX, IDC_StaticRouterTable_LIST, m_ListRouter);
	DDX_Control(pDX, IDC_IPBlock_LIST, m_ListIPBlock);
	
	//DDX_Control(pDX, IDC_COMBO_NIC, m_ComboNIC);
	//}}AFX_DATA_MAP
}


// MEssage 별 핸들러 연결 
BEGIN_MESSAGE_MAP(CARPAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER() 

	ON_MESSAGE(UM_UPDATEDATA,OnUpdateDataFalse)
	ON_MESSAGE(UM_ARPTABLEUPDATE, OnARPTableUpdate)

	ON_BN_CLICKED(IDC_ARPCacheTable_DeleteBnt, &CARPAppDlg::OnBnClickedArpcachetableDeletebnt)
	ON_BN_CLICKED(IDC_IPBlockList_AddBnt, &CARPAppDlg::OnBnClickedIpblocklistAddbnt)
	ON_BN_CLICKED(IDC_IPBlockList_DeleteBnt, &CARPAppDlg::OnBnClickedIpblocklistDeletebnt)
	ON_BN_CLICKED(IDC_StaticRoutingTable_DeleteBnt, &CARPAppDlg::OnBnClickedStaticroutingtableDeletebnt)
	ON_BN_CLICKED(IDC_StaticRoutingTable_AddBnt, &CARPAppDlg::OnBnClickedStaticroutingtableAddbnt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CARPAppDlg message handlers

// 수정 필요
BOOL CARPAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	
	// 제작자 설명

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon
	
	// TODO: Add extra initialization here
	// ListControl 구성
	m_ListRouter.InsertColumn(0, _T("Destination"), NULL, 130);
	m_ListRouter.InsertColumn(1, _T("NetMask"), NULL, 130);
	m_ListRouter.InsertColumn(2, _T("Gateway"), NULL, 130);
	m_ListRouter.InsertColumn(3, _T("Flag"), NULL, 45);
	m_ListRouter.InsertColumn(4, _T("Interface"), NULL, 130);
	m_ListRouter.InsertColumn(5, _T("Metric"), NULL,45);
	
	m_ListARP.InsertColumn(0, _T("IP Address"), NULL, 100);
	m_ListARP.InsertColumn(1, _T("Ethernet Address"), NULL, 110);
	m_ListARP.InsertColumn(2, _T("Interface"), NULL, 100);
	m_ListARP.InsertColumn(3, _T("Flag"), NULL, 45);

	m_ListIPBlock.InsertColumn(0, _T("IP Address"), NULL, 360);

	//SetDlgState( arp_INITIALIZING ) ; // 기본 통신 준비
	//SetDlgState( CFT_COMBO_SET );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// 수정 필요

void CARPAppDlg::SetDlgState(int state)
{
	//CComboBox*	pNICCombo = (CComboBox*) GetDlgItem( IDC_COMBO_NIC ) ;

	UpdateData( TRUE ) ;

	switch( state ){
	case arp_INITIALIZING: 
		// 기본 화면 세팅, arp 테이블 지우는 거 필요
		m_ListRouter.DeleteAllItems();
		m_ListARP.DeleteAllItems();
		m_ListIPBlock.DeleteAllItems();

		break;
	case CFT_COMBO_SET:
		/*
		CString NIC_description;

		for(int i = 0 ; i< NI_COUNT_NIC ; i ++ )
		{
			if( !m_NI->GetAdapter( i ) )
				break;
			NIC_description = m_NI->GetAdapter( i )->description ;
			NIC_description.Trim();
			m_ComboNIC.AddString( NIC_description ) ;
			
		}
		m_ComboNIC.SetCurSel( 0 ) ;
		*/
		break;
	}
	UpdateData( FALSE ) ;

}

void CARPAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ( nID == SC_CLOSE )
	{
		if ( MessageBox( "Are you sure ?", 
						 "Question", 
						 MB_YESNO | MB_ICONQUESTION )  == IDNO )
			return ;
		else EndofProcess( ) ;
	}

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CARPAppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CARPAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ARP 정보 수신하여, listbox 업데이트, 아마 안 쓸듯
BOOL CARPAppDlg::Receive(unsigned char *ppayload)
{
	return TRUE ;
}


LRESULT CARPAppDlg::OnUpdateDataFalse(WPARAM wParam, LPARAM lParam)
{
      UpdateData(FALSE);
      return 0;
}

LRESULT CARPAppDlg::OnARPTableUpdate(WPARAM wParam, LPARAM lParam)
{
	int table_count = m_ARP->getTableCount();
		int i = 0;

		m_ListARP.DeleteAllItems();

		while ( table_count != 0 && i < table_count ){

			CString i_info = m_ARP->checkTableState(i, 0);
			if( i_info.CompareNoCase("deleted") == 0 ){
				i--;
			}else{ // 리스트 출력 
				CString ip, mac, interfaceName, flag;

				AfxExtractSubString(ip, i_info, 0,'/');
				AfxExtractSubString(mac, i_info, 1,'/');
				AfxExtractSubString(interfaceName, i_info, 2,'/');
				AfxExtractSubString(flag, i_info, 3,'/');

				LVITEM lvItem ={0};
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;

				m_ListARP.InsertItem(&lvItem);
				m_ListARP.SetItemText(i, 0, ip);
				m_ListARP.SetItemText(i, 1, mac);
				m_ListARP.SetItemText(i, 2, interfaceName);
				m_ListARP.SetItemText(i, 3, flag);
			}
			i++;
			table_count = m_ARP->getTableCount();
		}
	return 0;
}

BOOL CARPAppDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch( pMsg->message )
	{
	case WM_KEYDOWN :
		switch( pMsg->wParam )
		{
		case VK_RETURN : 
			return FALSE ;
		case VK_ESCAPE : 
			return FALSE ;
		}
		break ;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CARPAppDlg::EndofProcess()
{
	m_LayerMgr.DeAllocLayer( ) ;
}

// 수정 필요 // 1초마다 arp table 확인 
void CARPAppDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	// ARP TABLE CHECK TIMER 인 경우
	if( nIDEvent == 2018 ){
		// 테이블 내의 접근
		int table_count = m_ARP->getTableCount();
		int i = 0;

		m_ListARP.DeleteAllItems();

		while ( table_count != 0 && i < table_count ){

			CString i_info = m_ARP->checkTableState(i, 0);
			if( i_info.CompareNoCase("deleted") == 0 ){
				i--;
			}else{ // 리스트 출력 
				CString ip, mac, interfaceName, flag;

				AfxExtractSubString(ip, i_info, 0,'/');
				AfxExtractSubString(mac, i_info, 1,'/');
				AfxExtractSubString(interfaceName, i_info, 2,'/');
				AfxExtractSubString(flag, i_info, 3,'/');

				LVITEM lvItem ={0};
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;

				m_ListARP.InsertItem(&lvItem);
				m_ListARP.SetItemText(i, 0, ip);
				m_ListARP.SetItemText(i, 1, mac);
				m_ListARP.SetItemText(i, 2, interfaceName);
				m_ListARP.SetItemText(i, 3, flag);
			}
			i++;
			table_count = m_ARP->getTableCount();
		}
	}
}
/*
void CARPAppDlg::OnBnClickedButtonSet(){
	UpdateData( TRUE ) ;

	if ( m_bSendReady ){
		SetDlgState( arp_ADDR_RESET ) ;
		SetDlgState( arp_INITIALIZING ) ;
	}
	else{
		// NI_수신 시작
		int index = m_ComboNIC.GetCurSel(); // 현재 선택한 랜카드 인덱스가져오기

		m_NI->SetAdapterNum( index ); // 가져온 인덱스로 통신 준비
		m_NI->OpenSocket() ; // 통신 가능 설정

		SetDlgState( arp_ADDR_SET );
	}

	m_bSendReady = !m_bSendReady; // 이전 상태와 반대로 바꿈
}
*/

void CARPAppDlg::OnBnClickedArpcachetableDeletebnt()
{
	// TODO: Arp Cache Table 아이템 지우기

	POSITION pos = m_ListARP.GetFirstSelectedItemPosition();
	int nItemIdx = 0;

	if( pos != NULL ){
		nItemIdx = m_ListARP.GetNextSelectedItem(pos);
		m_ListRouter.DeleteItem(nItemIdx);
		m_ARP->itemDelete( nItemIdx );
	}
}


void CARPAppDlg::OnBnClickedIpblocklistAddbnt()
{
	// TODO: IP Block List 추가 창 띄우기
	IPBlockListDlg dlg;
	BYTE ip_block[4];

	if( IDOK != dlg.DoModal()) return;

	dlg.getIp_block(ip_block[0], ip_block[1], ip_block[2], ip_block[3]);

	// IP Block Table 에 추가

}


void CARPAppDlg::OnBnClickedIpblocklistDeletebnt()
{
	// TODO: IP Block List 아이템 지우기
	POSITION pos = m_ListIPBlock.GetFirstSelectedItemPosition();
	int nItemIdx = 0;

	if( pos != NULL ){
		nItemIdx = m_ListIPBlock.GetNextSelectedItem(pos);
		m_ListRouter.DeleteItem(nItemIdx);
		// IP Block List 삭제
	}

}


void CARPAppDlg::OnBnClickedStaticroutingtableDeletebnt()
{
	// TODO: Routing Table 아이템 지우기
	POSITION pos = m_ListRouter.GetFirstSelectedItemPosition();
	int nItemIdx = 0;

	if( pos != NULL ){
		nItemIdx = m_ListRouter.GetNextSelectedItem(pos);
		m_ListRouter.DeleteItem(nItemIdx);
		// Routing table List 삭제
	}
}


void CARPAppDlg::OnBnClickedStaticroutingtableAddbnt()
{
	// TODO: Routing Table 아이템 추가 창 띄우기
	TableAddDlg dlg;
	BYTE ip_dst[4];
	BYTE ip_subnet[4];
	BYTE ip_gateway[4];
	CString interfaceName;
	CString	flag;
	CString index_info;

	if( IDOK != dlg.DoModal()) return;
	// 정보 받고, 테이블에 저장 필요
	dlg.getIp_dst(ip_dst[0],ip_dst[1],ip_dst[2],ip_dst[3]);
	dlg.getIp_subnet(ip_subnet[0],ip_subnet[1],ip_subnet[2],ip_subnet[3]);
	dlg.getIp_gateway(ip_gateway[0],ip_gateway[1],ip_gateway[2],ip_gateway[3]);
	dlg.getInterfaceName(interfaceName);
	dlg.getFalg(flag);

	index_info.Format("IP:%d.%d.%d.%d, SUB:%d.%d.%d.%d, gateway:%d.%d.%d.%d",
			ip_dst[0],
			ip_dst[1],
			ip_dst[2],
			ip_dst[3],
			ip_subnet[0],
			ip_subnet[1],
			ip_subnet[2],
			ip_subnet[3],
			ip_gateway[0],
			ip_gateway[1],
			ip_gateway[2],
			ip_gateway[3]);

	AfxMessageBox(index_info);
	AfxMessageBox(interfaceName);
	AfxMessageBox(flag);


	// ip layer 테이블에 추가

	
}

CNILayer* CARPAppDlg::Get_NI(){
	return m_NI;
}