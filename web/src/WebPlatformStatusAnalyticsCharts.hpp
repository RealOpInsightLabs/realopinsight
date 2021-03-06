/*
 * WebBiDashlet.hpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2015 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Creation: 17-07-2015                                                     #
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

#ifndef WEBBIDASHLET_HPP
#define WEBBIDASHLET_HPP

#include "WebPlatformStatusAnalyticsData.hpp"
#include "WebPlatformStatusRaw.hpp"
#include "WebPieChart.hpp"
#include "WebPlatformStatusDateFilter.hpp"
#include "WebCsvReportResource.hpp"
#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WDatePicker.h>



class WebPlatformStatusAnalyticsCharts : public QObject, public Wt::WContainerWidget
{
  Q_OBJECT

public:
  WebPlatformStatusAnalyticsCharts(const DbViewsT& listOfViews, const SourceListT& listOfSources);
  ~WebPlatformStatusAnalyticsCharts();
  void updateByView (const std::string& vame, const PlatformMappedStatusHistoryT& statusHistory);
  long startTime(void) {return m_dateFilterRef->epochStartTime();}
  long endTime(void) {return m_dateFilterRef->epochEndTime();}
  Wt::Signal<long, long>& reportPeriodChanged() { return m_reportPeriodChanged; }


private:
  Wt::Signal<long, long> m_reportPeriodChanged;
  Wt::WVBoxLayout* m_layoutRef;
  WebPlatformStatusDateFilter* m_dateFilterRef;
  QMap<std::string, WebPieChart*> m_slaReportsRef;
  QMap<std::string, WebPlatformStatusRaw*> m_problemReportRef;
  QMap<std::string, WebCsvExportIcon*> m_csvLinksRef;
};

#endif // WEBBIDASHLET_HPP
