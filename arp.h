// arp.h : main header file for the arp application
//

#if !defined(AFX_arp_H__3D24E705_A889_431A_B388_17290947B99B__INCLUDED_)
#define AFX_arp_H__3D24E705_A889_431A_B388_17290947B99B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CarpApp:
// See arp.cpp for the implementation of this class
//

class CarpApp : public CWinApp
{
public:
	CarpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CarpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CarpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_arp_H__3D24E705_A889_431A_B388_17290947B99B__INCLUDED_)
