#include "..\include\FDTDProgressDialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace userInterface;

FDTDProgressDialog::FDTDProgressDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("FDTD Calculation"));
	txtLabel = new QLabel();
	mainBar = new QProgressBar();
	mainBar->setRange(0, 3);
	mainBar->setMinimumWidth(500);
	slaverBar = new QProgressBar();
	slaverBar->setRange(0, 100);
	slaverBar->setMinimumWidth(500);
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(txtLabel);
	layout->addWidget(mainBar);
	layout->addWidget(slaverBar);

	closeBtn = new QPushButton(tr("Close"));
	connect(closeBtn, SIGNAL(clicked()), this, SLOT(on_close()));
	stopBtn = new QPushButton(tr("Stop"));
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addSpacing(300);
	hlayout->addWidget(closeBtn);
	hlayout->addWidget(stopBtn);

	layout->addLayout(hlayout);

}

FDTDProgressDialog::~FDTDProgressDialog()
{
}

void FDTDProgressDialog::setMainValue(int val)
{
	mainBar->setValue(val);
	switch (val)
	{
	case 0:
		txtLabel->setText(tr("Initializing..."));
		break;
	case 1:
		txtLabel->setText(tr("FDTD computing..."));
		break;
	case 2:
		txtLabel->setText(tr("Huygens Box Computing..."));
		break;
	case 3:
		txtLabel->setText(tr("All computed, Loading Fields..."));
		break;
	default:
		break;
	}
}

void FDTDProgressDialog::setSlaverValue(int val)
{
	slaverBar->setValue(val);
}

void FDTDProgressDialog::on_close()
{
	hide();
}

void FDTDProgressDialog::on_stop()
{
	switch (QMessageBox::question(this, tr("Question"),
		tr("Whether to terminate the calculation immediately?"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
	{
	case QMessageBox::Yes: // 加载源
		close();
		emit sendStop();
		break;
	case QMessageBox::No: //  不加载源
		return;
	default:
		break;
	}
}

void FDTDProgressDialog::closeEvent(QCloseEvent * event)
{
	hide();
}

