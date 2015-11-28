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
#include "timeline-view.hh"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QAction>
#include <QWheelEvent>
#include <QElapsedTimer>
#include <QMenu>
#include <QGridLayout>
#include <QDebug>

#include "graphics-node-item.hh"
#include "scene.hh"

CTimeLineView::CTimeLineView(CScene *p_scene) : QGraphicsView()
, m_zoomInAct(0)
, m_zoomOutAct(0)
, m_currentSceneItem(0)
{
  QElapsedTimer timer;
  timer.start();

  setDragMode(QGraphicsView::ScrollHandDrag);
  setBackgroundRole(QPalette::Dark);
  createActions();

  setScene(p_scene);
  fitInView(p_scene->sceneRect());

  updateLabelsVisibility();

  qDebug() << "Build timeline view in" << timer.elapsed() << "ms";
}

CTimeLineView::~CTimeLineView(){}

void CTimeLineView::createActions()
{
  m_zoomInAct = new QAction(tr("Zoom &In"), this);
  m_zoomInAct->setIcon(QIcon::fromTheme("zoom-in"));
  m_zoomInAct->setShortcut(QKeySequence::ZoomIn);
  m_zoomInAct->setStatusTip(tr("Zoom in"));
  connect(m_zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

  m_zoomOutAct = new QAction(tr("Zoom &Out"), this);
  m_zoomOutAct->setIcon(QIcon::fromTheme("zoom-out"));
  m_zoomOutAct->setShortcut(QKeySequence::ZoomOut);
  m_zoomOutAct->setStatusTip(tr("Zoom out"));
  connect(m_zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void CTimeLineView::wheelEvent(QWheelEvent *p_event)
{
  if (p_event->delta() > 0)
  {
    zoomIn();
  }
  else
  {
    zoomOut();
  }

  // center zoom on mouse position
  centerOn(mapToScene(p_event->pos()));

  // but ensure selection remains visible
  if (m_currentSceneItem)
  {
    ensureVisible(m_currentSceneItem, 20, 20);
  }

  updateLabelsVisibility();

  QGraphicsView::wheelEvent(p_event);
}

void CTimeLineView::zoomIn()
{
  scale(1.2, 1.0);
}

void CTimeLineView::zoomOut()
{
  scale(1 / 1.2, 1.0);
}

void CTimeLineView::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu *menu = new QMenu;

  menu->addAction(m_zoomInAct);
  menu->addAction(m_zoomOutAct);

  menu->exec(event->globalPos());
  delete menu;
}

void CTimeLineView::updateLabelsVisibility()
{
  const QList<QGraphicsItem*> & items = QGraphicsView::items();

  foreach (QGraphicsItem *item, items)
  {
    QGraphicsSimpleTextItem *t = dynamic_cast<QGraphicsSimpleTextItem*>(item);
    if (t && t->parentItem())
    {
      const QRectF textRect = t->boundingRect();
      const QRectF nodeRect = t->parentItem()->boundingRect();
      const QRectF viewRect = mapFromScene(nodeRect).boundingRect();

      const bool visibility = (textRect.width() < viewRect.width());
      t->setVisible(visibility);
    }
  }
}

void CTimeLineView::currentSceneItemChanged(QGraphicsItem *p_item)
{
  m_currentSceneItem = p_item;
}
