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

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include "MyData.h"

class mainWindow : public QMainWindow
{
	Q_OBJECT

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

	void createProject();

	// 
	void isNeedSave();



private slots:

	void openFile();
	void newFile();


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
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//----------- ToolBar ------------------- 
	QToolBar * fileTool;   //工具栏
	QToolBar * constructTool;  //模型栏

	//----------- Action ----------------- 
	//文件菜单项
	QAction * saveFileAction;
	QAction * openFileAction;
	QAction * newFileAction;
	QAction * viewAction;  // 视角
	QLabel * viewLabel;
	QComboBox * viewComboBox;

	//------------- TreeWidgetItem------------------- 
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;
	QTreeWidgetItem * lightTreeItem;
	QTreeWidgetItem * fieldTreeItem;

	QTreeWidgetItem * rightSelectItem;
	QTreeWidgetItem * leftSelectItem; // 右键选中的节点

	MyData * myData;

};

#endif // MAINWINDOW_H
