/*
 * ngrt4n.cpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2012 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 24-05-2012                                                 #
#                                                                          #
# This file is part of NGRT4N (http://ngrt4n.com).                         #
#                                                                          #
# NGRT4N is free software: you can redistribute it and/or modify           #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# NGRT4N is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with NGRT4N.  If not, see <http://www.gnu.org/licenses/>.          #
#--------------------------------------------------------------------------#
 */

#ifndef MONITORBROKER_HPP_
#define MONITORBROKER_HPP_
#include<string>
#include<iostream>
#include <unordered_map>

using namespace std ;

class MonitorBroker {
public:
    enum StatusT {
	  OK = 0,
	  WARNING = 1,
	  CRITICAL = 2,
	  UNKNOWN = 3,
	  UNSET_STATUS = 4
    };

    enum SeverityT {
      UNSET = 0,
      INFO = 1,
      WARN = 2,
      AVERAGE = 3,
      HIGH = 4,
      DISASTER = 5
    };

    enum MonirorTypeT {
      NAGIOS = 0,
      ZABBIX = 1
    } ;

    typedef struct _CheckT{
		string id;
		string host ;
		string check_command ;
		string last_state_change ;
		string alarm_msg ;
		int status ;
    }CheckT;
    typedef CheckT NagiosCheckT;
	typedef unordered_map<string, NagiosCheckT> NagiosChecksT ;

	MonitorBroker(const string & _sfile);
	virtual ~MonitorBroker();

	string getInfOfService(const string & _sid) ;
	static bool loadNagiosCollectedData(const string & _sfile, NagiosChecksT & _checks) ;

	static const int DEFAULT_PORT ;
	static const int DEFAULT_UPDATE_INTERVAL ;
	static const int MAX_MSG ;

private:
	int lastUpdate ;
	string statusFile ;
	NagiosChecksT services ;
};

#endif /* MONITORBROKER_HPP_ */
