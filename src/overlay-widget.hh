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

#ifndef __OVERLAY_WIDGET_HH__
#define __OVERLAY_WIDGET_HH__

#include <QWidget>
#include <QColor>
#include <QSize>

class QLabel;
class QPaintEvent;

class COverlayWidget : public QWidget
{
  Q_OBJECT

public:
  /// Constructor.
  COverlayWidget(QWidget *parent = nullptr);

  /// Destructor.
  ~COverlayWidget() override;

  float opacity() const;

protected:
  QSize sizeHint() const override;
  void paintEvent(QPaintEvent* event) override;

public slots:
  void setText(const QString & p_msg);
  void setOpacity(const float p_opacity);

private:
  QLabel *m_label;
  float m_opacity;
};

#endif  // __OVERLAY_WIDGET_HH__
