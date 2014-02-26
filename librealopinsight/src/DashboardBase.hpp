/*
 * SvNavigator.hpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2012 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 24-05-2012                                                 #
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

#ifndef SVNAVIGATOR_HPP
#define SVNAVIGATOR_HPP

#include <QString>
#include "Base.hpp"
#include "Parser.hpp"
#include "Preferences.hpp"
#include "ZmqSocket.hpp"
#include "ZbxHelper.hpp"
#include "ZnsHelper.hpp"

class QScriptValueIterator;
class QSystemTrayIcon;

class LIBREALOPINSIGHTSHARED_EXPORT DashboardBase : public QObject
{
  Q_OBJECT

public:
  DashboardBase(Preferences* preferences, const QString& _config);
  virtual ~DashboardBase();

  static StringMapT propRules();
  static StringMapT calcRules();
  void initSettings(void);
  qint64 updateCounter(void) const {return m_updateCounter;}
  QString config(void) const {return m_config;}
  void setSelectedNode(const QString& nodeid) {m_selectedNode = nodeid;}
  QString selectedNode(void) const {return m_selectedNode;}
  void setTimerId(qint32 id) {m_timerId = id;}
  qint32 timerId(void) const {return m_timerId;}
  qint32 timerInterval(void) const {return m_interval;}
  NodeT& rootNode(void) const {return *(m_cdata->root);}
  qint32 userRole(void) const { return m_userRole;}
  bool errorState() const {return m_errorState;}
  QString lastError(void) const {return m_lastError;}

public Q_SLOTS:
  void runMonitor();
  void runMonitor(SourceT& src);
  void runNgrt4ndUpdate(int srcId);
  void runNgrt4ndUpdate(const SourceT& src);
  void runLivestatusUpdate(int srcId);
  void runLivestatusUpdate(const SourceT& src);
  void resetStatData(void);
  void prepareUpdate(const SourceT& src);
  void updateBpNode(const QString& _node);
  void processZbxReply(QNetworkReply* reply, SourceT& src);
  void processZnsReply(QNetworkReply* reply, SourceT& src);
  void processRpcError(QNetworkReply::NetworkError code, const SourceT& src);
  bool allocSourceHandler(SourceT& src);
  void handleSourceSettingsChanged(QList<qint8> ids);
  void handleErrorOccurred(QString msg) {Q_EMIT errorOccurred(msg);}

Q_SIGNALS:
  void hasToBeUpdate(QString);
  void sortEventConsole(void);
  void updateStatusBar(const QString& msg);
  void settingsLoaded(void);
  void updateSourceUrl(void);
  void timerIntervalChanged(qint32 interval);
  void errorOccurred(QString msg);

protected:
  qint64 m_updateCounter;
  CoreDataT* m_cdata;
  QString m_config;
  QString m_selectedNode;
  qint32 m_userRole;
  qint32 m_interval;
  qint32 m_timerId;
  Settings* m_settings;
  QSize m_msgConsoleSize;
  bool m_showOnlyTroubles;
  SourceListT m_sources;
  NodeListIteratorT m_root;
  int m_firstSrcIndex;
  bool m_errorState;
  QString m_lastError;

protected:
  void load(const QString& _file);
  void computeStatusInfo(NodeT& _node, const SourceT& src);
  int extractSourceIndex(const QString& sid) {return sid.at(6).digitValue();}
  virtual void updateDashboard(const NodeT& _node);
  virtual void buildMap(void) = 0;
  virtual void updateMap(const NodeT& _node, const QString& _tip) = 0;
  virtual void buildTree(void) = 0;
  virtual void updateTree(const NodeT& _node, const QString& _tip) = 0;
  virtual void updateMsgConsole(const NodeT& _node) = 0;
  virtual void finalizeUpdate(const SourceT& src);
  virtual void updateChart(void) = 0;
  virtual void updateEventFeeds(const NodeT& node) = 0;

private:
  Preferences* m_preferences;

  void resetInterval(void);
  void updateCNodes(const CheckT & check, const SourceT& src);
  QStringList getAuthInfo(int srcId);
  QStringList getAuthInfo(const QString& authString);
  void openRpcSessions(void);
  void openRpcSession(int srcId);
  void openRpcSession(SourceT& src);
  void requestZbxZnsData(SourceT& src);
  void computeFirstSrcIndex(void);
  void updateDashboardOnError(const SourceT& src, const QString& msg);
  QString getNodeToolTip(const NodeT& _node);
};

#endif /* SVNAVIGATOR_HPP */
