/////////////////////////////////////////////////////////////////////////////
// Name:    dataset.cpp
// Purpose: dataset implementation
// Author:    Moskvichev Andrey V.
// Created:    2008/11/07
// Copyright:    (c) 2008-2010 Moskvichev Andrey V.
// Licence:    wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/dataset.h>
#include <wx/dataset1.h>

//
// Dataset
//

IMPLEMENT_CLASS(Dataset, wxObject)

wxDEFINE_EVENT(EVT_DATASET_CHANGED, wxCommandEvent);

Dataset::Dataset()
{
    m_renderer = NULL;
    m_updating = false;
    m_changed = false;
}

Dataset::~Dataset()
{
    for (size_t n = 0; n < m_markers.Count(); n++) 
    {
        Marker *marker = m_markers[n];
        wxDELETE(marker);
    }

    SAFE_UNREF(m_renderer);
}

void Dataset::SetRenderer(Renderer *renderer)
{
    SAFE_REPLACE_UNREF(m_renderer, renderer);
    DatasetChanged();
}

Renderer *Dataset::GetBaseRenderer()
{
    return m_renderer;
}

void Dataset::NeedRedraw(DrawObject *WXUNUSED(obj))
{
    DatasetChanged();
}

void Dataset::BeginUpdate()
{
    m_updating = true;
}

void Dataset::EndUpdate()
{
    if (m_updating) {
        m_updating = false;
        if (m_changed) 
        {
            DatasetChanged();
        }
    }
}

void Dataset::DatasetChanged()
{
    if (m_updating)
        m_changed = true;

    else 
    {
        wxQueueEvent(this, new wxCommandEvent(EVT_DATASET_CHANGED));
        m_changed = false;
    }
}

void Dataset::AddMarker(Marker *marker)
{
    m_markers.Add(marker);
}

size_t Dataset::GetMarkersCount()
{
    return m_markers.Count();
}

Marker *Dataset::GetMarker(size_t index)
{
    return m_markers[index];
}


