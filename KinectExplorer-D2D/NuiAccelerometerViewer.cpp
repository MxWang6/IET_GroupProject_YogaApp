//------------------------------------------------------------------------------
// <copyright file="NuiAccelerometerViewer.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <limits>
#include "NuiAccelerometerViewer.h"
#include "resource.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="pParent">The pointer to parent window</param>
NuiAccelerometerViewer::NuiAccelerometerViewer(const NuiViewer* pParent)  : NuiViewer(pParent)  , m_accelerationX(0) , m_accelerationY(0) , m_accelerationZ(0){
}

/// <summary>
/// Destructor
/// </summary>
NuiAccelerometerViewer::~NuiAccelerometerViewer()
{
}

/// <sumamry>
/// Process message or send it to coreponding handler
/// </summary>
/// <param name="hWnd">The handle to the window which receives the message</param>
/// <param name="uMsg">Message identifier</param>
/// <param name="wParam">Additional message information</param>
/// <param name="lParam">Additional message information</param>
/// <returns>Result of message process. Depends on message type</returns>
LRESULT NuiAccelerometerViewer::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Pass the message to default dialog procedure
    return FALSE;
}

/// <summary>
/// Returns the id of the dialog
/// </summary>
UINT NuiAccelerometerViewer::GetDlgId()
{
    return IDD_ACCEL_VIEW;
}

/// <summary>
/// Set accelerometer readings to display
/// </summary>
/// <param name="x">Acceleration component on x-axis</param>
/// <param name="y">Acceleration component on y-axis</param>
/// <param name="z">Acceleration component on z-axis</param>
void NuiAccelerometerViewer::SetAccelerometerReadings(float *angle1, float *angle2, float *angle3, float *angle4,
													  float *angle5, float *angle6, float *angle7, float *angle8,float *match)
{
	float noAngle = -10;
	CompareUpdateValue(*angle1, noAngle, m_hWnd, IDC_TESTANGLE, L"%.2f");
	CompareUpdateValue(*angle2, noAngle, m_hWnd, IDC_ANGLE2, L"%.2f");
	CompareUpdateValue(*angle3, noAngle, m_hWnd, IDC_ANGLE3, L"%.2f");
	CompareUpdateValue(*angle4, noAngle, m_hWnd, IDC_ANGLE4, L"%.2f");
	CompareUpdateValue(*angle5, noAngle, m_hWnd, IDC_ANGLE5, L"%.2f");
	CompareUpdateValue(*angle6, noAngle, m_hWnd, IDC_ANGLE6, L"%.2f");
	CompareUpdateValue(*angle7, noAngle, m_hWnd, IDC_ANGLE7, L"%.2f");
	CompareUpdateValue(*angle8, noAngle, m_hWnd, IDC_ANGLE8, L"%.2f");
	CompareUpdateValue(*match, noAngle, m_hWnd, IDC_ACCEL_Y_READING, L"%.2f");
}