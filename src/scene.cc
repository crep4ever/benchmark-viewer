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
#include "scene.hh"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QAction>
#include <QElapsedTimer>
#include <QCryptographicHash>
#include <QDebug>

#include "tango-colors.hh"
#include "node.hh"
#include "graphics-node-item.hh"

CScene::CScene(const QList<CNode*> & p_nodes) : QGraphicsScene()
, m_nodes(p_nodes)
{
  QElapsedTimer timer;
  timer.start();

  const int h = 10;
  const int margin = 2;

  const qint64 origin = start().toMSecsSinceEpoch();
  const qint64 end    = stop().toMSecsSinceEpoch();

  setSceneRect(0, 0, end - origin, depth() * (h + margin) + h);

  foreach (CNode* node, m_nodes)
  {
    const int w = node->duration();
    const int x = node->start().toMSecsSinceEpoch() - origin;
    const int y = node->level() * (margin + h);

    CGraphicsNodeItem *item = new CGraphicsNodeItem();
    item->setPos(x, y);
    item->setRect(0, 0, w, h);
    item->setNode(node);
    item->setBrush(pickColor(qHash(node->label())));
    item->setPen(Qt::NoPen);
    item->setToolTip(node->toString());
    item->setFlags(QGraphicsItem::ItemIsSelectable);

    QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(node->label());
    label->setParentItem(item);
    label->setPos(0, 0); // coordinates relative to parent item
    label->setBrush(_TangoAluminium1);
    label->setFlags(QGraphicsItem::ItemIgnoresTransformations);

    connect(item, SIGNAL(leftClicked()), this, SLOT(nodeLeftClicked()));
    connect(item, SIGNAL(rightClicked()), this, SLOT(nodeRightClicked()));

    addItem(item);
  }

  qDebug() << "Build scene in" << timer.elapsed() << "ms";
}

CScene::~CScene(){}

void CScene::nodeLeftClicked()
{
  CGraphicsNodeItem *item = qobject_cast<CGraphicsNodeItem*>(sender());
  if (item)
  {
    //qDebug() << "left cliked" << item->node()->toString();
    emit currentItemChanged(item);
  }
}

void CScene::nodeRightClicked()
{
  CGraphicsNodeItem *item = qobject_cast<CGraphicsNodeItem*>(sender());
  if (item)
  {
    //qDebug() << "right cliked" << item->node()->toString();
    emit currentItemChanged(item);
  }
}


QDateTime CScene::start() const
{
  if (m_nodes.empty())
  {
    return QDateTime();
  }

  QDateTime first = m_nodes[0]->start();
  foreach (CNode *node, m_nodes)
  {
    if (node->level() == 0 && node->start() < first)
    {
      first = node->start();
    }
  }

  return first;
}

QDateTime CScene::stop() const
{
  if (m_nodes.empty())
  {
    return QDateTime();
  }

  QDateTime last = m_nodes[0]->stop();
  foreach (CNode *node, m_nodes)
  {
    if (node->level() == 0 && node->stop() > last)
    {
      last = node->stop();
    }
  }
  return last;
}

qint64 CScene::duration() const
{
  return stop().toMSecsSinceEpoch() - start().toMSecsSinceEpoch();
}

int CScene::depth() const
{
  int depth = 0;
  foreach (CNode *node, m_nodes)
  {
    if (node->level() > depth)
    {
      depth = node->level();
    }
  }
  return depth;
}
