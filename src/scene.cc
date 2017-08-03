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
#include <QtGlobal>
#include <QDebug>
#include <utility>

#include "tango-colors.hh"
#include "node.hh"
#include "graphics-node-item.hh"

CScene::CScene(QList<CNode*>  p_nodes) : QGraphicsScene()
, m_nodes(std::move(p_nodes))
{
  QElapsedTimer timer;
  timer.start();

  const int h = 10;
  const int margin = 2;

  const qint64 origin = first().toMSecsSinceEpoch();
  const qint64 end    = last().toMSecsSinceEpoch();

  setSceneRect(0, 0, end - origin, depth() * (h + margin) + h);

  foreach (CNode* node, m_nodes)
  {
    const int w = node->duration();
    const int x = node->startMs() - origin;
    const int y = node->level() * (margin + h);

    CGraphicsNodeItem *item = new CGraphicsNodeItem();
    item->setPos(x, y);
    item->setRect(0, 0, w, h);
    item->setNode(node);
    item->setBrush(node->color());
    item->setPen(Qt::NoPen);
    item->setToolTip(node->toString());
    item->setFlags(QGraphicsItem::ItemIsSelectable);

    QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(node->label());
    label->setParentItem(item);
    label->setPos(w * 0.02, h / 2 -1); // coordinates relative to parent item
    label->setBrush(isColorBright(node->color()) ? _TangoAluminium6 : _TangoAluminium1);
    label->setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemIgnoresParentOpacity);

    addItem(item);
  }

  qDebug() << "Build scene in" << timer.elapsed() << "ms";
}

CScene::~CScene()
{
  clear();
}

QDateTime CScene::first() const
{
  if (m_nodes.empty())
  {
    return QDateTime();
  }

  QDateTime date = m_nodes[0]->start();
  foreach (CNode *node, m_nodes)
  {
    if (node->start() < date)
    {
      date = node->start();
    }
  }

  return date;
}

QDateTime CScene::last() const
{
  if (m_nodes.empty())
  {
    return QDateTime();
  }

  QDateTime date = m_nodes[0]->stop();
  foreach (CNode *node, m_nodes)
  {
    if (node->stop() > date)
    {
      date = node->stop();
    }
  }
  return date;
}

qint64 CScene::duration() const
{
  return last().toMSecsSinceEpoch() - first().toMSecsSinceEpoch();
}

int CScene::depth() const
{
  int d = 0;
  foreach (CNode *node, m_nodes)
  {
    d = qMax(node->level(), d);
  }
  return d;
}
