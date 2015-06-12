#ifndef ARROW_WIDGET_H
#define ARROW_WIDGET_H

#include <qframe.h>
#include <QWidget>
#include "ArrowButton.h"

class ArrowWidget : public QFrame
{
	Q_OBJECT
public:
	ArrowWidget();

private:
	ArrowButton * arrowButton1;
	QWidget * placeHolder;
	ArrowButton * arrowButton2;
};

#endif //ARROW_SLIDER_H
