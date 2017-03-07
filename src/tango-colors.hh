// Copyright (C) 2014, Romain Goffe <romain.goffe@gmail.com>
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

#ifndef __TANGO_COLORS_HH__
#define __TANGO_COLORS_HH__

#include <QColor>

// yellow
static QColor _TangoButter1(QColor(252, 233, 79));
static QColor _TangoButter2(QColor(237, 212, 0));
static QColor _TangoButter3(QColor(196, 160, 0));

// green
static QColor _TangoChameleon1(QColor(138, 226, 52));
static QColor _TangoChameleon2(QColor(115, 210, 22));
static QColor _TangoChameleon3(QColor(78, 154, 6));

// orange
static QColor _TangoOrange1(QColor(252, 175, 62));
static QColor _TangoOrange2(QColor(245, 121, 0));
static QColor _TangoOrange3(QColor(206, 92, 0));

// blue
static QColor _TangoSkyBlue1(QColor(114, 159, 207));
static QColor _TangoSkyBlue2(QColor(52, 101, 164));
static QColor _TangoSkyBlue3(QColor(32, 74, 135));

// purple
static QColor _TangoPlum1(QColor(173, 127, 168));
static QColor _TangoPlum2(QColor(117, 80, 123));
static QColor _TangoPlum3(QColor(92, 53, 102));

// brown
static QColor _TangoChocolate1(QColor(233, 185, 110));
static QColor _TangoChocolate2(QColor(193, 125, 17));
static QColor _TangoChocolate3(QColor(143, 89, 2));

// red
static QColor _TangoScarletRed1(QColor(239, 41, 41));
static QColor _TangoScarletRed2(QColor(204, 0, 0));
static QColor _TangoScarletRed3(QColor(164, 0, 0));

// grey
static QColor _TangoAluminium1(QColor(238, 238, 236));
static QColor _TangoAluminium2(QColor(211, 215, 207));
static QColor _TangoAluminium3(QColor(186, 189, 182));
static QColor _TangoAluminium4(QColor(136, 138, 133));
static QColor _TangoAluminium5(QColor(85, 87, 83));
static QColor _TangoAluminium6(QColor(46, 52, 54));

static inline QColor pickColor(uint i)
{
  const int id = i % 24;
  switch (id)
  {
    case 0:
    return _TangoSkyBlue1;
    case 1:
    return _TangoScarletRed1;
    case 2:
    return _TangoChameleon1;
    case 3:
    return _TangoAluminium2;
    case 4:
    return _TangoOrange1;
    case 5:
    return _TangoPlum1;
    case 6:
    return _TangoButter1;
    case 7:
    return _TangoChocolate1;

    case 8:
    return _TangoSkyBlue3;
    case 9:
    return _TangoScarletRed3;
    case 10:
    return _TangoChameleon3;
    case 11:
    return _TangoAluminium6;
    case 12:
    return _TangoOrange3;
    case 13:
    return _TangoPlum3;
    case 14:
    return _TangoButter3;
    case 15:
    return _TangoChocolate3;

    case 16:
    return _TangoSkyBlue2;
    case 17:
    return _TangoScarletRed2;
    case 18:
    return _TangoChameleon2;
    case 19:
    return _TangoAluminium4;
    case 20:
    return _TangoOrange2;
    case 21:
    return _TangoPlum2;
    case 22:
    return _TangoButter2;
    case 23:
    return _TangoChocolate2;
  }

  return QColor();
}

static inline bool isColorBright(const QColor & p_color)
{
  // yellow
  return (p_color == _TangoButter1 ||
          p_color == _TangoButter2 ||
          p_color == _TangoChameleon1 ||
          p_color == _TangoChameleon2 ||
          p_color == _TangoOrange1 ||
          p_color == _TangoOrange2 ||
          p_color == _TangoSkyBlue1 ||
          p_color == _TangoPlum1 ||
          p_color == _TangoChocolate1 ||
          p_color == _TangoScarletRed1 ||
          p_color == _TangoAluminium1 ||
          p_color == _TangoAluminium2 ||
          p_color == _TangoAluminium3);
}

static inline bool isColorDark(const QColor & p_color)
{
  return !isColorBright(p_color);
}

#endif  // __TANGO_COLORS_HH__
