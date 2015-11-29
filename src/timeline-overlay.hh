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

#ifndef __TIMELINE_OVERLAY_HH__
#define __TIMELINE_OVERLAY_HH__

#include <QWidget>
#include <QColor>
#include <QSize>

class QLabel;
class QPaintEvent;

class CTimeLineOverlay : public QWidget
{
  Q_OBJECT

public:
  /// Constructor.
  CTimeLineOverlay(QWidget *parent = 0);

  /// Destructor.
  virtual ~CTimeLineOverlay();

protected:
  virtual QSize sizeHint() const;
  void paintEvent(QPaintEvent* event);

public slots:
  void setText(const QString & p_msg);

private:
  QLabel *m_sessionInfoLabel;
};

#endif  // __TIMELINE_OVERLAY_HH__
