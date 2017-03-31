//------------------------------------------------------------------------------
// <copyright file="NuiStreamViewer.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "NuiAccelerometerStream.h"
#include "NuiStreamViewer.h"
#include "resource.h"
#include "NuiAccelerometerViewer.h"
#include <strsafe.h>	//
#include <iostream>			//
#include <string>			//
#include <sstream>			//
#include <vector>			//
#include <fstream>			//
#include <time.h>			//
#include <windows.h>		//
#include "MainWindow.h"


using namespace std;

//----------ANGLES---------------

float noAngle, angle1, angle2, angle3, angle4,
angle5, angle6, angle7, angle8 = 0.0;

float				m_matchAngles[8];

//-------------------------------

ifstream in("Angles.csv");
string line, field;
vector<string> v;

class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
};

unsigned long seconds = 4;
timer t;
vector<bool> bn;	//

bool timerRunning = false;
bool timeFinished = false;

/// <summary>
/// Constructor
/// </summary>
/// <param name="pParent">The pointer to parent window</param>
NuiStreamViewer::NuiStreamViewer(const NuiViewer* pParent)
    : NuiViewer(pParent)
    , m_imageType(NUI_IMAGE_TYPE_COLOR)
    , m_pImage(nullptr)
    , m_pauseSkeleton(false)													////////////////////////INitialize here
    , m_pSkeletonFrame(nullptr)
    , m_drawEdgeFlags(0)
    , m_frameCount(0)
    , m_lastFrameCount(0)
    , m_fps(0)
	
{
    m_pImageRenderer = new ImageRenderer();
    m_lastTick = GetTickCount();

	///////////////////////////////////////////////////////////////////////////////////////////////NEW CODE
	while (getline(in, line))    // get next line in file
	{
		v.clear();
		stringstream ss(line);

		while (getline(ss, field, ','))  // break line into comma delimitted fields
		{
			v.push_back(field);  // add each field to the 1D array
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < 8; i++) {
		bn.push_back(false);
		m_matchAngles[i] = 0;
	}
	m_matchPercentage = 0;

}

/// <summary>
/// Destructor
/// </summary>
NuiStreamViewer::~NuiStreamViewer()
{
    SafeDelete(m_pImageRenderer);
}

/// <summary>
/// Dispatch window message to message handlers.
/// </summary>
/// <param name="hWnd">Handle to window</param>
/// <param name="uMsg">Message type</param>
/// <param name="wParam">Extra message parameter</param>
/// <param name="lParam">Extra message parameter</param>
/// <returns>
/// If message is handled, non-zero is returned. Otherwise FALSE is returned and message is passed to default dialog procedure
/// </returns>
LRESULT NuiStreamViewer::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CTLCOLORDLG:
        // Set background color as black
        return (LRESULT)GetStockObject(BLACK_BRUSH);

    case WM_PAINT:
        OnPaint(wParam, lParam);
        break;

    case WM_SIZE:
        {
            UINT width  = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            m_pImageRenderer->ResizeRenderTarget(width, height);
        }
        break;

    default:
        break;
    }

    return (LRESULT)FALSE;
}

/// <summary>
/// Returns the ID of the dialog
/// </summary>
/// <returns>ID of dialog</returns>
UINT NuiStreamViewer::GetDlgId()
{
    return IDD_STREAM_VIEW;
}

/// <summary>
/// Message handler of WM_PAINT.
/// </summary>
/// <param name="wParam">Extra message parameter</param>
/// <param name="lParam">Extra message parameter</param>
void NuiStreamViewer::OnPaint(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr = m_pImageRenderer->BeginDraw(m_hWnd);
    if (FAILED(hr))
        return;

    // Get viewer window client rect
    RECT clientRect;
    if (!::GetClientRect(m_hWnd, &clientRect))
    {
        return;
    }

    // Calculate the area the stream image is to streched to fit
    D2D1_RECT_F imageRect = GetImageRect(clientRect);

    // Draw stream images
    DrawImage(imageRect);

    // Draw skeletons
    DrawSkeletons(imageRect);

    // Draw image resolution
    DrawResolution(clientRect);

    // Draw FPS
    DrawFPS(clientRect);

    // Draw red edges if skeleton is close to edges of image
    DrawRedEdges(imageRect);

    m_pImageRenderer->EndDraw();
}

/// <summary>
/// Draw the image on screen by D2D
/// </summary>
/// <param name="imageRect">The rect which the color or depth image is streched to fit</param>
void NuiStreamViewer::DrawImage(const D2D1_RECT_F& imageRect)
{
    if (m_pImage && m_pImage->GetBufferSize())
    {
        D2D1_SIZE_U imageSize = D2D1::SizeU(m_pImage->GetWidth(), m_pImage->GetHeight());
        m_pImageRenderer->DrawImage(m_pImage->GetBuffer(), imageSize, imageRect);
    }
}

/// <summary>
/// Draw skeletons
/// </summary>
/// <param name="imageRect">The rect which the color or depth stream image is streched to fit</param>
void NuiStreamViewer::DrawSkeletons(const D2D1_RECT_F& imageRect)
{
    if (m_pSkeletonFrame && !m_pauseSkeleton)
    {
        // Clip the area to avoid drawing outside the image
        m_pImageRenderer->SetClipRect(imageRect);

        for (int i = 0; i < NUI_SKELETON_COUNT; i++)
        {
            NUI_SKELETON_TRACKING_STATE state = m_pSkeletonFrame->SkeletonData[i].eTrackingState;
            if (NUI_SKELETON_TRACKED == state)
            {
                // Draw bones and joints of tracked skeleton
                DrawSkeleton(m_pSkeletonFrame->SkeletonData[i], imageRect);
            }
            else if (NUI_SKELETON_POSITION_ONLY == state)
            {
                DrawPosition(m_pSkeletonFrame->SkeletonData[i], imageRect);
            }
        }

        m_pImageRenderer->ResetClipRect();
    }
}

/// <summary>
/// Draw skeleton.
/// </summary>
/// <param name="skeletonData">Skeleton coordinates</param>
/// <param name="imageRect">The rect which the color or depth stream image is streched to fit</param>
void NuiStreamViewer::DrawSkeleton(const NUI_SKELETON_DATA& skeletonData, const D2D1_RECT_F& imageRect)
{
	if (timeFinished)
		exit(0);
		//DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_FOOT_RIGHT, &noAngle);


    // Torso
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_HEAD,               NUI_SKELETON_POSITION_SHOULDER_CENTER, &noAngle);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_SHOULDER_CENTER,    NUI_SKELETON_POSITION_SHOULDER_LEFT, &noAngle);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_SHOULDER_CENTER,    NUI_SKELETON_POSITION_SHOULDER_RIGHT, &noAngle);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_SHOULDER_CENTER,    NUI_SKELETON_POSITION_SPINE, &noAngle);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_SPINE,              NUI_SKELETON_POSITION_HIP_CENTER, &noAngle);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_HIP_CENTER,         NUI_SKELETON_POSITION_HIP_LEFT, &noAngle);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_HIP_CENTER,         NUI_SKELETON_POSITION_HIP_RIGHT, &noAngle);

    // Left arm
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_SHOULDER_LEFT,      NUI_SKELETON_POSITION_ELBOW_LEFT, &angle1);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_ELBOW_LEFT,         NUI_SKELETON_POSITION_WRIST_LEFT, &angle2);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_WRIST_LEFT,         NUI_SKELETON_POSITION_HAND_LEFT, &noAngle);

	BoneAngle(skeletonData, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT, 0, imageRect);
	BoneAngle(skeletonData, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT, 1, imageRect);

    // Right arm
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_SHOULDER_RIGHT,     NUI_SKELETON_POSITION_ELBOW_RIGHT, &angle3);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_ELBOW_RIGHT,        NUI_SKELETON_POSITION_WRIST_RIGHT, &angle4);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_WRIST_RIGHT,        NUI_SKELETON_POSITION_HAND_RIGHT, &noAngle);

	BoneAngle(skeletonData, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT, 2, imageRect);
	BoneAngle(skeletonData, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT, 3, imageRect);

    // Left leg
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_HIP_LEFT,           NUI_SKELETON_POSITION_KNEE_LEFT, &angle5);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_KNEE_LEFT,          NUI_SKELETON_POSITION_ANKLE_LEFT, &angle6);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_ANKLE_LEFT,         NUI_SKELETON_POSITION_FOOT_LEFT, &noAngle);

	BoneAngle(skeletonData, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT, 4, imageRect);
	BoneAngle(skeletonData, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT, 5, imageRect);

    // Right leg
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_HIP_RIGHT,          NUI_SKELETON_POSITION_KNEE_RIGHT, &angle7);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_KNEE_RIGHT,         NUI_SKELETON_POSITION_ANKLE_RIGHT, &angle8);
    DrawBone(skeletonData, imageRect, NUI_SKELETON_POSITION_ANKLE_RIGHT,        NUI_SKELETON_POSITION_FOOT_RIGHT, &noAngle);

	BoneAngle(skeletonData, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT, 6, imageRect);
	BoneAngle(skeletonData, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT, 7, imageRect);


    // Draw joints
    for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
    {
        DrawJoint(skeletonData, imageRect, (NUI_SKELETON_POSITION_INDEX)i);
    }
}

/// <summary>
/// Draw a circle to indicate a skeleton of which only position info is available
/// </summary>
/// <param name="skeletonData">Skeleton coordinates</param>
/// <param name="imageRect">The rect which the color or depth stream image is streched to fit</param>
void NuiStreamViewer::DrawPosition(const NUI_SKELETON_DATA& skeletonData, const D2D1_RECT_F& imageRect)
{
    D2D1_POINT_2F center = ToImageRect(skeletonData.Position, imageRect);
    m_pImageRenderer->DrawCircle(center, 5.0f, ImageRendererBrushGreen, 2.5f);
}

/// <summary>
/// Draw a bone between 2 tracked joint.
/// <summary>
/// <param name="skeletonData">Skeleton coordinates</param>
/// <param name="imageRect">The rect which the color or depth image is streched to fit</param>
/// <param name="joint0">Index for the first joint</param>
/// <param name="joint1">Index for the second joint</param>
void NuiStreamViewer::DrawBone(const NUI_SKELETON_DATA& skeletonData, const D2D1_RECT_F& imageRect, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1,float *angle)
{
    NUI_SKELETON_POSITION_TRACKING_STATE state0 = skeletonData.eSkeletonPositionTrackingState[joint0];
    NUI_SKELETON_POSITION_TRACKING_STATE state1 = skeletonData.eSkeletonPositionTrackingState[joint1];

	// -----------------------------------CALCULATING-ANGLES-------------------------

	float angleRad = (skeletonData.SkeletonPositions[joint0].y - skeletonData.SkeletonPositions[joint1].y) / (skeletonData.SkeletonPositions[joint0].x - skeletonData.SkeletonPositions[joint1].x);
	float angle_temp = atan(angleRad);
	*angle = angle_temp * 57.29564553;

	//-------------------------------------------------------------------------------

    // Any is not tracked
    if (NUI_SKELETON_POSITION_NOT_TRACKED == state0 || NUI_SKELETON_POSITION_NOT_TRACKED == state1)
    {
        return;
    }

    // Both are inferred
    if (NUI_SKELETON_POSITION_INFERRED == state0 && NUI_SKELETON_POSITION_INFERRED == state1)
    {
        return;
    }

    D2D1_POINT_2F point0 = ToImageRect(skeletonData.SkeletonPositions[joint0], imageRect);
    D2D1_POINT_2F point1 = ToImageRect(skeletonData.SkeletonPositions[joint1], imageRect);

    // We assume all drawn bones are inferred unless BOTH joints are tracked
    if (NUI_SKELETON_POSITION_TRACKED == state0 && NUI_SKELETON_POSITION_TRACKED == state1)
    {
        m_pImageRenderer->DrawLine(point0, point1, ImageRendererBrushBoneTracked, 4.0f);
    }
    else
    {
        m_pImageRenderer->DrawLine(point0, point1, ImageRendererBrushBoneInferred, 4.0f);
    }
}
void NuiStreamViewer::BoneAngle(const NUI_SKELETON_DATA &skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1, int bone, const D2D1_RECT_F& imageRect) {
	//Check if bone is vertical
	float angle1;
	float angle2;
	float angle;
	float angleRad;
	if (skel.SkeletonPositions[joint0].x - skel.SkeletonPositions[joint1].x == 0.0)	//IF THE BONE ANGLE IS VERTICAL
	{
		angle = 90.0;

	}
	else
	{
		angleRad = (skel.SkeletonPositions[joint0].y - skel.SkeletonPositions[joint1].y) / (skel.SkeletonPositions[joint0].x - skel.SkeletonPositions[joint1].x);
		angle1 = atan(angleRad);
		angle = angle1 * 57.29564553;
	}


	D2D1_POINT_2F point0 = ToImageRect(skel.SkeletonPositions[joint0], imageRect);
	D2D1_POINT_2F point1 = ToImageRect(skel.SkeletonPositions[joint1], imageRect);


	float tol;
	if (bone> 3)
		tol = 15.0;
	else
		tol = 10.0;
	float target = stof(v[bone]);
	if (angle<(target + tol) && angle>(target - tol))
	{
		m_matchAngles[bone] = 12.5;
		m_pImageRenderer->DrawLine(point0, point1, ImageRendererBrushWhite, 5.0f);
		bn[bone] = true;
	}
	else
	{
		m_matchAngles[bone] = 0;
		bn[bone] = false;
	}

	m_matchPercentage = m_matchAngles[0] + m_matchAngles[1] + m_matchAngles[2] + m_matchAngles[3] + m_matchAngles[4]
							+ m_matchAngles[5] + m_matchAngles[6] + m_matchAngles[7];
}
/// <summary>
/// Draw a joint of the skeleton
/// </summary>
/// <param name="skeletonData">Skeleton coordinates</param>
/// <param name="imageRect">The rect which the color or depth image is streched to fit</param>
/// <param name="joint">Index for the joint to be drawn</param>
void NuiStreamViewer::DrawJoint(const NUI_SKELETON_DATA& skeletonData, const D2D1_RECT_F& imageRect, NUI_SKELETON_POSITION_INDEX joint)
{
    NUI_SKELETON_POSITION_TRACKING_STATE state = skeletonData.eSkeletonPositionTrackingState[joint];

    // Not tracked
    if (NUI_SKELETON_POSITION_NOT_TRACKED == state)
    {
        return;
    }

    D2D1_POINT_2F point = ToImageRect(skeletonData.SkeletonPositions[joint], imageRect);

    if (NUI_SKELETON_POSITION_TRACKED == state)
    {
        m_pImageRenderer->DrawCircle(point, 3.0f, ImageRendererBrushJointTracked, 3.0f);
    }
    else
    {
        if (!IsOutOfImageRect(point, imageRect))
        {
            m_pImageRenderer->DrawCircle(point, 3.0f, ImageRendererBrushJointInferred, 3.0f);
        }

        UpdateDrawEdgeFlags(point, imageRect);
    }
}

/// <summary>
/// Draw frame FPS counter
/// </summary>
/// <param name="clientRect">Client area of viewer's window</param>
void NuiStreamViewer::DrawFPS(const RECT& clientRect)
{
    // Get rectangle position and size
    D2D1_RECT_F rect = D2D1::RectF((FLOAT)clientRect.right - 50.0f, 0.0f, (FLOAT)clientRect.right, (FLOAT)clientRect.top + 50.0f);

    // Fill rectangle
    m_pImageRenderer->FillRectangle(rect, ImageRendererBrushGray);

    // Draw a while circle
    D2D1_POINT_2F center = D2D1::Point2F((rect.right + rect.left) / 2.0f, (rect.bottom + rect.top) / 2.0f);
    m_pImageRenderer->DrawCircle(center, 23.0f, ImageRendererBrushWhite, 4.0f);
    
    // Draw FPS text
    WCHAR text[MaxStringChars];
    swprintf_s(text, sizeof(text) / sizeof(WCHAR) - 1, L"%d", m_fps);
    UINT cch = (UINT)wcsnlen_s(text, MaxStringChars);
    m_pImageRenderer->DrawText(text, cch, rect, ImageRendererBrushWhite, ImageRendererTextFormatFps);
}

/// <summary>
/// Draw image resolution text
/// </summary>
/// <param name="clientRect">Client area of viewer's window</param>
void NuiStreamViewer::DrawResolution(const RECT& clientRect)
{
    if (m_pImage)
    {
        WCHAR buffer[MaxStringChars];
        D2D1_RECT_F rect = D2D1::RectF((FLOAT)clientRect.left, (FLOAT)clientRect.top, (FLOAT)clientRect.right, 10.0f);
        swprintf_s(buffer, sizeof(buffer) / sizeof(WCHAR), L"Resolution: %dx%d", m_pImage->GetWidth(), m_pImage->GetHeight());
        m_pImageRenderer->DrawText(buffer, (UINT)wcsnlen_s(buffer, MaxStringChars), rect, ImageRendererBrushGreen, ImageRendererTextFormatResolution);
    }
}

/// <summary>
/// Draw red edge on image when skeleton is close to or out of the image edge
/// </summary>
/// <param name="imageRect">The rectangle of the image</param>
void NuiStreamViewer::DrawRedEdges(const D2D1_RECT_F& imageRect)
{
    D2D1_RECT_F   rect;
    D2D1_POINT_2F start;
    D2D1_POINT_2F end;

    FLOAT edgeWidth = (imageRect.right - imageRect.left) / 40.0f;

    if (m_drawEdgeFlags & DRAW_EDGE_FLAG_LEFT)
    {
        rect.left   = imageRect.left;
        rect.top    = imageRect.top;
        rect.right  = imageRect.left + edgeWidth;
        rect.bottom = imageRect.bottom;

        start.x = rect.left;
        start.y = (rect.top + rect.bottom) / 2.0f;

        end.x   = rect.left + edgeWidth;
        end.y   = start.y;

        m_pImageRenderer->DrawEdge(rect, start, end);
    }

    if (m_drawEdgeFlags & DRAW_EDGE_FLAG_RIGHT)
    {
        rect.left   = imageRect.right - edgeWidth;
        rect.right  = imageRect.right;
        rect.top    = imageRect.top;
        rect.bottom = imageRect.bottom;

        start.x = rect.right;
        start.y = (rect.top + rect.bottom) / 2.0f;

        end.x   = rect.left;
        end.y   = start.y;

        m_pImageRenderer->DrawEdge(rect, start, end);
    }

    if (m_drawEdgeFlags & DRAW_EDGE_FLAG_TOP)
    {
        rect.left   = imageRect.left;
        rect.top    = imageRect.top;
        rect.right  = imageRect.right;
        rect.bottom = imageRect.top + edgeWidth;

        start.x = (rect.left + rect.right) / 2.0f;
        start.y = rect.top;

        end.x   = start.x;
        end.y   = rect.top + edgeWidth;

        m_pImageRenderer->DrawEdge(rect, start, end);
    }

    if (m_drawEdgeFlags & DRAW_EDGE_FLAG_BOTTOM)
    {
        rect.left   = imageRect.left;
        rect.top    = imageRect.bottom - edgeWidth;
        rect.right  = imageRect.right;
        rect.bottom = imageRect.bottom;

        start.x = (rect.left + rect.right) / 2.0f;
        start.y = rect.bottom;

        end.x   = start.x;
        end.y   = rect.top;

        m_pImageRenderer->DrawEdge(rect, start, end);
    }

    m_drawEdgeFlags = 0;
}

/// <summary>
/// Set the buffer containing the image pixels.
/// </summary>
/// <param name="pImage">The pointer to image buffer object</param>
void NuiStreamViewer::SetImage(const NuiImageBuffer* pImage)
{
    m_pImage = pImage;
    if (m_pImage &&  m_pImage->GetBufferSize() && m_hWnd)
    {
        InvalidateRect(m_hWnd, nullptr, FALSE);

        UpdateFrameRate();
    }
}

/// <summary>
/// Attach skeleton data.
/// </summary>
/// <param name="pFrame">The pointer to skeleton frame</param>
void NuiStreamViewer::SetSkeleton(const NUI_SKELETON_FRAME* pFrame)
{
    if (!m_hWnd)
    {
        return;
    }

    m_pSkeletonFrame = pFrame;

    InvalidateRect(m_hWnd, nullptr, FALSE);
}

/// <summary>
/// Update frame rate
/// </summary>
void NuiStreamViewer::UpdateFrameRate()
{
    m_frameCount++;

    DWORD tickCount = GetTickCount();
    DWORD span      = tickCount - m_lastTick;
    if (span >= 1000)
    {
       // m_fps            = (UINT)((double)(m_frameCount - m_lastFrameCount) * 1000.0 / (double)span + 0.5);
		//m_fps = 123456;
		m_lastTick       = tickCount;
        m_lastFrameCount = m_frameCount;
    }

}

/// <summary>
/// Check which red edge should be drawn
/// </summary>
/// <param name="point">Coordinates of a tracked joint</param>
/// <param name="imageRect">Rectangle of the image</param>
void NuiStreamViewer::UpdateDrawEdgeFlags(const D2D1_POINT_2F& point, const D2D1_RECT_F& imageRect)
{
    FLOAT detectWidth = (imageRect.right - imageRect.left) / 40.0f;

    if (point.x < imageRect.left + detectWidth)
    {
        m_drawEdgeFlags |= DRAW_EDGE_FLAG_LEFT;
    }

    if (point.x > imageRect.right - detectWidth)
    {
        m_drawEdgeFlags |= DRAW_EDGE_FLAG_RIGHT;
    }

    if (point.y < imageRect.top + detectWidth)
    {
        m_drawEdgeFlags |= DRAW_EDGE_FLAG_TOP;
    }

    if (point.y > imageRect.bottom - detectWidth)
    {
        m_drawEdgeFlags |= DRAW_EDGE_FLAG_BOTTOM;
    }
}

/// <summary>
/// Decide if skeleton is out of image
/// </summary>
/// <param name="point">Coordinates of a tracked joint</param>
/// <param name="imageRect">Rectangle of the image</param>
bool NuiStreamViewer::IsOutOfImageRect(const D2D1_POINT_2F& point, const D2D1_RECT_F& imageRect)
{
    if (point.x < imageRect.left || point.x > imageRect.right)
    {
        return true;
    }
    
    if (point.y < imageRect.top || point.y > imageRect.bottom)
    {
        return true;
    }

    return false;
}

/// <summary>
/// Calculate the coordinates of the image to be displayed in client area.
/// </summary>
/// <param name="client">Client area of viewer's window</param>
D2D1_RECT_F NuiStreamViewer::GetImageRect(const RECT &client)
{
	if (bn[0] && bn[1])//&&bn[2]&&bn[3]&&bn[4]&&bn[5]&&bn[6]&&bn[7])
	{
		if (timerRunning)
		{
			m_fps = seconds - (int) t.elapsedTime();

			if (t.elapsedTime() >= seconds)
				timeFinished = true;
		}
		else
		{
			t.start();
			timerRunning = true;
		}
	}
	else
	{
		t.start();
		m_fps = 0;
		timerRunning = false;
	}

    D2D1_RECT_F imageRect = D2D1::RectF();
    if (m_pImage && m_pImage->GetBuffer())
    {
        float ratio  = static_cast<float>(m_pImage->GetWidth()) / static_cast<float>(m_pImage->GetHeight());
        float width  = static_cast<float>(client.right);
        float height = width / ratio;

        if (height > client.bottom)
        {
            height = static_cast<float>(client.bottom);
            width  = height * ratio;
        }

        imageRect.left   = (client.right  - width  + 1) / 2.0f;
        imageRect.top    = (client.bottom - height + 1) / 2.0f;
        imageRect.right  = imageRect.left + width;
        imageRect.bottom = imageRect.top  + height;
    }

    return imageRect;
}

/// <summary>
/// Map skeleton point to window coordinate in image rect.
/// </summary>
/// <param name="skeletonPoint">Skeleton point to map.</param>
/// <param name="imageRect">The rectangle of image</param>
/// <returns>Mapped coordinate in client area</returns>
D2D1_POINT_2F NuiStreamViewer::ToImageRect(const Vector4& skeletonPoint, const D2D1_RECT_F& imageRect)
{
    const NUI_IMAGE_RESOLUTION imageResolution = NUI_IMAGE_RESOLUTION_640x480;
    LONG x = 0, y = 0;
    USHORT depthValue = 0;
    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depthValue, imageResolution); // Returns coordinates in depth space

    if (NUI_IMAGE_TYPE_COLOR == m_imageType || NUI_IMAGE_TYPE_COLOR_INFRARED == m_imageType
        || NUI_IMAGE_TYPE_COLOR_RAW_BAYER == m_imageType || NUI_IMAGE_TYPE_COLOR_RAW_YUV == m_imageType
        || NUI_IMAGE_TYPE_COLOR_YUV == m_imageType)
    {
        LONG backupX = x, backupY = y;
        if (FAILED(NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(imageResolution, imageResolution, nullptr, x, y, depthValue, &x, &y)))
        {
            x = backupX;
            y = backupY;
        }
    }

    DWORD imageWidth, imageHeight;
    NuiImageResolutionToSize(imageResolution, imageWidth, imageHeight);

    FLOAT resultX, resultY;
    resultX = x * (imageRect.right  - imageRect.left + 1.0f) / imageWidth + imageRect.left;
    resultY = y * (imageRect.bottom - imageRect.top  + 1.0f) / imageHeight + imageRect.top;

    return D2D1::Point2F(resultX, resultY);
}

/// <summary>
/// Pause the skeleton
/// </summary>
/// <param name="pause">Pause or resume the skeleton</param>
void NuiStreamViewer::PauseSkeleton(bool pause)
{
    m_pauseSkeleton = pause;
}


void NuiStreamViewer::ProcessStream(NuiAccelerometerViewer* m_pAccelerometerViewer)
{
	// Get the reading
	Vector4 reading;
	float angletemp = 1.0f;
	//HRESULT hr = m_NuiSensor->NuiAccelerometerGetCurrentReading(&reading);

	if (m_pAccelerometerViewer)
	{
		// Set the reading to viewer
		m_pAccelerometerViewer->SetAccelerometerReadings(&angle1, &angle2, &angle3, &angle4,
														&angle5, &angle6, &angle7, &angle8,&m_matchPercentage);
	}
}

void NuiStreamViewer::SetStreamViewerAccel(NuiAccelerometerViewer* pViewer)
{
	m_pAccelerometerViewer = pViewer;
}