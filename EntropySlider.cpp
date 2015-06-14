#include "EntropySlider.h"

//Constructor
EntropySlider::EntropySlider(DataManager * dataManager, AppSettings * appSettings)
{
	this->dataManager = dataManager;
	gradient = new QLinearGradient(0, 0, 0, this->rect().height());

	const double INCREMENT = 0.01;

	double color[3];

	//Build the gradient
	const int NUM_ITERATIONS = 100;
	for (double i = appSettings->minEntropyThreshold; i <= appSettings->maxEntropyThreshold; i += (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) / NUM_ITERATIONS) {
		((DataMgrVect *)dataManager)->getEntropyColor(i, color);
		gradient->setColorAt(1.0 - (i - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold), QColor(255 * color[0], 255 * color[1], 255 * color[2], 255));
	}

	//Do the rest of the set up for the slider widget
	//based on http://www.codeprogress.com/cpp/libraries/qt/showQtExample.php?key=QLinearGradientManyColor&index=583
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(*gradient));
	setPalette(palette);
	setWindowTitle("Entropy Query");

}