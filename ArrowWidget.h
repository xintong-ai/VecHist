#ifndef ARROW_WIDGET_H
#define ARROW_WIDGET_H

#include <qframe.h>
#include <QWidget>
#include <QVBoxLayout>
#include "ArrowButton.h"
#include "AppSettings.h"

class ArrowWidget : public QFrame
{
	Q_OBJECT
public:
	ArrowWidget(AppSettings * appSettings);
public slots:
	void doQueryUp();
	void doQueryDown();

private:
	AppSettings * appSettings = nullptr;
	ArrowButton * arrowButton1 = nullptr;
	QWidget * placeHolder = nullptr;
	ArrowButton * arrowButton2 = nullptr;
	QVBoxLayout * layout = nullptr;

	double currentEntropyThreshold = 0;
};

#endif //ARROW_SLIDER_H
