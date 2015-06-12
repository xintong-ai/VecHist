#ifndef ARROW_WIDGET_H
#define ARROW_WIDGET_H

#include <qframe.h>
#include <QWidget>
#include <QVBoxLayout>
#include <qslider.h>
#include "ArrowButton.h"
#include "AppSettings.h"
#include "EntropySlider.h"

//Must #include "Scene.h" in any .cpp file that uses this header file

class Scene;

class ArrowWidget : public QFrame
{
	Q_OBJECT
public:
	ArrowWidget(AppSettings * appSettings, Scene * scene, EntropySlider * slider);
public slots:
	void doQueryUp();
	void doQueryDown();

private:
	EntropySlider * slider = nullptr;
	Scene * scene = nullptr;
	AppSettings * appSettings = nullptr;
	ArrowButton * arrowButton1 = nullptr;
	QWidget * placeHolder = nullptr;
	ArrowButton * arrowButton2 = nullptr;
	QVBoxLayout * layout = nullptr;
};

#endif //ARROW_SLIDER_H
