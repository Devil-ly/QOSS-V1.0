#ifndef GRAPHTRANSWIDGET_H
#define GRAPHTRANSWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>  
#include "Tools/Parameterization.h"
#include "Tools/GraphTrans.h"
#include "VTK/include/RaySource.h"
#include "VTK/include/Field.h"
class GraphTransWidget : public QDialog
{
	Q_OBJECT

public:
	GraphTransWidget(QWidget *parent = 0, int wayButton = 0);
	~GraphTransWidget();

	void addDefGroupBox(QGroupBox * _defGroupBox, QString filename);
	void addBaseGroupBox(QGroupBox * _baseGroupBox);
	void addNameLabel(QGridLayout * layout);
	void addRotateWidget(QWidget * RotateWidget, QString filename);
	void addBtn(QGridLayout * _layoutbt, int wayButton = 0);

	GraphTrans getGraphTransData(Surface *_modleData,
		Parameterization* para, bool &ok1) const;
	GraphTrans getGraphTransData(RaySource *
		_LightSource, bool &ok1) const;
	GraphTrans getGraphTransData(Field *_LightSource,
		Parameterization* para, bool &ok1) const;
	void setGraphTransData(const GraphTrans& graphTransPara);

	QString getGraphTransName()const;
	void setGraphTransName(QString name);

	void setNameLineEdit(QString name);

	int Num();
	void init();
	void setAddBtn();
	void setNameHide(bool ok);
	void setConnect(bool ok);

	void changeTransText(int, string);
	bool isLegitimate(const string &express, double & val) const;

	void setWayButton(int);
protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void buttonClose();
	void buttonAdd();
	void buttonCreate();
	void buttonApply();
	void buttonOk();
	void on_GraphChange(QString);

signals:
	/*************************
	* 0: 表示close 不画任何图形
	* 1: 表示Create
	* 2: 表示Add
	* 3: 表示OK
	* 4: 表示close (修改后的close)
	* 5: 表示临时显示修改后的图形
	**************************/
	void sendData(int);

private:

	//page1
	QLabel * Ulabel;
	QLabel * Vlabel;
	QLabel * Nlabel;

	QLineEdit * ULineEidt;
	QLineEdit * VLineEidt;
	QLineEdit * NLineEidt;

	QLabel * label;
	QLineEdit * nameLineEidt;

	//page2
	QGroupBox * imgGroupBox;
	QGroupBox * RotateGroupBox;
	QGroupBox * thetaGroupBox;

	QLabel * imgLlabel1;

	QLabel * xlabel;
	QLabel * ylabel;
	QLabel * zlabel;

	QLineEdit * xRotateLineEidt;
	QLineEdit * yRotateLineEidt;
	QLineEdit * zRotateLineEidt;

	QLabel * thetalabel;
	QLineEdit * thetaLineEidt;

	QPushButton * createbtn;
	QPushButton * addbtn;
	QPushButton * closebtn;

	int wayButton; // 1表示Create 2表示Add 3表示OK 4表示apply 其他表示close
				   //int wayButton;
};

#endif // ParaboloidWidget_H