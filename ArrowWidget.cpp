#include <iostream>

#include "ArrowWidget.h"
#include "qpushbutton.h"

using namespace std;

ArrowWidget::ArrowWidget(AppSettings * appSettings)
{
	this->appSettings = appSettings;

	currentEntropyThreshold = appSettings->minEntropyThreshold;

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
	this->currentEntropyThreshold += appSettings->entropyThresholdIncrement;
	if (currentEntropyThreshold > appSettings->maxEntropyThreshold) {
		currentEntropyThreshold = appSettings->maxEntropyThreshold;
	}
	cout << "Current entropy threshold value: " << currentEntropyThreshold << endl;

}

void ArrowWidget::doQueryDown()
{
	this->currentEntropyThreshold -= appSettings->entropyThresholdIncrement;
	if (currentEntropyThreshold < appSettings->minEntropyThreshold) {
		currentEntropyThreshold = appSettings->minEntropyThreshold;
	}
	cout << "Current entropy threshold value: " << currentEntropyThreshold << endl;
}