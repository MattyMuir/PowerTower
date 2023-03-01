#pragma once
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

#include <iostream>
#include <complex>

#include "../eval.h"

using namespace std::complex_literals;

class Main;

class Canvas : public wxPanel
{
public:
	Canvas(wxWindow* parent);
	~Canvas();
	Main* mainPtr;

protected:
	int w, h;
	wxBitmap backBMP;
	int cacheW = 0;
	int cacheH = 0;

	std::complex<double> z{ 1, 0 };

	void SetZ(double screenX, double screenY);

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);
	void OnLeftDown(wxMouseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};