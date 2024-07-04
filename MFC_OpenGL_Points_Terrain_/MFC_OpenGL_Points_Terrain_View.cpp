
// MFC_OpenGL_Points_Terrain_View.cpp : implementation of the CMFCOpenGLPointsTerrainView class
//

#include "pch.h"
#include "framework.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFC_OpenGL_Points_Terrain_.h"
#endif

#include "MFC_OpenGL_Points_Terrain_Doc.h"
#include "MFC_OpenGL_Points_Terrain_View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCOpenGLPointsTerrainView Class ...............................................................................................................................................//

IMPLEMENT_DYNCREATE(CMFCOpenGLPointsTerrainView, CView)

BEGIN_MESSAGE_MAP(CMFCOpenGLPointsTerrainView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()

	// Mouse commands
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

CMFCOpenGLPointsTerrainView::CMFCOpenGLPointsTerrainView() noexcept
	:
	m_hRC(nullptr), 
	m_hDC(nullptr), 	
	m_mouseDrag(false),
	m_rotateX(0.0f), 
	m_rotateY(0.0f), 
	m_zoomLevel(1.0f)
{
}

CMFCOpenGLPointsTerrainView::~CMFCOpenGLPointsTerrainView()
{
}

BOOL CMFCOpenGLPointsTerrainView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


void CMFCOpenGLPointsTerrainView::OnDraw(CDC* /*pDC*/)
{
	if (wglMakeCurrent(m_hDC, m_hRC)) {
		RenderScene();
		wglMakeCurrent(nullptr, nullptr);
	}
}

BOOL CMFCOpenGLPointsTerrainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CMFCOpenGLPointsTerrainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMFCOpenGLPointsTerrainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void CMFCOpenGLPointsTerrainView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCOpenGLPointsTerrainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCOpenGLPointsTerrainDoc* CMFCOpenGLPointsTerrainView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCOpenGLPointsTerrainDoc)));
	return (CMFCOpenGLPointsTerrainDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CMFCOpenGLPointsTerrainView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {
	   sizeof(PIXELFORMATDESCRIPTOR),
	   1,
	   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	   PFD_TYPE_RGBA,
	   24,
	   0, 0, 0, 0, 0, 0,
	   0,
	   0,
	   0,
	   0, 0, 0, 0,
	   32,
	   0,
	   0,
	   PFD_MAIN_PLANE,
	   0,
	   0, 0, 0
	};

	int pixelformat = ChoosePixelFormat(m_hDC, &pfd);
	if (pixelformat == 0)
		return FALSE;

	if (!SetPixelFormat(m_hDC, pixelformat, &pfd))
		return FALSE;

	return TRUE;
}

int CMFCOpenGLPointsTerrainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hDC = ::GetDC(m_hWnd);
	if (!SetupPixelFormat()) {
		return -1;
	}

	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC) {
		return -1;
	}
	if (!wglMakeCurrent(m_hDC, m_hRC)) {
		return -1;
	}

	static bool glutInitialized = false;
	if (!glutInitialized) {
		int argc = 1;
		char* argv[1] = { (char*)"Something" };
		glutInit(&argc, argv);
		glutInitialized = true;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
		return -1;
	}

	SetupOpenGL();
	m_points = SimulateLiDARData();

	return 0;
}

void CMFCOpenGLPointsTerrainView::SetupOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPointSize(2.0f);
}

void CMFCOpenGLPointsTerrainView::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 20 * m_zoomLevel, 0, 0, 0, 0, 1, 0);

	glRotatef(m_rotateX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_rotateY, 0.0f, 1.0f, 0.0f);

	glBegin(GL_POINTS);
	for (const auto& point : m_points) {
		glVertex3f(point.x, point.y, point.z);
	}
	glEnd();

	SwapBuffers(m_hDC);
}

void CMFCOpenGLPointsTerrainView::UpdateProjection()
{
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.Width();
	int cy = rect.Height();

	if (cy == 0)
		cy = 1;

	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (GLdouble)cx / (GLdouble)cy, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

std::vector<Point> CMFCOpenGLPointsTerrainView::SimulateLiDARData()
{
	std::vector<Point> points;

	// Terrain parameters
	float stepSize = 0.1f; // Grid step size
	float scale = 1.0f;    // Elevation scale
	int gridSize = 100;    // Grid

	for (int x = -gridSize; x <= gridSize; ++x) {
		for (int y = -gridSize; y <= gridSize; ++y) {
			float fx = x * stepSize;
			float fy = y * (stepSize * 0.5);

			// Calculates z height as a function of x and y
			float fz = sinf(fx) * cosf(fy) * scale; 

			// Add the point in the vector container
			points.push_back({ fx, fy, fz });
		}
	}

	return points;
}


void CMFCOpenGLPointsTerrainView::OnDestroy()
{
	if (m_hRC) {
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_hRC);
		m_hRC = nullptr;
	}
	if (m_hDC) {
		::ReleaseDC(m_hWnd, m_hDC);
		m_hDC = nullptr;
	}	

	CView::OnDestroy();
}

void CMFCOpenGLPointsTerrainView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CView::OnSize(nType, cx, cy);
	if (wglMakeCurrent(m_hDC, m_hRC)) {
		UpdateProjection();
		wglMakeCurrent(nullptr, nullptr);
	}
}

BOOL CMFCOpenGLPointsTerrainView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// Adjust zoom factor as needed
	m_zoomLevel += zDelta / 1200.0f; 

	// Limit minimum zoom
	if (m_zoomLevel < 0.1f)
		m_zoomLevel = 0.1f; 
	
	// Limit maximum zoom
	if (m_zoomLevel > 10.0f)
		m_zoomLevel = 10.0f; 

	Invalidate(); 

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMFCOpenGLPointsTerrainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_mouseDrag = true;
	m_lastMousePos = point;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CMFCOpenGLPointsTerrainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_mouseDrag = false;
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CMFCOpenGLPointsTerrainView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_mouseDrag) {
		int dx = point.x - m_lastMousePos.x;
		int dy = point.y - m_lastMousePos.y;

		m_rotateX += dy * 0.5f;
		m_rotateY += dx * 0.5f;

		m_lastMousePos = point;
		InvalidateRect(nullptr, FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}