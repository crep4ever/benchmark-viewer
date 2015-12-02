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
#ifndef __UTILS_HH__
#define __UTILS_HH__

#include <QString>

static inline QString mSecsToString(const qint64 p_msecs)
{
  qint64 msecs = p_msecs;
  QString sign;
  if (p_msecs < 0)
  {
    msecs = -p_msecs;
    sign = "-";
  }

  const int h  = msecs / (60  * 60 * 1000);
  const int mn = msecs / (60 * 1000) - h * 60;
  const int s  = msecs / 1000 - h * 60 * 60 - mn * 60;
  const int ms = msecs - h * 60 * 60 * 1000 - mn * 60 * 1000 - s * 1000;

  if (h > 0)
  {
    return QString("%1%2h %3mn").arg(sign).arg(h).arg(mn);
  }
  else if (mn > 0)
  {
    return QString("%1%2mn %3s").arg(sign).arg(mn).arg(s);
  }
  else if (s > 0)
  {
    return QString("%1%2.%3s").arg(sign).arg(s).arg(ms);
  }
  else
  {
    return QString("%1%2ms").arg(sign).arg(ms);
  }
}

#endif // __UTILS_HH__
