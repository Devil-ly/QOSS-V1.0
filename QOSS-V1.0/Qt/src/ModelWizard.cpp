#include "Qt/include/ModelWizard.h"
#include "../VTK/include/LimitBox.h"

#include "util/Definition.h"
#include "MyData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

namespace  userInterface {
	ModelWizard::ModelWizard(QWidget *parent)
	{
		addPage(new IntroPage);
		addPage(new BasePage);
		addPage(new SourcePage);
		addPage(new PositionPage);
		setWizardStyle(ModernStyle);
		//setPixmap(QWizard::BannerPixmap, QPixmap("Qt/images/WizardBanner.png"));
		//setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

		resize(800, 600);

		setWindowTitle(tr("Class Wizard"));
	}

	ModelWizard::~ModelWizard()
	{

	}

	void ModelWizard::accept()
	{

		QDialog::accept();
	}

	IntroPage::IntroPage(QWidget *parent)
		: QWizardPage(parent)
	{
		// 设置标题  
		setTitle(tr("Introduction"));
		// 设置图片  
		setPixmap(QWizard::WatermarkPixmap, QPixmap("Qt/images/WizardBanner.png"));

		// 展示标签  
		label = new QLabel(tr("This wizard will generate a skeleton C++ class "
			"definition, including a few functions. You simply "
			"need to specify the class name and set a few "
			"options to produce a header file and an "
			"implementation file for your new C++ class."));
		label->setWordWrap(true);  // 由于标签比较长，在分开的地方需要设置断开到下一行  

		QVBoxLayout *layout = new QVBoxLayout; // 布局管理  
		layout->addWidget(label);
		setLayout(layout);
	}

	BasePage::BasePage(QWidget *parent)
		: QWizardPage(parent)
	{
		setTitle(tr("Base Information"));
		setSubTitle(tr("Specify basic information about the class for which you "
			"want to generate skeleton source code files."));
		//setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png")); // Logo  

		projectLabel = new QLabel(tr("Project name:"));
		projectNameLineEdit = new QLineEdit(tr("Project1"));
		projectLabel->setBuddy(projectNameLineEdit);

		freLabel = new QLabel(tr("Frequency:"));
		freLineEdit = new QLineEdit(tr("44"));
		freLabel->setBuddy(freLineEdit);
		QRegExp regx("[0-9]+$");
		QValidator *validator = new QRegExpValidator(regx, freLineEdit);
		freLineEdit->setValidator(validator);
		freUnitBox = new QComboBox();
		freUnitBox->addItem("GHz");
		freUnitBox->addItem("MHz");
		freUnitBox->addItem("KHz");
		freUnitBox->addItem("Hz");
		freUnitBox->setCurrentIndex(0);
		QHBoxLayout * hBoxLayout = new QHBoxLayout();
		hBoxLayout->addWidget(freLineEdit);
		hBoxLayout->addWidget(freUnitBox);

		unitLabel = new QLabel(tr("Unit:"));
		unitBox = new QComboBox();
		unitBox->addItem("m");
		unitBox->addItem("cm");
		unitBox->addItem("mm");

		QGridLayout *layoutBase = new QGridLayout;
		layoutBase->addWidget(projectLabel, 0, 0);
		layoutBase->addWidget(projectNameLineEdit, 0, 1);
		layoutBase->addWidget(freLabel, 1, 0);
		layoutBase->addLayout(hBoxLayout, 1, 1);
		layoutBase->addWidget(unitLabel, 2, 0);
		layoutBase->addWidget(unitBox, 2, 1);

		baseGroupBox = new QGroupBox(tr("Base"));
		baseGroupBox->setLayout(layoutBase);

		groupBox = new QGroupBox(tr("Pattern"));

		lowerRadioButton = new QRadioButton(tr("Lower order"));
		higherRadioButton = new QRadioButton(tr("Higher order "));
		waveguideRadioButton = new QRadioButton(tr("Waveguide"));
		//copyCtorCheckBox = new QCheckBox(tr("&Generate copy constructor and "
		//	"operator="));

		lowerRadioButton->setChecked(true);

		//connect(defaultCtorRadioButton, SIGNAL(toggled(bool)),
		//	copyCtorCheckBox, SLOT(setEnabled(bool)));

		QVBoxLayout *groupBoxLayout = new QVBoxLayout;
		groupBoxLayout->addWidget(lowerRadioButton);
		groupBoxLayout->addWidget(higherRadioButton);
		groupBoxLayout->addWidget(waveguideRadioButton);
		groupBox->setLayout(groupBoxLayout);

		QGridLayout *layout = new QGridLayout;        // InfoPage的布局  
		layout->addWidget(baseGroupBox, 0, 0, 1, 2);
		layout->addWidget(groupBox, 1, 0, 1, 2);

		// 注册域  
		registerField("projectName", projectNameLineEdit);
		registerField("freLine", freLineEdit);
		registerField("freUnit", freUnitBox);
		registerField("unitBox", unitBox);
		registerField("lowerRadioButton", lowerRadioButton);
		registerField("higherRadioButton", higherRadioButton);
		registerField("waveguideRadioButton", waveguideRadioButton);

		setLayout(layout);
	}

	bool BasePage::validatePage()
	{
		double fre = freLineEdit->text().toDouble();
		switch (field("freUnit").toInt())
		{
		case 0:
			 fre = fre * 1e9;
			break;
		case 1:
			fre = fre * 1e6;
			break;
		case 2:
			fre = fre * 1e3;
			break;
		default:
			break;
		}
		MyData::getInstance()->setFrequency(fre);
		double unit;
		switch (field("unitBox").toInt())
		{
		case 0:  // m
			unit = 1;
			break;
		case 1: // cm
			unit = 1e-2;
			break;
		case 2: // mm
			unit = 1e-3;
			break;
		default:
			unit = 1;
			break;
		}
		MyData::getInstance()->setUnit(unit);

		return true;
	}

	SourcePage::SourcePage(QWidget *parent)
		: QWizardPage(parent)
	{
		m = 0;
		n = 1;
		amplitude = 1;
		type = 1;
		rotation = 2;
		radius = 0.064;
		fre = 44.0e9;
		kind = 1;

		setTitle(tr("Excitation source"));
		setSubTitle(tr("Specify basic information about the class for which you "
			"want to generate skeleton source code files."));

		typeLabel = new QLabel(tr("Type:"));
		typeComboBox = new QComboBox();
		typeLabel->setBuddy(typeComboBox);
		typeComboBox->addItem(tr("TE"));
		typeComboBox->addItem(tr("TM"));

		mLabel = new QLabel(tr("m:"));
		mLineEdit = new QLineEdit(tr("0"));
		mLabel->setBuddy(mLineEdit);
		QRegExp regx("[0-9]+$");
		QValidator *mvalidator = new QRegExpValidator(regx, mLineEdit);
		mLineEdit->setValidator(mvalidator);
		

		nLabel = new QLabel(tr("n:"));
		nLineEdit = new QLineEdit(tr("1"));
		nLabel->setBuddy(nLineEdit);
		QValidator *nvalidator = new QRegExpValidator(regx, nLineEdit);
		nLineEdit->setValidator(nvalidator);


		amplitudeLabel = new QLabel(tr("Amplitude:"));
		amplitudeLineEdit = new QLineEdit(tr("1"));
		amplitudeLabel->setBuddy(amplitudeLineEdit);
		QValidator *amplitudevalidator = new QRegExpValidator(regx, amplitudeLineEdit);
		amplitudeLineEdit->setValidator(amplitudevalidator);


		radiusLabel = new QLabel(tr("Waveguide radius:"));
		radiusLineEdit = new QLineEdit(tr("0.0016"));
		radiusLabel->setBuddy(radiusLineEdit);


		rotationLabel = new QLabel(tr("Rotation:"));
		rotationComboBox = new QComboBox();
		rotationLabel->setBuddy(rotationComboBox);
		rotationComboBox->addItem(tr("Left"));
		rotationComboBox->addItem(tr("Right"));
		

		// 注册域  
		//registerField("type", typeComboBox);
		//registerField("rotation", rotationComboBox);
		//registerField("m", mLineEdit);
		//registerField("n", nLineEdit);
		//registerField("radius", radiusLineEdit);
		//registerField("amplitude", amplitudeLineEdit);

		QGridLayout *layout = new QGridLayout;
		layout->setColumnMinimumWidth(0, 20);
		layout->addWidget(typeLabel, 0, 0);
		layout->addWidget(typeComboBox, 0, 1);
		layout->addWidget(mLabel, 1, 0);
		layout->addWidget(mLineEdit, 1, 1);
		layout->addWidget(nLabel, 2, 0);
		layout->addWidget(nLineEdit, 2, 1);
		layout->addWidget(amplitudeLabel, 3, 0);
		layout->addWidget(amplitudeLineEdit, 3, 1);
		layout->addWidget(radiusLabel, 4, 0);
		layout->addWidget(radiusLineEdit, 4, 1);
		layout->addWidget(rotationLabel, 5, 0);
		layout->addWidget(rotationComboBox, 5, 1);

		groupBox = new  QGroupBox(tr("Source Parameter"));
		groupBox->setLayout(layout);

		nnLabel = new QLabel(tr("n:"));
		nnLineEdit = new QLineEdit(tr("5"));
		nLabel->setBuddy(nnLineEdit);
		QValidator *nnvalidator = new QRegExpValidator(regx, nnLineEdit);
		nnLineEdit->setValidator(nnvalidator);

		mmLabel = new QLabel(tr("n:"));
		mmLineEdit = new QLineEdit(tr("5"));
		nLabel->setBuddy(mmLineEdit);
		QValidator *mmvalidator = new QRegExpValidator(regx, mmLineEdit);
		mmLineEdit->setValidator(mmvalidator);

		dsLabel = new QLabel(tr("ds:"));
		dsLineEdit = new QLineEdit(tr("0.05"));

		openLabel = new QLabel(tr("file"));
		openLineEdit = new QLineEdit(tr(""));
		openLineEdit->setEnabled(false);
		openBtn = new QPushButton(tr("Open"));
		connect(openBtn, SIGNAL(clicked()), this, SLOT(on_openBtn()));


		QGridLayout *layout1 = new QGridLayout;
		layout1->setColumnMinimumWidth(0, 20);

		layout1->addWidget(nnLabel, 0, 0);
		layout1->addWidget(nnLineEdit, 0, 1);
		layout1->addWidget(mmLabel, 1, 0);
		layout1->addWidget(mmLineEdit, 1, 1);
		layout1->addWidget(dsLabel, 2, 0);
		layout1->addWidget(dsLineEdit, 2, 1);
		layout1->addWidget(openLabel, 3, 0);
		layout1->addWidget(openLineEdit, 3, 1);
		layout1->addWidget(openBtn, 3, 2);

		recGroupBox = new  QGroupBox(tr("Source Parameter"));
		recGroupBox->setLayout(layout1);
		recGroupBox->setHidden(true);

		QImage* img = new QImage;
		img->load("Qt/images/Pattern.png");
		imgLabel = new QLabel;
		imgLabel->setPixmap(QPixmap::fromImage(*img));

		
		source = make_shared<calculation::SourceModeGeneration>();
		//source->SetOutputProperty(300);
		source->SetOutputProperty(250);
				
		if (!source->FieldCalculation_Circular())
		{
			// do something圆波导参数设置有误！
		}	
		vector<vector<complex<double>>> Ex, Ey;
		source->GetEY(Ey);
		source->GetEX(Ex);

		imgLabel2 = new QPaint2DField();
		imgLabel2->calcData(Ex, Ey);

		QHBoxLayout *hBoxLayout = new QHBoxLayout;
		hBoxLayout->addWidget(imgLabel);
		hBoxLayout->addWidget(imgLabel2);

		QVBoxLayout *groupBoxLayout = new QVBoxLayout;
		groupBoxLayout->addWidget(groupBox);
		groupBoxLayout->addWidget(recGroupBox);
		groupBoxLayout->addSpacing(20);
		groupBoxLayout->addLayout(hBoxLayout);
		setLayout(groupBoxLayout);

	}

	void SourcePage::initializePage()
	{
		disconnect(mLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_mLineEditChange(QString)));
		disconnect(rotationComboBox, SIGNAL(activated(int)),
			this, SLOT(on_rotationComboBoxChange(int)));
		disconnect(radiusLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_radiusLineEditChange(QString)));
		disconnect(amplitudeLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_amplitudeLineEditChange(QString)));
		disconnect(nLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_nLineEditChange(QString)));
		switch (field("unitBox").toInt())
		{
		case 0:  // m
			unit = 1;
			break;
		case 1: // cm
			unit = 1e-2;
			break;
		case 2: // mm
			unit = 1e-3;
			break;
		default:
			unit = 1;
			break;
		}
		
		if (field("lowerRadioButton").toBool())
		{
			typeComboBox->setCurrentIndex(0);
			typeComboBox->setEnabled(false);
			rotationComboBox->setCurrentIndex(1);
			rotationComboBox->setEnabled(true);
			mLineEdit->setEnabled(false);
			mLineEdit->setText(tr("0"));
			nLineEdit->setText(tr("1"));
			amplitudeLineEdit->setText(tr("1"));
			rotationComboBox->setCurrentIndex(0);
			kind = 1;
			radiusLabel->setHidden(false);
			radiusLineEdit->setHidden(false);
			radiusLineEdit->setText(QString::number(0.016 / unit));
			radius = 0.016 / unit;
			n = 1;
			m = 0;
			amplitude = 1;
			rotation = 1;
			MyData::getInstance()->setPattern(0);

			groupBox->setHidden(false);
			recGroupBox->setHidden(true);
			imgLabel->setHidden(false);
			imgLabel2->setHidden(false);
		}
		else if (field("higherRadioButton").toBool())
		{
			typeComboBox->setCurrentIndex(0);
			typeComboBox->setEnabled(false);
			rotationComboBox->setCurrentIndex(1);
			rotationComboBox->setEnabled(true);
			mLineEdit->setEnabled(true);
			mLineEdit->setText(tr("22"));
			nLineEdit->setText(tr("6"));
			amplitudeLineEdit->setText(tr("1"));
			radiusLabel->setHidden(false);
			radiusLineEdit->setHidden(false);
			radiusLineEdit->setText(QString::number(0.064 / unit));
			rotationComboBox->setCurrentIndex(0);
			kind = 2;
			rotation = 1;
			n = 6;
			m = 22;
			amplitude = 1;
			radius = 0.064 / unit;
			MyData::getInstance()->setPattern(1);

			groupBox->setHidden(false);
			recGroupBox->setHidden(true);
			imgLabel->setHidden(false);
			imgLabel2->setHidden(false);
		}
		else if (field("waveguideRadioButton").toBool())
		{
			typeComboBox->setEnabled(true);
			rotationComboBox->setEnabled(false);
			radiusLabel->setHidden(true);
			radiusLineEdit->setHidden(true);
			kind = 3;

			MyData::getInstance()->setPattern(2);
			groupBox->setHidden(true);
			recGroupBox->setHidden(false);
			imgLabel->setHidden(true);
			imgLabel2->setHidden(true);
			//QWizard::NextButton;
		}
		fre = field("freLine").toDouble();
		switch (field("freUnit").toInt())
		{
		case 0:  // GHz
			fre = fre * 1e9;
			break;
		case 1: // MHz
			fre = fre * 1e6;
			break;
		case 2: // kHz
			fre = fre * 1e3;
			break;
		default:
			break;
		}
		connect(mLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_mLineEditChange(QString)));
		connect(rotationComboBox, SIGNAL(activated(int)),
			this, SLOT(on_rotationComboBoxChange(int)));
		connect(radiusLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_radiusLineEditChange(QString)));
		connect(amplitudeLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_amplitudeLineEditChange(QString)));
		connect(nLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_nLineEditChange(QString)));
		updatePaint();
	}

	bool SourcePage::validatePage()
	{
		source->SetSource_Circular(kind, type, rotation, m, n, fre, radius);

		if (!source->FieldCalculation_Circular())
		{
			QMessageBox::warning(NULL, "Warning",
				"123456");
			radiusLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
		MyData::getInstance()->setSource(source);
		return true;
	}

	void SourcePage::updatePaint()
	{
		source->SetSource_Circular(kind, type, rotation, m, n, fre, radius);

		if (!source->FieldCalculation_Circular())
		{
			QMessageBox::warning(NULL, "Warning",
				"Error of input parameter!");
			radiusLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		radiusLineEdit->setStyleSheet("background-color:rgba(255,255,255,255)");
		vector<vector<complex<double>>> Ex, Ey;
		source->GetEY(Ey);
		source->GetEX(Ex);

		imgLabel2->calcData(Ex, Ey);
		imgLabel2->update();

	}

	void SourcePage::on_nLineEditChange(const QString & valStr)
	{
		bool ok = false;
		n = valStr.toInt(&ok);
		updatePaint();
	}

	void SourcePage::on_radiusLineEditChange(const QString &valStr)
	{
		bool ok = false;
		double _radius = valStr.toDouble(&ok);
		if (!ok)
		{
			radiusLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (_radius < 0.0001)
		{
			radiusLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		radius = _radius;
		radiusLineEdit->setStyleSheet("background-color:rgba(255,255,255,255)");
		updatePaint();
	}

	void SourcePage::on_amplitudeLineEditChange(const QString &valStr)
	{
		bool ok = false;
		amplitude = valStr.toDouble(&ok);
	}

	void SourcePage::on_rotationComboBoxChange(int index)
	{
		rotation = index + 1;
	}

	void SourcePage::on_openBtn()
	{
		QString filename = QFileDialog::getOpenFileName(this,
			tr("Open the file"),
			"",
			tr("*.txt"));
		if (!filename.isEmpty())
		{
			openLineEdit->setText(filename);
		}
	}

	void SourcePage::on_mLineEditChange(const QString & valStr)
	{
		bool ok = false;
		m = valStr.toInt(&ok);

		updatePaint();
	}

	PositionPage::PositionPage(QWidget *parent)
		: QWizardPage(parent)
	{		
		setTitle(tr("Position of Mirrors"));
		setSubTitle(tr("Specify basic information about the class for which you "
			"want to generate skeleton source code files."));

		numLabel = new QLabel(tr("Number of mirrors:"));
		numComboBox = new QComboBox();
		int sumMirrors = MAX_NUM_OF_MIRROS;
		for (int i = 2; i <= sumMirrors; ++i)
		{
			numComboBox->addItem(QString::number(i));
		}
		numComboBox->setCurrentIndex(1);
		numLabel->setBuddy(numLabel);
		QHBoxLayout *numLayout = new QHBoxLayout;
		numLayout->addWidget(numLabel);
		numLayout->addWidget(numComboBox);
		connect(numComboBox, SIGNAL(activated(int)), this, SLOT(numComboBoxChange(int)));

		int num = 3;

		allMirrorsLayout = new QGridLayout(this);
		// 镜子 1
		phiLineLabel = new QLabel(tr("Phi:"));
		propagationLengthLabel = new QLabel(tr("Propagation length:"));
		phiLineEdit1 = new QLineEdit;
		propagationLengthLineEdit1 = new QLineEdit;
		QGridLayout *mirrorLayout1 = new QGridLayout;
		mirrorsGroupBox1 = new QGroupBox;
		mirrorLayout1->addWidget(phiLineLabel, 0, 0);
		mirrorLayout1->addWidget(phiLineEdit1, 0, 1);
		mirrorLayout1->addWidget(propagationLengthLabel, 1, 0);
		mirrorLayout1->addWidget(propagationLengthLineEdit1, 1, 1);
		mirrorsGroupBox1->setTitle(tr("Mirror ") + QString::number(1));
		mirrorsGroupBox1->setLayout(mirrorLayout1);
		allMirrorsLayout->addWidget(mirrorsGroupBox1, 0, 0);

		// 镜子 2
		phiLineLabe2 = new QLabel(tr("Phi:"));
		propagationLengthLabe2 = new QLabel(tr("Propagation length:"));
		phiLineEdit2 = new QLineEdit;
		propagationLengthLineEdit2 = new QLineEdit;
		QGridLayout *mirrorLayout2 = new QGridLayout;
		mirrorsGroupBox2 = new QGroupBox;
		mirrorLayout2->addWidget(phiLineLabe2, 0, 0);
		mirrorLayout2->addWidget(phiLineEdit2, 0, 1);
		mirrorLayout2->addWidget(propagationLengthLabe2, 1, 0);
		mirrorLayout2->addWidget(propagationLengthLineEdit2, 1, 1);
		mirrorsGroupBox2->setTitle(tr("Mirror ") + QString::number(2));
		mirrorsGroupBox2->setLayout(mirrorLayout2);
		allMirrorsLayout->addWidget(mirrorsGroupBox2, 1, 0);

		// 镜子 3
		phiLineLabe3 = new QLabel(tr("Phi:"));
		propagationLengthLabe3 = new QLabel(tr("Propagation length:"));
		phiLineEdit3 = new QLineEdit;
		propagationLengthLineEdit3 = new QLineEdit;
		QGridLayout *mirrorLayout3 = new QGridLayout;
		mirrorsGroupBox3 = new QGroupBox;
		mirrorLayout3->addWidget(phiLineLabe3, 0, 0);
		mirrorLayout3->addWidget(phiLineEdit3, 0, 1);
		mirrorLayout3->addWidget(propagationLengthLabe3, 1, 0);
		mirrorLayout3->addWidget(propagationLengthLineEdit3, 1, 1);
		mirrorsGroupBox3->setTitle(tr("Mirror ") + QString::number(3));
		mirrorsGroupBox3->setLayout(mirrorLayout3);
		allMirrorsLayout->addWidget(mirrorsGroupBox3, 2, 0);

		// 镜子 4
		phiLineLabe4 = new QLabel(tr("Phi:"));
		propagationLengthLabe4 = new QLabel(tr("Propagation length:"));
		phiLineEdit4 = new QLineEdit;
		propagationLengthLineEdit4 = new QLineEdit;
		QGridLayout *mirrorLayout4 = new QGridLayout;
		mirrorsGroupBox4 = new QGroupBox;
		mirrorLayout4->addWidget(phiLineLabe4, 0, 0);
		mirrorLayout4->addWidget(phiLineEdit4, 0, 1);
		mirrorLayout4->addWidget(propagationLengthLabe4, 1, 0);
		mirrorLayout4->addWidget(propagationLengthLineEdit4, 1, 1);
		mirrorsGroupBox4->setTitle(tr("Mirror ") + QString::number(4));
		mirrorsGroupBox4->setLayout(mirrorLayout4);
		allMirrorsLayout->addWidget(mirrorsGroupBox4, 3, 0);
		mirrorsGroupBox4->setHidden(true);

		xLabel = new QLabel(tr("Length:"));
		xLineEdit = new QLineEdit(tr("1"));
		xLabel->setBuddy(xLineEdit);
		connect(xLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_xLineEditChange(QString)));

		yLabel = new QLabel(tr("Height:"));
		yLineEdit = new QLineEdit(tr("0.5"));
		yLabel->setBuddy(yLineEdit);

		zLabel = new QLabel(tr("Width:"));
		zLineEdit = new QLineEdit(tr("1"));
		zLabel->setBuddy(zLineEdit);
		connect(zLineEdit, SIGNAL(textChanged(QString)),
			this, SLOT(on_zLineEditChange(QString)));

		QGridLayout *xyzLayout = new QGridLayout;
		xyzLayout->addWidget(xLabel, 0, 0);
		xyzLayout->addWidget(xLineEdit, 0, 1);
		xyzLayout->addWidget(yLabel, 1, 0);
		xyzLayout->addWidget(yLineEdit, 1, 1);
		xyzLayout->addWidget(zLabel, 2, 0);
		xyzLayout->addWidget(zLineEdit, 2, 1);

		limitGroupBox = new QGroupBox(tr("Limited size"));
		limitGroupBox->setLayout(xyzLayout);

		scrollArea = new QScrollArea();
		scrollArea->setLayout(allMirrorsLayout);

		QGridLayout *layout = new QGridLayout();
		layout->addLayout(numLayout, 1, 0);
		layout->addWidget(limitGroupBox, 0, 0);
		layout->addWidget(scrollArea, 2, 0);

		mirrorPosition = make_shared<calculation::MirrorPosition>();

		imgLabel = new QPaint2DMirror(mirrorPosition);
		QPixmap pixmap(400, 500);
		imgLabel->setPixmap(pixmap);

		QHBoxLayout *hBoxLayout = new QHBoxLayout;
		hBoxLayout->addLayout(layout);
		hBoxLayout->addWidget(imgLabel);

		setLayout(hBoxLayout);

	}

	PositionPage::~PositionPage()
	{

	}

	void PositionPage::on_xLineEditChange(const QString &valStr)
	{
		bool ok = false;
		double length = valStr.toDouble(&ok);
		if (!ok)
		{
			xLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (length < 0.0001)
		{
			xLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		imgLabel->setLength(length);
		xLineEdit->setStyleSheet("background-color:rgba(255,255,255,255)");
		imgLabel->update();
	}

	void PositionPage::on_zLineEditChange(const QString &valStr)
	{
		bool ok = false;
		double width = valStr.toDouble(&ok);
		if (!ok)
		{
			zLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (width < 0.0001)
		{
			zLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		imgLabel->setWidth(width);
		zLineEdit->setStyleSheet("background-color:rgba(255,255,255,255)");
		imgLabel->update();
	}

	void PositionPage::on_propagationLengthLineEdit1Change(const QString &valStr)
	{
		bool ok = false;
		double length = valStr.toDouble(&ok);
		if (!ok)
		{
			propagationLengthLineEdit1->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (length < 0.0001)
		{
			propagationLengthLineEdit1->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		propagationLengthLineEdit1->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->setLightLength(0, length);
		imgLabel->update();
	}

	void PositionPage::on_propagationLengthLineEdit2Change(const QString &valStr)
	{
		bool ok = false;
		double length = valStr.toDouble(&ok);
		if (!ok)
		{
			propagationLengthLineEdit2->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (length < 0.0001)
		{
			propagationLengthLineEdit2->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		propagationLengthLineEdit2->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->setLightLength(1, length);
		imgLabel->update();
	}

	void PositionPage::on_propagationLengthLineEdit3Change(const QString &valStr)
	{
		bool ok = false;
		double length = valStr.toDouble(&ok);
		if (!ok)
		{
			propagationLengthLineEdit2->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (length < 0.0001)
		{
			propagationLengthLineEdit2->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		propagationLengthLineEdit3->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->setLightLength(2, length);
		imgLabel->update();
	}

	void PositionPage::on_propagationLengthLineEdit4Change(const QString &valStr)
	{
		bool ok = false;
		double length = valStr.toDouble(&ok);
		if (!ok)
		{
			propagationLengthLineEdit3->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (length < 0.0001)
		{
			propagationLengthLineEdit3->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		propagationLengthLineEdit3->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->setLightLength(3, length);
		imgLabel->update();
	}

	void PositionPage::on_phiLineEdit1Change(const QString &valStr)
	{
		bool ok = false;
		double phi = valStr.toDouble(&ok);
		if (!ok)
		{
			phiLineEdit1->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (phi < 0.0001 || phi > 179.99)
		{
			phiLineEdit1->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		phiLineEdit1->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->calcLightPhi(0, phi);
		imgLabel->update();
	}

	void PositionPage::on_phiLineEdit2Change(const QString &valStr)
	{
		bool ok = false;
		double phi = valStr.toDouble(&ok);
		if (!ok)
		{
			phiLineEdit2->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (phi < 0.0001 || phi > 179.99)
		{
			phiLineEdit2->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		phiLineEdit2->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->calcLightPhi(1, phi);
		imgLabel->update();
	}

	void PositionPage::on_phiLineEdit3Change(const QString &valStr)
	{
		bool ok = false;
		double phi = valStr.toDouble(&ok);
		if (!ok)
		{
			phiLineEdit3->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (phi < 0.0001 || phi > 179.99)
		{
			phiLineEdit3->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		phiLineEdit3->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->calcLightPhi(2, phi);
		imgLabel->update();
	}

	void PositionPage::on_phiLineEdit4Change(const QString &valStr)
	{
		bool ok = false;
		double phi = valStr.toDouble(&ok);
		if (!ok)
		{
			phiLineEdit4->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		if (phi < 0.0001 || phi > 179.99)
		{
			phiLineEdit4->setStyleSheet("background-color:rgba(255,0,0,255)");
			return;
		}
		phiLineEdit4->setStyleSheet("background-color:rgba(255,255,255,255)");
		mirrorPosition->calcLightPhi(3, phi);
		imgLabel->update();
	}

	void PositionPage::initializePage()
	{

		disconnect(phiLineEdit1, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit1Change(QString)));
		disconnect(phiLineEdit2, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit2Change(QString)));
		disconnect(phiLineEdit3, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit3Change(QString)));
		disconnect(phiLineEdit4, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit4Change(QString)));
		disconnect(propagationLengthLineEdit1, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit1Change(QString)));
		disconnect(propagationLengthLineEdit2, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit2Change(QString)));
		disconnect(propagationLengthLineEdit3, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit3Change(QString)));
		disconnect(propagationLengthLineEdit4, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit4Change(QString)));
		
		source = MyData::getInstance()->getSource();
		Vector3 org, V1; double Length1; double theta1;
		Vector3 V2; double Length2; double theta2;
		Vector3 V3; double Length3; double theta3;
		Vector3 V4;
		source->GetCircularSystemOpticalPath_TE0n_3Mirrors(org, V1, Length1, theta1,
			V2, Length2, theta2, V3, Length3, theta3, V4);

		imgLabel->setMirrorNum(3);
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
		double width;
		double length;
		mirrorPosition->getBoundaryByDefault(length, width);
		imgLabel->setBoundary(length, width);
		xLineEdit->setText(QString::number(length));
		zLineEdit->setText(QString::number(width));

		if (field("waveguideRadioButton").toBool())
		{
			xLineEdit->setText(tr("0.5"));
			zLineEdit->setText(tr("0.5"));

		}
		propagationLengthLineEdit1->setText(QString::number(Length1));
		propagationLengthLineEdit2->setText(QString::number(Length2));
		propagationLengthLineEdit3->setText(QString::number(Length3));
		phiLineEdit1->setText(QString::number(theta1 / Pi * 180));
		phiLineEdit2->setText(QString::number(theta2 / Pi * 180));
		phiLineEdit3->setText(QString::number(theta3 / Pi * 180));

		connect(phiLineEdit1, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit1Change(QString)));
		connect(phiLineEdit2, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit2Change(QString)));
		connect(phiLineEdit3, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit3Change(QString)));
		connect(phiLineEdit4, SIGNAL(textChanged(QString)),
			this, SLOT(on_phiLineEdit4Change(QString)));
		connect(propagationLengthLineEdit1, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit1Change(QString)));
		connect(propagationLengthLineEdit2, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit2Change(QString)));
		connect(propagationLengthLineEdit3, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit3Change(QString)));
		connect(propagationLengthLineEdit4, SIGNAL(textChanged(QString)),
			this, SLOT(on_propagationLengthLineEdit4Change(QString)));

		if (field("waveguideRadioButton").toBool())
		{
			scrollArea->setHidden(true);
			imgLabel->setHidden(true);
			numLabel->setHidden(true);
			numComboBox->setHidden(true);
			return;
		}
		scrollArea->setHidden(false);
		imgLabel->setHidden(false);
		numLabel->setHidden(false);
		numComboBox->setHidden(false);

	}

	bool PositionPage::validatePage()
	{
		double length = xLineEdit->text().toDouble();
		double width = zLineEdit->text().toDouble();
		double height = yLineEdit->text().toDouble();
		MyData::getInstance()->setLimitBox(make_shared<LimitBox>(length, width, height));
		MyData::getInstance()->setMirrorPosition(mirrorPosition);
		return true;
	}

	void PositionPage::numComboBoxChange(int index)
	{
		if (index == 0) {
			mirrorsGroupBox3->setHidden(true);
			mirrorsGroupBox4->setHidden(true);
		}
		else if (index == 1) {
			mirrorsGroupBox3->setHidden(false);
			mirrorsGroupBox4->setHidden(true);
		}
		else
		{
			mirrorsGroupBox3->setHidden(false);
			mirrorsGroupBox4->setHidden(false);
		}
		limitGroupBox->setHidden(true);
		limitGroupBox->setHidden(false);
		scrollArea->setHidden(true);
		scrollArea->setHidden(false);
		
	}



}