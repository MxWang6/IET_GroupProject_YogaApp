//------------------------------------------------------------------------------
// <copyright file="SkeletonBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "SkeletonBasics.h"
#include "resource.h"
#include <math.h>
#include <iostream>

static const float g_JointThickness = 3.0f;
static const float g_TrackedBoneThickness = 6.0f;
static const float g_InferredBoneThickness = 1.0f;

//----------ANGLES---------------

float noAngle, angle1, angle2, angle3, angle4,
	  angle5, angle6, angle7, angle8 = 0.0;

//-------------------------------

#define PI 3.14159265

using namespace std;

/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    CSkeletonBasics application;
    application.Run(hInstance, nCmdShow);
}

/// <summary>
/// Constructor
/// </summary>
CSkeletonBasics::CSkeletonBasics() :
    m_pD2DFactory(NULL),
	m_pDrawColor(NULL),
	m_hNextColorFrameEvent(INVALID_HANDLE_VALUE),
	m_pColorStreamHandle(INVALID_HANDLE_VALUE),
    m_hNextSkeletonEvent(INVALID_HANDLE_VALUE),
    m_pSkeletonStreamHandle(INVALID_HANDLE_VALUE),
    m_bSeatedMode(false),
    m_pRenderTarget(NULL),
    m_pBrushJointTracked(NULL),
    m_pBrushJointInferred(NULL),
    m_pBrushBoneTracked(NULL),
    m_pBrushBoneInferred(NULL),
    m_pNuiSensor(NULL)
{
    ZeroMemory(m_Points,sizeof(m_Points));
}

/// <summary>
/// Destructor
/// </summary>
CSkeletonBasics::~CSkeletonBasics()
{
    if (m_pNuiSensor)
    {
        m_pNuiSensor->NuiShutdown();
    }

    if (m_hNextSkeletonEvent && (m_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
    {
        CloseHandle(m_hNextSkeletonEvent);
    }

	if (m_hNextColorFrameEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hNextColorFrameEvent);
	}

	// clean up Direct2D renderer
	delete m_pDrawColor;
	m_pDrawColor = NULL;

    // clean up Direct2D objects
    DiscardDirect2DResources();

    // clean up Direct2D
    SafeRelease(m_pD2DFactory);

    SafeRelease(m_pNuiSensor);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CSkeletonBasics::Run(HINSTANCE hInstance, int nCmdShow)
{
    MSG       msg = {0};
    WNDCLASS  wc  = {0};

    // Dialog custom window class
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
    wc.lpfnWndProc   = DefDlgProcW;
    wc.lpszClassName = L"SkeletonBasicsAppDlgWndClass";

    if (!RegisterClassW(&wc))
    {
        return 0;
    }

    // Create main application window
    HWND hWndApp = CreateDialogParamW(
        hInstance,
        MAKEINTRESOURCE(IDD_APP),
        NULL,
        (DLGPROC)CSkeletonBasics::MessageRouter, 
        reinterpret_cast<LPARAM>(this));

    // Show window
    ShowWindow(hWndApp, nCmdShow);

    const int eventCount = 1;
    HANDLE hEvents[eventCount];

    // Main message loop
    while (WM_QUIT != msg.message)
    {
        hEvents[0] = m_hNextSkeletonEvent;

        // Check to see if we have either a message (by passing in QS_ALLEVENTS)
        // Or a Kinect event (hEvents)
        // Update() will check for Kinect events individually, in case more than one are signalled
        MsgWaitForMultipleObjects(eventCount, hEvents, FALSE, INFINITE, QS_ALLINPUT);

        // Explicitly check the Kinect frame event since MsgWaitForMultipleObjects
        // can return for other reasons even though it is signaled.
        Update(&hWndApp);

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // If a dialog message will be taken care of by the dialog proc
            if ((hWndApp != NULL) && IsDialogMessageW(hWndApp, &msg))
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

/// <summary>
/// Main processing function
/// </summary>
void CSkeletonBasics::Update(HWND *hwnd)
{
    if (NULL == m_pNuiSensor)
    {
        return;
    }

    // Wait for 0ms, just quickly test if it is time to process a skeleton
    if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0) )
    {
        ProcessSkeleton(hwnd);
		ProcessColor();
    }

}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CSkeletonBasics::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CSkeletonBasics* pThis = NULL;

    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<CSkeletonBasics*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<CSkeletonBasics*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CSkeletonBasics::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            // Bind application window handle
            m_hWnd = hWnd;

            // Init Direct2D
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

			// Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
			// We'll use this to draw the data we receive from the Kinect to the screen
			m_pDrawColor = new ImageRenderer();
			HRESULT hr = m_pDrawColor->Initialize(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), m_pD2DFactory, cColorWidth, cColorHeight, cColorWidth * sizeof(long));
			if (FAILED(hr))
			{
				SetStatusMessage(L"Failed to initialize the Direct2D draw device.");
			}

            // Look for a connected Kinect, and create it if found
            CreateFirstConnected();
        }
        break;

        // If the titlebar X is clicked, destroy app
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        // Quit the main message pump
        PostQuitMessage(0);
        break;

        // Handle button press
    case WM_COMMAND:
        // If it was for the near mode control and a clicked event, change near mode
        if (IDC_CHECK_SEATED == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
        {
            // Toggle out internal state for near mode
            m_bSeatedMode = !m_bSeatedMode;

            if (NULL != m_pNuiSensor)
            {
                // Set near mode for sensor based on our internal state
                m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, m_bSeatedMode ? NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT : 0);
            }
        }
        break;
    }

    return FALSE;
}

/// <summary>
/// Create the first connected Kinect found 
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CSkeletonBasics::CreateFirstConnected()
{
    INuiSensor * pNuiSensor;

    int iSensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&iSensorCount);

    if (FAILED(hr))
    {
        return hr;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);

        if (FAILED(hr))
        {
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr)
        {
            m_pNuiSensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

    if (NULL != m_pNuiSensor)
    {
        // Initialize the Kinect and specify that we'll be using skeleton
        hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
        if (SUCCEEDED(hr))
        {
			// Create an event that will be signaled when color data is available
			m_hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			// Open a color image stream to receive color frames
			hr = m_pNuiSensor->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_COLOR,
				NUI_IMAGE_RESOLUTION_640x480,
				0,
				2,
				m_hNextColorFrameEvent,
				&m_pColorStreamHandle);

			
            // Create an event that will be signaled when skeleton data is available
            m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

          // Open a skeleton stream to receive skeleton data
            m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0); 
        }
    }

    if (NULL == m_pNuiSensor || FAILED(hr))
    {
        SetStatusMessage(L"No ready Kinect found!");
        return E_FAIL;
    }

//    return hr;
}

/// <summary>
/// Handle new skeleton data
/// </summary>

void CSkeletonBasics::ProcessColor()
{
	
}


void CSkeletonBasics::ProcessSkeleton(HWND *hwnd)
{

	HRESULT hr;
	NUI_IMAGE_FRAME imageFrame;

	// Attempt to get the color frame
	hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0, &imageFrame);
	if (FAILED(hr))
	{
		return;
	}

	INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;

	// Lock the frame data so the Kinect knows not to modify it while we're reading it
	pTexture->LockRect(0, &LockedRect, NULL, 0);

	// Make sure we've received valid data
	if (LockedRect.Pitch != 0)
	{
		// Draw the data with Direct2D
		m_pDrawColor->Draw(static_cast<BYTE *>(LockedRect.pBits), LockedRect.size);

	}

	// We're done with the texture so unlock it
	pTexture->UnlockRect(0);

	// Release the frame
	m_pNuiSensor->NuiImageStreamReleaseFrame(m_pColorStreamHandle, &imageFrame);

	//--------------------------SKELETON-DRAWING------------------------------------------
    NUI_SKELETON_FRAME skeletonFrame = {0};

     hr = m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
    if ( FAILED(hr) )
    {
        return;
    }

    // smooth out the skeleton data
    m_pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);

    // Endure Direct2D is ready to draw
    hr = EnsureDirect2DResources();
    if ( FAILED(hr) )
    {
        return;
    }

    m_pRenderTarget->BeginDraw();
	
	m_pRenderTarget->Clear( );

    RECT rct;
    GetClientRect( GetDlgItem( m_hWnd, IDC_VIDEOVIEW ), &rct);
    int width = rct.right;
    int height = rct.bottom;

    for (int i = 0 ; i < NUI_SKELETON_COUNT; ++i)
    {
        NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;

        if (NUI_SKELETON_TRACKED == trackingState)
        {
            // We're tracking the skeleton, draw it
            DrawSkeleton(skeletonFrame.SkeletonData[i], width, height,hwnd,&imageFrame);
        }
        else if (NUI_SKELETON_POSITION_ONLY == trackingState)
        {
            // we've only received the center point of the skeleton, draw that
            D2D1_ELLIPSE ellipse = D2D1::Ellipse(
                SkeletonToScreen(skeletonFrame.SkeletonData[i].Position, width, height,&imageFrame),
                g_JointThickness,
                g_JointThickness
                );

            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);
        }
    }

    hr = m_pRenderTarget->EndDraw();

    // Device lost, need to recreate the render target
    // We'll dispose it now and retry drawing
    if (D2DERR_RECREATE_TARGET == hr)
    {
        hr = S_OK;
        DiscardDirect2DResources();
    }
}

/// <summary>
/// Draws a skeleton
/// </summary>
/// <param name="skel">skeleton to draw</param>
/// <param name="windowWidth">width (in pixels) of output buffer</param>
/// <param name="windowHeight">height (in pixels) of output buffer</param>
void CSkeletonBasics::DrawSkeleton(const NUI_SKELETON_DATA & skel, int windowWidth, int windowHeight, HWND *hwnd, const NUI_IMAGE_FRAME *imgFrame)
{      
    int i;

    for (i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i)
    {
       m_Points[i] = SkeletonToScreen(skel.SkeletonPositions[i], windowWidth, windowHeight,imgFrame);
					
    }	

    // Render Torso
    DrawBone(skel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER, hwnd, &noAngle);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT, hwnd, &noAngle);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT, hwnd, &noAngle);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE, hwnd, &noAngle);
    DrawBone(skel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER, hwnd, &noAngle);
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT, hwnd, &noAngle);
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT, hwnd, &noAngle);

    // Left Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT, hwnd,&angle1);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT, hwnd,&angle2);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT, hwnd, &noAngle);

	BoneAngle(skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT, 53.0);
	BoneAngle(skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT, -50.0);

    // Right Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT, hwnd, &angle3);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT, hwnd, &angle4);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT, hwnd, &noAngle);

	BoneAngle(skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT, -53.0);
	BoneAngle(skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT, 50.0);

    // Left Leg
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT, hwnd, &angle5);
    DrawBone(skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT, hwnd, &angle6);
    DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT, hwnd, &noAngle);

	BoneAngle(skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT, -50.0);
	BoneAngle(skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT, 50.0);

    // Right Leg
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT, hwnd, &angle7);
    DrawBone(skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT, hwnd, &angle8);
    DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT, hwnd, &noAngle);

	BoneAngle(skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT, 90.0);
	BoneAngle(skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT, 90.0);

    // Draw the joints in a different color
    for (i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i)
    {
        D2D1_ELLIPSE ellipse = D2D1::Ellipse( m_Points[i], g_JointThickness, g_JointThickness );

        if ( skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED )
        {
            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointInferred);
        }
        else if ( skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_TRACKED )
        {
            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);
        }
    }
}


void CSkeletonBasics::BoneAngle(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1, float target)
{
	//Check if bone is vertical
	float angle1;
	float angle2;
	float angle;
	float angleRad;
	if(m_Points[joint0].x - m_Points[joint1].x == 0.0)	//IF THE BONE ANGLE IS VERTICAL
	{
		angle = 90.0;
		 
	}
	else
	{
		angleRad = (m_Points[joint0].y - m_Points[joint1].y)/(m_Points[joint0].x - m_Points[joint1].x);
		angle1 =atan(angleRad);
		angle = angle1 * 57.29564553;
	}

	float tol = 10.0;
	if(angle<(target+tol) &&  angle>(target-tol))
		m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneAngle, g_TrackedBoneThickness);

	 cout << "The arm angle is: "<< angle ;
}

/// <summary>
/// Draws a bone line between two joints
/// </summary>
/// <param name="skel">skeleton to draw bones from</param>
/// <param name="joint0">joint to start drawing from</param>
/// <param name="joint1">joint to end drawing at</param>

void CSkeletonBasics::DrawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1, HWND *hwnd, float *angle)
{
    NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[joint0];
    NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[joint1];

// -------------CALCULATING-ANGLES-AND-ASSIGNING-THEM---------------------------

		float angleRad = (m_Points[joint0].y - m_Points[joint1].y) / (m_Points[joint0].x - m_Points[joint1].x);
		float angle_temp = atan(angleRad);
		*angle = angle_temp * 57.29564553;

		WCHAR buffer[128];
		swprintf_s(buffer, 128, L"%.2f", angle1);
		SetDlgItemTextW(*hwnd, IDC_ANGLE1, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle2);
		SetDlgItemTextW(*hwnd, IDC_ANGLE2, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle3);
		SetDlgItemTextW(*hwnd, IDC_ANGLE3, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle4);
		SetDlgItemTextW(*hwnd, IDC_ANGLE4, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle5);
		SetDlgItemTextW(*hwnd, IDC_ANGLE5, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle6);
		SetDlgItemTextW(*hwnd, IDC_ANGLE6, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle7);
		SetDlgItemTextW(*hwnd, IDC_ANGLE7, buffer);

		swprintf_s(buffer, 128, L"%.2f", angle8);
		SetDlgItemTextW(*hwnd, IDC_ANGLE8, buffer);
//-------------------------------------------------------------------------------

    // If we can't find either of these joints, exit
    if (joint0State == NUI_SKELETON_POSITION_NOT_TRACKED || joint1State == NUI_SKELETON_POSITION_NOT_TRACKED)
    {
        return;
    }

    // Don't draw if both points are inferred
    if (joint0State == NUI_SKELETON_POSITION_INFERRED && joint1State == NUI_SKELETON_POSITION_INFERRED)
    {
        return;
    }

    // We assume all drawn bones are inferred unless BOTH joints are tracked
    if (joint0State == NUI_SKELETON_POSITION_TRACKED && joint1State == NUI_SKELETON_POSITION_TRACKED)
    {
        m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneTracked, g_TrackedBoneThickness);
    }
    else
    {
        m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneInferred, g_InferredBoneThickness);
    }
}

/// <summary>
/// Converts a skeleton point to screen space
/// </summary>
/// <param name="skeletonPoint">skeleton point to tranform</param>
/// <param name="width">width (in pixels) of output buffer</param>
/// <param name="height">height (in pixels) of output buffer</param>
/// <returns>point in screen-space</returns>
D2D1_POINT_2F CSkeletonBasics::SkeletonToScreen(Vector4 skeletonPoint, int width, int height, const NUI_IMAGE_FRAME *imgFrame)
{

//    LONG x, y;
//    USHORT depth;
//
//    // Calculate the skeleton's position on the screen
//    // NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
//    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);
//
//    float screenPointX = static_cast<float>(x * width) / cScreenWidth;
//    float screenPointY = static_cast<float>(y * height) / cScreenHeight;
//
//    return D2D1::Point2F(screenPointX, screenPointY);

	LONG x, y;
	USHORT depth;
	LONG ColorX, ColorY;
	NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);
	NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480,
		NUI_IMAGE_RESOLUTION_320x240,
		&imgFrame->ViewArea,
		x,
		y,
		depth,
		&ColorX,
		&ColorY);

	float screenPointX = static_cast<float>(ColorX * width) / 640;
	float screenPointY = static_cast<float>(ColorY * height) / 480;

	return D2D1::Point2F(screenPointX, screenPointY);
}

/// <summary>
/// Ensure necessary Direct2d resources are created
/// </summary>
/// <returns>S_OK if successful, otherwise an error code</returns>
HRESULT CSkeletonBasics::EnsureDirect2DResources()
{
    HRESULT hr = S_OK;

    // If there isn't currently a render target, we need to create one
    if (NULL == m_pRenderTarget)
    {
        RECT rc;
        GetWindowRect( GetDlgItem( m_hWnd, IDC_VIDEOVIEW ), &rc );  

        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        D2D1_SIZE_U size = D2D1::SizeU( width, height );
        D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
        rtProps.pixelFormat = D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
        rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

        // Create a Hwnd render target, in order to render to the window set in initialize
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            rtProps,
            D2D1::HwndRenderTargetProperties(GetDlgItem( m_hWnd, IDC_VIDEOVIEW), size),
            &m_pRenderTarget
            );
        if ( FAILED(hr) )
        {
            SetStatusMessage(L"Couldn't create Direct2D render target!");
            return hr;
        }

        //light green
       m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.27f, 0.75f, 0.27f), &m_pBrushJointTracked);
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &m_pBrushJointInferred);
       m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &m_pBrushBoneTracked);
       m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &m_pBrushBoneInferred);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 1.0f), &m_pBrushBoneAngle);
    }

    return hr;
}

/// <summary>
/// Dispose Direct2d resources 
/// </summary>
void CSkeletonBasics::DiscardDirect2DResources( )
{
    SafeRelease(m_pRenderTarget);
    SafeRelease(m_pBrushJointTracked);
    SafeRelease(m_pBrushJointInferred);
    SafeRelease(m_pBrushBoneTracked);
    SafeRelease(m_pBrushBoneInferred);
}

/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
void CSkeletonBasics::SetStatusMessage(WCHAR * szMessage)
{
    SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, (LPARAM)szMessage);
}