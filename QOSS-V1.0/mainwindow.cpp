#include "mainwindow.h"
#include <Qt/include/PreDialog.h>
#include "Qt/include/ModelWizard.h"
#include "Qt/include/MirrorTypeWidget.h"
#include "Qt/include/CalculateFDTDThread.h"
#include "Qt/include/PhsCorrectionDialog.h"
#include "Qt/include/CalculatePhsCorThread.h"
#include "Qt/include/CalculatePVVAThread.h"

#include "VTK/include/Mirror.h"
#include "VTK/include/Restriction.h"
#include "VTK/include/MirrorFactory.h"
#include "VTK/include/LimitBox.h"
#include "VTK/include/LightShow.h"
#include "VTK/include/Radiator.h"

#include "util/Definition.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkOrientationMarkerWidget.h>
#include <QApplication>

#include <QMessageBox>
#include <thread>



using namespace userInterface;

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	tabWidget = new QTabWidget;
	tabWidget->addTab(&widget, QString::fromLocal8Bit("Main"));
	
	//tabWidget->setTabsClosable(true);
	setCentralWidget(tabWidget);

	//setCentralWidget(&widget);
	resize(1200, 800);

	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1.0, 1.0, 1.0);

	auto window = widget.GetRenderWindow();
	window->AddRenderer(renderer);

	PreDialog preDialog;
	if (preDialog.exec() != QDialog::Accepted)
	{
		exit(1);
	}
	myData = MyData::getInstance();

	if (0 == myData->getPattern()) // 低阶
	{
		showFDTDPtr = new showFDTD;
		tabWidget->addTab(showFDTDPtr, QString::fromLocal8Bit("Radiator"));

		//创建默认辐射器
		myData->createRadiator();
		renderer->AddActor(myData->getRadiator()->getActorModel());
		renderer->AddActor(myData->getRadiator()->getActorRay());

		if (preDialog.getIsModel())
		{
			// 创建默认的镜子
			myData->createModelMirror();
		}
		else
		{
			// 创建默认的镜子
			myData->createDefaultMirror();
		}

		//for (int i = 0; i < myData->getNumOfMirrors(); ++i)
		for (int i = 0; i < 3; ++i)
		{
			renderer->AddActor(myData->getMirrorByNum(i)->getActor());
		}

		// 加入限制盒子
		renderer->AddActor(myData->getLimitBox()->getActor());

		// 创建默认的光线
		myData->createDefaultLigthShow();
		std::list<vtkSmartPointer<vtkActor>> tempActors =
			myData->getDefaultLightShow()->getActors();
		for (auto& x : tempActors)
			renderer->AddActor(x);
	}
	else if(1 == myData->getPattern()) // 高阶
	{
		showDenisovPtr = new showDenisov;
		tabWidget->addTab(showDenisovPtr, QString::fromLocal8Bit("Radiator"));
		// 创建默认的镜子
		myData->createDefaultMirror();
		//for (int i = 0; i < myData->getNumOfMirrors(); ++i)
		for (int i = 0; i < 4; ++i)
		{
			renderer->AddActor(myData->getMirrorByNum(i)->getActor());
		}
	}
	else
	{
		// 创建默认的镜子
		myData->createDefaultMirror();
		GraphTrans graphTrans;
		graphTrans.setGraphTransPar(0, 0, 0, 0, 1, 0, 0);
		Mirror * mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01, 0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01, 0, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(0, 0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(0, 0.81*2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01, 0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01*2, 0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01 * 2, 0, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01 * 2, 0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01, -0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01*2, -0.81*2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01, -0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01 * 2, -0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01 * 2, 0, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(0, -0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01 * 2, -0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01, -0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01 * 2, 0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01 * 2, 0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01, 0.81 * 2, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01, 0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(-1.01, 0, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(0, -0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01 * 2, -0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());
		graphTrans.setGraphTransPar(1.01, -0.81, 0, 0, 1, 0, 0);
		mirror = MirrorFactory::getMirror(PLANEMIRROR, graphTrans);
		renderer->AddActor(mirror->getActor());

		renderer->AddActor(myData->getMirrorByNum(0)->getActor());
	}
	
	double axesScale = myData->getLimitBox()->getMaxSize();
	// 初始化vtk窗口
	axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);//修改X字体颜色为红色  
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 2, 0);//修改Y字体颜色为绿色  
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 3);//修改Z字体颜色为蓝色  

	axes->SetConeRadius(0.3);
	axes->SetConeResolution(20);
	//axes->SetTotalLength(10, 10, 10); //修改坐标尺寸
	
	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(window);

	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);
	interactor->Initialize();

	vtkCamera *aCamera = vtkCamera::New();
	aCamera->SetViewUp(0, 0, 1);//设视角位置 
	aCamera->SetPosition(0, -3 * axesScale, 0);//设观察对象位
	aCamera->SetFocalPoint(0, 0, 0);//设焦点 
	aCamera->ComputeViewPlaneNormal();//自动
	renderer->SetActiveCamera(aCamera);

	renderer->ResetCamera();
	window->Render();

	widget1 = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	widget1->SetOutlineColor(0.9300, 0.5700, 0.1300);
	widget1->SetOrientationMarker(axes);
	widget1->SetInteractor(interactor);
	widget1->SetViewport(0.0, 0.0, 0.25, 0.25);
	widget1->SetEnabled(1);
	widget1->InteractiveOff();

	// 设置dock窗口
	leftWidget = new QDockWidget("Navigation", this);
	leftWidget->setFeatures(QDockWidget::DockWidgetMovable);
	leftWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);

	// treeWidget
	treeWidget = new QTreeWidget;
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
		this, SLOT(on_treeWidget_ContextMenuRequested(QPoint)));
	connect(treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
		this, SLOT(on_treeWidget_leftPressed(QTreeWidgetItem*, int)));
	treeWidget->setHeaderHidden(true);  // 隐藏表头
	leftWidget->setWidget(treeWidget);

	//RightWidget
	detailsDockWidget = new QDockWidget("Details", this);
	detailsDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
	detailsDockWidget->setFeatures(QDockWidget::DockWidgetClosable);
	detailsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, detailsDockWidget);

	createTreeWidgetItem();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createRightMenu();
	createDetails();
	//creareWindows();

	init();

	//showDenisovPtr =  new showDenisov; 
	//showDenisovPtr->show();
}

mainWindow::~mainWindow()
{

}

void mainWindow::init()
{
	isExistenceOpenWin = false;
	isNew = true;
	fieldNum = 1;
	FDTDprogressDialog = nullptr;
	radiatorField = nullptr;
	PVVAprogressDialog = nullptr;
	phsCorprogressDialog = nullptr;
}

void mainWindow::createActions()
{
	//**************菜单*****************
	//保存
	saveFileAction = new QAction(QIcon(tr("Qt/images/save.png")), tr("Save"), this);
	saveFileAction->setShortcut(tr("Ctrl +　s"));
	saveFileAction->setStatusTip(tr("Save a file"));
	connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	//打开
	openFileAction = new QAction(QIcon(tr("Qt/images/open.png")), tr("Open"), this);
	openFileAction->setShortcut(tr("Ctrl +　O"));
	openFileAction->setStatusTip(tr("Open a file"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

	//新建
	newFileAction = new QAction(QIcon(tr("Qt/images/new.png")), tr("New"), this);
	newFileAction->setShortcut(tr("Ctrl +　N"));
	newFileAction->setStatusTip(tr("New a file"));
	connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

	// view 视图

	//初始化视角
	viewAction = new QAction(QIcon(tr("Qt/images/view.png")), tr("View"), this);
	viewAction->setStatusTip(tr("Initialize the view"));
	connect(viewAction, SIGNAL(triggered()), this, SLOT(viewInitFile()));

	//修改视角
	viewLabel = new QLabel(tr("View"));
	viewComboBox = new QComboBox;
	viewComboBox->addItem("View XZ plane(original)");
	viewComboBox->addItem("View YZ plane");
	viewComboBox->addItem("View XY plane");
	viewComboBox->addItem("View -YZ plane");
	viewComboBox->addItem("View -XZ plane");
	viewComboBox->addItem("View -XY plane");
	connect(viewComboBox, SIGNAL(activated(int)), this, SLOT(setView(int)));

	isShowBoxAction = new QAction(tr("Box"), this);
	isShowBoxAction->setStatusTip(tr("is show Box"));
	isShowBoxAction->setCheckable(true);
	isShowBoxAction->setChecked(true);
	connect(isShowBoxAction, SIGNAL(triggered()), this, SLOT(on_isShowBox()));

	//高斯源
	GaussianAction = new QAction(QIcon(tr("Qt/images/Gaussian.png")), tr("Gaussian"), this);
	GaussianAction->setStatusTip(tr("Create a Gaussian source"));
	connect(GaussianAction, SIGNAL(triggered()), this, SLOT(createGaussian()));

	//任意场
	ApertureFieldAction = new QAction(QIcon(tr("Qt/images/ApertureField.png")), 
		tr("Gaussian"), this);
	ApertureFieldAction->setStatusTip(tr("Create a ApertureField source"));
	connect(ApertureFieldAction, SIGNAL(triggered()), 
		this, SLOT(createApertureField()));

	// 
	PVVAAction = new QAction(QIcon(tr("Qt/images/PVVA.png")), tr("Fast calculation"), this);
	PVVAAction->setStatusTip(tr("Fast calculation by PVVA"));
	connect(PVVAAction, SIGNAL(triggered()), this, SLOT(on_PVVA()));

	FDTDAction = new QAction(QIcon(tr("Qt/images/FDTD.png")), tr("FDTD calculation"), this);
	FDTDAction->setStatusTip(tr("calculate Radiator by FDTD"));
	connect(FDTDAction, SIGNAL(triggered()), this, SLOT(on_FDTD()));

	loadFDTDAction = new QAction(QIcon(tr("Qt/images/loadFDTD.png")), tr("load FDTD"), this);
	loadFDTDAction->setStatusTip(tr("load FDTD"));
	loadFDTDAction->setEnabled(false);
	connect(loadFDTDAction, SIGNAL(triggered()), this, SLOT(loadFDTDField()));

	PhaseAction = new QAction(QIcon(tr("Qt/images/Phase.png")), tr("Phase Correction"), this);
	PhaseAction->setStatusTip(tr("Phase Correction"));
	//PhaseAction->setEnabled(false);
	connect(PhaseAction, SIGNAL(triggered()), this, SLOT(on_PhaseCor()));
}

void mainWindow::createMenus()
{
	// 文件菜单
	//this->menuBar()
	fileMenu = this->menuBar()->addMenu(tr("Files"));

	fileMenu->addAction(saveFileAction);
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(newFileAction);
	fileMenu->addSeparator();
	//fileMenu->addAction(LightSourceAction);

	viewMenu = this->menuBar()->addMenu(tr("View"));
	viewMenu->addAction(viewAction);

	viewMenu->addAction(isShowBoxAction);

	eidtMenu = this->menuBar()->addMenu(tr("Edit"));
	ModelMenu = this->menuBar()->addMenu(tr("Model"));
	SourceMenu = this->menuBar()->addMenu(tr("Source"));
	SourceMenu->addAction(GaussianAction);
	SourceMenu->addAction(ApertureFieldAction);
	SourceMenu->addAction(FDTDAction);
	SourceMenu->addAction(loadFDTDAction);

	CalMenu = this->menuBar()->addMenu(tr("simulation"));
	CalMenu->addAction(PVVAAction);
	CalMenu->addAction(PhaseAction);
}

void mainWindow::createToolBars()
{
	//file 栏
	fileTool = addToolBar("Files");
	fileTool->addAction(saveFileAction);
	fileTool->addAction(openFileAction);
	fileTool->addAction(newFileAction);

	fileTool->addSeparator();
	fileTool->addAction(viewAction);
	fileTool->addSeparator();
	fileTool->addWidget(viewLabel);
	fileTool->addWidget(viewComboBox);
	fileTool->addSeparator();
	fileTool->addAction(GaussianAction);
	fileTool->addAction(ApertureFieldAction);
	fileTool->addAction(FDTDAction);
	fileTool->addAction(loadFDTDAction);
	fileTool->addSeparator();
	fileTool->addAction(PVVAAction);
	fileTool->addAction(PhaseAction);
}

void mainWindow::createStatusBar()
{
	locationLabel = new QLabel("    ");
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());
	this->statusBar()->addWidget(locationLabel, 90);

	unitBtn = new QPushButton(tr("Unit: m"));
	connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	//connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	this->statusBar()->addWidget(unitBtn);
}

void mainWindow::createTreeWidgetItem()
{
	definitionsTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Definitions")));
	modelTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Model")));

	definitionsTreeItem->setExpanded(true);
	modelTreeItem->setExpanded(true);

	variablesTreeItem = new QTreeWidgetItem(QStringList(QString("Variables")));
	variablesTreeItem->setData(0, Qt::UserRole, QVariant(3));
	// 这里3是个标志

	QTreeWidgetItem *child1;
	child1 = new QTreeWidgetItem;
	child1->setText(0, tr("eps0") + tr(" = ") +
		tr("8.85418781761e-12"));
	variablesTreeItem->addChild(child1);

	QTreeWidgetItem *child2;
	child2 = new QTreeWidgetItem;
	child2->setText(0, tr("Pi") + tr(" = ") +
		tr("3.14159265358979"));
	variablesTreeItem->addChild(child2);

	QTreeWidgetItem *child3;
	child3 = new QTreeWidgetItem;
	child3->setText(0, tr("mu0") + tr(" = ") +
		tr("Pi*4e-7"));
	variablesTreeItem->addChild(child3);

	QTreeWidgetItem *child4;
	child4 = new QTreeWidgetItem;
	child4->setText(0, tr("c0") + tr(" = ") +
		tr("1/sqrt(eps0*mu0)"));
	variablesTreeItem->addChild(child4);

	QTreeWidgetItem *child5;
	child5 = new QTreeWidgetItem;
	child5->setText(0, tr("zf0") + tr(" = ") +
		tr("sqrt(mu0/eps0))"));
	variablesTreeItem->addChild(child5);
	definitionsTreeItem->addChild(variablesTreeItem);
	variablesTreeItem->setExpanded(true);

	sourceTreeItem = new QTreeWidgetItem(QStringList(QString("Source")));
	modelTreeItem->addChild(sourceTreeItem);
	QTreeWidgetItem *childFre = new QTreeWidgetItem;
	childFre->setText(0, tr("Fre = ") + QString::number(myData->getFrequency()));
	sourceTreeItem->addChild(childFre);

	// 模式
	QTreeWidgetItem *childPar = new QTreeWidgetItem;
	// 辐射器
	QTreeWidgetItem *childRadiator = new QTreeWidgetItem;

	if (0 == myData->getPattern())
	{
		childPar->setText(0, tr("Pattern: Lower order"));
		childRadiator->setText(0, tr("Vlasov Launcher"));
	}	
	else if (1 == myData->getPattern())
	{
		childPar->setText(0, tr("Pattern: Higher order"));
		childRadiator->setText(0, tr("Denisov Launcher"));
		childRadiator->setData(0, Qt::UserRole, QVariant(DENISOVVAL));
	}
	else
		childPar->setText(0, tr("Pattern: Waveguide"));


	sourceTreeItem->addChild(childPar);
	sourceTreeItem->addChild(childRadiator);


	sourceTreeItem->setExpanded(true);

	geometryTreeItem = new QTreeWidgetItem(QStringList(QString("Geometry")));
	modelTreeItem->addChild(geometryTreeItem);

	// 镜子的tree
	for (int i = 0; i < myData->getNumOfMirrors(); ++i)
	{
		QTreeWidgetItem *childMirror = new QTreeWidgetItem;
		childMirror->setText(0, tr("Mirror") + QString::number(i+1));
		childMirror->setData(0, Qt::UserRole, QVariant(0));
		childMirror->setData(1, Qt::UserRole, QVariant(i)); 
		childMirror->setData(2, Qt::UserRole, QVariant(i));
		geometryTreeItem->addChild(childMirror);
		childMirror->addChild(myData->getMirrorByNum(i)->getTree());
		childMirror->child(0)->setData(2, Qt::UserRole, QVariant(i));
		mirrorTreeWidgetItem.push_back(childMirror);
		childMirror->setExpanded(true);
		if (myData->getMirrorByNum(i)->getRestriction(0))
		{
			childMirror->addChild(myData->getMirrorByNum(i)->getRestriction(0)->getTree());
		}
	}

	// 盒子的tree
	QTreeWidgetItem *childBox = new QTreeWidgetItem;
	childBox->setText(0, tr("LimitBox"));
	geometryTreeItem->addChild(childBox);
	QTreeWidgetItem *childBoxX = new QTreeWidgetItem;
	childBoxX->setText(0, tr("X = ") + QString::number(myData->getLimitBox()->getLength()));
	childBox->addChild(childBoxX);
	QTreeWidgetItem *childBoxY = new QTreeWidgetItem;
	childBoxY->setText(0, tr("Y = ") + QString::number(myData->getLimitBox()->getHeight()));
	childBox->addChild(childBoxY);
	QTreeWidgetItem *childBoxZ = new QTreeWidgetItem;
	childBoxZ->setText(0, tr("Z = ") + QString::number(myData->getLimitBox()->getWidth()));
	childBox->addChild(childBoxZ);
	childBox->setExpanded(true);

	geometryTreeItem->setExpanded(true);

	lightTreeItem = new QTreeWidgetItem(QStringList(QString("Light")));
	modelTreeItem->addChild(lightTreeItem);

	fieldTreeItem = new QTreeWidgetItem(QStringList(QString("Field")));
	modelTreeItem->addChild(fieldTreeItem);
	soucreFieldTreeItem = new QTreeWidgetItem(QStringList(QString("SourceField")));
	soucreFieldTreeItem->setData(0, Qt::UserRole, QVariant(FIELD));
	soucreFieldTreeItem->setData(1, Qt::UserRole, QVariant(0));
	fieldTreeItem->addChild(soucreFieldTreeItem);

}

void mainWindow::createRightMenu()
{
	R_Tree_MirrorTypeMenu = new QMenu(this);
	R_BlankMenu = new QMenu(this);

	modifyingMirrorAction = new QAction(tr("Modifying the mirror type"), this);
	QFont font("Microsoft YaHei", 10, 75);
	modifyingMirrorAction->setFont(font);
	modifyingMirrorAction->setStatusTip(tr("Modifying the mirror type"));
	connect(modifyingMirrorAction, SIGNAL(triggered()), this, SLOT(on_modifyingMirror()));

	restrictionAction = new QAction(tr("Add restriction"), this);
	restrictionAction->setFont(font);
	restrictionAction->setStatusTip(tr("Add restriction"));
	connect(restrictionAction, SIGNAL(triggered()), this, SLOT(on_restriction()));

	isShowMirrorAction = new QAction(tr("Show"), this);
	isShowMirrorAction->setStatusTip(tr("isShow"));
	isShowMirrorAction->setCheckable(true);
	isShowMirrorAction->setChecked(true);
	connect(isShowMirrorAction, SIGNAL(triggered()), this, SLOT(on_isShowMirror()));

	isTransparentAction = new QAction(tr("Transparent"), this);
	isTransparentAction->setStatusTip(tr("isTransparent"));
	isTransparentAction->setCheckable(true);
	isTransparentAction->setChecked(false);
	connect(isTransparentAction, SIGNAL(triggered()), this, SLOT(on_isTransparentMirror()));

	saveSTLction = new QAction(tr("Save STL"), this);
	saveSTLction->setStatusTip(tr("Save STL"));
	connect(saveSTLction, SIGNAL(triggered()), this, SLOT(on_saveSTL()));

	R_Tree_MirrorTypeMenu->addAction(modifyingMirrorAction);
	R_Tree_MirrorTypeMenu->addAction(restrictionAction);
	R_Tree_MirrorTypeMenu->addAction(isShowMirrorAction);
	R_Tree_MirrorTypeMenu->addAction(isTransparentAction);
	R_Tree_MirrorTypeMenu->addAction(saveSTLction);

	R_Tree_MirrorParMenu = new QMenu(this);
	modifyParametersAction = new QAction(tr("Modifying parameters"), this);
	modifyParametersAction->setFont(font);
	modifyParametersAction->setStatusTip(tr("Modifying parameters"));
	connect(modifyParametersAction, SIGNAL(triggered()), this, SLOT(on_modifyParameters()));

	R_Tree_MirrorParMenu->addAction(modifyParametersAction);

	R_Tree_RestrictionMenu = new QMenu(this);

	modifyingRestrictionAction = new QAction(tr("Modifying Restriction"), this);
	modifyingRestrictionAction->setStatusTip(tr("Modifying Restriction"));
	connect(modifyingRestrictionAction, SIGNAL(triggered()), this, SLOT(on_modifyingRestriction()));

	delRestrictionAction = new QAction(tr("Delete Restriction"), this);
	delRestrictionAction->setStatusTip(tr("Delete Restriction"));
	connect(delRestrictionAction, SIGNAL(triggered()), this, SLOT(on_delRestriction()));

	R_Tree_RestrictionMenu->addAction(modifyingRestrictionAction);
	R_Tree_RestrictionMenu->addAction(delRestrictionAction);

	radiatorParametersAction = new QAction(tr("Modifying Restriction"), this);
	radiatorParametersAction->setStatusTip(tr("Modifying Restriction"));
	connect(radiatorParametersAction, SIGNAL(triggered()), this, SLOT(on_DenisovParameters()));

	R_Tree_RadiatorMenu = new QMenu(this);
	R_Tree_RadiatorMenu->addAction(radiatorParametersAction);

}

void mainWindow::createDetails()
{
	dimensionGroupBtn = new QButtonGroup();
	ThreeDBtn = new QRadioButton(tr("3D"));
	TwoDBtn = new QRadioButton(tr("2D"));
	dimensionGroupBtn->addButton(ThreeDBtn, 0);
	dimensionGroupBtn->addButton(TwoDBtn, 1);
	connect(ThreeDBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(TwoDBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * dimensionLayout = new QGridLayout;
	dimensionLayout->addWidget(ThreeDBtn, 0, 0);
	dimensionLayout->addWidget(TwoDBtn, 0, 1);

	QGroupBox * dimensionGroupBox = new QGroupBox;
	dimensionGroupBox->setTitle(tr("dimension"));
	dimensionGroupBox->setLayout(dimensionLayout);

	fieldGroupBtn = new QButtonGroup();
	ExBtn = new QRadioButton(tr("Ex"));
	EyBtn = new QRadioButton(tr("Ey"));
	EzBtn = new QRadioButton(tr("Ez"));
	HxBtn = new QRadioButton(tr("Hx"));
	HyBtn = new QRadioButton(tr("Hy"));
	HzBtn = new QRadioButton(tr("Hz"));

	fieldGroupBtn->addButton(ExBtn, 0);
	fieldGroupBtn->addButton(EyBtn, 1);
	fieldGroupBtn->addButton(EzBtn, 2);
	fieldGroupBtn->addButton(HxBtn, 3);
	fieldGroupBtn->addButton(HyBtn, 4);
	fieldGroupBtn->addButton(HzBtn, 5);

	connect(ExBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(EyBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(EzBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HxBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HyBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HzBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * fieldLayout = new QGridLayout;
	fieldLayout->addWidget(ExBtn, 0, 0);
	fieldLayout->addWidget(EyBtn, 1, 0);
	fieldLayout->addWidget(EzBtn, 2, 0);
	fieldLayout->addWidget(HxBtn, 0, 1);
	fieldLayout->addWidget(HyBtn, 1, 1);
	fieldLayout->addWidget(HzBtn, 2, 1);

	QGroupBox * fieldGroupBox = new QGroupBox;
	fieldGroupBox->setTitle(tr("field"));
	fieldGroupBox->setLayout(fieldLayout);

	// pmGroupBox
	pmGroupBtn = new QButtonGroup();
	magnitudeBtn = new QRadioButton(tr("magnitude"));
	phaseBtn = new QRadioButton(tr("phase"));

	pmGroupBtn->addButton(magnitudeBtn, 0);
	pmGroupBtn->addButton(phaseBtn, 1);

	connect(magnitudeBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(phaseBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * pmLayout = new QGridLayout;
	pmLayout->addWidget(magnitudeBtn, 0, 0);
	pmLayout->addWidget(phaseBtn, 0, 1);

	QGroupBox * pmGroupBox = new QGroupBox;
	pmGroupBox->setTitle(tr("parameter"));
	pmGroupBox->setLayout(pmLayout);

	// pmGroupBox
	powerGroupBtn = new QButtonGroup();
	linearBtn = new QRadioButton(tr("linear"));
	dbBtn = new QRadioButton(tr("dB"));
	powerGroupBtn->addButton(dbBtn, 0);
	powerGroupBtn->addButton(linearBtn, 1);

	QGridLayout * powerLayout = new QGridLayout;
	powerLayout->addWidget(linearBtn, 0, 0);
	powerLayout->addWidget(dbBtn, 0, 1);

	connect(linearBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(dbBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	// powerGroupBox
	QGroupBox * powerGroupBox = new QGroupBox;
	powerGroupBox->setTitle(tr("power"));
	powerGroupBox->setLayout(powerLayout);

	QLabel * effLabel = new QLabel(tr("Energy conversion efficiency:"));
	effLabelVal = new QLabel(tr(""));
	QLabel * scaleLabel = new QLabel(tr("Gauss scale coefficient:"));
	scaleLabelVal = new QLabel(tr(""));
	QLabel * vecLabel = new QLabel(tr("Gauss vector coefficient:"));
	vecLabelVal = new QLabel(tr(""));

	QGridLayout * coefficientLayout = new QGridLayout;
	coefficientLayout->addWidget(effLabel, 0, 0);
	coefficientLayout->addWidget(effLabelVal, 0, 1);
	coefficientLayout->addWidget(scaleLabel, 1, 0);
	coefficientLayout->addWidget(scaleLabelVal, 1, 1);
	coefficientLayout->addWidget(vecLabel, 2, 0);
	coefficientLayout->addWidget(vecLabelVal, 2, 1);

	QGroupBox * coefficientGroupBox = new QGroupBox;
	coefficientGroupBox->setTitle(tr("Coefficient"));
	coefficientGroupBox->setLayout(coefficientLayout);

	QVBoxLayout * boxLayout = new QVBoxLayout;
	boxLayout->addWidget(dimensionGroupBox);
	boxLayout->addWidget(fieldGroupBox);
	boxLayout->addWidget(pmGroupBox);
	boxLayout->addWidget(powerGroupBox);
	boxLayout->addWidget(coefficientGroupBox);

	detailsWidget = new QWidget;
	detailsWidget->setLayout(boxLayout);
	detailsDockWidget->setWidget(detailsWidget);
	detailsDockWidget->close();
}

void mainWindow::createProject()
{

}

void mainWindow::isNeedSave()
{
	MyData * mydata = MyData::getInstance();
	if (mydata->getModifiedFlag())
	{
		QMessageBox::StandardButton rb = QMessageBox::question(NULL, "question", "The data has been changed. Is it saved?",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (rb == QMessageBox::Yes)
		{
			QMessageBox::aboutQt(NULL, "About Qt");
		}
	}
}

// -------------------- slots 函数 ------------------------------------

void mainWindow::openFile()
{
	isNeedSave();

}

void mainWindow::newFile()
{
	isNeedSave();
	ModelWizard modelWizard;
	if (modelWizard.exec() != QDialog::Accepted)
	{
		return;
	}

}

void mainWindow::viewInitFile()
{
	double axesScale = myData->getLimitBox()->getMaxSize();
	vtkCamera *aCamera = vtkCamera::New();

	aCamera->SetViewUp(0, 0, 1);//设视角位置 
	aCamera->SetPosition(0, -3 * axesScale, 0);//设观察对象位
	
	aCamera->SetFocalPoint(0, 0, 0);//设焦点 
	aCamera->ComputeViewPlaneNormal();//自动
	renderer->SetActiveCamera(aCamera);
	renderer->ResetCamera();
	updateVtk();
	viewComboBox->setCurrentIndex(0);
}

void mainWindow::setView(int index)
{
	double axesScale = myData->getLimitBox()->getMaxSize();
	vtkCamera *aCamera = vtkCamera::New();

	switch (index)
	{
	case 0: // xz
		aCamera->SetViewUp(0, 0, 1);//设视角位置 
		aCamera->SetPosition(0, -3 * axesScale, 0);//设观察对象位
		break;
	case 1: // yz
		aCamera->SetViewUp(0, 1, 0);//设视角位置
		aCamera->SetPosition(-3 * axesScale, 0, 0);//设观察对象位
		break;
	case 2: // xy
		aCamera->SetViewUp(1, 0, 0);//设视角位置 
		aCamera->SetPosition(0, 0, -3 * axesScale);//设观察对象位
		break;
	case 3: // -yz
		aCamera->SetViewUp(0, 0, 1);//设视角位置
		aCamera->SetPosition(-3 * axesScale, 0, 0);//设观察对象位
		break;
	case 4:// -xz
		aCamera->SetViewUp(0, 0, 1);//设视角位置 
		aCamera->SetPosition(0, 3 * axesScale, 0);//设观察对象位
		break;
	case 5:// -xy
		aCamera->SetViewUp(0, 1, 0);//设视角位置 
		aCamera->SetPosition(0, 0, -3 * axesScale);//设观察对象位
		break;
	default:
		break;
	}

	aCamera->SetFocalPoint(0, 0, 0);//设焦点 
	aCamera->ComputeViewPlaneNormal();//自动
	renderer->SetActiveCamera(aCamera);
	renderer->ResetCamera();
	updateVtk();
}

void mainWindow::on_isShowBox()
{
	//isShowBoxAction->isChecked();
	myData->getLimitBox()->setIsTransparent(isShowBoxAction->isChecked());

	updateVtk();
}

void mainWindow::on_isTransparentMirror()
{
	int index = rightSelectItem->data(1, Qt::UserRole).toInt();
	myData->getMirrorByNum(index)->switchIsTransparent();
	updateVtk();
}

void mainWindow::on_isShowMirror()
{
	int index = rightSelectItem->data(1, Qt::UserRole).toInt();
	myData->getMirrorByNum(index)->switchIsShow();
	updateVtk();
}

void mainWindow::on_modifyingRestriction()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	int index1 = rightSelectItem->data(2, Qt::UserRole).toInt();
	int indexRestriction = rightSelectItem->data(3, Qt::UserRole).toInt();
	Restriction* tempPtr = myData->getMirrorByNum(index1)
		->getRestriction(indexRestriction - 1);
	isNew = false;
	tempRestriction = new Restriction(*tempPtr);
	on_restriction();
}

void mainWindow::on_delRestriction()
{
	int index = rightSelectItem->data(2, Qt::UserRole).toInt();
	int index2 = rightSelectItem->data(3, Qt::UserRole).toInt();
	myData->getMirrorByNum(index)->removeRestriction(index2-1);
	// 对后面的所有Restriction重新编码
	int tempNum = mirrorTreeWidgetItem[index]->childCount();
	for (int i = index2 + 1; i < tempNum; ++i)
	{
		mirrorTreeWidgetItem[index]->child(i)->setData(3,
			Qt::UserRole, QVariant(i-1));
	}
	mirrorTreeWidgetItem[index]->removeChild(mirrorTreeWidgetItem[index]->child(index2));
	updateLight();
	updateVtk();
}

void mainWindow::on_modifyingMirror()
{
	MirrorTypeWidget mirrorTypeWidget;
	connect(&mirrorTypeWidget, SIGNAL(sendMirrorType(int)),
		this, SLOT(toReceiveMirrorType(int)));

	if (mirrorTypeWidget.exec() != QDialog::Accepted)
	{
		return;
	}
}

void mainWindow::on_modifyParameters()
{
	int varInt = rightSelectItem->data(1, Qt::UserRole).toInt();
	int index = rightSelectItem->data(2, Qt::UserRole).toInt();
	renderer->RemoveActor(myData->getMirrorByNum(index)->getActor());
	switch (varInt)
	{
	case PARABOLOID:
		tempMirror = MirrorFactory::cloneMirror(myData->getMirrorByNum(index));
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createParaboloid();
		break;
	case PARABOLICCYLINDER:
		tempMirror = MirrorFactory::cloneMirror(myData->getMirrorByNum(index));
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createParabolicCylinder();
		break;
	case PLANEMIRROR:
		tempMirror = MirrorFactory::cloneMirror(myData->getMirrorByNum(index));
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createPlaneMirror();
		break;
	case ELLIPSOID:
		tempMirror = MirrorFactory::cloneMirror(myData->getMirrorByNum(index));
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createEllipsoid();
		break;
	default:
		break;
	}
	updateVtk();
}

void mainWindow::on_restriction()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	if(isNew)
		tempRestriction = new Restriction;
	renderer->AddActor(tempRestriction->getActor());
	restrictionWidget = new RestrictionWidget();
	restrictionWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(restrictionWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveRestriction(int)));

	restrictionWidget->setRestriction(tempRestriction);
	restrictionWidget->show();
	isExistenceOpenWin = true;
}

void mainWindow::toReceiveRestriction(int index)
{
	if (1 == index) // 点击确认
	{
		int index1; 
		
		if (!isNew)
		{
			index1 = rightSelectItem->data(2, Qt::UserRole).toInt();
			int indexRestriction = rightSelectItem->data(3, Qt::UserRole).toInt();
			myData->getMirrorByNum(index1)->setRestriction(indexRestriction - 1,
				tempRestriction);
			mirrorTreeWidgetItem[index1]->insertChild(indexRestriction,
				tempRestriction->getTree());
			mirrorTreeWidgetItem[index1]->removeChild(
				mirrorTreeWidgetItem[index1]->child(indexRestriction));

			isNew = true;
		}
		else
		{
			index1 = rightSelectItem->data(1, Qt::UserRole).toInt();
			// 加入tree 中并编码
			myData->getMirrorByNum(index1)->addRestriction(tempRestriction);

			int tempNum = mirrorTreeWidgetItem[index1]->childCount();
			mirrorTreeWidgetItem[index1]->addChild(tempRestriction->getTree());
			mirrorTreeWidgetItem[index1]->child(tempNum)->setData(2,
				Qt::UserRole, QVariant(index1));
			mirrorTreeWidgetItem[index1]->child(tempNum)->setData(3,
				Qt::UserRole, QVariant(tempNum));
		}

		renderer->RemoveActor(myData->getMirrorByNum(index1)->getActor());
		renderer->RemoveActor(tempRestriction->getActor());

		renderer->AddActor(myData->getMirrorByNum(index1)->getActor());

		tempRestriction = nullptr;
		delete restrictionWidget;
		restrictionWidget = nullptr;
		isExistenceOpenWin = false;
		mirrorTreeWidgetItem[index1]->setExpanded(true);
		updateLight();
	}
	else if (0 == index)// 点击取消
	{
		renderer->RemoveActor(tempRestriction->getActor());
	
		delete tempRestriction;
		tempRestriction = nullptr;
		delete restrictionWidget;
		restrictionWidget = nullptr;
		isExistenceOpenWin = false;
		if (!isNew)
		{
			isNew = true;
		}
	}
	updateVtk();
}

void mainWindow::createApertureField()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempMirror = MirrorFactory::getMirror(PLANEMIRROR, GraphTrans());
	tempMirror->setSelected(true);
	renderer->AddActor(tempMirror->getActor());
	apertureFieldWidget = new ApertureFieldWidget;
	apertureFieldWidget->setMirror(tempMirror);
	apertureFieldWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	apertureFieldWidget->show();

	connect(apertureFieldWidget, SIGNAL(sendData(int)),
		this, SLOT(toApertureField(int)));

	isExistenceOpenWin = true;
}

void mainWindow::toApertureField(int caseIndex)
{
	if (1 == caseIndex)
	{
		if (nullptr != myData->getSourceField()) // 如果已有源了 则会覆盖以前的源
		{
			// 判断是否保留原来的限制条件
			switch (QMessageBox::question(this, tr("Question"),
				tr("Whether or not to cover the original field?"),
				QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Ok))
			{
			case QMessageBox::Ok:
				renderer->RemoveActor(myData->getSourceField()->getActor());
				renderer->RemoveActor(myData->getSourceField()->getActor3D());
				soucreFieldTreeItem->removeChild(soucreFieldTreeItem->child(0));
				break;
			case QMessageBox::No:
				toApertureField(0);
				break;
			case QMessageBox::Cancel:
				return;
			default:
				break;
			}
		}
		Field * temPtr;
		if (!apertureFieldWidget->getField(temPtr))
		{
			return;
		}
		renderer->AddActor(temPtr->getActor());

		soucreFieldTreeItem->addChild(temPtr->getTree());
		myData->setSourceField(temPtr);
		toApertureField(0);

		isExistenceOpenWin = false;
	}
	else if (0 == caseIndex)// 点击取消
	{
		renderer->RemoveActor(tempMirror->getActor());

		delete tempMirror;
		tempMirror = nullptr;
		delete apertureFieldWidget;
		apertureFieldWidget = nullptr;
		isExistenceOpenWin = false;
	}

	updateVtk();
}

void mainWindow::createGaussian()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempMirror = MirrorFactory::getMirror(PLANEMIRROR, GraphTrans());
	tempMirror->setSelected(true);
	renderer->AddActor(tempMirror->getActor());
	gaussianWidget = new GaussianWidget;
	gaussianWidget->setMirror(tempMirror);
	gaussianWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	gaussianWidget->show();

	connect(gaussianWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveGaussian(int)));

	isExistenceOpenWin = true;
}

void mainWindow::toReceiveGaussian(int caseIndex)
{
	if (1 == caseIndex)
	{
		if (nullptr != myData->getSourceField()) // 如果已有源了 则会覆盖以前的源
		{
			// 判断是否保留原来的限制条件
			switch (QMessageBox::question(this, tr("Question"),
				tr("Whether or not to cover the original field?"),
				QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Ok))
			{
			case QMessageBox::Ok:
				renderer->RemoveActor(myData->getSourceField()->getActor());
				renderer->RemoveActor(myData->getSourceField()->getActor3D());
				soucreFieldTreeItem->removeChild(soucreFieldTreeItem->child(0));
				break;
			case QMessageBox::No:
				toReceiveGaussian(0);
				break;
			case QMessageBox::Cancel:
				return;
			default:
				break;
			}
		}
		Field * temPtr;
		if (!gaussianWidget->getField(temPtr))
		{
			return;
		}
		renderer->AddActor(temPtr->getActor());

		soucreFieldTreeItem->addChild(temPtr->getTree());
		myData->setSourceField(temPtr);
		toReceiveGaussian(0);
		isExistenceOpenWin = false;
	}
	else if (0 == caseIndex)// 点击取消
	{
		renderer->RemoveActor(tempMirror->getActor());

		delete tempMirror;
		tempMirror = nullptr;
		delete gaussianWidget;
		gaussianWidget = nullptr;
		isExistenceOpenWin = false;
	}

	updateVtk();
}

void mainWindow::on_createParaboloid()
{ 
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempWidget = new ParaboloidWidget();
	tempWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶

	connect(tempWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveMirror(int)));

	dynamic_cast<ParaboloidWidget*>(tempWidget)->setMirror(tempMirror);
	tempWidget->show();
	isExistenceOpenWin = true;
	
}

void mainWindow::on_createParabolicCylinder()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempWidget = new ParabolicCylinderWidget();
	tempWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶

	connect(tempWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveMirror(int)));

	dynamic_cast<ParabolicCylinderWidget*>(tempWidget)->setMirror(tempMirror);
	tempWidget->show();
	isExistenceOpenWin = true;
}

void mainWindow::on_createEllipsoid()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempWidget = new EllipsoidWidget();
	tempWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶

	connect(tempWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveMirror(int)));

	dynamic_cast<EllipsoidWidget*>(tempWidget)->setMirror(tempMirror);
	tempWidget->show();
	isExistenceOpenWin = true;
}

void mainWindow::on_createPlaneMirror()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempWidget = new PlaneMirrorWidget();
	tempWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶

	connect(tempWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveMirror(int)));

	dynamic_cast<PlaneMirrorWidget*>(tempWidget)->setMirror(tempMirror);
	tempWidget->show();
	isExistenceOpenWin = true;
}

void mainWindow::on_createSTLMirror()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	tempWidget = new STLMirrorWidget();
	tempWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶

	connect(tempWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveMirror(int)));

	dynamic_cast<STLMirrorWidget*>(tempWidget)->setMirror(tempMirror);
	tempWidget->show();
	isExistenceOpenWin = true;
}

void mainWindow::on_saveSTL()
{
	int index = rightSelectItem->data(1, Qt::UserRole).toInt();
	myData->getMirrorByNum(index)->saveSTL();
}

void mainWindow::toReceiveMirror(int caseIndex)
{
	if (1 == caseIndex) // 点击确认
	{
		int index1 = rightSelectItem->data(2, Qt::UserRole).toInt();

		bool isFlag = false;
		// 判断是否保留原来的限制条件
		if (nullptr != myData->getMirrorByNum(index1)->getRestriction(0))
		{
			switch (QMessageBox::question(this, tr("Question"),
				tr("Whether or not the existing restrictions are retained?"),
				QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Ok))
			{
			case QMessageBox::Ok:
				//displayTextEdit->setText(tr("询问按钮 / 确定"));
				tempMirror->moveRestriction(myData->getMirrorByNum(index1));
				isFlag = true;
				break;
			case QMessageBox::No:
				isFlag = false;
				break;
			case QMessageBox::Cancel:
				return;
			default:
				break;
			}
		}
		tempMirror->setSelected(false);
		if (isFlag) // 保留
		{
			mirrorTreeWidgetItem[index1]->removeChild(mirrorTreeWidgetItem[index1]->child(0));
		}
		else // 不保留
		{
			QTreeWidgetItem *childMirror = new QTreeWidgetItem;
			childMirror->setText(0, tr("Mirror") + QString::number(index1 + 1));
			childMirror->setData(0, Qt::UserRole, QVariant(0));
			childMirror->setData(1, Qt::UserRole, QVariant(index1));
			childMirror->setData(2, Qt::UserRole, QVariant(index1));

			geometryTreeItem->removeChild(mirrorTreeWidgetItem[index1]);
			geometryTreeItem->insertChild(index1, childMirror);
			delete mirrorTreeWidgetItem[index1];
			mirrorTreeWidgetItem[index1] = childMirror;
		}

		myData->setMirror(index1, tempMirror);
		mirrorTreeWidgetItem[index1]->insertChild(0, tempMirror->getTree());
		mirrorTreeWidgetItem[index1]->child(0)->setData(2,
			Qt::UserRole, QVariant(index1));
		mirrorTreeWidgetItem[index1]->setExpanded(true);

		renderer->RemoveActor(tempMirror->getActorAxes());
		if (tempMirror->getMirrorsType() == STLMIRROR)
		{
			renderer->AddActor(tempMirror->getActor());
		}

		tempMirror = nullptr;
		delete tempWidget;
		tempWidget = nullptr;
		isExistenceOpenWin = false;
		rightSelectItem = nullptr;
		updateLight();
	}
	else if (0 == caseIndex)// 点击取消
	{
		renderer->RemoveActor(tempMirror->getActor());
		renderer->RemoveActor(tempMirror->getActorAxes());
		int index1 = rightSelectItem->data(2, Qt::UserRole).toInt();

		renderer->AddActor(myData->getMirrorByNum(index1)->getActor());
		
		delete tempMirror;
		tempMirror = nullptr;
		delete tempWidget;
		tempWidget = nullptr;
		rightSelectItem = nullptr;
		isExistenceOpenWin = false;
	}
	updateVtk();
}

void mainWindow::toReceiveMirrorType(int caseInd)
{
	int index = rightSelectItem->data(2, Qt::UserRole).toInt();
	renderer->RemoveActor(myData->getMirrorByNum(index)->getActor());
	switch (caseInd)
	{
	case PARABOLOID:
		tempMirror = MirrorFactory::getMirror(PARABOLOID,GraphTrans());
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createParaboloid();
		break;
	case PARABOLICCYLINDER:
		tempMirror = MirrorFactory::getMirror(PARABOLICCYLINDER, GraphTrans());
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createParabolicCylinder();
		break;
	case PLANEMIRROR:
		tempMirror = MirrorFactory::getMirror(PLANEMIRROR, GraphTrans());
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createPlaneMirror();
		break;
	case ELLIPSOID:
		tempMirror = MirrorFactory::getMirror(ELLIPSOID, GraphTrans());
		tempMirror->setSelected(true);
		renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createEllipsoid();
		break;
	case STLMIRROR:
		tempMirror = MirrorFactory::getMirror(STLMIRROR, GraphTrans());
		tempMirror->setSelected(true);
		//renderer->AddActor(tempMirror->getActor());
		renderer->AddActor(tempMirror->getActorAxes());
		on_createSTLMirror();
		break;
	default:
		break;
	}
}

void mainWindow::on_PVVA()
{
	if (PVVAprogressDialog)
	{
		PVVAprogressDialog->show();
		return;
	}
	if (!myData->getSourceField()) // 如果没有设置源 不能计算
	{
		switch (QMessageBox::question(this, tr("Question"),
			tr("The source that can be used is not generated. Whether or not the source is generated now?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
		{
		case QMessageBox::Yes:
			on_FDTD();
			return;
		case QMessageBox::No:
			return;
		default:
			break;
		}
	}
	CalculationWidget calculationDialog;
	calculationDialog.setFre(myData->getFrequency());
	if (calculationDialog.exec() != QDialog::Accepted)
	{
		return;
	}
	double dis = calculationDialog.getDistance();
	double fre = calculationDialog.getFre();
	int numMirror = calculationDialog.getMirrorNum();

	PVVAprogressDialog = new PVVAProgressDialog();
	PVVAprogressDialog->setMirrorNum(numMirror);
	PVVAprogressDialog->show();

	CalculatePVVAThread *calThr = new CalculatePVVAThread(fre, dis, numMirror);
	connect(PVVAprogressDialog, SIGNAL(sendStop()), this, SLOT(toReceiveFDTDStop()));
	connect(PVVAprogressDialog, SIGNAL(sendStop()), calThr, SLOT(killFDTD()));

	connect(calThr, SIGNAL(sendMainValue(int)), PVVAprogressDialog, SLOT(setMainValue(int)));
	connect(calThr, SIGNAL(sendSlaverValue(int)), PVVAprogressDialog, SLOT(setSlaverValue(int)));
	connect(calThr, SIGNAL(finished()), calThr, SLOT(deleteLater()));
	connect(calThr, SIGNAL(sendField(Field *)), this, SLOT(toReceivePVVAField(Field *)));

	calThr->start();

	//myData->
}

void mainWindow::on_FDTD()
{
	if (FDTDprogressDialog)
	{
		FDTDprogressDialog->show();
		return;
	}
	if (radiatorField)
	{
		switch (QMessageBox::question(this, tr("Question"),
			tr("An unloaded source has already existed and will be covered after calculation.Whether to continue ?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
		{
		case QMessageBox::Yes: // 加载源
			delete radiatorField;
			radiatorField = nullptr;
			break;
		case QMessageBox::No: //  不加载源
			return;
		default:
			break;
		}
	}
	FDTDprogressDialog = new FDTDProgressDialog();
	FDTDprogressDialog->show();
	FDTDradiator = new FDTDRadiator;
	radiatorField = new Field;
	CalculateFDTDThread *calThr = new CalculateFDTDThread(FDTDradiator, radiatorField);
	connect(FDTDprogressDialog, SIGNAL(sendStop()), this, SLOT(toReceiveFDTDStop()));
	connect(FDTDprogressDialog, SIGNAL(sendStop()), calThr, SLOT(killFDTD()));

	connect(calThr, SIGNAL(sendMainValue(int)), FDTDprogressDialog, SLOT(setMainValue(int)));
	connect(calThr, SIGNAL(sendSlaverValue(int)), FDTDprogressDialog, SLOT(setSlaverValue(int)));
	connect(calThr, SIGNAL(finished()), calThr, SLOT(deleteLater()));
	connect(calThr, SIGNAL(finished()), this, SLOT(toReceiveFDTD()));

	calThr->start();
}

void mainWindow::toReceiveFDTD()
{
	switch (QMessageBox::question(this, tr("Question"),
		tr("The radiator has been calculated, and whether the latest source is loaded now?"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
	{
	case QMessageBox::Yes: // 加载源
		loadFDTDField();
		break;
	case QMessageBox::No: //  不加载源
		loadFDTDAction->setEnabled(true);
		break;
	default:
		break;
	}
	delete FDTDprogressDialog;
	FDTDprogressDialog = nullptr;
}

void mainWindow::loadFDTDField()
{
	if (nullptr != myData->getSourceField()) // 如果已有源了 则会覆盖以前的源
	{
		// 判断是否保留原来的限制条件
		switch (QMessageBox::question(this, tr("Question"),
			tr("Whether or not to cover the original field?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
		{
		case QMessageBox::Yes:
			renderer->RemoveActor(myData->getSourceField()->getActor());
			renderer->RemoveActor(myData->getSourceField()->getActor3D());
			soucreFieldTreeItem->removeChild(soucreFieldTreeItem->child(0));
			break;
		case QMessageBox::No:
			return;
		default:
			break;
		}
	}
	radiatorField->updateData();
	renderer->AddActor(radiatorField->getActor());

	soucreFieldTreeItem->addChild(radiatorField->getTree());
	myData->setSourceField(radiatorField);
	radiatorField = nullptr;
	loadFDTDAction->setEnabled(false);
}

void mainWindow::toReceiveFDTDStop()
{
	delete radiatorField;
	radiatorField = nullptr;
	delete FDTDprogressDialog;
	FDTDprogressDialog = nullptr;
}

void mainWindow::on_PhaseCor()
{
	if (phsCorprogressDialog)
	{
		phsCorprogressDialog->show();
	}

	if (!myData->getSourceField()) // 如果没有设置源 不能计算
	{
		switch (QMessageBox::question(this, tr("Question"),
			tr("The source that can be used is not generated. Whether or not the source is generated now?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
		{
		case QMessageBox::Yes:
			on_FDTD();
			return;
		case QMessageBox::No:
			return;
		default:
			break;
		}
	}
	MirrorsType tempType = myData->getMirrorByNum(myData->getNumOfMirrors() - 1)->getMirrorsType();
	bool isNeedMesh = false;
	switch (tempType)
	{
	case QUADRICSURFACE:
	case PARABOLICCYLINDER:
	case PARABOLOID:
	case ELLIPSOID:
		isNeedMesh = true;
		break;
	case PHSCORMIRROR:
		isNeedMesh = false;
		break;
	default:
		QMessageBox::warning(NULL, "Warning",
			"Unsupported mirror type!");
		return;
	}
	if (myData->getPhsCorField())
	{
		if (myData->getIsNeedCalcPhsCorFlag()) // 如果模型或源被修改过
		{
			switch (QMessageBox::question(this, tr("Question"),
				tr("The model has changed and whether the computing field is updated?"),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
			{
			case QMessageBox::Yes:
				myData->calcPhsCorField();
				break;
			case QMessageBox::No:
				break;
			default:
				break;
			}
		}
	}
	else
	{
		myData->calcPhsCorField();
	}

	PhsCorrectionDialog dialog;
	dialog.setIsNeedMesh(isNeedMesh);
	if (dialog.exec() != QDialog::Accepted)
	{
		return;
	}
	
	CalculatePhsCorThread *calThr = new CalculatePhsCorThread();
	if (isNeedMesh)
	{
		int dsIndex;
		double length;
		dialog.getData(dsIndex, length);
		calThr->setDs_Length(dsIndex, length);
	}
	double targetW = 0.0;
	dialog.getTarget(targetW);

	calThr->setTarget_W(targetW);

	//connect(FDTDprogressDialog, SIGNAL(sendStop()), this, SLOT(toReceiveFDTDStop()));
	//connect(FDTDprogressDialog, SIGNAL(sendStop()), calThr, SLOT(killFDTD()));

	//connect(calThr, SIGNAL(sendMainValue(int)), FDTDprogressDialog, SLOT(setMainValue(int)));
	//connect(calThr, SIGNAL(sendSlaverValue(int)), FDTDprogressDialog, SLOT(setSlaverValue(int)));
	phsCorprogressDialog = new PhsCorProgressDialog();
	phsCorprogressDialog->show();

	connect(calThr, SIGNAL(error(int)), phsCorprogressDialog, SLOT(ReceiveError(int)));
	connect(calThr, SIGNAL(sendMainValue(int)), phsCorprogressDialog, SLOT(setMainValue(int)));
	
	connect(calThr, SIGNAL(finished()), calThr, SLOT(deleteLater()));
	connect(calThr, SIGNAL(sendMirror(Mirror*)), this, SLOT(toReceivePhaseCor(Mirror*)));

	calThr->start();
}

void mainWindow::toReceivePhaseCor(Mirror * mirror)
{
	delete phsCorprogressDialog;
	phsCorprogressDialog = nullptr;
	int index1 = myData->getNumOfMirrors() - 1;
	renderer->RemoveActor(myData->getMirrorByNum(index1)->getActor());
	QTreeWidgetItem *childMirror = new QTreeWidgetItem;
	childMirror->setText(0, tr("Mirror") + QString::number(index1 + 1));
	childMirror->setData(0, Qt::UserRole, QVariant(0));
	childMirror->setData(1, Qt::UserRole, QVariant(index1));
	childMirror->setData(2, Qt::UserRole, QVariant(index1));

	geometryTreeItem->removeChild(mirrorTreeWidgetItem[index1]);
	geometryTreeItem->insertChild(index1, childMirror);
	delete mirrorTreeWidgetItem[index1];
	mirrorTreeWidgetItem[index1] = childMirror;

	myData->setMirror(index1, mirror);
	mirrorTreeWidgetItem[index1]->insertChild(0, mirror->getTree());
	mirrorTreeWidgetItem[index1]->child(0)->setData(2,
		Qt::UserRole, QVariant(index1));
	mirrorTreeWidgetItem[index1]->setExpanded(true);
	renderer->AddActor(mirror->getActor());
	updateLight();
	updateVtk();
}

void mainWindow::toReceivePVVAField(Field *temPtr)
{
	//Field * temPtr = myData->calculateByPVVA(fre, dis, numMirror);
	renderer->AddActor(temPtr->getActor());

	myData->addField(temPtr);
	QTreeWidgetItem * tree = new QTreeWidgetItem(QStringList
	(QString("Field") + QString::number(fieldNum)));
	tree->setData(0, Qt::UserRole, QVariant(FIELD));
	tree->setData(1, Qt::UserRole, QVariant(fieldNum));
	fieldTreeItem->addChild(tree);
	fieldNum++;
	delete PVVAprogressDialog;
	PVVAprogressDialog = nullptr;
	updateVtk();
}

void mainWindow::on_DenisovParameters()
{
	//showDenisovPtr = new showDenisov; 
	//showDenisovPtr->show();
	//showDenisovPtr->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
}

void mainWindow::on_treeWidget_ContextMenuRequested(QPoint pos)
{
	rightSelectItem = treeWidget->itemAt(pos);
	if (rightSelectItem == NULL)
	{
		return;
	}
	QVariant var = rightSelectItem->data(0, Qt::UserRole);

	if (var == 0)      //data(...)返回的data已经在之前建立节点时用setdata()设置好  
	{
		//菜单出现的位置为当前鼠标的位置  
		if (R_BlankMenu->isEmpty())
		{
			int index = rightSelectItem->data(1, Qt::UserRole).toInt();
			if (myData->getMirrorByNum(index)->getIsShow())
			{
				isShowMirrorAction->setChecked(true);
			}
			else
			{
				isShowMirrorAction->setChecked(false);
			}
			if (myData->getMirrorByNum(index)->getIsTransparent())
			{
				isTransparentAction->setChecked(true);
			}
			else
			{
				isTransparentAction->setChecked(false);
			}
			R_Tree_MirrorTypeMenu->exec(QCursor::pos());

		}		
	}
	else if (var == 1)     
	{
		//菜单出现的位置为当前鼠标的位置  
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_MirrorParMenu->exec(QCursor::pos());

		}
	}
	else if (var == 2)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_RestrictionMenu->exec(QCursor::pos());

		}
	}
	else if (var == DENISOVVAL)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_RadiatorMenu->exec(QCursor::pos());

		}
	}
}

void mainWindow::on_treeWidget_leftPressed(QTreeWidgetItem * item, int column)
{
	if (item->parent() != NULL)     // 根节点
		if (qApp->mouseButtons() == Qt::LeftButton)
		{
			//RectangleWidget dialog(this, isSet); 
			if (item->data(0, Qt::UserRole) == 0)
			{
				int num = item->data(1, Qt::UserRole).toInt();
				//myData->getMirrorByNum(num)->setSelected();
			}
			if (item->data(0, Qt::UserRole) == FIELD)
			{
				int index = item->data(1, Qt::UserRole).toInt();
				leftSelectItem = item;
				showDetails(index);
			}

		}
	updateVtk();
}

void mainWindow::on_Details_FieldClicked()
{
	int index = leftSelectItem->data(1, Qt::UserRole).toInt();
	Field *tempField = myData->getFieldByNum(index);
	int dim = dimensionGroupBtn->checkedId();
	if (dim == 0) // 3D
	{
		tempField->set3D(true);
		renderer->RemoveActor(tempField->getActor());
		renderer->RemoveActor(tempField->getActor3D());
		tempField->setShowPara(fieldGroupBtn->checkedId(),
			powerGroupBtn->checkedId(), pmGroupBtn->checkedId());
		magnitudeBtn->setChecked(true);
		phaseBtn->setEnabled(false);
		magnitudeBtn->setEnabled(false);
		tempField->calActor3D();
		renderer->AddActor(tempField->getActor3D());
	}
	else // 2D
	{
		tempField->set3D(false);
		phaseBtn->setEnabled(true);
		magnitudeBtn->setEnabled(true);

		int i = pmGroupBtn->checkedId();
		if (i == 0)  // 相位没有db形式
		{
			linearBtn->setEnabled(true);
			dbBtn->setEnabled(true);
		}
		else
		{
			linearBtn->setEnabled(false);
			dbBtn->setEnabled(false);
		}
		tempField->setShowPara(fieldGroupBtn->checkedId(),
			powerGroupBtn->checkedId(), i);

		renderer->RemoveActor(tempField->getActor());
		renderer->RemoveActor(tempField->getActor3D());
		tempField->calActor();
		renderer->AddActor(tempField->getActor());
	}

	updateVtk();
}

void mainWindow::updateVtk()
{
	auto window = widget.GetRenderWindow();

	//window->AddRenderer(renderer);
	window->Render();
}

void mainWindow::updateLight()
{
	myData->getDefaultLightShow()->updateData();
}

void mainWindow::showDetails(int index)
{
	int content; bool isLinear; bool isPhs;
	Field *tempField = myData->getFieldByNum(index);
	if (!tempField)
		return;
	tempField->getShowPara(content, isLinear, isPhs);

	switch (tempField->get3D())
	{
	case 0:
		TwoDBtn->setChecked(true);
		phaseBtn->setEnabled(true);
		magnitudeBtn->setEnabled(true);
		break;
	case 1:
		ThreeDBtn->setChecked(true);
		magnitudeBtn->setChecked(true);
		phaseBtn->setEnabled(false);
		magnitudeBtn->setEnabled(false);
		break;
	default:
		break;
	}

	switch (content)
	{
	case 0:
		ExBtn->setChecked(true);
		break;
	case 1:
		EyBtn->setChecked(true);
		break;
	case 2:
		EzBtn->setChecked(true);
		break;
	case 3:
		HxBtn->setChecked(true);
		break;
	case 4:
		HyBtn->setChecked(true);
		break;
	case 5:
		HzBtn->setChecked(true);
		break;
	default:
		break;
	}

	switch (isPhs)
	{
	case true:
		phaseBtn->setChecked(true);
		linearBtn->setEnabled(false);
		dbBtn->setEnabled(false);
		break;
	case false:
		magnitudeBtn->setChecked(true);
		linearBtn->setEnabled(true);
		dbBtn->setEnabled(true);
		break;
	default:
		break;
	}

	switch (isLinear)
	{
	case true:
		linearBtn->setChecked(true);
		break;
	case false:
		dbBtn->setChecked(true);
		break;
	}

	if (0 == index)  // 如果是源 EzHxHyHz 不让点
	{
		EzBtn->setEnabled(false);
		HxBtn->setEnabled(false);
		HyBtn->setEnabled(false);
		HzBtn->setEnabled(false);
	}
	else
	{
		EzBtn->setEnabled(true);
		HxBtn->setEnabled(true);
		HyBtn->setEnabled(true);
		HzBtn->setEnabled(true);
	}
	
	effLabelVal->setText(QString::number(tempField->getEfficiency()));
	scaleLabelVal->setText(QString::number(tempField->getScalarCorrelationCoefficient()));
	vecLabelVal->setText(QString::number(tempField->getVectorCorrelationCoefficient()));

	detailsDockWidget->show();
}

