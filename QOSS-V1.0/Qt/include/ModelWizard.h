/*
*	created by liyun 2017/11/3
*   function 绘制源的图像
*   version 1.0
*/

#ifndef MODELWIZARD_H
#define MODELWIZARD_H

#include <QtWidgets/QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QScrollArea>
#include <QPainter>
#include <memory>

#include "..\include\QPaintField.h"
#include "..\include\QPaint2DMirror.h"
#include "..\Calculation\SourceModeGeneration.h"
#include "MirrorPosition.h"

namespace  userInterface {

	class ModelWizard : public QWizard
	{
		Q_OBJECT
	public:
		ModelWizard(QWidget *parent = 0);
		~ModelWizard();

		void accept();

	};

	// 介绍页
	class IntroPage : public QWizardPage
	{
		Q_OBJECT

	public:
		IntroPage(QWidget *parent = 0);

	private:
		QLabel *label;
	};

	// 设置基本参数页
	class BasePage : public QWizardPage
	{
		Q_OBJECT

	public:
		BasePage(QWidget *parent = 0);
	protected:
		bool validatePage();
	private:
		QLabel *projectLabel;
		QLabel *freLabel;
		QLabel *unitLabel;

		QLineEdit *projectNameLineEdit;
		QLineEdit *freLineEdit;
		QComboBox *unitBox;
		QComboBox *freUnitBox;
		QGroupBox *baseGroupBox;
		QGroupBox *groupBox;

		QRadioButton *lowerRadioButton;
		QRadioButton *higherRadioButton;
		QRadioButton *waveguideRadioButton;
	};

	// 设置激励源页
	class SourcePage : public QWizardPage
	{
		Q_OBJECT

	public:
		SourcePage(QWidget *parent = 0);

	protected:
		void initializePage();
		bool validatePage();

	private slots:
		void on_mLineEditChange(const QString &);
		void on_nLineEditChange(const QString &);
		void on_radiusLineEditChange(const QString &);
		void on_amplitudeLineEditChange(const QString &);
		void on_rotationComboBoxChange(int);

	private:
		void updatePaint();

	private:
		QLabel *typeLabel;
		QLabel *mLabel;
		QLabel *nLabel;
		QLabel *radiusLabel;
		QLabel *waveLengthLabel;
		QLabel *waveWidthLabel;
		QLabel *amplitudeLabel;
		QLabel *rotationLabel;
		QLabel *imgLabel;

		QComboBox *typeComboBox;
		QComboBox *rotationComboBox;
		QLineEdit *mLineEdit;
		QLineEdit *nLineEdit;
		QLineEdit *radiusLineEdit;
		QLineEdit *amplitudeLineEdit;
		QLineEdit *waveLengthLineEdit;
		QLineEdit *waveWidthLineEdit;

		QGroupBox *groupBox;

		QPainter *paint;

		QPaintField* imgLabel2;

		shared_ptr<calculation::SourceModeGeneration> source;

		int m;
		int n;
		int amplitude;
		int type;
		int rotation;
		double radius;
		int kind;
		double fre;

		double unit;
	};

	// 设置镜子位置页
	class PositionPage : public QWizardPage
	{
		Q_OBJECT

	public:
		PositionPage(QWidget *parent = 0);
		~PositionPage();

	protected:
		void initializePage();
		bool validatePage();

	private slots:
		void numComboBoxChange(int);
		void on_xLineEditChange(const QString &);
		void on_zLineEditChange(const QString &);

		void on_propagationLengthLineEdit1Change(const QString &);
		void on_propagationLengthLineEdit2Change(const QString &);
		void on_propagationLengthLineEdit3Change(const QString &);
		void on_propagationLengthLineEdit4Change(const QString &);

		void on_phiLineEdit1Change(const QString &);
		void on_phiLineEdit2Change(const QString &);
		void on_phiLineEdit3Change(const QString &);
		void on_phiLineEdit4Change(const QString &);

	private:
		QLabel *numLabel;

		QLabel *xLabel;
		QLabel *yLabel;
		QLabel *zLabel;

		QLineEdit *xLineEdit;
		QLineEdit *yLineEdit;
		QLineEdit *zLineEdit;

		QComboBox *numComboBox;

		QLabel *phiLineLabel;
		QLabel *phiLineLabe2;
		QLabel *phiLineLabe3;
		QLabel *phiLineLabe4;
		QLineEdit *phiLineEdit1;
		QLineEdit *phiLineEdit2;
		QLineEdit *phiLineEdit3;
		QLineEdit *phiLineEdit4;
		QLabel *propagationLengthLabel;
		QLabel *propagationLengthLabe2;
		QLabel *propagationLengthLabe3;
		QLabel *propagationLengthLabe4;
		QLineEdit *propagationLengthLineEdit1;
		QLineEdit *propagationLengthLineEdit2;
		QLineEdit *propagationLengthLineEdit3;
		QLineEdit *propagationLengthLineEdit4;
		QGroupBox *mirrorsGroupBox1;
		QGroupBox *mirrorsGroupBox2;
		QGroupBox *mirrorsGroupBox3;
		QGroupBox *mirrorsGroupBox4;

		QGroupBox *limitGroupBox;

		QGridLayout *allMirrorsLayout;

		QScrollArea *scrollArea;

		QPaint2DMirror * imgLabel;

		QWidget *widget;

		shared_ptr<calculation::SourceModeGeneration> source;
		shared_ptr<calculation::MirrorPosition> mirrorPosition;
	};

}
#endif //MODELWIZARD_H