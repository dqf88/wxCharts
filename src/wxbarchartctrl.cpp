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

#include "wxbarchartctrl.h"
#include <sstream>

wxBarChartCtrl::Bar::Bar(wxDouble value,
						 const wxChartTooltipProvider::ptr tooltipProvider,
						 wxDouble x,
						 wxDouble y,
						 const wxColor &fillColor,
						 const wxColor &strokeColor,
						 int directions)
	: wxChartRectangle(x, y, tooltipProvider, wxChartRectangleOptions(fillColor, strokeColor, directions)),
	m_value(value)
{
}

wxDouble wxBarChartCtrl::Bar::GetValue() const
{
	return m_value;
}

wxBarChartCtrl::Dataset::Dataset()
{
}

const wxVector<wxBarChartCtrl::Bar::ptr>& wxBarChartCtrl::Dataset::GetBars() const
{
	return m_bars;
}

void wxBarChartCtrl::Dataset::AppendBar(Bar::ptr bar)
{
	m_bars.push_back(bar);
}

wxBarChartCtrl::wxBarChartCtrl(wxWindow *parent,
							   wxWindowID id,
							   const wxBarChartData &data,
							   const wxPoint &pos,
							   const wxSize &size,
							   long style)
	: wxChartCtrl(parent, id, pos, size, style),
	m_grid(
		wxPoint2DDouble(m_options.GetPadding().GetLeft(), m_options.GetPadding().GetRight()), 
		size, data.GetLabels(), GetMinValue(data.GetDatasets()),
		GetMaxValue(data.GetDatasets()), m_options.GetGridOptions()
		),
    m_needsFit(true)
{
	const wxVector<wxBarChartDataset::ptr>& datasets = data.GetDatasets();
	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxBarChartDataset& dataset = *datasets[i];
		Dataset::ptr newDataset(new Dataset());

		const wxVector<wxDouble>& datasetData = dataset.GetData();
		for (size_t j = 0; j < datasetData.size(); ++j)
		{
			std::stringstream tooltip;
			tooltip << datasetData[j];
			wxChartTooltipProvider::ptr tooltipProvider(
				new wxChartTooltipProviderStatic(data.GetLabels()[j], tooltip.str(), dataset.GetFillColor())
				);

			newDataset->AppendBar(Bar::ptr(new Bar(
				datasetData[j], tooltipProvider, 25, 50, dataset.GetFillColor(),
				dataset.GetStrokeColor(), wxTOP | wxRIGHT | wxBOTTOM
				)));
		}

		m_datasets.push_back(newDataset);
	}
}

wxBarChartCtrl::wxBarChartCtrl(wxWindow *parent, 
							   wxWindowID id,
							   const wxBarChartData &data,
							   const wxBarChartOptions &options, 
							   const wxPoint &pos,
							   const wxSize &size, 
							   long style)
	: wxChartCtrl(parent, id, pos, size, style), m_options(options),
	m_grid(
		wxPoint2DDouble(m_options.GetPadding().GetLeft(), m_options.GetPadding().GetRight()),
		size, data.GetLabels(), GetMinValue(data.GetDatasets()),
		GetMaxValue(data.GetDatasets()), m_options.GetGridOptions()
		),
    m_needsFit(true)
{
	const wxVector<wxBarChartDataset::ptr>& datasets = data.GetDatasets();
	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxBarChartDataset& dataset = *datasets[i];
		Dataset::ptr newDataset(new Dataset());

		const wxVector<wxDouble>& datasetData = dataset.GetData();
		for (size_t j = 0; j < datasetData.size(); ++j)
		{
			std::stringstream tooltip;
			tooltip << datasetData[j];
			wxChartTooltipProvider::ptr tooltipProvider(
				new wxChartTooltipProviderStatic(data.GetLabels()[j], tooltip.str(), dataset.GetFillColor())
				);

			newDataset->AppendBar(Bar::ptr(new Bar(
				datasetData[j], tooltipProvider, 25, 50, dataset.GetFillColor(),
				dataset.GetStrokeColor(), wxTOP | wxRIGHT | wxBOTTOM
				)));
		}

		m_datasets.push_back(newDataset);
	}
}

const wxBarChartOptions& wxBarChartCtrl::GetOptions() const
{
	return m_options;
}

wxDouble wxBarChartCtrl::GetMinValue(const wxVector<wxBarChartDataset::ptr>& datasets)
{
	wxDouble result = 0;
	bool foundValue = false;

	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxVector<wxDouble>& values = datasets[i]->GetData();
		for (size_t j = 0; j < values.size(); ++j)
		{
			if (!foundValue)
			{
				result = values[j];
				foundValue = true;
			}
			else if (result > values[j])
			{
				result = values[j];
			}
		}
	}

	return result;
}

wxDouble wxBarChartCtrl::GetMaxValue(const wxVector<wxBarChartDataset::ptr>& datasets)
{
	wxDouble result = 0;
	bool foundValue = false;

	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxVector<wxDouble>& values = datasets[i]->GetData();
		for (size_t j = 0; j < values.size(); ++j)
		{
			if (!foundValue)
			{
				result = values[j];
				foundValue = true;
			}
			else if (result < values[j])
			{
				result = values[j];
			}
		}
	}

	return result;
}

void wxBarChartCtrl::Fit()
{
    if (!m_needsFit)
    {
        return;
    }

    wxDouble barHeight = GetBarHeight();

    for (size_t i = 0; i < m_datasets.size(); ++i)
    {
        Dataset& currentDataset = *m_datasets[i];
        for (size_t j = 0; j < currentDataset.GetBars().size(); ++j)
        {
            Bar& bar = *(currentDataset.GetBars()[j]);

            wxPoint2DDouble upperLeftCornerPosition = m_grid.GetMapping().GetXAxis().GetTickMarkPosition(j + 1);
            upperLeftCornerPosition.m_y += m_options.GetBarSpacing() + (i * (barHeight + m_options.GetDatasetSpacing()));

            wxPoint2DDouble bottomLeftCornerPosition = upperLeftCornerPosition;
            bottomLeftCornerPosition.m_y += barHeight;

            wxPoint2DDouble upperRightCornerPosition = m_grid.GetMapping().GetWindowPositionAtTickMark(j + 1, bar.GetValue());

            bar.SetPosition(upperLeftCornerPosition);
            bar.SetSize(upperRightCornerPosition.m_x - upperLeftCornerPosition.m_x,
                bottomLeftCornerPosition.m_y - upperLeftCornerPosition.m_y);
        }
    }

    m_needsFit = false;
}

void wxBarChartCtrl::DoDraw(wxGraphicsContext &gc)
{
    m_grid.Draw(gc);

    Fit();

    for (size_t i = 0; i < m_datasets.size(); ++i)
    {
        Dataset& currentDataset = *m_datasets[i];
        for (size_t j = 0; j < currentDataset.GetBars().size(); ++j)
        {
            currentDataset.GetBars()[j]->Draw(gc);
        }
    }
}

void wxBarChartCtrl::Resize(const wxSize &size)
{
	wxSize newSize(
		size.GetWidth() - m_options.GetPadding().GetTotalHorizontalPadding(),
		size.GetHeight() - m_options.GetPadding().GetTotalVerticalPadding()
		);
	m_grid.Resize(newSize);
    m_needsFit = true;
}

wxSharedPtr<wxVector<const wxChartElement*> > wxBarChartCtrl::GetActiveElements(const wxPoint &point)
{
	wxSharedPtr<wxVector<const wxChartElement*> > activeElements(new wxVector<const wxChartElement*>());

	for (size_t i = 0; i < m_datasets.size(); ++i)
	{
		const wxVector<Bar::ptr>& bars = m_datasets[i]->GetBars();
		for (size_t j = 0; j < bars.size(); ++j)
		{
			if (bars[j]->HitTest(point))
			{
				for (size_t k = 0; k < m_datasets.size(); ++k)
				{
					activeElements->push_back(m_datasets[k]->GetBars()[j].get());
				}
			}
		}
	}

	return activeElements;
}

wxDouble wxBarChartCtrl::GetBarHeight() const
{
	wxDouble availableHeight = m_grid.GetMapping().GetXAxis().GetDistanceBetweenTickMarks() -
		(2 * m_options.GetBarSpacing()) - ((m_datasets.size() - 1) * m_options.GetDatasetSpacing());
	return (availableHeight / m_datasets.size());
}
