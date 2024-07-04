
// MFC_OpenGL_Points_Terrain_View.h : interface of the CMFCOpenGLPointsTerrainView class
//

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <vector>

struct Point {
	float x, y, z;
};

class CMFCOpenGLPointsTerrainView : public CView
{
protected: // create from serialization only
	CMFCOpenGLPointsTerrainView() noexcept;
	DECLARE_DYNCREATE(CMFCOpenGLPointsTerrainView)

// Attributes
public:
	CMFCOpenGLPointsTerrainDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCOpenGLPointsTerrainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// OpenGL events
	BOOL SetupPixelFormat();
	void SetupOpenGL();
	void RenderScene();
	void UpdateProjection();

	// Funtion that generates the container of simulated LiDAR points
	std::vector<Point> SimulateLiDARData();

// Generated message map functions
protected:
	// Window events
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Mouse events
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()

private:
	HGLRC m_hRC;				 // OpenGL Rendering Context
	HDC m_hDC;					 // Device Context

	float m_zoomLevel;			 // Zoom level

	float m_rotateX;			 // Rotation around the X-axis
	float m_rotateY;			 // Rotation around the Y-axis

	bool m_mouseDrag;			 // Mouse drag state
	CPoint m_lastMousePos;		 // Last mouse position

	std::vector<Point> m_points; // Container of point of the simulated LiDAR map. 
};

#ifndef _DEBUG  // debug version in MFC_OpenGL_Points_Terrain_View.cpp
inline CMFCOpenGLPointsTerrainDoc* CMFCOpenGLPointsTerrainView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLPointsTerrainDoc*>(m_pDocument); }
#endif

