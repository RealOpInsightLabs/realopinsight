/*
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2014 Rodrigue Chakode (rodrigue.chakode@gmail.com)    #
# Last Change: 17-12-2017                                                  #
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

#ifndef SETTINGSHANDLER_HPP
#define SETTINGSHANDLER_HPP
#include "Base.hpp"
#include "utilsCore.hpp"
#include <QString>
#include <QSettings>
#include <ctime>


class SettingFactory : public QSettings
{
  Q_OBJECT
public:
  static const QString GLOBAL_LANGUAGE_KEY;
  static const QString GLOBAL_SRC_BUCKET_KEY;
  static const QString GLOBAL_DB_STATE_KEY;
  static const QString GLOBAL_GRAPH_LAYOUT;
  static const QString GLOBAL_UPDATE_INTERVAL_KEY;

  static const QString DB_TYPE;
  static const QString DB_SERVER_ADDR;
  static const QString DB_SERVER_PORT;
  static const QString DB_NAME;
  static const QString DB_USER;
  static const QString DB_PASSWORD;

  static const QString AUTH_ADM_UNSERNAME_KEY;
  static const QString AUTH_OP_UNSERNAME_KEY;
  static const QString AUTH_ADM_PASSWD_KEY;
  static const QString AUTH_OP_PASSWD_KEY;
  static const QString AUTH_MODE_KEY;
  static const QString AUTH_LDAP_SERVER_URI;
  static const QString AUTH_LDAP_BIND_USER_DN;
  static const QString AUTH_LDAP_BIND_USER_PASSWORD;
  static const QString AUTH_LDAP_SEARCH_BASE;
  static const QString AUTH_LDAP_VERSION;
  static const QString AUTH_LDAP_ID_FIELD;
  static const QString AUTH_LDAP_SSL_USE_CERT;
  static const QString AUTH_LDAP_SSL_CERT_FILE;
  static const QString AUTH_LDAP_SSL_CA_FILE;

  static const QString NOTIF_TYPE;
  static const QString NOTIF_MAIL_SMTP_SERVER_ADRR;
  static const QString NOTIF_MAIL_SMTP_SERVER_PORT;
  static const QString NOTIF_MAIL_SMTP_USE_SSL;
  static const QString NOTIF_MAIL_SMTP_USERNAME;
  static const QString NOTIF_MAIL_SMTP_PASSWORD;

  static const QString DASHBOARD_THUMBNAILS_PER_ROW;

  SettingFactory();

  SettingFactory(const QString& path);

  virtual ~SettingFactory(void);

  void init(void);

  void setKeyValue(const QString & _key, const QString & _value);

  QString keyValue(const QString & _key) {return QSettings::value(_key).toString();}

  qint32 updateInterval() const;

  void setEntry(const QString& key, const QString& value);

  QString entry(const QString& key) const {return QSettings::value(key).toString();}

  bool loadSource(qint32 _idx, SourceT& _src);

  bool loadSource(const QString& _id, SourceT& _src);

  bool setSource(const QString& _info, SourceT& _src);

  void emitTimerIntervalChanged(qint32 _interval) {Q_EMIT timerIntervalChanged(_interval);}

  static QString language(void);

  int getGraphLayout(void) const;


Q_SIGNALS:
  void timerIntervalChanged(qint32);
};


#endif // SETTINGSHANDLER_HPP
