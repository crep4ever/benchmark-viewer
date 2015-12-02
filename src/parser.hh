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
#ifndef __PARSER_HH__
#define __PARSER_HH__

#include <QString>
#include <QList>

class CNode;

class CParser
{
private:

enum TOKEN
{
  TOKEN_DATE_TIME=0,
  TOKEN_LABEL=1,
  TOKEN_ACTION=2,
  TOKEN_DURATION=3,
  TOKEN_COMMENT=4,
  TOKEN_CATEGORY=5
};

public:
  CParser(const QString & p_fileName);
  ~CParser();

const QList<CNode*> & nodes() const;

private:
  bool parse(const QString & p_fileName);
  void computeTreeModel();
  int computeDepth() const;

private:
  QList<CNode*> m_nodes;
};

#endif // __NODE_HH__
