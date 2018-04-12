#include "Qt/include/ModelBtnDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "MyData.h"
#include "MirrorPosition.h"

#include "..\Calculation\SourceModeGeneration.h"

namespace  userInterface {

	ModelBtnDialog::ModelBtnDialog(QWidget *parent)
	{
	
		QPushButton *TE01Btn = new QPushButton(tr("TE01"));
		connect(TE01Btn, SIGNAL(clicked()), this, SLOT(on_TE01()));

		QPushButton *TE226Btn = new QPushButton(tr("TE226"));
		connect(TE226Btn, SIGNAL(clicked()), this, SLOT(on_TE226()));

		QPushButton *powerBtn = new QPushButton(tr("5*5 Power synthesis"));
		connect(powerBtn, SIGNAL(clicked()), this, SLOT(on_power()));

		QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addWidget(TE01Btn);
		layout->addWidget(TE226Btn);
		layout->addWidget(powerBtn);
	}

	ModelBtnDialog::~ModelBtnDialog()
	{

	}

	void ModelBtnDialog::on_TE226()
	{
		double fre = 140.0e9;
		MyData::getInstance()->setFrequency(fre);
		MyData::getInstance()->setUnit(1);
		MyData::getInstance()->setPattern(1);
		shared_ptr<calculation::SourceModeGeneration> source;
		source = make_shared<calculation::SourceModeGeneration>();
		int m = 0;
		int n = 1;
		int amplitude = 1;
		int type = 1;
		int rotation = 2;
		double radius = 0.016;
		//double fre = 44.0e9;
		int kind = 1;
		source->SetSource_Circular(kind, type, rotation, m, n, fre, radius);
		MyData::getInstance()->setSource(source);
		shared_ptr<calculation::MirrorPosition> mirrorPosition;
		mirrorPosition = make_shared<calculation::MirrorPosition>();

		Vector3 org, V1; double Length1; double theta1;
		Vector3 V2; double Length2; double theta2;
		Vector3 V3; double Length3; double theta3;
		Vector3 V4;
		source->GetCircularSystemOpticalPath_TE0n_3Mirrors(org, V1, Length1, theta1,
			V2, Length2, theta2, V3, Length3, theta3, V4);

		double length = 0.3;
		double width = 0.5;
		double height = 0.8;

		mirrorPosition->setMirrorNum(3);
		mirrorPosition->setStartingPoint(org);
		mirrorPosition->setLightDirection(0, V1);
		mirrorPosition->setLightDirection(1, V2);
		mirrorPosition->setLightDirection(2, V3);
		mirrorPosition->setLightDirection(3, V4);
		mirrorPosition->setLightLength(0, Length1);
		mirrorPosition->setLightLength(1, Length2);
		mirrorPosition->setLightLength(2, Length3);
		mirrorPosition->setLightPhi(0, theta1 / Pi * 180);
		mirrorPosition->setLightPhi(1, theta2 / Pi * 180);
		mirrorPosition->setLightPhi(2, theta3 / Pi * 180);

		MyData::getInstance()->setLimitBox(make_shared<LimitBox>(length, width, height));
		MyData::getInstance()->setMirrorPosition(mirrorPosition);

		accept();
	}

	void ModelBtnDialog::on_power()
	{
		double fre = 140.0e9;
		MyData::getInstance()->setFrequency(fre);
		MyData::getInstance()->setUnit(1);
		MyData::getInstance()->setPattern(2);
		shared_ptr<calculation::SourceModeGeneration> source;
		source = make_shared<calculation::SourceModeGeneration>();
		int m = 0;
		int n = 1;
		int amplitude = 1;
		int type = 1;
		int rotation = 2;
		double radius = 0.016;
		//double fre = 44.0e9;
		int kind = 1;
		source->SetSource_Circular(kind, type, rotation, m, n, fre, radius);
		MyData::getInstance()->setSource(source);
		shared_ptr<calculation::MirrorPosition> mirrorPosition;
		mirrorPosition = make_shared<calculation::MirrorPosition>();

		Vector3 org, V1; double Length1; double theta1;
		Vector3 V2; double Length2; double theta2;
		Vector3 V3; double Length3; double theta3;
		Vector3 V4;
		source->GetCircularSystemOpticalPath_TE0n_3Mirrors(org, V1, Length1, theta1,
			V2, Length2, theta2, V3, Length3, theta3, V4);

		double length = 0.3;
		double width = 0.5;
		double height = 0.8;

		mirrorPosition->setMirrorNum(3);
		mirrorPosition->setStartingPoint(org);
		mirrorPosition->setLightDirection(0, V1);
		mirrorPosition->setLightDirection(1, V2);
		mirrorPosition->setLightDirection(2, V3);
		mirrorPosition->setLightDirection(3, V4);
		mirrorPosition->setLightLength(0, Length1);
		mirrorPosition->setLightLength(1, Length2);
		mirrorPosition->setLightLength(2, Length3);
		mirrorPosition->setLightPhi(0, theta1 / Pi * 180);
		mirrorPosition->setLightPhi(1, theta2 / Pi * 180);
		mirrorPosition->setLightPhi(2, theta3 / Pi * 180);

		MyData::getInstance()->setLimitBox(make_shared<LimitBox>(length, width, height));
		MyData::getInstance()->setMirrorPosition(mirrorPosition);

		accept();
	}

	void ModelBtnDialog::on_TE01()
	{
		double fre = 44.0e9;
		MyData::getInstance()->setFrequency(fre);
		MyData::getInstance()->setUnit(1);
		MyData::getInstance()->setPattern(0);
		shared_ptr<calculation::SourceModeGeneration> source;
		source = make_shared<calculation::SourceModeGeneration>();
		int m = 0;
		int n = 1;
		int amplitude = 1;
		int type = 1;
		int rotation = 2;
		double radius = 0.016;
		//double fre = 44.0e9;
		int kind = 1;
		source->SetSource_Circular(kind, type, rotation, m, n, fre, radius);
		MyData::getInstance()->setSource(source);
		shared_ptr<calculation::MirrorPosition> mirrorPosition;
		mirrorPosition = make_shared<calculation::MirrorPosition>();

		Vector3 org, V1; double Length1; double theta1;
		Vector3 V2; double Length2; double theta2;
		Vector3 V3; double Length3; double theta3;
		Vector3 V4;
		source->GetCircularSystemOpticalPath_TE0n_3Mirrors(org, V1, Length1, theta1,
			V2, Length2, theta2, V3, Length3, theta3, V4);

		double length = 0.3;
		double width = 0.8;
		double height = 0.5;

		mirrorPosition->setMirrorNum(3);
		mirrorPosition->setStartingPoint(org);
		mirrorPosition->setLightDirection(0, V1);
		mirrorPosition->setLightDirection(1, V2);
		mirrorPosition->setLightDirection(2, V3);
		mirrorPosition->setLightDirection(3, V4);
		mirrorPosition->setLightLength(0, Length1);
		mirrorPosition->setLightLength(1, Length2);
		mirrorPosition->setLightLength(2, Length3);
		mirrorPosition->setLightPhi(0, theta1 / Pi * 180);
		mirrorPosition->setLightPhi(1, theta2 / Pi * 180);
		mirrorPosition->setLightPhi(2, theta3 / Pi * 180);

		MyData::getInstance()->setLimitBox(make_shared<LimitBox>(length, width, height));
		MyData::getInstance()->setMirrorPosition(mirrorPosition);

		accept();
	}
}
