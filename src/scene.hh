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
#ifndef __SCENE_HH__
#define __SCENE_HH__

#include <QGraphicsScene>
#include <QList>

class CNode;

class CScene : public QGraphicsScene
{
  Q_OBJECT

public:
  CScene(const QList<CNode*> & p_nodes);
  virtual ~CScene();

private slots:
void nodeLeftClicked();
void nodeRightClicked();

signals:
void currentItemChanged(QGraphicsItem *item);

};

#endif // __SCENE_HH__
