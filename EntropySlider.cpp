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

//This paint method works around 2 issues:
//1) For some reason, a background color applied to the widget will not show up a parent widget (currently EntropyQueryContainer).  Painting done by QPainter during paint events WILL show up.
//2) For some reason, within this function setting a QPainter object with the pointer to this class will disrupt the normal rendering for the base class (QSlider).  It becomes messed up.  We must delete the object to get around this.
//TODO: If we have time after the paper is done, we might post on the QT forum about the proper way to get this stuff to work.
void EntropySlider::paintEvent(QPaintEvent * p)
{
	QPainter * painter = new QPainter(this);
	QBrush brush(Qt::black, Qt::SolidPattern);
	painter->setBrush(brush);

	QRect rect = this->rect();
	painter->fillRect(rect, *gradient);
	delete painter;			//HACK to fix rendering issues - this allows the base class to render correctly
	QSlider::paintEvent(p);
}