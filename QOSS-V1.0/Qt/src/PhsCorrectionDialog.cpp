#include "..\include\PhsCorrectionDialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace userInterface;

PhsCorrectionDialog::PhsCorrectionDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Phase Correction"));
	
	lengthLabel = new QLabel(tr("Length:"));
	lengthLineEdit = new QLineEdit(tr("0.016"));
	dsLabel = new QLabel(tr("Accuracy:"));
	dsComboBox = new QComboBox;
	dsComboBox->addItem(tr("Crude"));
	dsComboBox->addItem(tr("Standard"));
	dsComboBox->addItem(tr("Fine"));
	dsComboBox->setCurrentIndex(1);

	gaussLabel = new QLabel(tr("Gaussain:"));
	gaussLineEdit = new QLineEdit(tr("0.016"));

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(lengthLabel, 0, 0);
	gridLayout->addWidget(lengthLineEdit, 0, 1);
	gridLayout->addWidget(dsLabel, 1, 0);
	gridLayout->addWidget(dsComboBox, 1, 1);
	gridLayout->addWidget(gaussLabel, 2, 0);
	gridLayout->addWidget(gaussLineEdit, 2, 1);

	OKBtn = new QPushButton(tr("OK"));
	connect(OKBtn, SIGNAL(clicked()), this, SLOT(on_OK()));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(on_cancel()));

	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addSpacing(100);
	hlayout->addWidget(OKBtn);
	hlayout->addWidget(cancelBtn);

	QVBoxLayout *layout = new QVBoxLayout(this);

	layout->addLayout(gridLayout);
	layout->addLayout(hlayout);

}

PhsCorrectionDialog::~PhsCorrectionDialog()
{
}

void PhsCorrectionDialog::getData(int & dsIndex, double & length)
{
	length = lengthLineEdit->text().toDouble();
	dsIndex = dsComboBox->currentIndex();
}

void PhsCorrectionDialog::getTarget(double& temp)
{
	temp = gaussLineEdit->text().toDouble();
}

void PhsCorrectionDialog::setIsNeedMesh(bool isNeedMesh)
{
	if (isNeedMesh)
	{
		lengthLabel->setHidden(false);
		lengthLineEdit->setHidden(false);
		dsLabel->setHidden(false);
		dsComboBox->setHidden(false);
	}
	else
	{
		lengthLabel->setHidden(true);
		lengthLineEdit->setHidden(true);
		dsLabel->setHidden(true);
		dsComboBox->setHidden(true);
	}
}

void PhsCorrectionDialog::on_OK()
{
	accept();
}

void PhsCorrectionDialog::on_cancel()
{
	close();
}
