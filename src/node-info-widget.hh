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

#include "overlay-widget.hh"
#include <QSize>

class CNode;

class CNodeInfo : public COverlayWidget
{
  Q_OBJECT

public:
  /// Constructor.
  CNodeInfo(QWidget *p_parent = nullptr);

  /// Destructor.
  ~CNodeInfo() override;

  CNode *node() const;

  bool displaySteps() const;
  void setDisplaySteps(const bool p_value);

protected:
  QSize sizeHint() const override;

public slots:
  void setNode(CNode *p_node);

private:
  void clear();
  void update();

private:
  CNode *m_node;
  bool m_displaySteps;
};

#endif  // __NODE_INFO_WIDGET_HH__
