// Copyright (C) 2016, Romain Goffe <romain.goffe@gmail.com>
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
#ifndef _BENCHMARK_H
#define _BENCHMARK_H

#include <string>

class Benchmark
{
public:

  static void initSession(const std::string & p_filePath);

  Benchmark(const std::string & p_label);
  ~Benchmark();

  void addStep(const std::string & p_comment);
  const std::string currentDateTime() const;

private:
  static FILE * _file;
  std::string m_label;
  static const std::string _separator;
  static const std::string _startLabel;
  static const std::string _stopLabel;
  static const std::string _stepLabel;
};

#endif // _BENCHMARK_H

