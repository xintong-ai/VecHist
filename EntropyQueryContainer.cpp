
#include <QLinearGradient>

#include "EntropyQueryContainer.h"
#include "Scene.h"

//Constructor
EntropyQueryContainer::EntropyQueryContainer(AppSettings * appSettings, Scene * scene, EntropySlider * entropySlider)
{
	this->appSettings = appSettings;
	this->scene = scene;
	this->entropySlider = entropySlider;

	appSettings->currentEntropyThreshold = appSettings->minEntropyThreshold;

	arrowButton1 = new ArrowButton(false);
	arrowButton2 = new ArrowButton(true);

	//arrowButton1->resize(100, 100);
	//arrowButton2->resize(100, 100);

	//Sizing policies for the layout
	QSizePolicy arrow1Policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	arrow1Policy.setHorizontalStretch(5);
	arrowButton1->setSizePolicy(arrow1Policy);

	QSizePolicy arrow2Policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	arrow2Policy.setHorizontalStretch(5);
	arrowButton2->setSizePolicy(arrow2Policy);

	//Build the layout
	layout = new QVBoxLayout();
	layout->addWidget(arrowButton1);
	layout->addWidget(entropySlider);
	layout->addWidget(arrowButton2);

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(layout);

	//Set up events
	connect((QObject*)arrowButton1, SIGNAL(clicked()), this, SLOT(doQueryUp()));
	connect((QObject*)arrowButton2, SIGNAL(clicked()), this, SLOT(doQueryDown()));



}

//This function increases the current entropy threshold query value then runs the query on the split tree structure
//It will first try to increase by the configured threshold but will continue increasing the value bit by bit until some sort of
//change in the tree structure is detected.  This way a change always happens when the button is clicked.
void EntropyQueryContainer::doQueryUp()
{
	this->appSettings->currentEntropyThreshold += appSettings->entropyThresholdIncrement;
	if (appSettings->currentEntropyThreshold > appSettings->maxEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->maxEntropyThreshold;
	}
	cout << "Current entropy threshold value: " << appSettings->currentEntropyThreshold << endl;

	entropySlider->setRunEntropyQueries(false);

	bool entropyQueryChanged = false;
	do
	{
		entropyQueryChanged = scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);

		entropySlider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);

		if (!entropyQueryChanged) {
			this->appSettings->currentEntropyThreshold += appSettings->entropyThresholdIncrement;

		}
	} while (!entropyQueryChanged && appSettings->currentEntropyThreshold <= appSettings->maxEntropyThreshold);

	if (appSettings->currentEntropyThreshold > appSettings->maxEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->maxEntropyThreshold;
		scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);
		entropySlider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);
	}

	entropySlider->setRunEntropyQueries(true);
}

//This function decreases the current entropy threshold query value then runs the query on the split tree structure
//It will first try to decrease by the configured threshold but will continue decreasing the value bit by bit until some sort of
//change in the tree structure is detected.  This way a change always happens when the button is clicked.
void EntropyQueryContainer::doQueryDown()
{
	this->appSettings->currentEntropyThreshold -= appSettings->entropyThresholdIncrement;
	if (appSettings->currentEntropyThreshold < appSettings->minEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->minEntropyThreshold;
	}
	cout << "Current entropy threshold value: " << appSettings->currentEntropyThreshold << endl;

	entropySlider->setRunEntropyQueries(false);

	bool entropyQueryChanged = false;
	do
	{
		entropyQueryChanged = scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);

		entropySlider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);

		if (!entropyQueryChanged) {
			this->appSettings->currentEntropyThreshold -= appSettings->entropyThresholdIncrement;

		}
	} while (!entropyQueryChanged && appSettings->currentEntropyThreshold >= appSettings->minEntropyThreshold);

	if (appSettings->currentEntropyThreshold < appSettings->minEntropyThreshold) {
		appSettings->currentEntropyThreshold = appSettings->minEntropyThreshold;
		scene->initiateEntropyQuery(appSettings->currentEntropyThreshold);
		entropySlider->setValue((appSettings->currentEntropyThreshold - appSettings->minEntropyThreshold) / (appSettings->maxEntropyThreshold - appSettings->minEntropyThreshold) * 100);
	}

	entropySlider->setRunEntropyQueries(true);

}