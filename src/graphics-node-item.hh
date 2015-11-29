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
#ifndef __GRAPHICS_NODE_ITEM_HH__
#define __GRAPHICS_NODE_ITEM_HH__

#include <QObject>
#include <QGraphicsRectItem>
class QGraphicsSceneMouseEvent;


class CNode;
class CGraphicsNodeItem : public QGraphicsRectItem
{
public:
  CGraphicsNodeItem();
  virtual ~CGraphicsNodeItem();

  CNode* node() const;
  void setNode(CNode* p_node);

private:
  CNode *m_node;
};

#endif // __GRAPHICS_NODE_ITEM_HH__
