/*
 * WebServiceTree.cpp
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

#include "WebTree.hpp"
#include "utilsCore.hpp"

WebTree::WebTree(void)
  : Wt::WTreeView(0),
    m_model(new Wt::WStandardItemModel())
{
  setDefaultSettings();
}


WebTree::~WebTree()
{
  delete m_model;
}


void WebTree::setDefaultSettings(void)
{
  setHeaderHeight(0);
  setSelectionMode(Wt::SingleSelection);
  setSelectable(true);
  setSelectionBehavior(Wt::SelectItems);
  setColumnWidth(0, 500);
}



void WebTree::build(void)
{
  // Create a item for each individual service
  for(NodeListT::ConstIterator node  = m_cdata->bpnodes.begin(), end = m_cdata->bpnodes.end();  node != end; ++node) {
    m_items.insertMulti(node->id, WebTree::createItem(*node));
  }

  for(NodeListT::ConstIterator node=m_cdata->cnodes.begin(), end=m_cdata->cnodes.end();  node != end; ++node) {
    m_items.insertMulti(node->id, WebTree::createItem(*node));
  }

  for (StringListT::Iterator edge=m_cdata->edges.begin(), end=m_cdata->edges.end(); edge != end; ++edge) {
    Wt::WStandardItem* parent = findNodeItem(edge.key());
    Wt::WStandardItem* child = findNodeItem(edge.value());
    if (parent && child) {
      parent->appendRow(child);
    }
  }

  applyModel(m_items[ngrt4n::ROOT_ID]);
}

void WebTree::applyModel(Wt::WStandardItem* _rootItem)
{
  m_model->appendRow(_rootItem);
  setModel(m_model);
  expandToDepth(2); //TODO: check before
}



Wt::WStandardItem* WebTree::createItem(const NodeT& _node)
{
  Wt::WStandardItem* item = new Wt::WStandardItem();
  item->setText(Wt::WString(_node.name.toStdString()));
  item->setIcon("images/built-in/unknown.png");
  item->setData(_node.id, Wt::UserRole);
  return item;
}

Wt::WStandardItem* WebTree::findNodeItem(const QString& _nodeId)
{
  WebTreeItemsT::iterator tnode = m_items.find(_nodeId);
  return (tnode != m_items.end())? *tnode : NULL;
}

void WebTree::updateNodeItem(const NodeT& _node, const QString& _tip)
{
  Wt::WStandardItem* item = findNodeItem(_node.id);
  if (item) {
    item->setIcon(ngrt4n::getIconPath(_node.sev).toStdString());
    item->setToolTip(Wt::WString::fromUTF8(_tip.toStdString()));
  }
}

