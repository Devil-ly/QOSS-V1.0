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

namespace  userInterface {
	class GraphTransWidget : public QDialog
	{
		Q_OBJECT

	public:
		GraphTransWidget(QWidget *parent = 0, int wayButton = 0);
		~GraphTransWidget();

		void addDefGroupBox(QGroupBox * _defGroupBox, QString filename);
		void addBaseGroupBox(QGroupBox * _baseGroupBox);
		void addRotateWidget(QWidget * RotateWidget, QString filename);
		void addBtn(QGridLayout * _layoutbt, int wayButton = 0);

	protected:


	private slots :
		//void buttonClose();
		void buttonApply();
		void buttonOk();

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

}


#endif // ParaboloidWidget_H