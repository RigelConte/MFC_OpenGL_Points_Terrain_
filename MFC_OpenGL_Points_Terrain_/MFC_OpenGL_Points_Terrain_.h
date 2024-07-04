
// MFC_OpenGL_Points_Terrain_.h : main header file for the MFC_OpenGL_Points_Terrain_ application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCOpenGLPointsTerrainApp:
// See MFC_OpenGL_Points_Terrain_.cpp for the implementation of this class
//

class CMFCOpenGLPointsTerrainApp : public CWinApp
{
public:
	CMFCOpenGLPointsTerrainApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCOpenGLPointsTerrainApp theApp;
