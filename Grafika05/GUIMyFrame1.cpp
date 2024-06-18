#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
	: MainFrame(parent)
{
	mainImage.InitStandardHandlers();
	subImage.InitStandardHandlers();
	if (!wxFileExists("image.bmp"))
	{
		mainImage = wxImage(250, 200);
		subImage = wxImage(50, 50);
		for (auto& x : images)
			x = wxImage(50, 50);
		this->SetMinSize(wxSize(250 + 240, 200 + 50 + 100));
	}
	else
	{
		mainImage.LoadFile("image.bmp");
		loaded = true;
	}
	setPanelsOnLoad();
	m_slider1->SetValue(0);
	m_panel0->Bind(wxEVT_MOTION, &GUIMyFrame1::Mouse_Move, this);
	m_panel0->Bind(wxEVT_LEFT_DOWN, &GUIMyFrame1::LMB_click, this);
}

void GUIMyFrame1::m_panel0OnPaint(wxPaintEvent& event)
{

	wxClientDC dc1(m_panel0);
	wxBufferedDC dc(&dc1);
	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();

	setPanelSize();

	if (Bitmap.IsOk())
		dc.DrawBitmap(Bitmap, 0, 0, false);

	dc.SetPen(wxPen(RGB(0, 0, 0)));

	dc.DrawLine(magX - magH / 2., magY - magW / 2., magX + magH / 2., magY - magW / 2.);
	dc.DrawLine(magX - magH / 2., magY - magW / 2., magX - magH / 2., magY + magW / 2.);
	dc.DrawLine(magX - magH / 2., magY + magW / 2., magX + magH / 2., magY + magW / 2.);
	dc.DrawLine(magX + magH / 2., magY - magW / 2., magX + magH / 2., magY + magW / 2.);
}

void GUIMyFrame1::m_panel0OnUpdateUI(wxUpdateUIEvent& event)
{
	setPanelSize();
}

void GUIMyFrame1::LMB_click(wxMouseEvent& event)
{
	if (clicked)
		clicked = false;
	else
		clicked = true;

	magImageCrop();
	Refresh();
}

void GUIMyFrame1::Mouse_Move(wxMouseEvent& event)
{
	size mX_temp = event.GetX();
	size mY_temp = event.GetY();

	if (clicked)
		return;

	if (mX_temp < width - magW / 2. && mX_temp > magW / 2.)
		magX = mX_temp;
	if (mY_temp < height - magH / 2. && mY_temp > magH / 2.)
		magY = mY_temp;

	magImageCrop();
	Refresh();
}

void GUIMyFrame1::m_slider1OnScroll(wxScrollEvent& event)
{
	cropFactor = 1. - event.GetPosition() / 200.;
	zoomFactor = -2 * cropFactor + 3.;
	magW = p1Size.GetWidth() * cropFactor;
	magH = p1Size.GetHeight() * cropFactor;

	if (magX > width - magW / 2.)
		magX -= 1;
	if (magY > height - magH / 2.)
		magY -= 1;
	if (magX < magW / 2.)
		magX += 1;
	if (magY < magH / 2.)
		magY += 1;

	magImageCrop();
	Refresh();
}

void GUIMyFrame1::setPanelsOnLoad(wxString path)
{
	magX = width / 2;
	magY = height / 2;

	height = mainImage.GetHeight();
	width = mainImage.GetWidth();

	p0size.Set(width, height);

	p1Size.Set(width / 4 - 20, width / 4 - 20);

	magW = p1Size.GetWidth() * cropFactor;
	magH = p1Size.GetHeight() * cropFactor;
	magX = width / 2;
	magY = height / 2;

	setPanelSize();

	Bitmap = wxBitmap(mainImage);
	if (loaded)
	{
		size a = 5 * magW + 240;
		size b = width + 240;
		this->SetMinSize(wxSize(a > b ? a : b, height * 1.59));
		this->SetSize(wxSize(a > b ? a : b, height * 1.59));
	}

	magImageCrop();
	Refresh();
}

void GUIMyFrame1::magImageCrop()
{

	magRect.SetX(magX - magW / 2.);
	magRect.SetY(magY - magH / 2.);
	magRect.SetWidth(magW);
	magRect.SetHeight(magH);

	size dif_x = magRect.GetX() + magW;
	size dif_y = magRect.GetY() + magH;

	if (dif_x > width)
	{
		magRect.SetX(magRect.GetX() - (dif_x - width));
	}
	if (dif_y > height)
	{
		magRect.SetY(magRect.GetY() - (dif_y - height));
	}

	if (magRect.GetX() < 0)
		magRect.SetX(0);

	if (magRect.GetY() < 0)
		magRect.SetY(0);

	subImage = mainImage.GetSubImage(magRect);
}

void GUIMyFrame1::setPanelSize()
{
	m_panel0->SetSize(p0size);
	m_panel1->SetSize(p1Size);
	m_panel2->SetSize(p1Size);
	m_panel3->SetSize(p1Size);
	m_panel4->SetSize(p1Size);
	m_panel5->SetSize(p1Size);
}

void GUIMyFrame1::m_button3OnButtonClick(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save BMP"), wxGetCwd(), "", "(*.bmp)|*.bmp", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wxFileOutputStream output_stream(saveFileDialog.GetPath());
	if (!output_stream.IsOk())
		return;
	wxImage image = images[0];
	image.Resize(wxSize(images[0].GetWidth() * 5, images[0].GetHeight()), wxPoint(0, 0));
	for (size i = 1; i < images.size(); i++)
	{
		image.Paste(images[i], images[i].GetWidth() * i, 0);
	}
	image.SaveFile(output_stream, wxBITMAP_TYPE_BMP);
}

void GUIMyFrame1::m_button2OnButtonClick(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open BMP"), wxGetCwd(), "", "(*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	wxFileInputStream input_stream(openFileDialog.GetPath());
	loaded = true;
	if (!input_stream.IsOk())
		return;
	mainImage.LoadFile(input_stream);
	setPanelsOnLoad(openFileDialog.GetPath());
}

wxImage hermit(const wxImage& image, double factor)
{
	wxImage interpolatedImage(image.GetWidth() * factor, image.GetHeight() * factor);

	if (image.IsOk() && interpolatedImage.IsOk() &&
		interpolatedImage.GetWidth() > 0 && interpolatedImage.GetHeight() > 0)
	{
		for (int y = 0; y < interpolatedImage.GetHeight(); y++)
		{
			for (int x = 0; x < interpolatedImage.GetWidth(); x++)
			{
				double srcX = x / factor;
				double srcY = y / factor;

				int x0 = static_cast<int>(srcX);
				int y0 = static_cast<int>(srcY);

				if (x0 >= 0 && x0 < image.GetWidth() - 1 && y0 >= 0 && y0 < image.GetHeight() - 1)
				{
					double dx = srcX - x0;
					double dy = srcY - y0;

					double red = image.GetRed(wxCoord(x0), wxCoord(y0)) * (1 - dx) * (1 - dy) +
						image.GetRed(wxCoord(x0 + 1), wxCoord(y0)) * dx * (1 - dy) +
						image.GetRed(wxCoord(x0), wxCoord(y0 + 1)) * (1 - dx) * dy +
						image.GetRed(wxCoord(x0 + 1), wxCoord(y0 + 1)) * dx * dy;

					double green = image.GetGreen(wxCoord(x0), wxCoord(y0)) * (1 - dx) * (1 - dy) +
						image.GetGreen(wxCoord(x0 + 1), wxCoord(y0)) * dx * (1 - dy) +
						image.GetGreen(wxCoord(x0), wxCoord(y0 + 1)) * (1 - dx) * dy +
						image.GetGreen(wxCoord(x0 + 1), wxCoord(y0 + 1)) * dx * dy;

					double blue = image.GetBlue(wxCoord(x0), wxCoord(y0)) * (1 - dx) * (1 - dy) +
						image.GetBlue(wxCoord(x0 + 1), wxCoord(y0)) * dx * (1 - dy) +
						image.GetBlue(wxCoord(x0), wxCoord(y0 + 1)) * (1 - dx) * dy +
						image.GetBlue(wxCoord(x0 + 1), wxCoord(y0 + 1)) * dx * dy;

					interpolatedImage.SetRGB(x, y, static_cast<unsigned char>(red),
						static_cast<unsigned char>(green),
						static_cast<unsigned char>(blue));
				}
			}
		}
	}
	return interpolatedImage;
}

wxImage lanchos(const wxImage& image, double scaleFactor)
{
	int width = image.GetWidth();
	int height = image.GetHeight();

	int newWidth = static_cast<int>(width * scaleFactor);
	int newHeight = static_cast<int>(height * scaleFactor);

	wxImage newImage(newWidth, newHeight);

	for (int y = 0; y < newHeight; ++y)
	{
		for (int x = 0; x < newWidth; ++x)
		{
			double srcX = static_cast<double>(x) / scaleFactor;
			double srcY = static_cast<double>(y) / scaleFactor;

			int x1 = static_cast<int>(srcX);
			int y1 = static_cast<int>(srcY);

			int x2 = x1 + 1;
			int y2 = y1 + 1;

			if (x2 >= width)
				x2 = width - 1;
			if (y2 >= height)
				y2 = height - 1;

			double tempX = srcX - x1;
			double tempY = srcY - y1;

			unsigned char r1 = image.GetRed(x1, y1);
			unsigned char g1 = image.GetGreen(x1, y1);
			unsigned char b1 = image.GetBlue(x1, y1);

			unsigned char r2 = image.GetRed(x2, y1);
			unsigned char g2 = image.GetGreen(x2, y1);
			unsigned char b2 = image.GetBlue(x2, y1);

			unsigned char r3 = image.GetRed(x1, y2);
			unsigned char g3 = image.GetGreen(x1, y2);
			unsigned char b3 = image.GetBlue(x1, y2);

			unsigned char r4 = image.GetRed(x2, y2);
			unsigned char g4 = image.GetGreen(x2, y2);
			unsigned char b4 = image.GetBlue(x2, y2);

			int r = static_cast<int>((r1 * (1 - tempX) * (1 - tempY) +
				r2 * tempX * (1 - tempY) +
				r3 * (1 - tempX) * tempY +
				r4 * tempX * tempY) +
				0.5);

			int g = static_cast<int>((g1 * (1 - tempX) * (1 - tempY) +
				g2 * tempX * (1 - tempY) +
				g3 * (1 - tempX) * tempY +
				g4 * tempX * tempY) +
				0.5);

			int b = static_cast<int>((b1 * (1 - tempX) * (1 - tempY) +
				b2 * tempX * (1 - tempY) +
				b3 * (1 - tempX) * tempY +
				b4 * tempX * tempY) +
				0.5);

			newImage.SetRGB(x, y, r, g, b);
		}
	}

	return newImage;
}

void GUIMyFrame1::m_panel1OnPaint(wxPaintEvent& event)
{

	wxClientDC dc1(m_panel1);
	wxBufferedDC dc(&dc1);
	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();
	wxImage copyImage = subImage.Copy();
	wxImage image2(copyImage.Scale(subImage.GetWidth() * zoomFactor, subImage.GetHeight() * zoomFactor, wxIMAGE_QUALITY_BILINEAR));
	wxBitmap bitmap = wxBitmap(image2);

	if (bitmap.IsOk())
		dc.DrawBitmap(bitmap, 0, 0, false);

	images[0] = image2;
}

void GUIMyFrame1::m_panel2OnPaint(wxPaintEvent& event)
{

	wxClientDC dc1(m_panel2);
	wxBufferedDC dc(&dc1);
	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();
	wxImage copyImage = subImage.Copy();
	wxImage image2(copyImage.Scale(subImage.GetWidth() * zoomFactor, subImage.GetHeight() * zoomFactor, wxIMAGE_QUALITY_BICUBIC));
	wxBitmap bitmap = wxBitmap(image2);

	if (bitmap.IsOk())
		dc.DrawBitmap(bitmap, 0, 0, false);

	images[1] = image2;
}

void GUIMyFrame1::m_panel3OnPaint(wxPaintEvent& event)
{

	wxClientDC dc1(m_panel3);
	wxBufferedDC dc(&dc1);
	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();
	wxImage copyImage = subImage.Copy();
	copyImage.Rescale(subImage.GetWidth() * zoomFactor, subImage.GetHeight() * zoomFactor);
	wxBitmap bitmap = wxBitmap(copyImage);

	if (bitmap.IsOk())
		dc.DrawBitmap(bitmap, 0, 0, false);

	images[2] = copyImage;
}

void GUIMyFrame1::m_panel4OnPaint(wxPaintEvent& event)
{
	wxClientDC dc1(m_panel4);
	wxBufferedDC dc(&dc1);
	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();
	wxImage copyImage = subImage.Copy();
	copyImage = lanchos(copyImage, zoomFactor);
	wxBitmap bitmap = wxBitmap(copyImage);

	if (bitmap.IsOk())
		dc.DrawBitmap(bitmap, 0, 0, false);

	images[3] = copyImage;
}

void GUIMyFrame1::m_panel5OnPaint(wxPaintEvent& event)
{
	wxClientDC dc1(m_panel5);
	wxBufferedDC dc(&dc1);
	dc.SetBackground(wxBrush(RGB(255, 255, 255)));
	dc.Clear();
	wxImage copyImage = subImage.Copy();
	copyImage = hermit(copyImage, zoomFactor);
	wxBitmap bitmap = wxBitmap(copyImage);

	if (bitmap.IsOk())
		dc.DrawBitmap(bitmap, 0, 0, false);

	images[4] = copyImage;
}