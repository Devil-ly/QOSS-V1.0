#ifndef CALCULATIONWIDGET_H
#define CALCULATIONWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "Qt/include/GraphTransWidget.h"


class CalculationWidget : public QDialog
{
	Q_OBJECT

public:
	CalculationWidget(QWidget *parent = 0);
	~CalculationWidget();

	double getFre();
	double getDistance();
	int getMirrorNum();

private slots:
    void buttonOk();
    void buttonCancel();
	void buttonSave();

	void resultComboboxChange(int Num);
private:

	//page1
	QTabWidget * tabWidget;

	QGroupBox * paraGroupBox;
	QLabel * freLabel;
	QLineEdit * freLineEdit;

	QLabel *  calModLabel; // ¼ÆËãÄ£Ê½
	QComboBox * calModCombobox;
	QLabel *  MeshLabel;
	QComboBox * MeshCombobox;

	QGroupBox * surfaceNumGroupBox;
	QLabel *  surfaceLabel;
	QComboBox * surfaceCombobox;

	QGroupBox * resultGroupBox;
	QLabel *  resultLabel;
	QComboBox * resultCombobox;
	QLabel *  distanceLabel;
	QLabel * widthLabel;
	QLabel * depthLaebl;
	QLineEdit * distanceLineEdit;
	QLineEdit * widthLineEdit;
	QLineEdit * depthLineEdit;

	QLabel * saveLabel;
	QLineEdit * saveLineEdit;
	QPushButton * saveBtn;


	QPushButton * okBtn;
	QPushButton * cancelBtn;


	

};

#endif // CALCULATIONWIDGET_H
