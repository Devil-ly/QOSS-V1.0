#include "../DenisovRadiator/CodeJin/SourceModeGenerationD.h"//这个干掉 这个已经用了，最好
#include "../DenisovRadiator/CodeJin/Mathematical_Functions_Jin.h"
#include "../Calculation/Mathematical_Functions.h"
#include "showDenisov.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <cmath>
using namespace calculation;

showDenisov::showDenisov(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	resize(1400, 800);
	setWindowTitle("DenisovDesigner");
	//这是画图板 好像是李赟单独写的类
	paintField = new QPaintField;

	CreatePlot();

	CreateButtons();

	//ParaTable = new QTableView();
	//ParaTable -> setMaximumWidth(300);
	//ParaTable ->column
	
	CreateBasicParas();

	CreateDesignParas();

	CreateCutParas();

	Message = new QLabel(tr("Message"));
	Message -> setMaximumWidth(300);
	Percent = new QLabel(tr("Percentage"));
	Percent -> setMaximumWidth(300);

	Denisov1 = new DenisovRadiator;

	connect(btn1, SIGNAL(clicked()), this, SLOT(on_btn1()));
	//信号和槽函数的参数必须具有相同的顺序和相同的类型
	//connect(btn2, SIGNAL(clicked()), this, SLOT(ChangeValue()));
	connect(this, SIGNAL(SendValue(double)), this, SLOT(ChangeValue(double)));
	//连接Denisov设计类的进度
	connect(Denisov1, SIGNAL(SendValue(double)), this, SLOT(ChangeValue(double)));
	//连接Denisov设计类的说明
	connect(Denisov1, SIGNAL(SendText(QString)), this, SLOT(ChangeText(QString)));
	//连接按钮关
	connect(Denisov1, SIGNAL(SendBtnClose()), this, SLOT(BtnClose()));
	//连接按钮开
	connect(Denisov1, SIGNAL(SendBtnOpen()), this, SLOT(BtnOpen()));
	connect(btn2, SIGNAL(clicked()), this, SLOT(on_btn2()));
	connect(btn3, SIGNAL(clicked()), this, SLOT(on_btn3()));
	//连接Denisov计算得到的功率曲线
	connect(Denisov1, SIGNAL(SendCoefficients(double, double, double, double, int)), this, SLOT(RecieveCoefficients(double, double ,double, double, int)));
	//连接Denisov计算得到的辐射器切向场
	connect(Denisov1, SIGNAL(SendFieldUpdate()), this, SLOT(UpdateTanE()));
	//连接Denisov计算得到的辐射器表面电流
	connect(Denisov1, SIGNAL(SendCurrentUpdate()), this, SLOT(UpdateSurfaceJ()));
	//connect(Denisov1, SIGNAL(SendTangentialEField(std::vector<std::vector<std::complex<double>>>, std::vector<std::vector<std::complex<double>>>, int, int)),
	//	this, SLOT(RecieveTangentialEField(std::vector<std::vector<std::complex<double>>>, std::vector<std::vector<std::complex<double>>>, int, int)));


	QVBoxLayout * vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(BasicParas);
	vBoxLayout->addWidget(DesignParas);
	vBoxLayout->addWidget(CCParas);
	vBoxLayout->addWidget(Buttons);
	vBoxLayout->addWidget(Message);
	vBoxLayout->addWidget(Percent);

	//Horizontal
	QVBoxLayout * DrawZoneVC = new QVBoxLayout;
	DrawZoneVC->addWidget(PlotCurve);
	DrawZoneVC->addWidget(PlotPower);
	QHBoxLayout * DrawZoneHU = new QHBoxLayout;
	DrawZoneHU->addWidget(paintField);
	DrawZoneHU->addLayout(DrawZoneVC);
	QHBoxLayout * DrawZoneHD = new QHBoxLayout;
	DrawZoneHD->addWidget(DrawCurrent);
	
	//DrawZoneHD->addWidget(paintCurrent);
	//Vertrical
	QVBoxLayout * DrawZoneV = new QVBoxLayout;
	DrawZoneV->addLayout(DrawZoneHU);
	DrawZoneV->addLayout(DrawZoneHD);


	QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->addLayout(DrawZoneV);
	hBoxLayout->addLayout(vBoxLayout);

	/*//原2上2下布局
	QHBoxLayout * DrawZoneHU = new QHBoxLayout;
	DrawZoneHU->addWidget(paintField);
	DrawZoneHU->addWidget(PlotCurve);
	QHBoxLayout * DrawZoneHD = new QHBoxLayout;
	DrawZoneHD->addWidget(PlotPower);
	DrawZoneHD->addWidget(DrawCurrent);
	//Vertrical
	QVBoxLayout * DrawZoneV = new QVBoxLayout;
	DrawZoneV->addLayout(DrawZoneHU);
	DrawZoneV->addLayout(DrawZoneHD);
	

	QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->addLayout(DrawZoneV);
	hBoxLayout->addLayout(vBoxLayout);
	*/


}

showDenisov::~showDenisov()
{

}

void showDenisov::ChangeValue(double _in) {
	QString ss;
	ss.setNum(_in);
	Percent->setText(ss);
}

void showDenisov::ChangeText(QString _in) {
	Message->setText(_in);
}

void showDenisov::on_btn1()
{//按按钮1，读取基本参数，绘制输入模式场图，并计算基本参数

	ReadBasicParas();

	//绘制输入模式的场分布
	updatePaint();
	paintField->setWindowTitle("instantaneous E-Field");
	paintField->show();

	this->Nx = 250; this->Ny = 250;
	SourceModeGenerationD SourceMode(2,1,2,m,n,frequency,radius,0,0,Nx);
	SourceMode.FieldCalculation_Circular();
	vector<vector<double>> Absdata(Ny, vector<double>(Nx, 0));
	vector<vector<complex<double>>> FieldDatax(Ny, vector<complex<double>>(Nx, 0));
	vector<vector<complex<double>>> FieldDatay(Ny, vector<complex<double>>(Nx, 0));
	SourceMode.GetEX(FieldDatax);
	SourceMode.GetEY(FieldDatay);
	for (int j = 0; j < Ny; j++) {
		for (int i = 0; i < Nx; i++) {
			Absdata[j][i] = sqrt(FieldDatax[j][i].real()*FieldDatax[j][i].real() + FieldDatay[j][i].real()*FieldDatay[j][i].real());
		}
	}

	paintField->setNum(Ny, Nx);
	paintField->setData(Absdata);
	updatePaint();

	//完成绘图，下面更新相应的参数 lcut delbeta1, delbeta2;
	double temp1, temp2, temp3;
	SourceMode.GetCircularWaveguideProperty(temp1, temp2, temp3, lcut);
	lcut = lcut*1e3;
	//CutHeightEdit->setEnabled(true);
	CutHeightEdit->setText(QString::number(lcut, 10, 4));
	//CutHeightEdit->setEnabled(false);
	delbeta1 = (kzmnTE(m - 1, n,lambda,radius) - kzmnTE(m, n,lambda,radius))*1.03;
	//delbeta1 = ( kzmnTE(m-1, n, lambda, radius) - kzmnTE(m+1, n, lambda, radius) ) / 2;
	delbeta2 = -((kzmnTE(m,n,lambda,radius) - kzmnTE(m+3,n-1,lambda,radius))*0.5 + (kzmnTE(m,n,lambda,radius) - kzmnTE(m-3,n+1,lambda,radius))*0.5);
	delbeta1 = delbeta1 / 1e3;	delbeta2 = delbeta2 / 1e3;
	delBeta1Edit->setText(QString::number(delbeta1, 10, 4));
	delBeta2Edit->setText(QString::number(delbeta2, 10, 4));

	PlotCurve->clearGraphs();
	SourceMode.~SourceModeGenerationD();
	
}

void showDenisov::on_btn2()
{	
	ReadDesignParas();
	//Pass parameters into the Denisov Class
	Denisov1->SetupDenisov(this->frequency, this->radius, this->m, this->n);
	Denisov1->SetupDenisovParas(this->delbeta1, this->delbeta2, this->l1, this->l2, this->zc1, this->zc2, this->ls1, this->ls2, this->lc1, this->lc2, this->Hz, this->dz, this->Nz, this->mag1, this->mag2);
	Denisov1->SetTangentialEField(this->Nx, this->Ny);
	if (this->Nz % 6 == 0) { this->Sparse = 6; }
	else if (this->Nz % 5 == 0) { this->Sparse = 5; }
	else if (this->Nz % 4 == 0) { this->Sparse = 4; }

	this->Nphi = 240;	this->Nheight = this->Nz / this->Sparse;
	Denisov1->SetSurfaceCurrent(this->Nphi,this->Sparse);
	
	// 绘制扰动曲线
	std::vector<double> sig1, sig2;
	sig1.resize(Nz);	sig2.resize(Nz);
	Denisov1->GetDenisovTurbulence(sig1, sig2);

	QVector <double> Tx, T1y, T2y;
	Tx.resize(Nz);	T1y.resize(Nz);	T2y.resize(Nz);
	for (int i = 0; i < this->Nz; i++) {
		Tx[i] = (i + 0.5)*dz;
		T1y[i] = sig1[i];
		T2y[i] = sig2[i];
	}

	for (int i = 0; i < this->Nz; i++) {
		Tx[i] = Tx[i] * 1.0e3;
		T1y[i] = T1y[i] * 1.0e3;
		T2y[i] = T2y[i] * 1.0e3;
	}
	
	sig1.resize(0);	sig2.resize(0);
	//绘制扰动曲线
	PlotCurve->clearGraphs();
	PlotCurve->addGraph();
	PlotCurve->graph(0)->setPen(QPen(QColor(255, 110, 40)));//Red
	PlotCurve->graph(0)->setData(Tx, T1y);
	PlotCurve->addGraph();
	PlotCurve->graph(1)->setPen(QPen(QColor(40, 110, 255)));//Blue
	PlotCurve->graph(1)->setData(Tx, T2y);
	PlotCurve->xAxis->setRangeUpper(Hz*1e3);
	this->PlotCurve->replot();


	//绘制功率分布曲线-初值
	PowerTotal.resize(Nz+1);
	PowerMain.resize(Nz+1);
	PowerNeighbor.resize(Nz+1);
	PowerCorner.resize(Nz+1);
	ZAxis.resize(Nz+1);
	for (int i = 0; i < Nz+1; i++) {
		PowerTotal[i] = 100;
		PowerMain[i] = 100;
		PowerNeighbor[i] = 0;
		PowerCorner[i] = 0;
		ZAxis[i] = i*dz*1.0e3;
	}

	PlotPower->clearGraphs();
	PlotPower->xAxis->setRangeUpper(this->Hz * 1.0e3);
	PlotPower->addGraph();
	PlotPower->graph(0)->setPen(QPen(QColor(0, 0, 0)));
	PlotPower->graph(0)->setData(ZAxis, PowerTotal);
	PlotPower->graph(0)->setName("Total Modes Power");
	PlotPower->addGraph();
	PlotPower->graph(1)->setPen(QPen(QColor(255, 110, 40)));
	PlotPower->graph(1)->setData(ZAxis, PowerMain);
	PlotPower->graph(1)->setName("Central Modes Power");
	PlotPower->addGraph();
	PlotPower->graph(2)->setPen(QPen(QColor(40, 110, 255)));
	PlotPower->graph(2)->setData(ZAxis, PowerNeighbor);
	PlotPower->graph(2)->setName("Neighbor Modes Power");
	PlotPower->addGraph();
	PlotPower->graph(3)->setPen(QPen(QColor(110, 40, 255)));
	PlotPower->graph(3)->setData(ZAxis, PowerCorner);
	PlotPower->graph(3)->setName("Corner Modes Power");
	PlotPower->legend->setVisible(true);
	
	this->PlotPower->replot();
	

}

void showDenisov::RecieveCoefficients(double _CoeTotal, double _CoeMain, double _CoeNeighbor, double _CoeCorner,int _nn) {

	PowerTotal[_nn + 1] = _CoeTotal*100.0;
	PowerMain[_nn + 1] = _CoeMain*100.0;
	PowerNeighbor[_nn + 1] = _CoeNeighbor*100.0;
	PowerCorner[_nn + 1] = _CoeCorner*100.0;
	if (_nn % 10 == 0) {
		PlotPower->clearGraphs();
		PlotPower->addGraph();
		PlotPower->graph(0)->setPen(QPen(QColor(0, 0, 0)));
		PlotPower->graph(0)->setData(ZAxis, PowerTotal);
		PlotPower->graph(0)->setName("Total Modes Power");
		PlotPower->addGraph();
		PlotPower->graph(1)->setPen(QPen(QColor(255, 110, 40)));
		PlotPower->graph(1)->setData(ZAxis, PowerMain);
		PlotPower->graph(1)->setName("Central Modes Power");
		PlotPower->addGraph();
		PlotPower->graph(2)->setPen(QPen(QColor(40, 110, 255)));
		PlotPower->graph(2)->setData(ZAxis, PowerNeighbor);
		PlotPower->graph(2)->setName("Neighbor Modes Power");
		PlotPower->addGraph();
		PlotPower->graph(3)->setPen(QPen(QColor(110, 40, 255)));
		PlotPower->graph(3)->setData(ZAxis, PowerCorner);
		PlotPower->graph(3)->setName("Corner Modes Power");
		PlotPower->legend->setVisible(false);
		this->PlotPower->replot();
	}


}

void showDenisov::UpdateTanE() {
	vector<vector<double>> Absdata(Nx, vector<double>(Ny, 0));
	vector<vector<complex<double>>> _Ex(Nx, vector<complex<double>>(Ny, 0));
	vector<vector<complex<double>>> _Ey(Nx, vector<complex<double>>(Ny, 0));
	Denisov1->GetTangentialEField(_Ex, _Ey, this->Nx, this->Ny);
	for (int j = 0; j < Nx; j++) {
		for (int i = 0; i < Ny; i++) {
			Absdata[j][i] = sqrt(_Ex[j][i].real()*_Ex[j][i].real() + _Ey[j][i].real()*_Ey[j][i].real());
		}
	}

	paintField->setNum(Nx, Ny);
	paintField->setData(Absdata);
	updatePaint();
}

void showDenisov::UpdateSurfaceJ() {
	vector<vector<double>> SJ;
	SJ.resize(this->Nphi);
	for (int p = 0; p < Nphi; p++) {
		SJ[p].resize(Nheight);
	}
	Denisov1->GetSurfaceCurrent(SJ,Nphi,Nheight);

	//


	QCPColorMap *colorMap = new QCPColorMap(this->DrawCurrent->xAxis, this->DrawCurrent->yAxis);
	int nx = this->Nheight;
	int ny = this->Nphi;
	colorMap->data()->setSize(nx, ny);
	//colorMap->data()->setRange(QCPRange(0, 100.0), QCPRange(0, 360.0)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
	double x, y, z;
	for (int xIndex = 0; xIndex<nx; xIndex++)
	{
		for (int yIndex = 0; yIndex<ny; yIndex++)
		{

			colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
			//colorMap->data()->coordToCell(xIndex*1.5,yIndex*0.2,&xIndex,&yIndex);
			//x = xIndex*0.1;
			//y = yIndex*0.1;
			//double r = 3 * qSqrt(x*x + y*y) + 1e-2;
			//z = 2 * x*(qCos(r + 2) / r - qSin(r + 2) / r); // the B field strength of dipole radiation (modulo physical constants)
			z = SJ[yIndex][xIndex];
			colorMap->data()->setCell(xIndex, yIndex, z);

		}
	}
	// add a color scale:
	QCPColorScale *colorScale = new QCPColorScale(this->DrawCurrent);
	this->DrawCurrent->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
	colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
	colorMap->setColorScale(colorScale); // associate the color map with the color scale
	colorScale->axis()->setLabel("Induced Surface Current");

	// set the color gradient of the color map to one of the presets:
	colorMap->setGradient(QCPColorGradient::gpPolar);
	
	colorMap->rescaleDataRange();
	DrawCurrent->rescaleAxes();
	DrawCurrent->xAxis->setVisible(false);
	DrawCurrent->yAxis->setVisible(false);
	DrawCurrent->replot();
}

void showDenisov::RecieveTangentialEField(std::vector<std::vector<std::complex<double>>> _Ex, std::vector<std::vector<std::complex<double>>> _Ey, int _Nx, int _Ny) {
	
	showDenisov::Nx = _Nx; showDenisov::Ny = _Ny;
	vector<vector<double>> E_Paint;
	E_Paint.resize(Nx);
	for (int i = 0; i < Nx; i++) {
		E_Paint[i].resize(Ny);
	}
	for (int i = 0; i < Nx; i++) {
		for (int j = 0; j < Ny; j++) {
			E_Paint[i][j] = sqrt(abs(_Ex[i][j])*abs(_Ex[i][j]) + abs(_Ey[i][j])*abs(_Ey[i][j]));
		}
	}

	paintField->setNum(Nx, Ny);
	paintField->setData(E_Paint);
	updatePaint();

}



void showDenisov::on_btn3()
{	
	
	//Denisov1->run();

	Denisov1->start();
	//if (Denisov1->isFinished()) {
	//	QString message = "Denisov Simulation is done";
	//	emit SendText(message);
	//}
}

void showDenisov::BtnClose() {
	btn1->setEnabled(false);
	btn2->setEnabled(false);
	btn3->setEnabled(false);
}

void showDenisov::BtnOpen() {
	btn1->setEnabled(true);
	btn2->setEnabled(true);
	btn3->setEnabled(true);
}

void showDenisov::updatePaint()
{
	//每次更新时设置好新的数据  调用这个函数就行
	//paintField->setData(data);
	paintField->update();
}

void showDenisov::CreateButtons() {

	btn1 = new QPushButton(tr("SetBasic"));	btn1->setMaximumWidth(100);
	btn2 = new QPushButton(tr("SetDesign"));	btn2->setMaximumWidth(100);
	btn3 = new QPushButton(tr("RunCMT"));	btn3->setMaximumWidth(100);

	Buttons = new QGroupBox(tr("Buttons")); Buttons->setMaximumWidth(300);
	QHBoxLayout *ButtonH = new QHBoxLayout(Buttons);
	ButtonH->addWidget(btn1);
	ButtonH->addWidget(btn2);
	ButtonH->addWidget(btn3);
}

void showDenisov::CreateBasicParas() {

	//Create BasicPara Aera
	FreqLabel = new QLabel(tr("Freq(GHz)"));		FreqLabel->setMaximumWidth(150);
	FreqEdit = new QLineEdit(tr("140"));			FreqEdit->setMaximumWidth(150);

	RadiusLabel = new QLabel(tr("Radius(mm)"));		RadiusLabel->setMaximumWidth(150);
	RadiusEdit = new QLineEdit(tr("16.8"));			RadiusEdit->setMaximumWidth(150);

	MLabel = new QLabel(tr("Mode m"));				MLabel->setMaximumWidth(150);
	MEdit = new QLineEdit(tr("22"));				MEdit->setMaximumWidth(150);

	NLabel = new QLabel(tr("Mode n"));				NLabel->setMaximumWidth(150);
	NEdit = new QLineEdit(tr("6"));					NEdit->setMaximumWidth(150);

	CutHeightLabel = new QLabel(tr("Cut Height, mm"));				CutHeightLabel->setMaximumWidth(150);
	CutHeightEdit = new QLineEdit(tr("Lc"));						CutHeightEdit->setMaximumWidth(150);	CutHeightEdit->setEnabled(false);

	BasicParas = new QGroupBox(tr("Basic Parameters"));		BasicParas->setMaximumWidth(300);
	QVBoxLayout *BasicV1 = new QVBoxLayout;
	BasicV1->addSpacing(10);
	BasicV1->addWidget(FreqLabel);
	BasicV1->addWidget(RadiusLabel);
	BasicV1->addWidget(MLabel);
	BasicV1->addWidget(NLabel);
	BasicV1->addWidget(CutHeightLabel);
	BasicV1->addSpacing(10);
	QVBoxLayout *BasicV2 = new QVBoxLayout;
	BasicV2->addSpacing(10);
	BasicV2->addWidget(FreqEdit);
	BasicV2->addWidget(RadiusEdit);
	BasicV2->addWidget(MEdit);
	BasicV2->addWidget(NEdit);
	BasicV2->addWidget(CutHeightEdit);
	BasicV2->addSpacing(10);
	QHBoxLayout *BasicH = new QHBoxLayout(BasicParas);
	BasicH->addLayout(BasicV1);
	BasicH->addLayout(BasicV2);

}

void showDenisov::CreateDesignParas() {
	
	//Labels and Edit
	delBeta1Label = new QLabel(tr("delbeta1,rad/mm"));		delBeta1Label->setMaximumWidth(150);
	delBeta1Edit = new QLineEdit(tr("50"));		delBeta1Edit->setMaximumWidth(150);
	delBeta2Label = new QLabel(tr("delbeta2,rad/mm"));		delBeta2Label->setMaximumWidth(150);
	delBeta2Edit = new QLineEdit(tr("20"));		delBeta2Edit->setMaximumWidth(150);
	l1Label = new QLabel(tr("l1"));					l1Label->setMaximumWidth(150);
	l1Edit = new QLineEdit(tr("1"));				l1Edit->setMaximumWidth(150);	l1Edit->setEnabled(false);
	l2Label = new QLabel(tr("l2"));					l2Label->setMaximumWidth(150);
	l2Edit = new QLineEdit(tr("3"));				l2Edit->setMaximumWidth(150);	l2Edit->setEnabled(false);
	mag1Label = new QLabel(tr("mag1 mm"));			mag1Label->setMaximumWidth(150);
	mag1Edit = new QLineEdit(tr("0.058"));			mag1Edit->setMaximumWidth(150);
	mag2Label = new QLabel(tr("mag2 mm"));			mag2Label->setMaximumWidth(150);
	mag2Edit = new QLineEdit(tr("0.047"));			mag2Edit->setMaximumWidth(150);
	zc1Label = new QLabel(tr("Central Position, turb1, mm"));	zc1Label->setMaximumWidth(150);
	zc1Edit = new QLineEdit(tr("25"));							zc1Edit->setMaximumWidth(150);
	zc2Label = new QLabel(tr("Central Position, turb2, mm"));	zc2Label->setMaximumWidth(150);
	zc2Edit = new QLineEdit(tr("35"));							zc2Edit->setMaximumWidth(150);
	lc1Label = new QLabel(tr("Central Length, turb1, mm"));	lc1Label->setMaximumWidth(150);
	lc1Edit = new QLineEdit(tr("30"));						lc1Edit->setMaximumWidth(150);
	lc2Label = new QLabel(tr("Central Length, turb2, mm"));	lc2Label->setMaximumWidth(150);
	lc2Edit = new QLineEdit(tr("40"));						lc2Edit->setMaximumWidth(150);
	ls1Label = new QLabel(tr("ChangeOver Length, turb1, mm"));	ls1Label->setMaximumWidth(150);
	ls1Edit = new QLineEdit(tr("10"));							ls1Edit->setMaximumWidth(150);
	ls2Label = new QLabel(tr("ChangeOver Length, turb2, mm"));	ls2Label->setMaximumWidth(150);
	ls2Edit = new QLineEdit(tr("10"));							ls2Edit->setMaximumWidth(150);
	//Compute Domain setup
	ComHeightLabel = new QLabel(tr("Compute Height, mm"));			CutHeightLabel->setMaximumWidth(150);
	ComHeightEdit = new QLineEdit(tr("150"));						CutHeightEdit->setMaximumWidth(150);
	ComNsLabel = new QLabel(tr("Compute Nsampling"));				ComNsLabel->setMaximumWidth(150);
	ComNsEdit = new QLineEdit(tr("3000"));							ComNsEdit->setMaximumWidth(150);

	DesignParas = new QGroupBox(tr("Turbulence Parameters"));
	DesignParas->setMaximumWidth(300);
	QVBoxLayout *LabelV = new QVBoxLayout;		QVBoxLayout *EditV = new QVBoxLayout;
	LabelV->addWidget(delBeta1Label);			EditV->addWidget(delBeta1Edit);
	LabelV->addWidget(l1Label);					EditV->addWidget(l1Edit);
	LabelV->addWidget(mag1Label);				EditV->addWidget(mag1Edit);
	LabelV->addWidget(zc1Label);				EditV->addWidget(zc1Edit);
	LabelV->addWidget(lc1Label);				EditV->addWidget(lc1Edit);
	LabelV->addWidget(ls1Label);				EditV->addWidget(ls1Edit);
	LabelV->addWidget(delBeta2Label);			EditV->addWidget(delBeta2Edit);
	LabelV->addWidget(l2Label);					EditV->addWidget(l2Edit);
	LabelV->addWidget(mag2Label);				EditV->addWidget(mag2Edit);
	LabelV->addWidget(zc2Label);				EditV->addWidget(zc2Edit);
	LabelV->addWidget(lc2Label);				EditV->addWidget(lc2Edit);
	LabelV->addWidget(ls2Label);				EditV->addWidget(ls2Edit);
	LabelV->addWidget(ComHeightLabel);			EditV->addWidget(ComHeightEdit);
	LabelV->addWidget(ComNsLabel);				EditV->addWidget(ComNsEdit);
	QHBoxLayout *GroupH = new QHBoxLayout(DesignParas);
	GroupH->addLayout(LabelV);
	GroupH->addLayout(EditV);
}

void showDenisov::CreateCutParas() {
	
	CutPosLabel = new QLabel(tr("Cut Height Position, mm"));		CutPosLabel->setMaximumWidth(150);
	CutPosEdit = new QLineEdit(tr("Zcut"));							CutPosEdit->setMaximumWidth(150);
	CutAngLabel = new QLabel(tr("Cut Angular Position, deg"));		CutAngLabel->setMaximumWidth(150);
	CutAngEdit = new QLineEdit(tr("Phic"));							CutAngEdit->setMaximumWidth(150);
	TotalHLabel	= new QLabel(tr("Total Height, mm"));				TotalHLabel->setMaximumWidth(150);
	TotalHEdit = new QLineEdit(tr("Zcut+Lc"));						TotalHEdit->setMaximumWidth(150);		TotalHEdit->setEnabled(false);
	CCParas = new QGroupBox("Cut Paras");		CCParas->setMaximumWidth(300);
	QVBoxLayout *LabelV = new QVBoxLayout;		QVBoxLayout *EditV = new QVBoxLayout;
	LabelV->addWidget(CutPosLabel);				EditV->addWidget(CutPosEdit);
	LabelV->addWidget(CutAngLabel);				EditV->addWidget(CutAngEdit);
	LabelV->addWidget(TotalHLabel);				EditV->addWidget(TotalHEdit);
	QHBoxLayout *GroupH = new QHBoxLayout(CCParas);
	GroupH->addLayout(LabelV);
	GroupH->addLayout(EditV);

}

void showDenisov::CreatePlot() {
	
	//Create Turbulence Curve Plot
	PlotCurve = new QCustomPlot;

//
	PlotCurve->xAxis->setLabel("height, z, mm");
	PlotCurve->yAxis->setLabel("turbulence mag, mm");
	// 设置背景色
	//PlotCurve->setBackground(QColor(50, 50, 50));
	PlotCurve->xAxis->setRange(0, 100);
	PlotCurve->yAxis->setRange(0, 0.1);
	PlotCurve->setMaximumHeight(200);
	PlotCurve->setMaximumWidth(400);

	//Create PowerDistribution Curve Plot
	PlotPower = new QCustomPlot;
	PlotPower->xAxis->setLabel("height,z,mm");
	PlotPower->yAxis->setLabel("Power Percentage, %");
	PlotPower->xAxis->setRange(0, 100);
	PlotPower->yAxis->setRange(0, 110);
	PlotPower->setMaximumHeight(200);
	PlotPower->setMaximumWidth(400);

	DrawCurrent = new QCustomPlot;
	DrawCurrent->xAxis->setLabel("height,z,mm");
	DrawCurrent->yAxis->setLabel("Angular,phi,deg");
	DrawCurrent->xAxis->setRange(0, 100);
	DrawCurrent->yAxis->setRange(0, 360);
	DrawCurrent->setMaximumHeight(300);
	DrawCurrent->setMaximumWidth(900);

}

void showDenisov::ReadBasicParas() {
	bool ok = false;
	//amplitude = valStr.toDouble(&ok);
	//读取基本参数
	showDenisov::frequency = showDenisov::FreqEdit->text().toDouble(&ok);
	showDenisov::frequency = showDenisov::frequency*1.0e9;	
	showDenisov::lambda = C_Speed / showDenisov::frequency;
	showDenisov::radius = showDenisov::RadiusEdit->text().toDouble(&ok);
	showDenisov::radius = showDenisov::radius*1.0e-3;
	showDenisov::m = showDenisov::MEdit->text().toInt(&ok);
	showDenisov::n = showDenisov::NEdit->text().toInt(&ok);
}

void showDenisov::ReadDesignParas() {
	//Read Design Parameters
	bool ok = false;

	showDenisov::l1 = showDenisov::l1Edit->text().toInt(&ok);
	showDenisov::l2 = showDenisov::l2Edit->text().toInt(&ok);
	showDenisov::delbeta1 = showDenisov::delBeta1Edit->text().toDouble(&ok);	showDenisov::delbeta1 = showDenisov::delbeta1 / 1.0e-3;
	showDenisov::delbeta2 = showDenisov::delBeta2Edit->text().toDouble(&ok);	showDenisov::delbeta2 = showDenisov::delbeta2 / 1.0e-3;
	showDenisov::zc1 = showDenisov::zc1Edit->text().toDouble(&ok);	showDenisov::zc1 = showDenisov::zc1*1e-3;
	showDenisov::zc2 = showDenisov::zc2Edit->text().toDouble(&ok);	showDenisov::zc2 = showDenisov::zc2*1e-3;
	showDenisov::lc1 = showDenisov::lc1Edit->text().toDouble(&ok);	showDenisov::lc1 = showDenisov::lc1*1e-3;
	showDenisov::lc2 = showDenisov::lc2Edit->text().toDouble(&ok);	showDenisov::lc2 = showDenisov::lc2*1e-3;
	showDenisov::ls1 = showDenisov::ls1Edit->text().toDouble(&ok);	showDenisov::ls1 = showDenisov::ls1*1e-3;
	showDenisov::ls2 = showDenisov::ls2Edit->text().toDouble(&ok);	showDenisov::ls2 = showDenisov::ls2*1e-3;
	showDenisov::mag1 = showDenisov::mag1Edit->text().toDouble(&ok);	showDenisov::mag1 = showDenisov::mag1*1e-3;
	showDenisov::mag2 = showDenisov::mag2Edit->text().toDouble(&ok);	showDenisov::mag2 = showDenisov::mag2*1e-3;
	showDenisov::Hz = showDenisov::ComHeightEdit->text().toDouble(&ok);	showDenisov::Hz = showDenisov::Hz*1e-3;
	showDenisov::Nz = showDenisov::ComNsEdit->text().toInt(&ok);		//dz = dz*1e-3;
	showDenisov::dz = showDenisov::Hz/ showDenisov::Nz;
}

#include "moc_showDenisov.cpp"
