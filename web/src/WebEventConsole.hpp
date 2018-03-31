/*
 * WebMsgConsole.hpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2014 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 19-09-2014                                                 #
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

#ifndef WEBEVENTCONSOLE_HPP
#define WEBEVENTCONSOLE_HPP
#include <Wt/WTableView>
#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WSortFilterProxyModel>
#include <boost/any.hpp>
#include "Base.hpp"

class SortingProxyModel : public Wt::WSortFilterProxyModel {
public:
  SortingProxyModel(Wt::WObject* _parent=0)
    : Wt::WSortFilterProxyModel(_parent) { }
  virtual ~SortingProxyModel() {}

protected:
  virtual bool lessThan( const Wt::WModelIndex&lhs, const Wt::WModelIndex& rhs) const {
    if(lhs.data(Wt::UserRole).empty() || rhs.data(Wt::UserRole).empty())
      return lhs < rhs;
    return atol(boost::any_cast<std::string>(lhs.data(Wt::UserRole)).c_str())< atol(boost::any_cast<std::string>(rhs.data(Wt::UserRole)).c_str());
  }
};

class WebMsgConsole : public Wt::WTableView
{
public:
  WebMsgConsole();
  virtual ~WebMsgConsole();
  void clearAll(void) {m_model->clear(); setModelHeaders();}


  Wt::WStandardItemModel* getRenderingModel(void) const {return m_model;}
  void updateNodeMsg(const NodeT& _node);
  Wt::WStandardItem* createItem(const Wt::WString& text, int row);
  Wt::WStandardItem* createDateTimeItem(const std::string& _lastcheck, int row);


protected:
  void layoutSizeChanged (int width, int height);

private:
  Wt::WStandardItemModel* m_model;

  int findServiceRow(const std::string& _id);
  void setModel(void);
  void setModelHeaders(void);
};

#endif /* WEBEVENTCONSOLE_HPP */
