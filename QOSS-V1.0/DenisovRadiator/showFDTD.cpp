#include "CodeJin/SourceModeGenerationD.h"
#include "showFDTD.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <cmath>
using namespace calculation;
using namespace userInterface;

showFDTD::showFDTD(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	//resize(1200, 400);
	setWindowTitle("FDTDPanel_WaveguideRadiator");
	//这是画图板 好像是李赟单独写的类
	//paintField = new QPaintField;
	imgGroupBox = new QGroupBox;
	addDefGroupBox(imgGroupBox, "Defination.png");
	DrawField = new QCustomPlot;
	DrawAperture = new QCustomPlot;
	FDTD = new QTFDTDThread;

	CreateButtons();

	//ParaTable = new QTableView();
	//ParaTable -> setMaximumWidth(300);
	//ParaTable ->column

	CreateBasicParas();

	Message = new QLabel(tr("Message"));
	Message->setMaximumWidth(300);
	Message->setHidden(true);
	// change progressBar
	//Percent = new QLabel(tr("Percentage"));
	progressBar = new QProgressBar;
	progressBar->setMaximumWidth(300);
	progressBar->setRange(0, 100);
	progressBar->setValue(0);
	progressBar->setHidden(true);

	//computation Stage
	showFDTD::ComputationStage = 0;

	connect(btn1, SIGNAL(clicked()), this, SLOT(on_btn1()));
	connect(btn2, SIGNAL(clicked()), this, SLOT(on_btn2()));
	connect(btnLoad, SIGNAL(clicked()), this, SLOT(on_btnLoad()));
	//connect(btnStop, SIGNAL(clicked()), this, SLOT(on_btnStop()));

	//信号和槽函数的参数必须具有相同的顺序和相同的类型
	//connect(btn2, SIGNAL(clicked()), this, SLOT(ChangeValue()));
	connect(this, SIGNAL(SendValue(double)), this, SLOT(ChangeValue(double)));

	//连接 FDTDthread的信号和槽
	connect(showFDTD::FDTD, SIGNAL(sendMainValue(int)), this, SLOT(recieveInt(int)));
	connect(showFDTD::FDTD, SIGNAL(sendSlaverValue(float)), this, SLOT(recieveFloat(float)));

	//布局 右面板-操作列
	QVBoxLayout * vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(BasicParas);
	//vBoxLayout->addWidget(DesignParas);
	//vBoxLayout->addWidget(CCParas);
	vBoxLayout->addWidget(Buttons);
	vBoxLayout->addWidget(Message);
	vBoxLayout->addWidget(progressBar);

	QHBoxLayout * DrawZoneHU = new QHBoxLayout;
	//DrawZoneHU->addWidget(imgGroupBox);
	DrawZoneHU->addWidget(DrawField);
	DrawZoneHU->addWidget(DrawAperture);


	QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->addLayout(DrawZoneHU);
	hBoxLayout->addLayout(vBoxLayout);

	/*
	//Horizontal 左面板画图列
	QHBoxLayout * DrawZoneHU = new QHBoxLayout;
	DrawZoneHU->addWidget(imgGroupBox);
	DrawZoneHU->addWidget(paintField);
	QHBoxLayout * DrawZoneHD = new QHBoxLayout;
	DrawZoneHD->addWidget(DrawField);
	DrawZoneHD->addWidget(DrawAperture);

	//DrawZoneHD->addWidget(paintCurrent);
	//Vertrical
	QVBoxLayout * DrawZoneV = new QVBoxLayout;
	DrawZoneV->addLayout(DrawZoneHU);
	DrawZoneV->addLayout(DrawZoneHD);


	QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->addLayout(DrawZoneV);
	hBoxLayout->addLayout(vBoxLayout);
	*/

}

showFDTD::~showFDTD()
{

}

void showFDTD::ChangeValue(double _in) {
	QString ss;
	ss.setNum(_in);
	Percent->setText(ss);
}

void showFDTD::ChangeText(QString _in) {
	Message->setText(_in);
}

void showFDTD::recieveFloat(float _in) {
	//QString ss;
	//ss.setNum(_in);
	progressBar->setValue(_in);

	//Percent->setText(ss);
	if (this->ComputationStage == 1) {
		//Load and Paint Field
		FILE* Fieldin;
		vector<vector<float>> Ex;
		vector<vector<float>> Ey;
		vector<vector<float>> Ez;
		vector<vector<float>> E;
		int fNx, fNz;
		Fieldin = fopen("./XOZ.dat", "rb");
		fread(&fNx, sizeof(int), 1, Fieldin);
		fread(&fNz, sizeof(int), 1, Fieldin);
		Ex.resize(fNx);	Ey.resize(fNx);	Ez.resize(fNx);	E.resize(fNx);
		for (int i = 0; i < fNx; i++) {
			Ex[i].resize(fNz); Ey[i].resize(fNz); Ez[i].resize(fNz); E[i].resize(fNz);
		}
		for (int k = 0; k < fNz; k++) {
			for (int i = 0; i < fNx; i++) {
				fread(&Ex[i][k], sizeof(float), 1, Fieldin);
			}
		}
		for (int k = 0; k < fNz; k++) {
			for (int i = 0; i < fNx; i++) {
				fread(&Ey[i][k], sizeof(float), 1, Fieldin);
			}
		}
		for (int k = 0; k < fNz; k++) {
			for (int i = 0; i < fNx; i++) {
				fread(&Ez[i][k], sizeof(float), 1, Fieldin);
			}
		}
		fclose(Fieldin);
		for (int k = 0; k < fNz; k++) {
			for (int i = 0; i < fNx; i++) {
				E[i][k] = sqrt(Ex[i][k] * Ex[i][k] + Ey[i][k] * Ey[i][k]);// +Ez[i][k] * Ez[i][k]);
			}
		}

		//


		QCPColorMap *colorMap = new QCPColorMap(this->DrawField->xAxis, this->DrawField->yAxis);
		int nx = fNx;
		int ny = fNz;
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
				z = E[xIndex][yIndex];
				colorMap->data()->setCell(xIndex, yIndex, z);

			}
		}
		// add a color scale:
		DrawField->clearItems(); DrawField->clearGraphs();
		QCPColorScale *colorScale = new QCPColorScale(this->DrawField);
		this->DrawField->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
		colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
		colorMap->setColorScale(colorScale); // associate the color map with the color scale

											 // set the color gradient of the color map to one of the presets:
		colorMap->setGradient(QCPColorGradient::gpPolar);

		colorMap->rescaleDataRange();
		DrawField->rescaleAxes();
		DrawField->xAxis->setVisible(false);
		DrawField->yAxis->setVisible(false);
		DrawField->replot();
		colorScale->~QCPColorScale();
	}

}

void showFDTD::recieveInt(int _in) {
	QString ss;

	showFDTD::ComputationStage = _in;
	///this->ComputationStage = 3;

	if (showFDTD::ComputationStage == 0) {
		ss = "Setting Up FDTD computation";
		Message->setText(ss);
		progressBar->setValue(0);
	}
	else if (showFDTD::ComputationStage == 1) {
		ss = "Performing FDTD computation";
		Message->setText(ss);
		progressBar->setValue(0);
	}
	else if (showFDTD::ComputationStage == 2) {
		ss = "Performing Aperture computation";
		Message->setText(ss);
		progressBar->setValue(0);
	}
	else if (showFDTD::ComputationStage == 3) {
		ss = "All computated";
		Message->setText(ss);
		//Load and Paint Aperture Field
		//Load and Paint Field
		FILE* Fieldin;
		vector<vector<complex<float>>> Eu;
		vector<vector<complex<float>>> Ev;
		vector<vector<float>> E;
		Eu.resize(this->Na);	Ev.resize(this->Na);	E.resize(this->Na);
		for (int i = 0; i < this->Na; i++) {
			Eu[i].resize(this->Na); Ev[i].resize(this->Na); E[i].resize(this->Na);
		}


		FILE* Binread;
		Binread = fopen("./PropagatedEField.dat", "rb");
		int tempNu, tempNv;
		fread(&tempNu, sizeof(int), 1, Binread);	//Nu
		fread(&tempNv, sizeof(int), 1, Binread);	//Nv
		float temp;
		fread(&temp, sizeof(float), 1, Binread);	//du
		fread(&temp, sizeof(float), 1, Binread);	//dv
		fread(&temp, sizeof(float), 1, Binread);	//freq
													//注意 文件里是complex<float>的，目标位置是complex<double>的
		complex<float> readbuf;
		for (int v = 0; v<this->Na; v++) {
			for (int u = 0; u<this->Na; u++) {
				fread(&Eu[u][v], sizeof(complex<float>), 1, Binread);

			}
		}
		for (int v = 0; v<this->Na; v++) {
			for (int u = 0; u<this->Na; u++) {
				fread(&Eu[u][v], sizeof(complex<float>), 1, Binread);
			}
		}
		fclose(Binread);
		for (int v = 0; v<this->Na; v++) {
			for (int u = 0; u<this->Na; u++) {
				E[u][v] = sqrt(Eu[u][v].real()*Eu[u][v].real() + Eu[u][v].imag()* Eu[u][v].imag() + Ev[u][v].real()*Ev[u][v].real() + Ev[u][v].imag()* Ev[u][v].imag());
			}
		}


		QCPColorMap *colorMap = new QCPColorMap(this->DrawAperture->xAxis, this->DrawAperture->yAxis);
		int nx = this->Na;
		int ny = this->Na;
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
				z = E[yIndex][xIndex];
				colorMap->data()->setCell(xIndex, yIndex, z);

			}
		}
		// add a color scale:
		QCPColorScale *colorScale = new QCPColorScale(this->DrawAperture);
		this->DrawAperture->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
		colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
		colorMap->setColorScale(colorScale); // associate the color map with the color scale

											 // set the color gradient of the color map to one of the presets:
		colorMap->setGradient(QCPColorGradient::gpPolar);

		colorMap->rescaleDataRange();
		DrawAperture->rescaleAxes();
		DrawAperture->xAxis->setVisible(false);
		DrawAperture->yAxis->setVisible(false);
		DrawAperture->replot();
		colorScale->~QCPColorScale();

		FDTD->GetProPowerRatio(showFDTD::PowerRatio);
		QString ss;
		ss.setNum(showFDTD::PowerRatio);
		this->PREdit->setText(ss);

		this->BtnOpen();
	}
}

void showFDTD::on_btn1()
{//按按钮1，读取基本参数，绘制输入模式场图，并计算基本参数

	ReadBasicParas();

	//低阶辐射器
	if (showFDTD::m == 0) {
		SourceModeGenerationD SourceMode(1, 1, 0, showFDTD::m, showFDTD::n, showFDTD::frequency, showFDTD::radius, 0, 0, showFDTD::Ns);
		double temp1, temp2, temp3;
		SourceMode.GetCircularWaveguideProperty(temp1, temp2, temp3, showFDTD::lcut);
		CutHeightEdit->setEnabled(true);
		CutHeightEdit->setText(QString::number(lcut*1e3, 10, 4));
		CutHeightEdit->setEnabled(false);
	}
	//高阶辐射器 暂定右旋
	else if (showFDTD::m > 0) {
		SourceModeGenerationD SourceMode(2, 1, 2, showFDTD::m, showFDTD::n, showFDTD::frequency, showFDTD::radius, 0, 0, showFDTD::Ns);
		double temp1, temp2, temp3;
		SourceMode.GetCircularWaveguideProperty(temp1, temp2, temp3, showFDTD::lcut);
		CutHeightEdit->setEnabled(true);
		CutHeightEdit->setText(QString::number(lcut*1e3, 10, 4));
		CutHeightEdit->setEnabled(false);
	}
}

void showFDTD::on_btn2()
{
	this->ReadBasicParas();
	//NoPlotCurves	
	FDTD->SetFDTDThread(showFDTD::frequency, showFDTD::radius, showFDTD::Ns, showFDTD::m, showFDTD::n, showFDTD::lcut,
		showFDTD::prodis, showFDTD::aperlen, showFDTD::Na);
	FDTD->start();
	this->BtnClose();
	progressBar->setHidden(false);
	Message->setHidden(false);


}


void showFDTD::on_btnLoad() {
	//Load and Paint Aperture Field
	FILE* Binread;
	Binread = fopen("./PropagatedEField.dat", "rb");
	int tempNu, tempNv;
	fread(&tempNu, sizeof(int), 1, Binread);	//Nu
	fread(&tempNv, sizeof(int), 1, Binread);	//Nv

	vector<vector<complex<float>>> Eu;
	vector<vector<complex<float>>> Ev;
	vector<vector<float>> E;
	Eu.resize(tempNu);	Ev.resize(tempNu);	E.resize(tempNu);
	for (int i = 0; i < tempNu; i++) {
		Eu[i].resize(tempNv); Ev[i].resize(tempNv); E[i].resize(tempNv);
	}
	float temp;
	fread(&temp, sizeof(float), 1, Binread);	//du
	fread(&temp, sizeof(float), 1, Binread);	//dv
	fread(&temp, sizeof(float), 1, Binread);	//freq
												//ע�� �ļ�����complex<float>�ģ�Ŀ��λ����complex<double>��
	complex<float> readbuf;
	for (int v = 0; v<tempNv; v++) {
		for (int u = 0; u<tempNu; u++) {
			fread(&Eu[u][v], sizeof(complex<float>), 1, Binread);

		}
	}
	for (int v = 0; v<tempNv; v++) {
		for (int u = 0; u<tempNu; u++) {
			fread(&Eu[u][v], sizeof(complex<float>), 1, Binread);
		}
	}
	fclose(Binread);
	for (int v = 0; v<tempNv; v++) {
		for (int u = 0; u<tempNu; u++) {
			E[u][v] = sqrt(Eu[u][v].real()*Eu[u][v].real() + Eu[u][v].imag()* Eu[u][v].imag() + Ev[u][v].real()*Ev[u][v].real() + Ev[u][v].imag()* Ev[u][v].imag());
		}
	}


	QCPColorMap *colorMap = new QCPColorMap(this->DrawAperture->xAxis, this->DrawAperture->yAxis);
	int nx = tempNu;
	int ny = tempNv;
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
			z = E[yIndex][xIndex];
			colorMap->data()->setCell(xIndex, yIndex, z);

		}
	}
	// add a color scale:
	QCPColorScale *colorScale = new QCPColorScale(this->DrawAperture);
	this->DrawAperture->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
	colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
	colorMap->setColorScale(colorScale); // associate the color map with the color scale

										 // set the color gradient of the color map to one of the presets:
	colorMap->setGradient(QCPColorGradient::gpPolar);

	colorMap->rescaleDataRange();
	DrawAperture->rescaleAxes();
	DrawAperture->xAxis->setVisible(false);
	DrawAperture->yAxis->setVisible(false);
	DrawAperture->replot();
	colorScale->~QCPColorScale();
}

void showFDTD::BtnClose() {
	btn1->setEnabled(false);
	btn2->setEnabled(false);
}

void showFDTD::BtnOpen() {
	btn1->setEnabled(true);
	btn2->setEnabled(true);
}

void showFDTD::CreateButtons() {

	btn1 = new QPushButton(tr("SetBasic"));
	btn1->setMaximumWidth(100);
	btn2 = new QPushButton(tr("Compute"));
	btn2->setMaximumWidth(100);
	//btnStop = new QPushButton(tr("Stop"));	
	//btnStop->setMaximumWidth(150);
	btnLoad = new QPushButton(tr("LoadPAP"));
	btnLoad->setMaximumWidth(100);

	Buttons = new QGroupBox(tr("Buttons")); Buttons->setMaximumWidth(300);
	QHBoxLayout *ButtonH = new QHBoxLayout(Buttons);
	ButtonH->addWidget(btn1);
	ButtonH->addWidget(btn2);
	ButtonH->addWidget(btnLoad);

	//QHBoxLayout *ButtonH1 = new QHBoxLayout;
	//ButtonH1->addWidget(btn1);
	//ButtonH1->addWidget(btn2);
	//QHBoxLayout *ButtonH2 = new QHBoxLayout;
	//ButtonH2->addWidget(btnStop);
	//ButtonH2->addWidget(btnLoad);
	//QVBoxLayout *ButtonV = new QVBoxLayout(Buttons);
	//ButtonV->addLayout(ButtonH1);
	//ButtonV->addLayout(ButtonH2);

}

void showFDTD::CreateBasicParas() {

	//Create BasicPara Aera
	FreqLabel = new QLabel(tr("Freq(GHz)"));		FreqLabel->setMaximumWidth(150);
	FreqEdit = new QLineEdit(tr("44"));			FreqEdit->setMaximumWidth(150);

	RadiusLabel = new QLabel(tr("Radius(mm)"));		RadiusLabel->setMaximumWidth(150);
	RadiusEdit = new QLineEdit(tr("16.0"));			RadiusEdit->setMaximumWidth(150);

	NsLabel = new QLabel(tr("Nsample"));			NsLabel->setMaximumWidth(150);
	NsEdit = new QLineEdit(tr("80"));				NsEdit->setMaximumWidth(150);

	MLabel = new QLabel(tr("Mode m"));				MLabel->setMaximumWidth(150);
	MEdit = new QLineEdit(tr("0"));				MEdit->setMaximumWidth(150);

	NLabel = new QLabel(tr("Mode n"));				NLabel->setMaximumWidth(150);
	NEdit = new QLineEdit(tr("1"));					NEdit->setMaximumWidth(150);

	CutHeightLabel = new QLabel(tr("Cut Height, mm"));				CutHeightLabel->setMaximumWidth(150);
	CutHeightEdit = new QLineEdit(tr("Lc"));						CutHeightEdit->setMaximumWidth(150);	CutHeightEdit->setEnabled(false);

	PDLabel = new QLabel(tr("Pro D, mm"));			PDLabel->setMaximumWidth(150);
	PDEdit = new QLineEdit(tr("100"));				PDEdit->setMaximumWidth(150);

	ALLabel = new QLabel(tr("Aperture L, mm"));		ALLabel->setMaximumWidth(150);
	ALEdit = new QLineEdit(tr("100"));				ALEdit->setMaximumWidth(150);

	NaLabel = new QLabel(tr("ApertureSampling"));	NaLabel->setMaximumWidth(150);
	NaEdit = new QLineEdit(tr("101"));				NaEdit->setMaximumWidth(150);

	PRLabel = new QLabel(tr("PowerRatio, %"));		NaLabel->setMaximumWidth(150);
	//PREdit = new QLineEdit(tr("not Computed"));		PREdit->setMaximumWidth(150);
	//PREdit->setEnabled(false);

	BasicParas = new QGroupBox(tr("Basic Parameters"));		BasicParas->setMaximumWidth(300);
	QVBoxLayout *BasicV1 = new QVBoxLayout;
	BasicV1->addSpacing(10);
	BasicV1->addWidget(FreqLabel);
	BasicV1->addWidget(RadiusLabel);
	BasicV1->addWidget(NsLabel);
	BasicV1->addWidget(MLabel);
	BasicV1->addWidget(NLabel);
	BasicV1->addWidget(CutHeightLabel);
	BasicV1->addWidget(PDLabel);
	BasicV1->addWidget(ALLabel);
	BasicV1->addWidget(NaLabel);
	//BasicV1->addWidget(PRLabel);
	BasicV1->addSpacing(10);
	QVBoxLayout *BasicV2 = new QVBoxLayout;
	BasicV2->addSpacing(10);
	BasicV2->addWidget(FreqEdit);
	BasicV2->addWidget(RadiusEdit);
	BasicV2->addWidget(NsEdit);
	BasicV2->addWidget(MEdit);
	BasicV2->addWidget(NEdit);
	BasicV2->addWidget(CutHeightEdit);
	BasicV2->addWidget(PDEdit);
	BasicV2->addWidget(ALEdit);
	BasicV2->addWidget(NaEdit);
	//BasicV2->addWidget(PREdit);
	BasicV2->addSpacing(10);
	QHBoxLayout *BasicH = new QHBoxLayout(BasicParas);
	BasicH->addLayout(BasicV1);
	BasicH->addLayout(BasicV2);

}


void showFDTD::ReadBasicParas() {
	bool ok = false;
	//amplitude = valStr.toDouble(&ok);
	//读取基本参数
	showFDTD::frequency = showFDTD::FreqEdit->text().toDouble(&ok);
	showFDTD::frequency = showFDTD::frequency*1.0e9;
	showFDTD::lambda = C_Speed / showFDTD::frequency;
	showFDTD::radius = showFDTD::RadiusEdit->text().toDouble(&ok);
	showFDTD::radius = showFDTD::radius*1.0e-3;
	showFDTD::Ns = showFDTD::NsEdit->text().toInt(&ok);
	showFDTD::m = showFDTD::MEdit->text().toInt(&ok);
	showFDTD::n = showFDTD::NEdit->text().toInt(&ok);
	showFDTD::prodis = showFDTD::PDEdit->text().toDouble(&ok);
	showFDTD::prodis = showFDTD::prodis*1.0e-3;
	showFDTD::aperlen = showFDTD::ALEdit->text().toDouble(&ok);
	showFDTD::aperlen = showFDTD::aperlen*1.0e-3;
	showFDTD::Na = showFDTD::NaEdit->text().toInt(&ok);
}

void showFDTD::addDefGroupBox(QGroupBox * _defGroupBox, QString filename)
{
	// page1
	//defGroupBox
	QLabel * imgLlabel;
	imgLlabel = new QLabel;
	imgLlabel->setPixmap(QPixmap(filename));

	QGridLayout * layout1 = new QGridLayout;
	layout1->addWidget(imgLlabel);

	//defGroupBox = new QGroupBox;
	_defGroupBox->setLayout(layout1);
	_defGroupBox->setTitle(tr("Definition methods"));
}
//#include "moc_showFDTD.cpp"
