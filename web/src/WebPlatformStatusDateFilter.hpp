/*
 * WebBiDateFilter.hpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2015 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Creation: 26-07-2015                                                     #
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



#ifndef WEBBIDATEFILTER_HPP
#define WEBBIDATEFILTER_HPP

#include <QObject>
#include <Wt/WLabel.h>
#include <Wt/WDatePicker.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WAnchor.h>

class WebPlatformStatusDateFilter : public QObject, public  Wt::WContainerWidget
{
  Q_OBJECT

public:
  WebPlatformStatusDateFilter(void);
  ~WebPlatformStatusDateFilter();
  long epochStartTime(void){
    return Wt::WDateTime(m_startDatePickerRef->date()).toTime_t();
  }
  long epochEndTime(void) {
    return Wt::WDateTime(m_endDatePickerRef->date()).toTime_t() + 86399;
  }
  Wt::Signal<long, long>& reportPeriodChanged() {
    return m_reportPeriodChanged;
  }

private:
  Wt::Signal<long, long> m_reportPeriodChanged;
  Wt::WDatePicker* m_startDatePickerRef;
  Wt::WDatePicker* m_endDatePickerRef;
  void setupDatePicker(Wt::WDatePicker* datePicker, long defaultEpochTime);
};


#endif // WEBBIDATEFILTER_HPP
