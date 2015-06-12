#include <iostream>

#include "ArrowWidget.h"
#include "qpushbutton.h"
#include "Scene.h"

using namespace std;

ArrowWidget::ArrowWidget(AppSettings * appSettings, Scene * scene, QSlider * slider)
{
	this->appSettings = appSettings;
	this->scene = scene;
	this->slider = slider;

	appSettings->currentEntropyThreshold = appSettings->minEntropyThreshold;

	arrowButton1 = new ArrowButton(false);
	placeHolder = new QWidget();
	arrowButton2 = new ArrowButton(true);

	arrowButton1->resize(100, 100);
	arrowButton2->resize(100, 100);

	QSizePolicy arrow1Policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	arrow1Policy.setHorizontalStretch(5);
	arrowButton1->setSizePolicy(arrow1Policy);

	QSizePolicy placeHolderPolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	placeHolderPolicy.setHorizontalStretch(5);
	placeHolder->setSizePolicy(placeHolderPolicy);


	QSizePolicy arrow2Policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	arrow2Policy.setHorizontalStretch(5);
	arrowButton2->setSizePolicy(arrow2Policy);



	layout = new QVBoxLayout();
	layout->addWidget(arrowButton1);
	layout->addWidget(placeHolder);
	layout->addWidget(arrowButton2);

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(layout);

	//(QObject*)&arrowButton1;

	connect((QObject*)arrowButton1, SIGNAL(clicked()), this, SLOT(doQueryUp()));
	connect((QObject*)arrowButton2, SIGNAL(clicked()), this, SLOT(doQueryDown()));
}

void ArrowWidget::doQueryUp()
{
	this->appSettings->currentEntropyThreshold += appSettings->entropyThresholdIncrement;
	if (appSettings->currentEntropyThreshold > appSettings->maxEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->maxEntropyThreshold;
	}
	cout << "Current entropy threshold value: " << appSettings->currentEntropyThreshold << endl;

	bool entropyQueryChanged = false;
	do
	{
		entropyQueryChanged = scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);

		slider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);

		if (!entropyQueryChanged) {
			this->appSettings->currentEntropyThreshold += appSettings->entropyThresholdIncrement;
			
		}
	} while (!entropyQueryChanged && appSettings->currentEntropyThreshold <= appSettings->maxEntropyThreshold);

	if (appSettings->currentEntropyThreshold > appSettings->maxEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->maxEntropyThreshold;
	}
}

void ArrowWidget::doQueryDown()
{
	this->appSettings->currentEntropyThreshold -= appSettings->entropyThresholdIncrement;
	if (appSettings->currentEntropyThreshold < appSettings->minEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->minEntropyThreshold;
	}
	cout << "Current entropy threshold value: " << appSettings->currentEntropyThreshold << endl;

	//scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);

	//slider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);

	bool entropyQueryChanged = false;
	do
	{
		entropyQueryChanged = scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);

		slider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);

		if (!entropyQueryChanged) {
			this->appSettings->currentEntropyThreshold -= appSettings->entropyThresholdIncrement;

		}
	} while (!entropyQueryChanged && appSettings->currentEntropyThreshold >= appSettings->minEntropyThreshold);

	if (appSettings->currentEntropyThreshold < appSettings->minEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->minEntropyThreshold;
	}


}