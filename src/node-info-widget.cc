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

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>

#include "node.hh"
#include "utils.hh"

CNodeInfo::CNodeInfo(QWidget *p_parent) : COverlayWidget(p_parent)
  , m_node(0)
  , m_displayChildrenInfo(false)
{
  resize(sizeHint());
}

CNodeInfo::~CNodeInfo()
{
}

QSize CNodeInfo::sizeHint() const
{
  return QSize(250, 80);
}

void CNodeInfo::setNode(CNode *p_node)
{
  if (!p_node)
  {
    clearInfo();
    return;
  }

  m_node = p_node;
  updateInfo();
}

void CNodeInfo::updateInfo()
{
  Q_ASSERT(m_node);

  QString info = QString("<div style=\"float: left; border: 8px solid red\"></div><b>%1</b><br />").arg(m_node->label());

  info += QString("%1 -> %2 (%3)\n\n")
      .arg(m_node->start().time().toString())
      .arg(m_node->stop().time().toString())
      .arg(::mSecsToString(m_node->duration()));

  if (m_displayChildrenInfo)
  {
    info += QString("<hr>");

    QHash<QString, int> counter;
    foreach (CNode* child, m_node->children())
    {
      ++counter[child->label()];
    }

    QHash<QString, int>::const_iterator it = counter.constBegin();
    while (it != counter.constEnd())
    {
      const QString childLabel = it.key();
      const int childCount = it.value();

      info += QString("%1 (x %2)").arg(childLabel).arg(childCount);
      info += QString("<br />");
      ++it;
    }
  }

  setText(info);
}

void CNodeInfo::clearInfo()
{
  setText("");
  m_node = 0;
}


// void CNodeInfo::updateDiffInfo()
// {
//   Q_ASSERT(m_firstNode && m_secondNode);
//   if (m_firstNode->level() != m_secondNode->level())
//   {
//     return;
//   }
//
//   const qint64 firstNodeStart =  m_firstNode->startMs();
//   const qint64 firstNodeStop  =  m_firstNode->stopMs();
//
//   const qint64 secondNodeStart = m_secondNode->startMs();
//   const qint64 secondNodeStop  = m_secondNode->stopMs();
//
//   qint64 gap = 0;
//   if (secondNodeStart > firstNodeStop)
//   {
//     gap = secondNodeStart - firstNodeStop;
//   }
//   else
//   {
//     gap = firstNodeStart - secondNodeStop;
//   }
//
//   QString info = QString("<b>Gap:</b> %1<br />")
//     .arg(::mSecsToString(gap));
//
//   m_diffLabel->setText(info);
// }

bool CNodeInfo::displayChildrenInfo() const
{
  return m_displayChildrenInfo;
}

void CNodeInfo::setDisplayChildrenInfo(const bool p_value)
{
  m_displayChildrenInfo = p_value;
}
