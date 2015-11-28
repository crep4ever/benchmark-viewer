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
class CScene;

class CTimeLineView : public QGraphicsView
{
  Q_OBJECT

public:
  CTimeLineView(CScene *p_scene);
  virtual ~CTimeLineView();
  virtual void wheelEvent(QWheelEvent *event);

protected:
  virtual void contextMenuEvent(QContextMenuEvent *event);

public slots:
void zoomIn();
void zoomOut();
void currentSceneItemChanged(QGraphicsItem * p_item);

private:
  // zoom actions
  void createActions();

  // hide labels if they don't fit in rect
  void updateLabelsVisibility();

private:
  QAction *m_zoomInAct;
  QAction *m_zoomOutAct;

  QGraphicsItem *m_currentSceneItem;
};

#endif // __TIMELINE_VIEW_HH__
