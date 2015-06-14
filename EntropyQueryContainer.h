//Any cpp file that uses this header file must #include "Scene.h"

#ifndef ENTROPY_QUERY_CONTAINER_H
#define ENTROPY_QUERY_CONTAINER_H

#include <qframe.h>
#include <QVBoxLayout>
#include "AppSettings.h"
#include "ArrowButton.h"

#include "EntropySlider.h"
#include "ArrowWidget.h"

class Scene;

//This class manages is a widget containing both the entropy query slider and the two arrow buttons
class EntropyQueryContainer : public QFrame
{
public:
	EntropyQueryContainer(AppSettings * appSettings, Scene * scene, EntropySlider * entropySlider);

public slots:
	void doQueryUp();
	void doQueryDown();

private:
	EntropySlider * entropySlider;
	Scene * scene = nullptr;
	AppSettings * appSettings = nullptr;
	ArrowButton * arrowButton1 = nullptr;
	ArrowButton * arrowButton2 = nullptr;
	QVBoxLayout * layout = nullptr;
};

#endif //ENTROPY_QUERY_CONTAINER_H
