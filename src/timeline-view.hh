// Copyright (C) 2015, Romain Goffe <romain.goffe@gmail.com>
// This program is free software; you can redistribute it and/or
//
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
#ifndef __TIMELINE_VIEW_HH__
#define __TIMELINE_VIEW_HH__

#include <QGraphicsView>
#include <QList>

class QAction;
class QWheelEvent;
class QMouseEvent;
class QResizeEvent;
class CScene;
class COverlayWidget;
class CNode;
class CNodeInfo;

class CTimeLineView : public QGraphicsView
{
  Q_OBJECT

public:
  CTimeLineView(CScene *p_scene);
  virtual ~CTimeLineView();

  COverlayWidget *sessionInfo() const;

protected:
  virtual void wheelEvent(QWheelEvent *p_event);
  virtual void mousePressEvent(QMouseEvent *p_event);
  virtual void resizeEvent(QResizeEvent *p_event);

public slots:
void zoomIn();
void zoomOut();

signals:
void currentNodeChanged(CNode * p_node);

private:
  // zoom actions
  void createActions();

  // hide labels if they don't fit in rect
  void updateLabelsVisibility();

  // reposition overlays
  void updateOverlaysPositions();

  void updateDiffInfo();

private:
  QAction *m_zoomInAct;
  QAction *m_zoomOutAct;

  QGraphicsItem *m_currentSceneItem;
  COverlayWidget *m_sessionInfo;
  CNodeInfo *m_firstSelection;
  CNodeInfo *m_secondSelection;
  COverlayWidget *m_diffInfo;
};

#endif // __TIMELINE_VIEW_HH__
