#include "mainwindow.h"
#include <Qt/include/PreDialog.h>
#include "Qt/include/ModelWizard.h"
#include "Qt/include/MirrorTypeWidget.h"

#include "VTK/include/Mirror.h"
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

	//创建默认辐射器
	myData->createRadiator();
	renderer->AddActor(myData->getRadiator()->getActorModel());
	//renderer->AddActor(myData->getRadiator()->getActorRay());

	// 创建默认的镜子
	myData->createDefaultMirror();
	for (int i = 0; i < myData->getNumOfMirrors(); ++i)
	//for (int i = 0; i < 2; ++i)
	{
		renderer->AddActor(myData->getMirrorByNum(i)->getActor());
	}

	// 加入限制盒子
	renderer->AddActor(myData->getLimitBox()->getActor());

	// 创建默认的光线
	myData->createDefaultLigthShow();
	std::list<vtkSmartPointer<vtkActor>> tempActors = 
		myData->getDefaultLightShow()->getActors();
	//for (auto& x : tempActors)
	//	renderer->AddActor(x);

	
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
	createRightMenu();
	//createDetails();
	//creareWindows();

	init();
}

mainWindow::~mainWindow()
{

}

void mainWindow::init()
{
	isExistenceOpenWin = false;
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

	viewMenu->addAction(isShowBoxAction);

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
		childPar->setText(0, tr("Pattern: Higher order"));
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
		geometryTreeItem->addChild(childMirror);
		childMirror->addChild(myData->getMirrorByNum(i)->getTree());
		childMirror->child(0)->setData(2, Qt::UserRole, QVariant(i));
		mirrorTreeWidgetItem.push_back(childMirror);
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

	R_Tree_MirrorTypeMenu->addAction(modifyingMirrorAction);
	R_Tree_MirrorTypeMenu->addAction(restrictionAction);

	R_Tree_MirrorParMenu = new QMenu(this);
	modifyParametersAction = new QAction(tr("Modifying parameters"), this);
	modifyParametersAction->setFont(font);
	modifyParametersAction->setStatusTip(tr("Modifying parameters"));
	connect(modifyParametersAction, SIGNAL(triggered()), this, SLOT(on_modifyParameters()));

	R_Tree_MirrorParMenu->addAction(modifyParametersAction);

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

void mainWindow::on_isShowBox()
{
	//isShowBoxAction->isChecked();
	myData->getLimitBox()->setIsTransparent(isShowBoxAction->isChecked());

	updateVtk();
}

void mainWindow::on_modifyingMirror()
{
	MirrorTypeWidget mirrorTypeWidget;
	if (mirrorTypeWidget.exec() != QDialog::Accepted)
	{
		exit(1);
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
		on_createParaboloid();
		break;
	default:
		break;
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
	paraboloidWidget = new ParaboloidWidget();
	paraboloidWidget->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(paraboloidWidget, SIGNAL(sendData(int)),
		this, SLOT(toReceiveParaboloid(int)));

	paraboloidWidget->setMirror(tempMirror);
	paraboloidWidget->show();
	isExistenceOpenWin = true;
	
}

void mainWindow::toReceiveParaboloid(int index)
{
	if (1 == index) // 点击确认
	{
		int index1 = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempMirror->setSelected(false);
		int tempNum = mirrorTreeWidgetItem[index1]->childCount();
		for (int i = 0; i < tempNum; ++i)
		{
			mirrorTreeWidgetItem[index]->removeChild(mirrorTreeWidgetItem[index]->child(i));
		}
		myData->setMirror(index1, tempMirror);
		mirrorTreeWidgetItem[index]->addChild(tempMirror->getTree());
		mirrorTreeWidgetItem[index]->child(0)->setData(2,
			Qt::UserRole, QVariant(index));	
		if (true) // 判断是否保留原来的限制条件
		{
			
			
		}
		else
		{

		}
		tempMirror = nullptr;
		delete paraboloidWidget;
		paraboloidWidget = nullptr;
		isExistenceOpenWin = false;
	}
	else if (0 == index)// 点击取消
	{
		renderer->RemoveActor(tempMirror->getActor());
		int index = rightSelectItem->data(2, Qt::UserRole).toInt();

		renderer->AddActor(myData->getMirrorByNum(index)->getActor());
		delete tempMirror;
		tempMirror = nullptr;
		delete paraboloidWidget;
		paraboloidWidget = nullptr;
		isExistenceOpenWin = false;
	}
	updateVtk();
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

		}
	updateVtk();
}


void mainWindow::updateVtk()
{
	auto window = widget.GetRenderWindow();

	//window->AddRenderer(renderer);
	window->Render();
}