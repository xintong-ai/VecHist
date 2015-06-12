#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

class AppSettings
{
public:
	AppSettings();
	double minEntropyThreshold = 0;				//Minimun amount for entropy threshold for queries
	double maxEntropyThreshold = 0;				//Maximum value for entropy threshold for queries
	double entropyThresholdIncrement = 0;		//Amount by which to increment the entropy threshold for queries each time - currently only used in the arrow widget
	double currentEntropyThreshold = 0;			//Current value for entropy threshold for queries

};

#endif //ARROW_SETTIGS_H
