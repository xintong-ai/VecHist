#ifndef ARROW_SLIDER_H
#define ARROW_SLIDER_H

#include <QPushButton>

class ArrowButton : public QPushButton
{
	Q_OBJECT
public:
	ArrowButton(bool type);
	void paintEvent(QPaintEvent *);

private:
	bool type = false;		//Represents whether the arrow button points up or down when rendered
};

#endif //ARROW_SLIDER_H
