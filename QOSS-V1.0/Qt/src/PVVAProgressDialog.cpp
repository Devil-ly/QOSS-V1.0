#include "..\include\PVVAProgressDialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace userInterface;

PVVAProgressDialog::PVVAProgressDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("PVVA Calculation"));
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
	connect(stopBtn, SIGNAL(clicked()), this, SLOT(on_stop()));

	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addSpacing(300);
	hlayout->addWidget(closeBtn);
	hlayout->addWidget(stopBtn);

	layout->addLayout(hlayout);

}

PVVAProgressDialog::~PVVAProgressDialog()
{
}

void PVVAProgressDialog::setMirrorNum(int num)
{
	MirrorNum = num;
	mainBar->setRange(0, num+2);
}

void PVVAProgressDialog::setMainValue(int val)
{
	mainBar->setValue(val);
	if (0 == val)
		txtLabel->setText(tr("Initializing..."));
	else if (val <= MirrorNum)
		txtLabel->setText(tr("PVVA computing Mirror") + QString::number(val + 1) + tr(" ..."));
	else
	{
		txtLabel->setText(tr("All computed, Loading Fields..."));
	}
}

void PVVAProgressDialog::setSlaverValue(int val)
{
	slaverBar->setValue(val);
}

void PVVAProgressDialog::on_close()
{
	hide();
}

void PVVAProgressDialog::on_stop()
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

void PVVAProgressDialog::closeEvent(QCloseEvent * event)
{
	hide();
}

