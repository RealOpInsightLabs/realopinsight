/*
 * Parser.cpp
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

#include "core/ns.hpp"
#include "Parser.hpp"
#include "GraphView.hpp"
#include <QObject>

using namespace std;

const QString Parser::CHILD_NODES_SEP = "," ;
const QString Parser::dotFileHeader = "strict graph\n{\n node[shape=plaintext]\n";
const QString Parser::dotFileFooter = "}";


Parser::Parser(){}

Parser::~Parser()
{
    QFile dotFile;

    if(dotFile.exists(graphFilename))
        dotFile.remove(graphFilename);

    if(dotFile.exists(graphFilename + ".plain"))
        dotFile.remove(graphFilename + ".plain");
}

bool Parser::parseSvConfig(const QString & _configFile, Struct & _coreData)
{
    QString graph_content;
    QDomDocument xmlDoc;
    QDomElement xmlRoot;
    QFile file(_configFile);

    if ( ! file.open(QIODevice::ReadOnly) ) {
        qDebug() << QObject::tr("Unable to open the file %1").arg(_configFile);
        return false;
    }

    if (! xmlDoc.setContent(&file) ) {
        file.close();
        qDebug() << QObject::tr("Error while parsing the file %1").arg(_configFile);
        return false;
    }

    xmlRoot = xmlDoc.documentElement();
    QDomNodeList services = xmlRoot.elementsByTagName("Service");

    qint32 serviceCount = services.length();
    for (qint32 srv = 0; srv < serviceCount; srv++) {
        NodeT node;
        QDomElement service = services.item(srv).toElement();
        node.id = service.attribute("id").trimmed() ;
        node.type = service.attribute("type").toInt() ;
        node.status_crule = service.attribute("statusCalcRule").toInt() ;
        node.status_prule = service.attribute("statusPropRule").toInt() ;
        node.icon = service.firstChildElement("Icon").text().trimmed() ;
        node.name = service.firstChildElement("Name").text().trimmed() ;
        node.description = service.firstChildElement("Description").text().trimmed() ;
        node.propagation_rule = service.firstChildElement("PropagationRule").text().trimmed() ;
        node.alarm_msg = service.firstChildElement("AlarmMsg").text().trimmed() ;
        node.notification_msg = service.firstChildElement("NotificationMsg").text().trimmed() ;
        node.child_nodes = service.firstChildElement("SubServices").text().trimmed() ;

        if(node.icon.length() == 0) node.icon = GraphView::DEFAULT_ICON ;
        node.icon.remove("--> ") ; //For backward compatibility
        node.status = MonitorBroker::UNSET_STATUS ;
        node.parent = "NULL" ;
        if(node.status_crule < 0) node.status_crule = StatusCalcRules::HighCriticity ;  //For backward compatibility
        if(node.status_prule < 0) node.status_prule = StatusPropRules::Unchanged ;

        _coreData.nodes.insert(node.id, node) ;
        if( node.type == NodeType::ALARM_NODE ) {
            QString host = (node.child_nodes.split("/")).at(0) ;
            _coreData.hosts[host] << node.id;
            _coreData.checks << node.id;
            _coreData.cnodes.insert(node.id, node) ;
        }
    }
    file.close();
    updateNodeHierachy(_coreData.nodes, graph_content) ;
    buildNodeTree(_coreData.nodes, _coreData.tree_items) ;
    graph_content = dotFileHeader + graph_content ;
    graph_content += dotFileFooter;
    saveCoordinatesDotFile(graph_content);

    return true;
}

void Parser::updateNodeHierachy( NodeListT & _nodes_list, QString & _graph_content )
{
    _graph_content = "\n" ;
    for(NodeListT::iterator node_it = _nodes_list.begin(); node_it != _nodes_list.end(); node_it ++) {

        QString nname = node_it->name ;
        _graph_content = "\t" + node_it->id + "[label=\"" + nname.replace(' ', '#') + "\"];\n" + _graph_content;

        if( node_it->child_nodes != "" ) {
            QStringList node_ids_list = node_it->child_nodes.split(CHILD_NODES_SEP) ;

            for(QStringList::iterator node_id_it = node_ids_list.begin(); node_id_it != node_ids_list.end(); node_id_it ++) {
                NodeListT::iterator child_node_it = _nodes_list.find( (*node_id_it).trimmed() );
                if( child_node_it != _nodes_list.end() ) {
                    child_node_it->parent = node_it->id ;
                    _graph_content += "\t" + node_it->id + "--" + child_node_it->id + "\n";
                }
            }
        }
    }
}

void Parser::buildNodeTree( NodeListT & _nodes, TreeNodeItemListT & _tree)
{
    for(NodeListT::iterator node = _nodes.begin(); node != _nodes.end(); node++) {

        QTreeWidgetItem *item = new QTreeWidgetItem(QTreeWidgetItem::UserType) ;
        item->setIcon(0, QIcon(":/images/unknown.png")) ;
        item->setText(0, node->name) ;
        item->setData(0, QTreeWidgetItem::UserType, node->id) ;

        _tree.insert(node->id, item) ;
    }

    for(NodeListT::iterator node = _nodes.begin(); node != _nodes.end(); node++) {

        if( (node->type == NodeType::ALARM_NODE) || (node->child_nodes.length() == 0) ) continue ;

        QStringList childs = node->child_nodes.split(Parser::CHILD_NODES_SEP);
        for(QStringList::iterator child_id = childs.begin(); child_id != childs.end(); child_id++ ) {

            TreeNodeItemListT::iterator nitem = _tree.find(node->id) ;
            if ( nitem == _tree.end()) {
                qDebug() << QObject::tr("service not found %1").arg(node->name);
                continue ;
            }

            TreeNodeItemListT::iterator child = _tree.find(*child_id) ;
            if( child != _tree.end() ) _tree[node->id]->addChild(*child) ;
        }
    }
}

void Parser::saveCoordinatesDotFile(const QString& _graph_content)
{
    QFile file;

    graphFilename = QDir::tempPath() + "/graphviz-" + QTime().currentTime().toString("hhmmsszzz") + ".dot";
    file.setFileName(graphFilename);
    if(! file.open(QIODevice::WriteOnly)) {
        qDebug() << QObject::tr("Unable into write the file %1").arg(graphFilename) ;
        exit (1);
    }

    QTextStream file_stream(&file);
    file_stream << _graph_content;
    file.close();
}
