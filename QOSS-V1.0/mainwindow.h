#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QVTKWidget.h>
#include <QAction>
#include <QTreeWidget>
#include <QDockWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QstatusBar>
#include <QTreeWidgetItem>
#include <QButtonGroup>
#include <QRadioButton>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include "MyData.h"

#include "Qt/include/ParaboloidWidget.h"
#include "Qt/include/RestrictionWidget.h"
#include "Qt/include/GaussianWidget.h"
#include "Qt/include/ApertureFieldWidget.h"
#include "Qt/include/CalculationWidget.h"
#include "Qt/include/ParabolicCylinderWidget.h"
#include "Qt/include/PlaneMirrorWidget.h"
#include "Qt/include/STLMirrorWidget.h"
#include "Qt/include/GraphTransWidget.h"
#include "Qt/include/EllipsoidWidget.h"
#include "Qt/include/PhsCorProgressDialog.h"
#include "Qt/include/FDTDProgressDialog.h"
#include "Qt/include/PVVAProgressDialog.h"

#include <Calculation/FDTDRadiator.h>

using namespace userInterface;
class mainWindow : public QMainWindow
{
	Q_OBJECT
	enum QVariantType
	{	
		FIELD = 10,		
	};

public:
	mainWindow(QWidget *parent = 0);
	~mainWindow();

private:
	void init();

	//------------------- 窗口设置------------------- 
	void createActions();
	void createMenus(); // 菜单
	void createToolBars();
	void createStatusBar();

	void createTreeWidgetItem(); // 创建tree
	void createRightMenu(); // 右键菜单
	void createDetails(); //zuojian

	void createProject();
	// 
	void isNeedSave();

	void updateVtk();

	void updateLight();

	// 改变3D显示窗口
	void showDetails(int);


private slots:

	// 菜单响应函数
	void openFile();
	void newFile();
	void viewInitFile();
	void setView(int);

	void on_isShowBox();
	void on_isTransparentMirror();
	void on_isShowMirror();

	void on_modifyingRestriction();
	void on_delRestriction();

	void on_modifyingMirror();
	void on_modifyParameters();

	void on_restriction();
	void toReceiveRestriction(int);

	void toApertureField(int);
	void createApertureField();

	void createGaussian();
	void toReceiveGaussian(int);

	void on_createParaboloid();
	void on_createParabolicCylinder();
	void on_createEllipsoid();
	void on_createPlaneMirror();
	void on_createSTLMirror();

	void on_saveSTL();

	//void toReceiveParabolicCylinder(int);
	void toReceiveMirror(int);
	void toReceiveMirrorType(int);

	void on_PVVA();
	void on_FDTD();
	void toReceiveFDTD();
	void loadFDTDField(); //加载FDTD计算好的场
	void toReceiveFDTDStop();

	void on_PhaseCor();
	void toReceivePhaseCor(Mirror*);
	void toReceivePVVAField(Field*);

	// ------------------- 右键函数 ----------------------------------
	void on_treeWidget_ContextMenuRequested(QPoint pos);// 右键菜单

	// ------------------- 左键函数 ----------------------------------
	void on_treeWidget_leftPressed(QTreeWidgetItem *item, int column);
	void on_Details_FieldClicked(); //details Field 


private:
	vtkSmartPointer<vtkOrientationMarkerWidget> widget1;
	QVTKWidget widget; // vtk 显示窗口
	vtkSmartPointer<vtkRenderWindowInteractor> interactor; // 交互
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkAxesActor> axes; // 坐标

	QDockWidget * leftWidget; //放置treeWidget的窗口
	QTreeWidget * treeWidget;

	QDockWidget * detailsDockWidget;
	QWidget * detailsWidget;

	QLabel * locationLabel;  // 状态栏标签
	QPushButton * unitBtn;   // 单位按钮

	//----------- Menu ----------------- 
	QMenu * fileMenu;  //菜单栏
	QMenu * viewMenu;  //显示栏
	QMenu * eidtMenu;  //编辑栏
	QMenu * ModelMenu;  //模型栏
	QMenu * SourceMenu;  //源栏
	QMenu * CalMenu;  //计算栏

	//右键菜单
	QMenu *R_Tree_MirrorTypeMenu;
	QMenu *R_Tree_MirrorParMenu;
	QMenu *R_Tree_RestrictionMenu;
	QMenu *R_BlankMenu;

	//----------- ToolBar ------------------- 
	QToolBar * fileTool;   //工具栏
	QToolBar * constructTool;  //模型栏

	//----------- Action ----------------- 
	//文件菜单项
	QAction * saveFileAction;
	QAction * openFileAction;
	QAction * newFileAction;
	//文件菜单--view
	QAction * viewAction;  // 视角
	QLabel * viewLabel;
	QComboBox * viewComboBox;

	QAction * isShowBoxAction;

	QAction * GaussianAction;     // 高斯场源
	QAction * ApertureFieldAction;     // 任意场源
	QAction * PVVAAction;     // 计算pVVA
	QAction * FDTDAction;     // 计算FDTDAction
	QAction * PhaseAction;    // 相位修正

	// 右键
	QAction * modifyingMirrorAction;
	QAction * restrictionAction;
	QAction * modifyingRestrictionAction;
	QAction * delRestrictionAction;

	QAction * modifyParametersAction;
	QAction * isShowMirrorAction;
	QAction * isTransparentAction;
	QAction * saveSTLction;     // 保存模型

	QAction * loadFDTDAction; // 加载FDTD计算的源

	//------------- TreeWidgetItem------------------- 
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;
	QTreeWidgetItem * soucreFieldTreeItem;
	QTreeWidgetItem * lightTreeItem;
	QTreeWidgetItem * fieldTreeItem;

	QTreeWidgetItem * rightSelectItem;
	QTreeWidgetItem * leftSelectItem; // 右键选中的节点

	//****** Details********
	QButtonGroup * dimensionGroupBtn;
	QRadioButton * ThreeDBtn;
	QRadioButton * TwoDBtn;
	QButtonGroup * fieldGroupBtn;
	QRadioButton * ExBtn;
	QRadioButton * EyBtn;
	QRadioButton * EzBtn;
	QRadioButton * HxBtn;
	QRadioButton * HyBtn;
	QRadioButton * HzBtn;
	QButtonGroup * pmGroupBtn;
	QRadioButton * magnitudeBtn;
	QRadioButton * phaseBtn;
	QButtonGroup * powerGroupBtn;
	QRadioButton * linearBtn;
	QRadioButton * dbBtn;

	QLabel * effLabelVal;
	QLabel * scaleLabelVal;
	QLabel * vecLabelVal;

	vector<QTreeWidgetItem*> mirrorTreeWidgetItem;

	MyData * myData;
	Mirror * tempMirror;
	Restriction * tempRestriction;

	// 窗口指针
	ApertureFieldWidget * apertureFieldWidget;
	RestrictionWidget * restrictionWidget;
	GaussianWidget * gaussianWidget;
	//ParabolicCylinderWidget * parabolicCylinderWidget;

	GraphTransWidget * tempWidget;
	FDTDProgressDialog * FDTDprogressDialog;
	PhsCorProgressDialog * phsCorprogressDialog;
	PVVAProgressDialog * PVVAprogressDialog;
	FDTDRadiator *FDTDradiator;
	Field *radiatorField; //保存辐射器计算后的场

	bool isExistenceOpenWin;
	bool isNew;
	int fieldNum;

};

#endif // MAINWINDOW_H
