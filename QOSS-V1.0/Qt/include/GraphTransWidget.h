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
#include "../VTK/include/BasicParameters.h"

namespace  userInterface {
	class GraphTransWidget : public QDialog
	{
		Q_OBJECT

	public:
		GraphTransWidget(QWidget *parent = 0);
		~GraphTransWidget();

		void addDefGroupBox(QGroupBox * _defGroupBox, QString filename);
		void addBaseGroupBox(QGroupBox * _baseGroupBox);
		void addRotateWidget(QWidget * RotateWidget, QString filename);
		void addBtn(QGridLayout * _layoutbt, int wayButton = 0);

		void setMirror(BasicParameters*);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots :
		void buttonClose();
		void buttonOk();
		void on_GraphChange(QString);

	signals:
		/*************************
		* 0: 表示close 不画任何图形
		* 1: 表示Create
		* 2: 表示更新
		**************************/
		void sendData(int);

	private:
		BasicParameters* mirror;

		//page1
		QLabel * Ulabel;
		QLabel * Vlabel;
		QLabel * Nlabel;

		QLineEdit * ULineEidt;
		QLineEdit * VLineEidt;
		QLineEdit * NLineEidt;

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
		QPushButton * closebtn;

	};

}


#endif // ParaboloidWidget_H