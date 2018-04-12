#ifndef SHOWFDTD_H
#define SHOWFDTD_H

/*
这是显示主界面的类-可根据需要进行编辑
注意：这个是一个显示的窗口 只保留显示逻辑，
注意：所有运算部分调用DLL！ DLL剥离
*/

//这个是公共的
#include <QtWidgets/QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QTableView>
#include <QLineEdit>
#include <QString>
#include <iostream> 
#include <sstream> 
#include <string>
#include <windows.h>
#include <QProgressBar>

//#include "ui_showFDTD.h"		//这个是QT生成的


#include "CodeJin/qcustomplot.h"		//这个单独放到一个文件夹里
#include "../util/Constant_Var.h"	//这个干掉

#include "QTFDTDThread.h"

using namespace std;
using namespace userInterface;

class showFDTD : public QWidget
{
	Q_OBJECT

public:
	showFDTD(QWidget *parent = 0);
	~showFDTD();

	private slots :

	void on_btn1();	//
	void on_btn2();
	//void on_btnStop();	//Stop
	void on_btnLoad();	//LoadFiles

	void ChangeValue(double _in);
	void ChangeText(QString _in);
	void BtnClose();
	void BtnOpen();

	void recieveFloat(float _in);
	void recieveInt(int _in);


	//void DrawCurrents();



private: signals :
		 void SendValue(double _out);
		 void SendText(QString _Message);

private:
	//void updatePaint();
	void CreatePlot();
	void CreateButtons();

	void CreateBasicParas();
	void ReadBasicParas();

	void addDefGroupBox(QGroupBox * _defGroupBox, QString filename);

private:
	//Ui::showFDTDClass ui;

	QGroupBox *imgGroupBox;		//Image

	QCustomPlot *DrawField;		//瞬时场分布
	QCustomPlot *DrawAperture;	//输出场分布

	QLabel *Percent;
	QLabel *Message;
	//Button Interface
	QGroupBox *Buttons;
	QPushButton *btn1;	//SetBasic
	QPushButton *btn2;	//Compute
	QPushButton *btnStop;	//Stop
	QPushButton *btnLoad;	//LoadField

							//Basic Parameters InterFace All Parameter necessary
	QGroupBox *BasicParas;
	QLabel *FreqLabel;		QLineEdit *FreqEdit;
	QLabel *RadiusLabel;	QLineEdit *RadiusEdit;
	QLabel *NsLabel;		QLineEdit *NsEdit;
	QLabel *MLabel;			QLineEdit *MEdit;
	QLabel *NLabel;			QLineEdit *NEdit;
	QLabel *CutHeightLabel;	QLineEdit *CutHeightEdit;	//Cut Height
	QLabel *PDLabel;		QLineEdit * PDEdit;	//Propagation Distance
	QLabel *ALLabel;		QLineEdit * ALEdit;	//Aperture Size
	QLabel *NaLabel;		QLineEdit *	NaEdit;	//Aperture Sampling Number
	QLabel *PRLabel;		QLineEdit * PREdit;	//PowerRatio

												//basic parameters
	double frequency;	double lambda;
	double radius;		int Ns;
	//mode
	int m;
	int n;
	//Cut Height
	double lcut;
	double prodis;
	double aperlen;		int Na;

	double PowerRatio;

	//computation Control
	int ComputationStage;//0 before Computation //1 FDTD computation; //2 Huygens computation //3 After Computation

						 //calculation thread
	QTFDTDThread *FDTD;
	QProgressBar * progressBar;

};

#endif // SHOWFDTD_H
