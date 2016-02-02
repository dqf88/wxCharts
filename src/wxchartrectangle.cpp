/*
	Copyright (c) 2016 Xavier Leclercq

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
	IN THE SOFTWARE.
*/

/*
	Part of this file were copied from the Chart.js project (http://chartjs.org/)
	and translated into C++.

	The files of the Chart.js project have the following copyright and license.

	Copyright (c) 2013-2016 Nick Downie
	Released under the MIT license
	https://github.com/nnnick/Chart.js/blob/master/LICENSE.md
*/

#include "wxchartrectangle.h"
#include <wx/pen.h>
#include <wx/brush.h>

wxChartRectangle::wxChartRectangle(wxDouble x,
								   wxDouble y,
								   const wxChartRectangleOptions &options)
	: m_x(x), m_y(y), m_options(options)
{
}

bool wxChartRectangle::HitTest(const wxPoint &point) const
{
	return false;
}

wxPoint2DDouble wxChartRectangle::GetTooltipPosition() const
{
	return wxPoint2DDouble(0, 0);
}

void wxChartRectangle::Draw(wxGraphicsContext &gc)
{
	wxGraphicsPath path = gc.CreatePath();
	path.AddRectangle(m_x, m_y, 150, 150);
	path.CloseSubpath();

	wxBrush brush(m_options.GetFillColor());
	gc.SetBrush(brush);
	gc.FillPath(path);

	wxPen pen(m_options.GetStrokeColor(), 2);
	gc.SetPen(pen);
	gc.StrokePath(path);
}
