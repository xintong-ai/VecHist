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

//This class represents a widget with two arrow buttons, one up and down,
//used for entropy threshold querying
//It is likely to be deprecated if we continue using the equivalent functionality
//in the new EntropyQueryContainer class
class ArrowWidget : public QWidget
{
	Q_OBJECT
public:
	ArrowWidget(AppSettings * appSettings, Scene * scene, EntropySlider * slider);
public slots:
	void doQueryUp();
	void doQueryDown();

private:
	EntropySlider * slider = nullptr;	//Reference to the slider widget
	Scene * scene = nullptr;			//Reference to the scene object
	AppSettings * appSettings = nullptr;	//Reference to the app settings
	ArrowButton * arrowButton1 = nullptr;	//First arrow button widget
	QWidget * placeHolder = nullptr;		//Place holder
	ArrowButton * arrowButton2 = nullptr;	//Second arrow button widget 
	QVBoxLayout * layout = nullptr;			//Layout
};

#endif //ARROW_SLIDER_H
