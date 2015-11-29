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
#include <QDebug>

#include "node.hh"
#include "utils.hh"

CNodeInfo::CNodeInfo(QWidget *p_parent)
  : QWidget(p_parent)
  , m_firstNode(0)
  , m_secondNode(0)
  , m_firstLabel(new QLabel)
  , m_secondLabel(new QLabel)
  , m_diffLabel(new QLabel)
  , m_layout(new QVBoxLayout)
{
  m_layout->addWidget(m_firstLabel);
  m_layout->addWidget(m_secondLabel);
  m_layout->addWidget(m_diffLabel);
  m_layout->addStretch();
  setLayout(m_layout);
}

CNodeInfo::~CNodeInfo()
{
}

void CNodeInfo::setNode(CNode *p_node)
{
  if (!p_node)
  {
    clearInfo();
    return;
  }

  if (m_firstNode && (p_node == m_firstNode || p_node == m_secondNode))
  {
    return;
  }

  if (!m_firstNode && !m_secondNode)
  {
    m_firstNode = p_node;
    updateInfo(m_firstNode, m_firstLabel);
  }
  else if (m_firstNode && !m_secondNode)
  {
    m_secondNode = p_node;
    updateInfo(m_secondNode, m_secondLabel);
  }
  else if (m_firstNode && m_secondNode)
  {
    m_firstNode = m_secondNode;
    updateInfo(m_firstNode, m_firstLabel);

    m_secondNode = p_node;
    updateInfo(m_secondNode, m_secondLabel);
  }

  if (m_firstNode && m_secondNode)
  {
    updateDiffInfo();
  }
}

void CNodeInfo::updateInfo(CNode *p_node, QLabel *p_label)
{
  QString info = QString("<b>%1</b><br />").arg(p_node->label());

  if (p_node->parent())
  {
    info += QString("<b>Parent:</b> %1<br />").arg(p_node->parent()->label());
  }

  info += QString("%1 -> %2 (%3)\n\n")
      .arg(p_node->start().time().toString())
      .arg(p_node->stop().time().toString())
      .arg(::mSecsToString(p_node->duration()));

  p_label->setText(info);
}

void CNodeInfo::updateDiffInfo()
{
  Q_ASSERT(m_firstNode && m_secondNode);
  if (m_firstNode->level() != m_secondNode->level())
  {
    return;
  }

  const qint64 firstNodeStart =  m_firstNode->start().toMSecsSinceEpoch();
  const qint64 firstNodeStop  =  m_firstNode->stop().toMSecsSinceEpoch();

  const qint64 secondNodeStart = m_secondNode->start().toMSecsSinceEpoch();
  const qint64 secondNodeStop  = m_secondNode->stop().toMSecsSinceEpoch();

  qint64 gap = 0;
  if (secondNodeStart > firstNodeStop)
  {
    gap = secondNodeStart - firstNodeStop;
  }
  else
  {
    gap = firstNodeStart - secondNodeStop;
  }

  QString info = QString("<b>Gap:</b> %1<br />")
    .arg(::mSecsToString(gap));

  m_diffLabel->setText(info);
}

void CNodeInfo::clearInfo()
{
  m_firstLabel->setText("");
  m_secondLabel->setText("");
  m_diffLabel->setText("");
  m_firstNode = 0;
  m_secondNode = 0;
}
