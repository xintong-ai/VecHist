#ifndef ENTROPY_SLIDER_H
#define ENTROPY_SLIDER_H

#include <QSlider>

#include "DataManager.h"
#include "DataMgrVect.h"
#include "AppSettings.h"

//This class is an QSlider adapted for usage for entropy querying
//It stores its own gradient as well as additional entropy querying information
class EntropySlider : public QSlider
{
public:
	EntropySlider(DataManager * dataManager, AppSettings * appSettings);
	void setRunEntropyQueries(bool runEntropyQueries) { this->runEntropyQueries = runEntropyQueries; }
	bool getRunEntropyQueries() { return runEntropyQueries; }
	QLinearGradient * getGradient() { return gradient; }
	void paintEvent(QPaintEvent *);

private:
	bool runEntropyQueries = true;		//If true, slider events associated with this widget will initiate new entropy queries.  If false, they will not.  This is needed becuase sometimes we must do a setValue function call without having another event fire.
	DataManager * dataManager;			//A reference to the data manager object
	QLinearGradient * gradient;			//The gradient maintained by the entropy slider
	AppSettings * appSettings;			//Reference to the app settings object - can be useful when we need to check min and max entropy threshold values that are possible
};

#endif //ENTROPY_SLIDER_H
