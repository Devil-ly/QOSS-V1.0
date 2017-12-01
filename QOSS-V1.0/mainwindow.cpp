#include "mainwindow.h"
#include <Qt/include/PreDialog.h>
#include "Qt/include/ModelWizard.h"
#include "VTK/include/Mirror.h"
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

#include <QMessageBox>


using namespace userInterface;

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setCentralWidget(&widget);
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

	// 创建默认的镜子
	myData->createDefaultMirror();
	for (int i = 0; i < myData->getNumOfMirrors(); ++i)
	{
		renderer->AddActor(myData->getMirrorByNum(i)->getActor());
	}

	// 加入限制盒子
	//renderer->AddActor(myData->getLimitBox()->getActor());

	// 创建默认的光线
	//myData->createDefaultLigthShow();
	//std::list<vtkSmartPointer<vtkActor>> tempActors = 
	//	myData->getDefaultLightShow()->getActors();
	//for (auto& x : tempActors)
	//	renderer->AddActor(x);

	myData->createRadiator();
	renderer->AddActor(myData->getRadiator()->getActorModel());
	renderer->AddActor(myData->getRadiator()->getActorRay());


	double axesScale = myData->getLimitBox()->getMaxSize();
	// 初始化vtk窗口
	axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);//修改X字体颜色为红色  
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 2, 0);//修改Y字体颜色为绿色  
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 3);//修改Z字体颜色为蓝色  
	axes->SetConeRadius(0.3);
	axes->SetConeResolution(20);
	//axes->SetTotalLength(10, 10, 10); //修改坐标尺寸
	
	//renderer->AddActor(axes);

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
	//createRightMenu();
	//createDetails();
	//creareWindows();
}

mainWindow::~mainWindow()
{

}

void mainWindow::init()
{
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

	//初始化视角
	viewAction = new QAction(QIcon(tr("Qt/images/view.png")), tr("View"), this);
	viewAction->setStatusTip(tr("Initialize the view"));
	connect(viewAction, SIGNAL(triggered()), this, SLOT(viewInitFile()));

	//修改视角
	viewLabel = new QLabel(tr("View"));
	viewComboBox = new QComboBox;
	viewComboBox->addItem("View YZ plane(original)");
	viewComboBox->addItem("View XZ plane");
	viewComboBox->addItem("View XY plane");
	viewComboBox->addItem("View -YZ plane");
	viewComboBox->addItem("View -XZ plane");
	viewComboBox->addItem("View -XY plane");
	connect(viewComboBox, SIGNAL(activated(int)), this, SLOT(setView(int)));


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
	eidtMenu = this->menuBar()->addMenu(tr("Edit"));
	ModelMenu = this->menuBar()->addMenu(tr("Model"));
	SourceMenu = this->menuBar()->addMenu(tr("Source"));
	CalMenu = this->menuBar()->addMenu(tr("simulation"));
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
	QTreeWidgetItem *childPar = new QTreeWidgetItem;
	if (0 == myData->getPattern())
		childPar->setText(0, tr("Pattern: Lower order"));
	else if (1 == myData->getPattern())
		childPar->setText(0, tr("Pattern: Higher order"));
	else
		childPar->setText(0, tr("Pattern: Waveguide"));
	sourceTreeItem->addChild(childPar);
	sourceTreeItem->setExpanded(true);

	geometryTreeItem = new QTreeWidgetItem(QStringList(QString("Geometry")));
	modelTreeItem->addChild(geometryTreeItem);

	for (int i = 0; i < myData->getNumOfMirrors(); ++i)
	{
		QTreeWidgetItem *childMirror = new QTreeWidgetItem;
		childMirror->setText(0, tr("Mirror") + QString::number(i+1));
		geometryTreeItem->addChild(childMirror);
	}
	geometryTreeItem->setExpanded(true);

	lightTreeItem = new QTreeWidgetItem(QStringList(QString("Light")));
	modelTreeItem->addChild(lightTreeItem);

	fieldTreeItem = new QTreeWidgetItem(QStringList(QString("Field")));
	modelTreeItem->addChild(fieldTreeItem);

}

void mainWindow::createRightMenu()
{
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