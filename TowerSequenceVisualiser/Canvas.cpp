#include "Canvas.h"
#include "Main.h"

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
	EVT_LEFT_DOWN(Canvas::OnLeftDown)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	mainPtr = (Main*)parent;
}

Canvas::~Canvas()
{

}

void Canvas::SetZ(double screenX, double screenY)
{
	z = screenX / w * 10 - 5 + ((h - screenY) / h * 10 - 5) * std::complex<double>(0, 1);
	//z = screenX / w * 10 - 5 + ((h - screenY) / h * 10 - 5) * 0i;
}

void Canvas::OnDraw(wxDC& dc)
{
	if (cacheW != w || cacheH != h)
	{
		wxImage background("C:\\Users\\matty\\Documents\\Maths\\PowerTower\\ConvPlotFull.bmp", wxBITMAP_TYPE_BMP);
		background.Rescale(w, h, wxIMAGE_QUALITY_BICUBIC);
		backBMP = wxBitmap(background);
		cacheW = w;
		cacheH = h;
	}
	
	//Drawing here
	dc.DrawBitmap(backBMP, 0, 0);

	wxBrush brush = dc.GetBrush();
	brush.SetColour(255, 255, 255);
	dc.SetBrush(brush);

	dc.DrawCircle((z.real() + 5) / 10 * w, h - (z.imag() + 5) / 10 * h, 5);

	wxPen pen = dc.GetPen();
	pen.SetColour(255, 0, 0);
	dc.SetPen(pen);
	brush.SetColour(255, 0, 0);
	dc.SetBrush(brush);

	std::vector<std::complex<double>> values;
	Eval(z, values, 10000, 10000);

	std::complex<double> z1, z2;

	for (int i = 0; i < values.size(); i++)
	{
		z1 = values[i];
		z2 = values[(i + 1) % values.size()];

		// Draw Line
		int screenX = (z1.real() + 5) / 10 * w;
		int screenY = h - ((z1.imag() + 5) / 10 * h);
		dc.DrawLine((z2.real() + 5) / 10 * w, h - (z2.imag() + 5) / 10 * h,
			screenX, screenY);

		dc.DrawCircle(screenX, screenY, 3);
	}

	wxString num;
	num << values.size() << '\n';
	dc.SetTextForeground(wxColour(255, 0, 0));
	dc.DrawText(num, 20, 20);
}

void Canvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	wxGCDC gcdc(dc);
	this->GetSize(&w, &h);
	this->PrepareDC(gcdc);
	this->OnDraw(gcdc);
	evt.Skip();
}

void Canvas::Resized(wxSizeEvent& evt)
{
	this->Refresh();
	evt.Skip();
}

void Canvas::OnLeftDown(wxMouseEvent& evt)
{
	SetZ(evt.GetX(), evt.GetY());
	Refresh();
}