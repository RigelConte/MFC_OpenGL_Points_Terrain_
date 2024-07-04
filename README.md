# MFC_OpenGL_Points_Terrain_

This application generates a vector of points to simulate a LiDAR map (initially the simulation only generates a vector of points).

The way it works is like a simple MFC application with the option of creating a new child window and instantiating a new map, it even gives the opportunity to view several maps in parallel. It also has the functionality for organizing standard child windows from the Windows API.

the operation of OpenGL was handled in the class 'CMFCOpenGLPointsTerrainView' are generated:

 HGLRC m_hRC: OpenGL Rendering Context.
 HDC m_hDC: Device Context.
 std::vector<Point> m_points: Vector of generated points.

 float m_zoomLevel: Zoom level
 float m_rotateX: Rotation around the X-axis
 float m_rotateY: Rotation around the Y-axis
 bool m_mouseDrag: Mouse drag state
 CPoint m_lastMousePos: Last mouse position.

SetupPixelFormat: This function is responsible for setting the pixel format for the OpenGL device context. It defines the characteristics of how pixels will be represented and processed by the graphics system, including properties such as color, depth and buffer mode, essential for the correct rendering of 3D images.

SetupOpenGL: This function initializes the OpenGL environment required to draw 3D graphics. It configures global OpenGL parameters, such as the render window background color and scene depth settings. It can also enable features like depth testing, which are crucial for rendering 3D objects realistically.

RenderScene: The RenderScene function is responsible for drawing the entire 3D scene in the OpenGL viewport. It clears the screen, configures the projection and visualization matrix, draws 3D objects (such as points, lines, or polygons) with appropriate transformations (such as rotation, scale, and translation), and finally displays the rendered scene in the window.

UpdateProjection: This function updates the OpenGL projection matrix based on the viewport dimensions. It redefines how 3D objects are projected on the screen, ensuring that they are displayed correctly on different screen sizes and aspect ratios.

RenderText: The RenderText function draws text into the OpenGL scene. It receives a text string and the coordinates (x, y) where the text should be displayed on the screen. Using orthographic projection techniques, it disables the depth test and sets the text color to white, allowing the text to be rendered directly over the 3D scene.

LoadLiDARData: This function loads data from a LiDAR (Light Detection and Ranging) file into a data structure. It processes raw LiDAR data, converting it into a collection of 3D points or other types of geometric data that can be used to construct or visualize terrain, objects, or other 3D representations in the OpenGL environment.

To run the application, please make sure you have Visual Studio properly configured for the C++ language, including support for MFC applications and v143 compiler.
