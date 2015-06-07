/*
 * Copyright (c) 2010 Mark Liversedge (liversedge@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

//To use this in a cpp file, you may need to include:
//#include "DataManager.h"
//#include "DataMgrVect.h"

#ifndef _GC_TreeMapWindow_h
#define _GC_TreeMapWindow_h 1

#include <QtGui>
#include <QTimer>

#include <cmath>

#include <qtreewidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <QVBoxLayout>
#include <qscrollarea.h>



class NodeBi;
class DataManager;


class TreeMapPlot;

class MyScrollArea : public QScrollArea
{
private:
public:
	void wheelEvent(QWheelEvent *event)
	{
		if (event->type() == QEvent::Wheel) {
			event->ignore();
		}
	}
};

class TreeMapWindow : public QFrame
{
    Q_OBJECT

    public:

		TreeMapWindow(DataManager * dataManager);
        ~TreeMapWindow();
		void refreshPlot(NodeBi * root);
		#ifndef QT_NO_WHEELEVENT
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
		#endif
		void zoom(double factor, int x, int y);

		void setScrollArea(MyScrollArea * scrollArea) { this->scrollArea = scrollArea; }
		virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

		void updateChildren();

	public slots:

		void cellClicked(QString, QString); // cell clicked

	private:
		QHBoxLayout *mainLayout;
		MyScrollArea * scrollArea = nullptr;
		TreeMapPlot *ltmPlot;
		//QScrollArea * scrollArea = nullptr;
		DataManager * dataManager = nullptr;
		QWidget * placeholder = nullptr;
		QFrame * colorBar = nullptr;
};

#endif // _GC_TreeMapWindow_h
