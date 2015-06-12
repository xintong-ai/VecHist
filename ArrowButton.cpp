#include "ArrowButton.h"
#include <qpainter.h>
#include <qbrush.h>

//Constructor
//Parameter type - signifies whether the arrow button points up or down when rendered
ArrowButton::ArrowButton(bool type)
{
	this->type = type;
}

//Override of the paint event - paints the arrow on the button
void ArrowButton::paintEvent(QPaintEvent * p)
{
	QPushButton::paintEvent(p);
	QPainter painter(this);
	QBrush brush(Qt::black, Qt::SolidPattern);
	painter.setBrush(brush);

	QRect rect = this->rect();

	QPainterPath path;
	if (type) {
		path.moveTo(0, 0);
		path.lineTo(rect.width(), 0);
		path.lineTo(rect.width() / 2, rect.height());
		path.lineTo(0, 0);
	}
	else {
		path.moveTo(rect.width() / 2, 0);
		path.lineTo(rect.width(), rect.height());
		path.lineTo(0, rect.height());
		path.lineTo(rect.width() / 2.0, 0);
	}

	painter.setPen(Qt::NoPen);
	painter.fillPath(path, brush);

}