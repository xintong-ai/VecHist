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

#ifndef _GC_TreeMapPlot_h
#define _GC_TreeMapPlot_h 1

#include <QtGui>

#include "TreeMapWindow.h"


// for sorting
class TreeMap;

class TreeMap
{
    public:
        TreeMap(TreeMap *parent = NULL, QString name = "", double value = 0.0) :
            parent(parent), name(name), value(value) {}

		// The main user entry point - call this on the root
		// node and it will layout all the children in the
		// rectangle supplied. The children's rectangles can
		// then be passed directly to painter.drawRect etc
		void layout(QRect rect) {

			// I'll take that
			this->rect = rect;
			
			// need to sort in descending order
			///////////////////////////////////sort();

			// Use the squarified algorithm outlined
			// by Mark Bruls, Kees Huizing, and Jarke J. van Wijk
			// in "http://citeseerx.ist.psu.edu/viewdoc/
			// download?doi=10.1.1.36.6685&rep=rep1&type=pdf"
			// ... will recurse
			///////////////////////////////////squarifyLayout(children, rect);
		}

        // data
        TreeMap *parent;
        QString name;
        double value;

		// geometry
		QRect rect;
        
};


class TreeMapPlot : public QWidget
{
    Q_OBJECT
    


    public:
		TreeMapPlot(TreeMapWindow *);
        ~TreeMapPlot();
        
    protected:
		TreeMapWindow *parent;

		virtual void paintEvent(QPaintEvent *);
		virtual void resizeEvent(QResizeEvent *);

	private:
		TreeMap *root;      // the tree map data structure
		TreeMap *highlight; // currently needs to be highlighted
		QLabel myLabel;
    
};


#endif // _GC_TreeMapPlot_h

