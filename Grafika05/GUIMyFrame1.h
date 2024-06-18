#ifndef __GUIMyFrame1__
#define __GUIMyFrame1__

#include "GUI.h"
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <iostream>

class GUIMyFrame1 : public MainFrame
{
public:
	GUIMyFrame1(wxWindow* parent);
	~GUIMyFrame1() {};

protected:
	using size = short unsigned int;

	size width = wxDefaultSize.GetWidth();
	size height = wxDefaultSize.GetHeight();

	wxSize p0size = wxSize(width, height);
	wxSize p1Size = wxSize(wxDefaultSize.GetWidth(), wxDefaultSize.GetHeight());

	size magX;
	size magY;

	size magW;
	size magH;

	wxRect magRect;

	double cropFactor = 1.;
	double zoomFactor = 1.;

	wxImage mainImage;
	wxImage subImage;

	std::array<wxImage, 5> images;

	wxBitmap Bitmap;

	bool clicked = false;
	bool loaded = false;

	void setPanelsOnLoad(wxString path = _("image.bmp"));
	void setPanelSize();
	void magImageCrop();

	void Mouse_Move(wxMouseEvent& event);
	void LMB_click(wxMouseEvent& event);

public:
	virtual void m_panel0OnPaint(wxPaintEvent& event) override;
	virtual void m_panel0OnUpdateUI(wxUpdateUIEvent& event) override;

	virtual void m_slider1OnScroll(wxScrollEvent& event) override;

	virtual void m_panel1OnPaint(wxPaintEvent& event) override;
	virtual void m_panel2OnPaint(wxPaintEvent& event) override;
	virtual void m_panel3OnPaint(wxPaintEvent& event) override;
	virtual void m_panel4OnPaint(wxPaintEvent& event) override;
	virtual void m_panel5OnPaint(wxPaintEvent& event) override;

	virtual void m_button3OnButtonClick(wxCommandEvent& event) override;
	virtual void m_button2OnButtonClick(wxCommandEvent& event) override;
};

#endif