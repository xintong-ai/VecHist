#ifndef ARROW_WIDGET_H
#define ARROW_WIDGET_H

#include <qframe.h>
#include <QWidget>
#include <QVBoxLayout>
#include "ArrowButton.h"

class ArrowWidget : public QFrame
{
	Q_OBJECT
public:
	ArrowWidget();

private:
	ArrowButton * arrowButton1 = nullptr;
	QWidget * placeHolder = nullptr;
	ArrowButton * arrowButton2 = nullptr;
	QVBoxLayout * layout = nullptr;
};

#endif //ARROW_SLIDER_H
