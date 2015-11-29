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

#ifndef __NODE_INFO_WIDGET_HH__
#define __NODE_INFO_WIDGET_HH__

#include <QWidget>

class QBoxLayout;
class QLabel;
class CNode;

class CNodeInfo : public QWidget
{
  Q_OBJECT

public:
  /// Constructor.
  CNodeInfo(QWidget *parent = 0);

  /// Destructor.
  virtual ~CNodeInfo();

public slots:
  void setNode(CNode *p_node);

private:
  void clearInfo();
  void updateInfo(CNode *p_node, QLabel *p_label);
  void updateDiffInfo();

private:
  CNode *m_firstNode;
  CNode *m_secondNode;
  QLabel *m_firstLabel;
  QLabel *m_secondLabel;
  QLabel *m_diffLabel;
  QBoxLayout *m_layout;
};

#endif  // __NODE_INFO_WIDGET_HH__
