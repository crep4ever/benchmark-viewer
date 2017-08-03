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
#include "utils.hh"

CTimeLineView::CTimeLineView(CScene *p_scene) : QGraphicsView()
, m_zoomInAct(nullptr)
, m_zoomOutAct(nullptr)
, m_currentSceneItem(nullptr)
, m_sessionInfo(new COverlayWidget(this))
, m_firstSelection(new CNodeInfo(this))
, m_secondSelection(new CNodeInfo(this))
, m_diffInfo(new COverlayWidget(this))
{
  QElapsedTimer timer;
  timer.start();

  setDragMode(QGraphicsView::ScrollHandDrag);
  setBackgroundRole(QPalette::Dark);
  createActions();

  setScene(p_scene);
  fitInView(p_scene->sceneRect());

  updateLabelsVisibility();

  m_firstSelection->setVisible(false);
  m_secondSelection->setVisible(false);
  m_diffInfo->setVisible(false);
  m_diffInfo->resize(QSize(250, 90));

  qDebug() << "Build timeline view in" << timer.elapsed() << "ms";
}

CTimeLineView::~CTimeLineView()
{
  delete m_sessionInfo;
  delete m_firstSelection;
  delete m_secondSelection;
}

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
    QGraphicsSimpleTextItem *textItem = dynamic_cast<QGraphicsSimpleTextItem*>(item);
    if (textItem && textItem->parentItem())
    {
      item = textItem->parentItem();
    }

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

    m_currentSceneItem = nullptr;
    m_firstSelection->setNode(nullptr);
    m_secondSelection->setNode(nullptr);
    m_firstSelection->setVisible(false);
    m_secondSelection->setVisible(false);
    emit currentNodeChanged(nullptr);
  }

  updateDiffInfo();

  QGraphicsView::mousePressEvent(p_event);
}

void CTimeLineView::resizeEvent(QResizeEvent *p_event)
{
  updateOverlaysPositions();

  QGraphicsView::resizeEvent(p_event);
}

void CTimeLineView::updateLabelsVisibility()
{
  const QList<QGraphicsItem*> & allItems = QGraphicsView::items();

  foreach (QGraphicsItem *item, allItems)
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
  int posx = 0;
  int posy = geometry().height() - m_firstSelection->geometry().height() - 15;
  m_firstSelection->move(posx, posy);

  posx = geometry().width()  - m_secondSelection->geometry().width();
  posy = geometry().height() - m_secondSelection->geometry().height() - 15;
  m_secondSelection->move(posx, posy);

  posx = geometry().width()  / 2 - m_diffInfo->geometry().width() / 2;
  posy = geometry().height() - m_diffInfo->geometry().height() - 15;
  m_diffInfo->move(posx, posy);
}

void CTimeLineView::updateDiffInfo()
{
  if (m_firstSelection->isVisible() && m_secondSelection->isVisible())
  {
    CNode *n1 = m_firstSelection->node();
    CNode *n2 = m_secondSelection->node();
    Q_ASSERT(n1 && n2);

    const qint64 start1 = n1->startMs();
    const qint64 stop1  = n1->stopMs();
    const qint64 start2 = n2->startMs();
    const qint64 stop2  = n2->stopMs();

    QString info =
            QString("START -> STOP  \t%1\n").arg(mSecsToString(stop2 - start1));
    info += QString("STOP  -> START \t%1\n").arg(mSecsToString(start2 - stop1));
    info += QString("START -> START \t%1\n").arg(mSecsToString(start2 - start1));
    info += QString("STOP  -> STOP  \t%1\n").arg(mSecsToString(stop2 - stop1));

    m_diffInfo->setText(info);
    m_diffInfo->setVisible(true);
  }
  else
  {
    m_diffInfo->setVisible(false);
  }
}
