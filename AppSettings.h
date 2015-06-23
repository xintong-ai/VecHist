#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

//#define DEBUG_PRINTING 1

class AppSettings
{
public:
	AppSettings();
	double minEntropyThreshold = 0;				//Minimun amount for entropy threshold for queries
	double maxEntropyThreshold = 0;				//Maximum value for entropy threshold for queries
	double entropyThresholdIncrement = 0;		//Amount by which to increment the entropy threshold for queries each time - currently only used in the arrow widget
	double currentEntropyThreshold = 0;			//Current value for entropy threshold for queries
	bool useTreeLeavesForColorMap = true;		//True if color map is made based on values stored in leaves and false if not
	bool useEntropyForTreeMapColor = true;		//True if entropy should be used to determine tree map widget color; false if not
	bool useEntropyForTreeMapArea = true;		//True if entropy should be used to determione tree map widget square areas; false if not
	bool showStreamlines = false;				//True if should show streamlines in program.  False if not
	int glyphType = 1;							//0 = superquadrics, 1 = spheres
};

#endif //ARROW_SETTIGS_H
