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
#include "overlay-widget.hh"

#include <QBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QGraphicsView>
#include <QPainter>

COverlayWidget::COverlayWidget(QWidget *p_parent) : QWidget(p_parent)
  , m_label(new QLabel)
  , m_opacity(0.6)
{
  setStyleSheet("background: transparent;");
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint);

  m_label->setTextInteractionFlags(Qt::TextSelectableByMouse);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(m_label);
  mainLayout->addStretch();
  setLayout(mainLayout);

  resize(sizeHint());
}

COverlayWidget::~COverlayWidget()
{
}

QSize COverlayWidget::sizeHint() const
{
  return QSize(600, 70);
}

void COverlayWidget::paintEvent(QPaintEvent* /*event*/)
{
  QColor backgroundColor = palette().dark().color();
  backgroundColor.setAlpha((1 - m_opacity) * 255);
  QPainter customPainter(this);
  customPainter.fillRect(rect(), backgroundColor);
}

void COverlayWidget::setText(const QString & p_text)
{
  m_label->setText(p_text);
}

float COverlayWidget::opacity() const
{
  return m_opacity;
}

void COverlayWidget::setOpacity(const float p_opacity)
{
  m_opacity = p_opacity;
}
