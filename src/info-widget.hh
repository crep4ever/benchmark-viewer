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

#ifndef __INFO_WIDGET_HH__
#define __INFO_WIDGET_HH__

#include <QWidget>

class QBoxLayout;
class QTableWidget;
class CNode;
class CNodeInfo;

class CInfoWidget : public QWidget
{
  Q_OBJECT

public:
  /// Constructor.
  CInfoWidget(QWidget *p_parent = nullptr);

  /// Destructor.
  ~CInfoWidget() override;

  CNode *node() const;

protected:
  QSize sizeHint() const override;

public slots:
  void setNode(CNode *p_node);

private:
  void update();

private:
  CNode *m_node;
  CNodeInfo *m_nodeInfo;
  QTableWidget *m_childrenInfo;
};

#endif  // __INFO_WIDGET_HH__
