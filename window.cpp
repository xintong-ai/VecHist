#include "window.h"
#include "glwidget.h"
#include "VecReader.h"
#include "BoxRenderable.h"
//#include "Streamline.h"
//#include "LineRenderable.h"
#include "GlyphRenderable.h"
#include "Cubemap.h"
#include "GL2DProjWidget.h"
//#include "DataMgr.h"
//#include "GLPolyRenderable.h"
//#include "GLTexPlaneRenderable.h"
//#include "GLLineRenderable.h"
//#include "GLLensRenderable.h"
//#include "iostream"
//#include "defines.h"
//#include "MeshReader.h"
//#include "LineReader.h"
//#include "DTIVolumeReader.h"
//#include "GL2DProjWidget.h"

//const int Window::btnSize[2] = { 60, 40 };

//QPushButton* Window::CreateRegularButton(const char* name)
//{
//	QPushButton* btn = new QPushButton(name, this);
//	btn->setMinimumSize(btnSize[0], btnSize[1]);
//	return btn;
//}


class GLTextureCube;
Window::Window()
{
    setWindowTitle(tr("Glyph-based Flow Visualization"));
	QHBoxLayout *mainLayout = new QHBoxLayout;

	//DataMgr dataMgr;
	//QSizePolicy fixedPolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);

	//this->move(100, 100);

	/********GL widget******/
	openGL = new GLWidget;
	QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(2, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    openGL->setFormat(format); // must be called before the widget or its parent window gets shown

	//VecReader* vecReader = new VecReader("D:/data/plume/15plume3d421-504x504x2048.vec");
	//VecReader* vecReader = new VecReader("D:/OneDrive/data/plume/15plume3d421.vec");
	//VecReader* vecReader = new VecReader("D:/OneDrive/data/nek/nek.d_4.vec");

	VecReader* vecReader = new VecReader("E:/OSU-files/HaloWorkNew/vechist_data/15plume3d421.vec");
	//VecReader* vecReader = new VecReader("D:/OneDrive/data/plume/15plume3d421-504x504x2048.vec");
	//VecReader* vecReader = new VecReader("D:/OneDrive/data/isabel/UVWf01.vec");
	//VecReader* vecReader = new VecReader("D:/OneDrive/data/tornado/1.vec");


	//Streamline* streamline = new Streamline(vecReader->GetFileName().c_str());
	//LineRenderable* lineRenderable = new LineRenderable(streamline);
	//openGL->AddRenderable("streamlines", lineRenderable);

	cubemap = new Cubemap(vecReader);
	//cubemap = new Cubemap("D:/Dropbox/hist/VecHist/python/crystal/data/universe_hist.bin");
	//cubemap->GenCubeMap(55, 55, 300, 10, 10, 10);
	glyphRenderable = new GlyphRenderable(cubemap);
	int3 innerDim = cubemap->GetInnerDim();
	glyphRenderable->SetVolumeDim(innerDim.x, innerDim.y, innerDim.z);
	openGL->AddRenderable("glyphs", glyphRenderable);

	BoxRenderable* bbox = new BoxRenderable(cubemap->GetInnerDim());
	bbox->SetVisibility(true);
	openGL->SetVol(cubemap->GetInnerDim());
	openGL->AddRenderable("bbox", bbox);

	//QObject::connect(lineRenderable, SIGNAL(SigGenCubeAlongLine(float4*, int)),
	//	glyphRenderable, SLOT(SlotGenCubeAlongLine(float4*, int)));

	//QObject::connect(lineRenderable, SIGNAL(SigTest()),
	//	glyphRenderable, SLOT(SlotTest()));
	//lineRenderable->GenGlyphAlongLine(0);
	
	///********cursor******/
	//MeshReader *sphere = new MeshReader();
	//sphere->SphereMesh(1.0f, 16, 16);
	//Renderable* sphereRenderable = new GLPolyRenderable(sphere);//new glMeshRenderer ;
	//((GLPolyRenderable*)sphereRenderable)->SetAmbientColor(0.2, 0.0, 0.0);
	//((GLPolyRenderable*)sphereRenderable)->SetVisibility(false);
	//openGL->AddRenderable("cursor", sphereRenderable);

	///********slices******/
	//VolumeReader* image = new VolumeReader(dataMgr.GetConfig("image").c_str());
	//SlicePlaneRenderable0 = new GLTexPlaneRenderable(image, 0);//new glMeshRenderer ;
	//openGL->AddRenderable("slice0", SlicePlaneRenderable0);
	//SlicePlaneRenderable1 = new GLTexPlaneRenderable(image, 1);//new glMeshRenderer ;
	//openGL->AddRenderable("slice1", SlicePlaneRenderable1);
	//SlicePlaneRenderable2 = new GLTexPlaneRenderable(image, 2);//new glMeshRenderer ;
	//openGL->AddRenderable("slice2", SlicePlaneRenderable2);

	/********tracts******/
	//DTIVolumeReader* image_dti = new DTIVolumeReader(dataMgr.GetConfig("image_dti").c_str());
	//LineReader* tract = new LineReader(dataMgr.GetConfig("tract").c_str());
	//lineRenderable = new GLLineRenderable(tract);//new glMeshRenderer ;
	//((GLLineRenderable*)lineRenderable)->SetDTIVolumeReader(image_dti);
	//openGL->AddRenderable("tract", lineRenderable);
	//openGL->SetLineReader(tract);

	///********lens******/
	//lensRenderable = new GLLensRenderable();//new glMeshRenderer ;
	//((GLLensRenderable*)lensRenderable)->SetVolumeReader(image);
	//openGL->AddRenderable("lens", lensRenderable);
	//QObject::connect((GLLensRenderable*)lensRenderable, &GLLensRenderable::DrawLensFinished, this, &Window::UpdateStatusLabel);

	///********slices 2D******/
	//QVBoxLayout *slice2DLayout = new QVBoxLayout;
	//GL2DProjWidget* xSlice2DWidget = new GL2DProjWidget((GLTexPlaneRenderable*)SlicePlaneRenderable0);
	//GL2DProjWidget* ySlice2DWidget = new GL2DProjWidget((GLTexPlaneRenderable*)SlicePlaneRenderable1);
	//GL2DProjWidget* zSlice2DWidget = new GL2DProjWidget((GLTexPlaneRenderable*)SlicePlaneRenderable2);
	//slice2DLayout->addWidget(xSlice2DWidget);
	//slice2DLayout->addWidget(ySlice2DWidget);
	//slice2DLayout->addWidget(zSlice2DWidget);

	///********controls******/
	QVBoxLayout *controlLayout = new QVBoxLayout;

	//addLensBtn = CreateRegularButton("Add Lens");
	//addNodeBtn = CreateRegularButton("Add Node");
	//QHBoxLayout* addThingsLayout = new QHBoxLayout;
	//addThingsLayout->addWidget(addLensBtn);
	//addThingsLayout->addWidget(addNodeBtn);
	//viewBtn = CreateRegularButton("Navigation");

	//QHBoxLayout* lenWidLayout = new QHBoxLayout;
	//QLabel* lensWidLabel = new QLabel("Lens Width:", this);
	//lensWidSlider = new QSlider(Qt::Horizontal);
	//lensWidSlider->setRange(1, 16);
	//lensWidSlider->setValue(((GLLensRenderable*)lensRenderable)->GetLensWidth() * 10);
	//lensWidSlider->setFixedSize(120, 30);
	//lenWidLayout->addWidget(lensWidLabel);
	//lenWidLayout->addWidget(lensWidSlider);
	//connect(lensWidSlider, SIGNAL(valueChanged(int)), this, SLOT(LensWidSliderChanged(int)));

	//QHBoxLayout* transSizeLayout = new QHBoxLayout;
	//QLabel* transSizeLabel = new QLabel("Transition region size:", this);
	//QSlider* transSizeSlider = new QSlider(Qt::Horizontal);
	//transSizeSlider->setRange(0, 9);
	//transSizeSlider->setValue(((GLLineRenderable*)lineRenderable)->GetTransSize() * 10);
	//transSizeSlider->setFixedSize(60, 30);
	//transSizeLayout->addWidget(transSizeLabel);
	//transSizeLayout->addWidget(transSizeSlider);
	//connect(transSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(transSizeSliderChanged(int)));

	//QHBoxLayout* delThingsLayout = new QHBoxLayout;
	//QPushButton* delLens = CreateRegularButton("Delete Lens");
	//connect(delLens, SIGNAL(clicked()), this, SLOT(DeleteLens()));
	//QPushButton* delLensNode = CreateRegularButton("Delete Node");
	//connect(delLensNode, SIGNAL(clicked()), this, SLOT(DeleteLensNode()));
	//delThingsLayout->addWidget(delLens);
	//delThingsLayout->addWidget(delLensNode);

	//QPushButton* redoBtn = CreateRegularButton("Redo Deformation");
	//connect(redoBtn, SIGNAL(clicked()), this, SLOT(RedoDeform()));

	QGroupBox *groupBox = new QGroupBox(tr("Slice Orientation"));

	radioX = new QRadioButton(tr("&X"));
	radioY = new QRadioButton(tr("&Y"));
	radioZ = new QRadioButton(tr("&Z"));
	radioX->setChecked(true);
	QHBoxLayout *sliceOrieLayout = new QHBoxLayout;
	sliceOrieLayout->addWidget(radioX);
	sliceOrieLayout->addWidget(radioY);
	sliceOrieLayout->addWidget(radioZ);
	sliceOrieLayout->addStretch();
	groupBox->setLayout(sliceOrieLayout);
	controlLayout->addWidget(groupBox);

	connect(radioX, SIGNAL(clicked(bool)), this, SLOT(SlotSliceOrieChanged(bool)));
	connect(radioY, SIGNAL(clicked(bool)), this, SLOT(SlotSliceOrieChanged(bool)));
	connect(radioZ, SIGNAL(clicked(bool)), this, SLOT(SlotSliceOrieChanged(bool)));

	QCheckBox* blockBBoxCheck = new QCheckBox("Block bounding box", this);
	connect(blockBBoxCheck, SIGNAL(clicked(bool)), glyphRenderable, SLOT(SlotSetCubesVisible(bool)));

	//statusLabel = new QLabel("status: Navigation");
	QLabel* sliceLabel = new QLabel("Slice number:", this);
	sliceSlider = new QSlider(Qt::Horizontal);
	//sliceSlider->setFixedSize(120, 30);
	sliceSlider->setRange(0, cubemap->GetInnerDim( glyphRenderable->GetSliceDimIdx())/*vecReader->GetVolumeDim().z*/ - 1);
	sliceSlider->setValue(0);

	QLabel* sliceThicknessLabel = new QLabel("Number of layers:", this);
	QSlider* numPartSlider = new QSlider(Qt::Horizontal);
	//numPartSlider->setFixedSize(120, 30);
	numPartSlider->setRange(1, 32);
	numPartSlider->setValue(1);

	QLabel* heightScaleLabel = new QLabel("Glyph Height Scale:", this);
	heightScaleSlider = new QSlider(Qt::Horizontal);
	//numPartSlider->setFixedSize(120, 30);
	heightScaleSlider->setRange(0, nHeightScale);
	heightScaleSlider->setValue(nHeightScale * 0.8);
	glyphRenderable->SetHeightScale(nHeightScale * 0.8);

	QVBoxLayout *interactLayout = new QVBoxLayout;
	QGroupBox *interactGrpBox = new QGroupBox(tr("Interactions"));
	interactLayout->addWidget(blockBBoxCheck);
	interactLayout->addWidget(sliceLabel);
	interactLayout->addWidget(sliceSlider);
	interactLayout->addWidget(sliceThicknessLabel);
	interactLayout->addWidget(numPartSlider);
	interactLayout->addWidget(heightScaleLabel);
	interactLayout->addWidget(heightScaleSlider);
	//interactGrpBox->setSizePolicy(fixedPolicy);
	//interactLayout->addLayout(addThingsLayout);
	//interactLayout->addLayout(delThingsLayout);
	//interactLayout->addWidget(redoBtn);
	//interactLayout->addWidget(viewBtn);
	//interactLayout->addLayout(lenWidLayout);
	//interactLayout->addLayout(transSizeLayout);
	//interactLayout->addWidget(statusLabel);
	interactGrpBox->setLayout(interactLayout);
	controlLayout->addWidget(interactGrpBox);
	connect(sliceSlider, SIGNAL(valueChanged(int)), glyphRenderable, SLOT(SlotSliceNumChanged(int)));
	connect(numPartSlider, SIGNAL(valueChanged(int)), glyphRenderable, SLOT(SlotNumPartChanged(int)));
	connect(heightScaleSlider, SIGNAL(valueChanged(int)), glyphRenderable, SLOT(SlotHeightScaleChanged(int)));

	GL2DProjWidget* gl2DProjWidget = new GL2DProjWidget(this);
	controlLayout->addWidget(gl2DProjWidget);
	//gl2DProjWidget->SetCubeTexture(glyphRenderable->GetCubeTexture(0));
	connect(glyphRenderable, SIGNAL(SigChangeTex(GLTextureCube*, Cube*)), 
		gl2DProjWidget, SLOT(SlotSetCubeTexture(GLTextureCube*, Cube*)));

	aTimer = new QTimer;
	connect(aTimer,SIGNAL(timeout()),SLOT(animate()));
	aTimer->start(20);
	aTimer->stop();
	

	QCheckBox* animationCheck = new QCheckBox("Animation", this);
	connect(animationCheck, SIGNAL(clicked(bool)), this, SLOT(SlotSetAnimation(bool)));
	interactLayout->addWidget(animationCheck);
	//connect(addLensBtn, SIGNAL(clicked()), this, SLOT(AddLens()));
	//connect(addNodeBtn, SIGNAL(clicked()), this, SLOT(AddLensNode()));
	//connect(viewBtn, SIGNAL(clicked()), this, SLOT(SetToNavigation()));


	///*************Lines***********/
	//QCheckBox* linesCheck = new QCheckBox("tracts", this);
	//linesCheck->setChecked(true);
	//QVBoxLayout *linesLayout = new QVBoxLayout;
	//linesLayout->addWidget(linesCheck);
	//QGroupBox *linesGrpBox = new QGroupBox(tr("Fiber Tracts"));
	//linesGrpBox->setSizePolicy(fixedPolicy);
	//linesGrpBox->setLayout(linesLayout);
	//controlLayout->addWidget(linesGrpBox);

	//connect(linesCheck, SIGNAL(clicked(bool)), this, SLOT(linesToggled(bool)));

	///*************Models***********/
	//QVBoxLayout *meshLayout = new QVBoxLayout;
	//QGroupBox *meshGrpBox = new QGroupBox(tr("3D Models"));
	//meshGrpBox->setSizePolicy(fixedPolicy);
	//meshGrpBox->setLayout(meshLayout);
	//std::map<std::string, std::string> meshFiles = dataMgr.GetMeshFiles();
	//for (auto m : meshFiles) {
	//	MeshReader *meshReader = new MeshReader();
	//	meshReader->LoadPLY(m.second.c_str());
	//	Renderable* mesh_renderable = new GLPolyRenderable(meshReader);//new glMeshRenderer ;
	//	openGL->AddRenderable(m.first.c_str(), mesh_renderable);
	//	meshRenderables[m.first.c_str()] = mesh_renderable;
	//	((GLPolyRenderable*)mesh_renderable)->SetAmbientColor(0.0, 0.2, 0.2);

	//	QCheckBox* meshCheck = new QCheckBox(m.first.c_str(), this);
	//	meshCheck->setChecked(true);
	//	meshCheckBoxes[m.first.c_str()] = meshCheck;
	//	meshLayout->addWidget(meshCheck);
	//	connect(meshCheck, SIGNAL(clicked(bool)), this, SLOT(MeshToggled(bool)));
	//}
	//controlLayout->addWidget(meshGrpBox);

	///*************Slices***********/
	//QCheckBox* xSliceCheck = new QCheckBox("x:", this);
	//QCheckBox* ySliceCheck = new QCheckBox("y:", this);
	//QCheckBox* zSliceCheck = new QCheckBox("z:", this);
	//xSliceCheck->setStyleSheet("QCheckBox { color: red }");
	//ySliceCheck->setStyleSheet("QCheckBox { color: green }");
	//zSliceCheck->setStyleSheet("QCheckBox { color: blue }");
	//xSliceCheck->setChecked(true);
	//ySliceCheck->setChecked(true);
	//zSliceCheck->setChecked(true);
	//xSlider = CreateSliceSlider((TexPlaneRenderable*)SlicePlaneRenderable0);
	//ySlider = CreateSliceSlider((TexPlaneRenderable*)SlicePlaneRenderable1);
	//zSlider = CreateSliceSlider((TexPlaneRenderable*)SlicePlaneRenderable2);
	//xSlider->setFixedSize(150, 30);
	//ySlider->setFixedSize(150, 30);
	//zSlider->setFixedSize(150, 30);
	//QVBoxLayout *slicerLayout = new QVBoxLayout;
	//QGroupBox *slicerGrpBox = new QGroupBox(tr("Slicer Control"));
	//slicerGrpBox->setSizePolicy(fixedPolicy);
	//QHBoxLayout *xSliceLayout = new QHBoxLayout;
	//QHBoxLayout *ySliceLayout = new QHBoxLayout;
	//QHBoxLayout *zSliceLayout = new QHBoxLayout;
	//xSliceLayout->addWidget(xSliceCheck);
	//xSliceLayout->addWidget(xSlider);
	//ySliceLayout->addWidget(ySliceCheck);
	//ySliceLayout->addWidget(ySlider);
	//zSliceLayout->addWidget(zSliceCheck);
	//zSliceLayout->addWidget(zSlider);
	//slicerLayout->addLayout(xSliceLayout);
	//slicerLayout->addLayout(ySliceLayout);
	//slicerLayout->addLayout(zSliceLayout);
	//slicerGrpBox->setLayout(slicerLayout);
	//controlLayout->addWidget(slicerGrpBox);
	controlLayout->addStretch();

	//connect(xSlider, SIGNAL(valueChanged(int)), this, SLOT(XSliderChanged(int)));
	//connect(ySlider, SIGNAL(valueChanged(int)), this, SLOT(YSliderChanged(int)));
	//connect(zSlider, SIGNAL(valueChanged(int)), this, SLOT(ZSliderChanged(int)));

	//connect(xSliceCheck, SIGNAL(clicked(bool)), this, SLOT(xSliceToggled(bool)));
	//connect(ySliceCheck, SIGNAL(clicked(bool)), this, SLOT(ySliceToggled(bool)));
	//connect(zSliceCheck, SIGNAL(clicked(bool)), this, SLOT(zSliceToggled(bool)));
	//
	//mainLayout->addLayout(slice2DLayout, 1);
	mainLayout->addWidget(openGL,3);
	mainLayout->addLayout(controlLayout,1);
	setLayout(mainLayout);
}

void Window::SlotSliceOrieChanged(bool clicked)
{
	if (radioX->isChecked()){
		sliceSlider->setRange(0, cubemap->GetInnerDim(0)/*vecReader->GetVolumeDim().z*/ - 1);
		glyphRenderable->SlotSetSliceOrie(0);
	}	else if (radioY->isChecked()){
		sliceSlider->setRange(0, cubemap->GetInnerDim(1)/*vecReader->GetVolumeDim().z*/ - 1);
		glyphRenderable->SlotSetSliceOrie(1);
	}	else if (radioZ->isChecked()){
		sliceSlider->setRange(0, cubemap->GetInnerDim(2)/*vecReader->GetVolumeDim().z*/ - 1);
		glyphRenderable->SlotSetSliceOrie(2);
	}
	sliceSlider->setValue(0);
}

void Window::animate()
{
	int v = heightScaleSlider->value();
	v = (v + 1) % nHeightScale;
	heightScaleSlider->setValue(v);
}

void Window::SlotSetAnimation(bool doAnimation)
{
	if (doAnimation)
		aTimer->start();
	else
		aTimer->stop();
}


//
//void Window::XSliderChanged(int i)
//{
//	((GLTexPlaneRenderable*)SlicePlaneRenderable0)->SetSliceNum(i);
//	((GLTexPlaneRenderable*)SlicePlaneRenderable0)->UpdateData();
//}
//
//void Window::YSliderChanged(int i)
//{
//	((GLTexPlaneRenderable*)SlicePlaneRenderable1)->SetSliceNum(i);
//	((GLTexPlaneRenderable*)SlicePlaneRenderable1)->UpdateData();
//}
//
//void Window::ZSliderChanged(int i)
//{
//	((GLTexPlaneRenderable*)SlicePlaneRenderable2)->SetSliceNum(i);
//	((GLTexPlaneRenderable*)SlicePlaneRenderable2)->UpdateData();
//}
//
//void Window::LensWidSliderChanged(int i)
//{
//	((GLLensRenderable*)lensRenderable)->UpdateLensWidth(i * 0.1f);
//}
//
//void Window::transSizeSliderChanged(int i)
//{
//	((GLLineRenderable*)lineRenderable)->SetTransSize( 1 - i * 0.1f);
//}
//
//
//void Window::xSliceToggled(bool b)
//{
//	((GLTexPlaneRenderable*)SlicePlaneRenderable0)->SetVisibility(b);
//}
//
//void Window::linesToggled(bool b)
//{
//	((GLLineRenderable*)lineRenderable)->SetVisibility(b);
//}
//
//
//void Window::ySliceToggled(bool b)
//{
//	((GLTexPlaneRenderable*)SlicePlaneRenderable1)->SetVisibility(b);
//}
//
//void Window::zSliceToggled(bool b)
//{
//	((GLTexPlaneRenderable*)SlicePlaneRenderable2)->SetVisibility(b);
//}
//
//void Window::MeshToggled(bool b)
//{
//	for (auto m : meshCheckBoxes) {
//		meshRenderables[m.first]->SetVisibility(m.second->isChecked());
//	}
//}
//
//void Window::DeleteLens()
//{
//	bool anyLens = ((GLLensRenderable*)lensRenderable)->RemoveOneLens();
//	//if (!anyLens)
//	//	viewBtn->setChecked(true);
//	openGL->SetInteractModeToTransform();
//	UpdateStatusLabel();
//}
//
//void Window::DeleteLensNode()
//{
//	((GLLensRenderable*)lensRenderable)->RemoveLensNode();
//	UpdateStatusLabel();
//}
//
//void Window::RedoDeform()
//{
//	((GLLineRenderable*)lineRenderable)->RedoDeformation();
//	UpdateStatusLabel();
//}
//
//void Window::AddLens()
//{
//	openGL->AddLens();
//	lensWidSlider->setValue(((GLLensRenderable*)lensRenderable)->GetLensWidth() * 10);
//	UpdateStatusLabel();
//}
//
//void Window::SetToNavigation()
//{
//	openGL->SetInteractModeToTransform();
//	UpdateStatusLabel();
//}
//
////void Window::InteractModeChanged()
////{
////	if (addLensBtn->isChecked())
////	{
////	}
////	else if (viewBtn->isChecked())
////}
//
//void Window::AddLensNode()
//{
//	openGL->AddLensNode();
//	UpdateStatusLabel();
//}

Window::~Window() {
}

//QSlider* Window::CreateSliceSlider(TexPlaneRenderable* renderable)
//{
//	QSlider* slider;
//	slider = new QSlider(Qt::Horizontal);
//	slider->setRange(0, renderable->GetNumOfSlices() - 1);
//	slider->setValue(renderable->GetSliceNum());
//	return slider;
//}
//
//void Window::UpdateStatusLabel()
//{
//	QString status;
//	switch (openGL->GetInteractMode())
//	{
//	case INTERACT_MODE::ADD_NODE:
//		status = QString("status: Add lens or node");
//		break;
//	case INTERACT_MODE::MODIFY_LENS:
//		status = QString("status: Manipulate lens");
//		break;
//	case INTERACT_MODE::TRANSFORMATION:
//		status = QString("status: Navigation");
//		break;
//	}
//	statusLabel->setText(status);
//}
