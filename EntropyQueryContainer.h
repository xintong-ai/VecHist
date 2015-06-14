//Any cpp file that uses this header file must #include "Scene.h"

#ifndef ENTROPY_QUERY_CONTAINER_H
#define ENTROPY_QUERY_CONTAINER_H

#include <qframe.h>
#include <QVBoxLayout>
#include "AppSettings.h"
#include "ArrowButton.h"

#include "EntropySlider.h"

class Scene;

//This class manages is a widget containing both the entropy query slider and the two arrow buttons
//It can be used to run entropy threshold queries on the entropy split tree structure
class EntropyQueryContainer : public QFrame
{
	Q_OBJECT
public:
	EntropyQueryContainer(AppSettings * appSettings, Scene * scene, EntropySlider * entropySlider);

public slots:
	void doQueryUp();
	void doQueryDown();

private:
	EntropySlider * entropySlider;			//The slider widget
	Scene * scene = nullptr;				//Reference to the Scene object
	AppSettings * appSettings = nullptr;	//Reference to the app settings object
	ArrowButton * arrowButton1 = nullptr;	//First arrow button widget
	ArrowButton * arrowButton2 = nullptr;	//Second arrow button widget
	QVBoxLayout * layout = nullptr;			//The layout used
};

#endif //ENTROPY_QUERY_CONTAINER_H
