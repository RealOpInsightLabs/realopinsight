/*
 * WebPieChart.hpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2014 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 23-03-2014                                                 #
#                                                                          #
# This file is part of RealOpInsight (http://RealOpInsight.com) authored   #
# by Rodrigue Chakode <rodrigue.chakode@gmail.com>                         #
#                                                                          #
# RealOpInsight is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# The Software is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with RealOpInsight.  If not, see <http://www.gnu.org/licenses/>.   #
#--------------------------------------------------------------------------#
 */

#ifndef WEBPIECHART_HPP
#define WEBPIECHART_HPP

#include "ChartBase.hpp"
#include "WebUtils.hpp"
#include <Wt/WStandardItemModel.h>
#include <Wt/Chart/WPieChart.h>
#include <Wt/WColor.h>
#include <Wt/Chart/WChartPalette.h>
#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WPen.h>
#include  <Wt/WStandardItemModel.h>


class WebChartPalette : public Wt::Chart::WChartPalette
{
public:
  WebChartPalette(void) { }
  virtual Wt::WBrush brush (int index) const { return Wt::WBrush(ngrt4n::severityWColor(index));}
  virtual Wt::WPen borderPen (int index) const { return Wt::WPen(Wt::WColor(255, 255, 255, 0)); }
  Wt::WPen strokePen (int) const { /* TODO: check value first */ return Wt::WPen(Wt::WColor(255, 255, 255, 1)); }
  Wt::WColor fontColor (int index) const { /* TOTO: check value first */ return Wt::WColor(255, 255, 255, 0);}
  virtual Wt::WColor color (int index) const { return Wt::WColor(255, 255, 255, 0); }

};


class WebPieChart : public ChartBase, public Wt::Chart::WPieChart
{
public:
  WebPieChart(int dataType);
  WebPieChart(void);
  virtual ~WebPieChart();
  void repaint();
  void setDataType(int dataType) {ChartBase::setDataType(dataType);}
  std::string defaultTooltipText(void);

private:
  Wt::WStandardItemModel* m_modelRef;

  void setupChartPalette(void);
  void setupChartStyle(void);
  void setupPieChartModel(void);
};

#endif // WEBPIECHART_HPP
