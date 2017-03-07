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
#include "info-widget.hh"

#include <QBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QtGlobal>
#include <QScrollArea>
#include <QDebug>

#include "node.hh"
#include "node-info-widget.hh"
#include "tango-colors.hh"
#include "utils.hh"

CInfoWidget::CInfoWidget(QWidget *p_parent) : QWidget(p_parent)
  , m_node(0)
  , m_nodeInfo(0)
  , m_childrenInfo(0)
  , m_mainLayout(new QVBoxLayout)
{
  m_nodeInfo = new CNodeInfo;
  m_nodeInfo->setOpacity(1);
  m_nodeInfo->setDisplaySteps(true);
  m_nodeInfo->setVisible(false);

  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(m_nodeInfo);

  m_mainLayout->addWidget(scrollArea);

  QStringList header;
  header << tr("Child");
  header << tr("Calls");
  header << tr("Use");
  header << tr("Sum");

  m_childrenInfo = new QTableWidget;
  m_childrenInfo->setAlternatingRowColors(true);
  m_childrenInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_childrenInfo->setSelectionMode(QAbstractItemView::NoSelection);
  m_childrenInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_childrenInfo->setColumnCount(4);
  m_childrenInfo->setWordWrap(true);
  m_childrenInfo->setHorizontalHeaderLabels(header);
  //m_childrenInfo->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
  m_childrenInfo->verticalHeader()->setVisible(false);
  m_childrenInfo->setShowGrid(false);
  m_childrenInfo->setVisible(false);
  m_mainLayout->addWidget(m_childrenInfo, 1);

  setLayout(m_mainLayout);
}

CInfoWidget::~CInfoWidget()
{
}

QSize CInfoWidget::sizeHint() const
{
  return QSize(450, 600);
}

void CInfoWidget::setNode(CNode *p_node)
{
  m_node = p_node;
  m_nodeInfo->setNode(p_node);
  m_nodeInfo->setVisible(p_node);
  update();
}

void CInfoWidget::update()
{
  if (!m_node)
  {
    m_childrenInfo->setVisible(false);
    return;
  }

  // Children info
  m_childrenInfo->setVisible(!m_node->children().isEmpty());

  QHash<QString, int> counter;
  QHash<QString, int> duration;
  foreach (CNode* child, m_node->children())
  {
    ++counter[child->label()];
    duration[child->label()] += child->duration();
  }

  m_childrenInfo->setRowCount(counter.size());
  m_childrenInfo->setSortingEnabled(false);

  int row = 0;
  QHash<QString, int>::const_iterator it = counter.constBegin();
  while (it != counter.constEnd())
  {
    int col = 0;

    const QString childLabel = it.key();
    const int childCount = it.value();
    const int childPercentage = qRound(duration[childLabel] * 100 / (float) m_node->duration());

    QPixmap pixmap(12, 12);
    pixmap.fill(pickColor(qHash(childLabel)));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(pixmap), childLabel);
    m_childrenInfo->setItem(row, col++, item);

    item = new QTableWidgetItem(QString("x %1").arg(childCount));
    m_childrenInfo->setItem(row, col++, item);

    item = new QTableWidgetItem(QString("%1%").arg(childPercentage, 2, 10, QChar('0')));
    m_childrenInfo->setItem(row, col++, item);

    item = new QTableWidgetItem(mSecsToString(duration[childLabel]));
    m_childrenInfo->setItem(row, col++, item);

    ++row;
    ++it;
  }
  m_childrenInfo->setSortingEnabled(true);
}
