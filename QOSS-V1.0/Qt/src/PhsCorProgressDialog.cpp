#include "..\include\PhsCorProgressDialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace userInterface;

PhsCorProgressDialog::PhsCorProgressDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Phs Calculation"));
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

PhsCorProgressDialog::~PhsCorProgressDialog()
{
}

void PhsCorProgressDialog::setMainValue(int val)
{
	mainBar->setValue(val);
	switch (val)
	{
	case 0:
		txtLabel->setText(tr("PVVA computing..."));
		break;
	case 1:
		txtLabel->setText(tr("Mesh..."));
		break;
	case 2:
		txtLabel->setText(tr("Phase Correction..."));
		break;
	case 3:
		txtLabel->setText(tr("All computed, Loading Mirror..."));
		break;
	default:
		break;
	}
}

void PhsCorProgressDialog::setSlaverValue(int val)
{
	slaverBar->setValue(val);
}

void PhsCorProgressDialog::on_close()
{
	hide();
}

void PhsCorProgressDialog::on_stop()
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

void PhsCorProgressDialog::ReceiveError(int index)
{
	switch (index)
	{
	case 0:
		QMessageBox::warning(NULL, "Error",
			"The output field is not intersected with the mirror. Please reset the source!");

		return;
	case 1:
		QMessageBox::warning(NULL, "Error",
			"The interception length is larger than the original mirror size, please reset it!");
		return;
	default:
		break;
	}
}

void PhsCorProgressDialog::closeEvent(QCloseEvent * event)
{
	hide();
}

