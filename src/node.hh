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
#ifndef __NODE_HH__
#define __NODE_HH__

#include <QString>
#include <QDateTime>
#include <QList>

class CNode
{
public:
  CNode();
  ~CNode();

const QDateTime & start() const;
void setStart(const QDateTime & p_dateTime);

const QDateTime & stop() const;
void setStop(const QDateTime & p_dateTime);

const QList<QDateTime> & steps() const;
void setSteps(const QList<QDateTime> & p_steps);
void addStep(const QDateTime & p_dateTime);


const QString & label() const;
void setLabel(const QString & p_label);

const QString & category() const;
void setCategory(const QString & p_category);

int level() const;
void setLevel(const int p_level);

bool isValid() const;

qint64 duration() const;

QString toString() const;

private:

  static QString mSecsToString(const qint64 p_msecs);

  QDateTime m_start;
  QDateTime m_stop;
  QList<QDateTime> m_steps;

  QString m_label;
  QString m_category;
  int m_level;
};

#endif // __NODE_HH__
