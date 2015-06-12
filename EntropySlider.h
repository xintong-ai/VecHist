#ifndef ENTROPY_SLIDER_H
#define ENTROPY_SLIDER_H

#include <QSlider>

class EntropySlider : public QSlider
{
public:
	EntropySlider();
	void setRunEntropyQueries(bool runEntropyQueries) { this->runEntropyQueries = runEntropyQueries; }
	bool getRunEntropyQueries() { return runEntropyQueries; }

private:
	bool runEntropyQueries = true;
};

#endif //ENTROPY_SLIDER_H
