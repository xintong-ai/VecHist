#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class DataMgr;
class GLWidget;
class MarchingCubes;
class QPushButton;
class QSlider;
class Tracer;
class TexPlaneTracer;
class QCheckBox;
class QLabel;

class Window : public QWidget
{
	Q_OBJECT	//without this line, the slot does not work
public:
    Window();
    ~Window();

private:
    GLWidget *openGL;

	//QPushButton* addLensBtn;
	//QPushButton* addNodeBtn;
	//QPushButton* viewBtn;
	//QSlider* xSlider;
	//QSlider* ySlider;
	//QSlider* zSlider;
	//QSlider* lensWidSlider;
	//QLabel* statusLabel;

	//Tracer* SlicePlaneRenderable0;
	//Tracer* SlicePlaneRenderable1;
	//Tracer* SlicePlaneRenderable2;
	//Tracer* lensRenderable;
	//Tracer* lineRenderable;

	//std::map < std::string, Tracer* > meshTracers;
	//std::map < std::string, QCheckBox* > meshCheckBoxes;

	//static const int btnSize[2];// = { 60, 40 };

	//QPushButton* CreateRegularButton(const char* name);
	//QSlider* CreateSliceSlider(TexPlaneTracer* renderable);

private slots:
	//void XSliderChanged(int i);
	//void YSliderChanged(int i);
	//void ZSliderChanged(int i);

	//void LensWidSliderChanged(int i);
	//void transSizeSliderChanged(int i);

	//void xSliceToggled(bool b);
	//void ySliceToggled(bool b);
	//void zSliceToggled(bool b);

	//void linesToggled(bool b);
	//void MeshToggled(bool b);

	//void AddLens();
	//void AddLensNode();
	//void DeleteLens();
	//void DeleteLensNode();
	//void SetToNavigation();
	//void UpdateStatusLabel();

	//void RedoDeform();
};

#endif
