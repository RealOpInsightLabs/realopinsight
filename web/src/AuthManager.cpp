/*
# AuthManager.cpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2014 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update: 23-03-2014                                                  #
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

#include "dbo/src/DbSession.hpp"
#include "WebUtils.hpp"
#include "WebMainUI.hpp"
#include "AuthManager.hpp"
#include "AuthModelProxy.hpp"
#include <Wt/Auth/Login>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/AbstractUserDatabase>
#include <Wt/WLineEdit>
#include <functional>
#include <Wt/Auth/AuthModel>
#include <Wt/WPushButton>
#include <Wt/WImage>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <ctime>

AuthManager::AuthManager(DbSession* dbSession)
  : Wt::Auth::AuthWidget(dbSession->loginObject()),
    m_isActivatedLicense(false),
    m_dbSession(dbSession),
    m_mainUI(nullptr)
{
  AuthModelProxy* authModelProxy = new AuthModelProxy(m_dbSession->auth(), m_dbSession->users());
  authModelProxy->loginFailed().connect(this, &AuthManager::handleLoginFailed);
  authModelProxy->setVisible(Wt::Auth::AuthModel::RememberMeField, false);
  authModelProxy->addPasswordAuth(m_dbSession->passwordAuthentificator());
  setModel(authModelProxy);
  setRegistrationEnabled(false);
  m_dbSession->loginObject().changed().connect(this, &AuthManager::handleAuthentication);
}


void AuthManager::handleAuthentication(void)
{
  if (m_dbSession->isLogged()) {
    m_dbSession->setLoggedUser();
    DboLoginSession sessionInfo;
    sessionInfo.username = m_dbSession->loggedUser().username;
    sessionInfo.sessionId = wApp->sessionId();
    sessionInfo.firstAccess
        = sessionInfo.lastAccess
          = Wt::WDateTime::currentDateTime().toString().toUTF8();
    sessionInfo.status = DboLoginSession::ActiveCookie;

    m_dbSession->addSession(sessionInfo);
    wApp->setCookie(sessionInfo.username, sessionInfo.sessionId, 3600, "", "", false);

    QString logMsg = QObject::tr("%1 logged in. Session Id: %2")
                     .arg(m_dbSession->loggedUser().username.c_str(), sessionInfo.sessionId.c_str());
    CORE_LOG("info",logMsg.toStdString());
  } else {
    wApp->removeCookie(m_dbSession->loggedUser().username, "", "");
    CORE_LOG("info", QObject::tr("%1 logged out").arg(m_dbSession->loggedUser().username.c_str()).toStdString());
  }
}

void AuthManager::createLoginView(void)
{
  Wt::Auth::AuthWidget::createLoginView();
  bindWidget("footer", ngrt4n::footer());
  bindWidget("info-box", m_infoBox = new Wt::WText());
}

void AuthManager::createLoggedInView(void)
{
  setTemplateText(tr("Wt.Auth.template.logged-in"));
  m_dbSession->setLoggedUser();
  DboLoginSession sessionInfo;
  sessionInfo.username = m_dbSession->loggedUser().username;
  try {
    bindWidget("main-ui", m_mainUI = new WebMainUI(this));
    bindEmpty("update-banner");
  } catch (const std::bad_alloc& ) {
    bindString("main-ui", "You are likely running low on memory, please upgrade your system !");
  }

  // create the logout button
  Wt::WImage* image = new Wt::WImage(Wt::WLink("images/built-in/logout.png"), m_mainUI);
  image->setToolTip(QObject::tr("Sign out").toStdString());
  image->clicked().connect(this, &AuthManager::logout);
  bindWidget("logout-item", image);
}

void AuthManager::logout(void)
{
  m_dbSession->loginObject().logout();
  // reload the application => don't use refresh();
  wApp->doJavaScript("location.reload();");
}


bool AuthManager::isLogged(void)
{
  return m_dbSession->loginObject().loggedIn();
}


void AuthManager::handleLoginFailed(std::string data)
{
  m_infoBox->setText(data);
  m_infoBox->setStyleClass("alert alert-danger");
}
