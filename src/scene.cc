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
#include <QDebug>

#include "tango-colors.hh"
#include "node.hh"
#include "graphics-node-item.hh"

CScene::CScene(const QList<CNode*> & p_nodes) : QGraphicsScene()
{
  QElapsedTimer timer;
  timer.start();

  qint64 origin = 0;
  if (!p_nodes.empty())
  {
    origin = p_nodes[0]->start().toMSecsSinceEpoch();
  }

  foreach (CNode* node, p_nodes)
  {
    const int w = (int) node->duration();
    const int h = 100;
    const int margin = 100;

    const int x = node->start().toMSecsSinceEpoch() - origin;
    const int y = node->level() * (margin + h);

    CGraphicsNodeItem *item = new CGraphicsNodeItem();
    item->setRect(x, y, w, h);
    item->setPos(x, y);
    item->setNode(node);
    item->setBrush(pickColor(node->level()));
    item->setToolTip(node->toString());
    item->setFlags(QGraphicsItem::ItemIsSelectable);

    QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(node->label());
    label->setParentItem(item);
    label->setPos(x, y);
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
