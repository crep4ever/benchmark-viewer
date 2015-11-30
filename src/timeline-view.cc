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
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QApplication>
#include <QDebug>

#include "scene.hh"
#include "node.hh"
#include "graphics-node-item.hh"
#include "overlay-widget.hh"
#include "node-info-widget.hh"

CTimeLineView::CTimeLineView(CScene *p_scene) : QGraphicsView()
, m_zoomInAct(0)
, m_zoomOutAct(0)
, m_currentSceneItem(0)
, m_sessionInfo(new COverlayWidget(this))
, m_firstSelection(new CNodeInfo(this))
, m_secondSelection(new CNodeInfo(this))
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

void CTimeLineView::zoomIn()
{
  scale(1.2, 1.0);
}

void CTimeLineView::zoomOut()
{
  scale(1 / 1.2, 1.0);
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

void CTimeLineView::mousePressEvent(QMouseEvent *p_event)
{
  if (QGraphicsItem *item = itemAt(p_event->pos()))
  {
    // restore style of previous item
    CGraphicsNodeItem *previous = dynamic_cast<CGraphicsNodeItem*>(m_currentSceneItem);
    if (previous)
    {
      previous->setOpacity(1.0);
    }

    // clicked item becomes current
    m_currentSceneItem = item;
    CGraphicsNodeItem *nodeItem = dynamic_cast<CGraphicsNodeItem*>(item);
    if (nodeItem)
    {
      nodeItem->setOpacity(0.8);
      if (Qt::ControlModifier == QApplication::keyboardModifiers())
      {
        m_secondSelection->setNode(nodeItem->node());
        m_secondSelection->setVisible(true);
      }
      else
      {
        m_firstSelection->setNode(nodeItem->node());
        emit currentNodeChanged(nodeItem->node());
        m_firstSelection->setVisible(true);
      }
    }
  }
  else
  {
    // restore style of previous item
    CGraphicsNodeItem *previous = dynamic_cast<CGraphicsNodeItem*>(m_currentSceneItem);
    if (previous)
    {
      previous->setOpacity(1.0);
    }

    m_currentSceneItem = 0;
    m_firstSelection->setNode(0);
    m_secondSelection->setNode(0);
    m_firstSelection->setVisible(false);
    m_secondSelection->setVisible(false);
  }

  QGraphicsView::mousePressEvent(p_event);
}

void CTimeLineView::resizeEvent(QResizeEvent *p_event)
{
  updateOverlaysPositions();

  QGraphicsView::resizeEvent(p_event);
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

COverlayWidget* CTimeLineView::sessionInfo() const
{
    return m_sessionInfo;
}

void CTimeLineView::updateOverlaysPositions()
{
  int x = 0;
  int y = geometry().height() - m_firstSelection->geometry().height() - 10;
  m_firstSelection->move(x, y);

  x = geometry().width()  - m_secondSelection->geometry().width();
  y = geometry().height() - m_secondSelection->geometry().height() - 10;
  m_secondSelection->move(x, y);
}
