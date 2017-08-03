// Copyright (C) 2015, Romain Goffe <romain.goffe@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//******************************************************************************
#include "node-info-widget.hh"

#include <QDebug>

#include "node.hh"
#include "utils.hh"

CNodeInfo::CNodeInfo(QWidget *p_parent) : COverlayWidget(p_parent)
  , m_node(nullptr)
  , m_displaySteps(false)
{
  resize(sizeHint());
}

CNodeInfo::~CNodeInfo()
= default;

QSize CNodeInfo::sizeHint() const
{
  return m_displaySteps ? QSize(250, 140) : QSize(250, 55);
}

CNode * CNodeInfo::node() const
{
  return m_node;
}

void CNodeInfo::setNode(CNode *p_node)
{
  if (!p_node)
  {
    clear();
    return;
  }

  m_node = p_node;
  update();
}

void CNodeInfo::update()
{
  Q_ASSERT(m_node);

  QString info = QString("<b>%1</b><br />").arg(m_node->label());

  info += QString("%1 -> %2 (%3)")
      .arg(m_node->start().time().toString())
      .arg(m_node->stop().time().toString())
      .arg(mSecsToString(m_node->duration()));

  if (m_displaySteps && !m_node->steps().isEmpty())
  {
    info += QString("<hr>");
    info += QString("<b>Steps</b><br />");

    foreach (CStep *step, m_node->steps())
    {
      info += QString("%1 (%2)<br />")
      .arg(step->label())
      .arg(mSecsToString(step->duration()));
    }
  }

  setText(info);
}

void CNodeInfo::clear()
{
  setText(QString());
  m_node = nullptr;
}

bool CNodeInfo::displaySteps() const
{
  return m_displaySteps;
}

void CNodeInfo::setDisplaySteps(const bool p_value)
{
  m_displaySteps = p_value;
}
