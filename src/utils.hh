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
#include <QDateTime>

namespace
{

  inline QString mSecsToString(const qint64 p_msecs)
  {
    const int ms = p_msecs;
    const int h  = p_msecs / (60  * 60 * 1000);
    const int mn = p_msecs / (60 * 1000) - h * 60;
    const int s  = p_msecs / 1000 - mn * 60;

    if (h > 1)
    {
      return QString("%1h %2mn").arg(h).arg(mn);
    }
    else if (mn > 1)
    {
      return QString("%1mn %2s").arg(mn).arg(s);
    }
    else if (s > 1)
    {
      return QString("%1s %2ms").arg(s).arg(ms);
    }
    else
    {
      return QString("%1ms").arg(ms);
    }
  }

}

#endif // __UTILS_HH__
