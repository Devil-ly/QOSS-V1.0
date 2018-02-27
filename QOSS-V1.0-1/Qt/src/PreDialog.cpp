#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
#include <vtkAutoInit.h> 

#include "Qt/include/PreDialog.h"
#include "Qt/include/ModelWizard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
namespace  userInterface {

	PreDialog::PreDialog(QWidget *parent)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		resize(416, 249);
		QImage _image;
		_image.load("Qt/images/logo.png");
		setAutoFillBackground(true);
		QPalette pal(palette());
		pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
			Qt::SmoothTransformation)));
		setPalette(pal);
		QLabel *logoLabel = new QLabel(tr("Quasi optical simulation system"));
		logoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		QPushButton *createBtn = new QPushButton(tr("Create a new model"));
		createBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		connect(createBtn, SIGNAL(clicked()), this, SLOT(createModel()));

		QPushButton *openBtn = new QPushButton(tr("Open an existing model"));
		openBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(logoLabel);
		layout->addWidget(createBtn);
		layout->addWidget(openBtn);
		QHBoxLayout *layoutH = new QHBoxLayout(this);
		layoutH->addLayout(layout, Qt::AlignLeft);
		layoutH->addSpacing(99);

	}

	PreDialog::~PreDialog()
	{

	}

	void PreDialog::createModel()
	{
		ModelWizard modelWizard;
		if (modelWizard.exec() != QDialog::Accepted)
		{
			rejected();
		}
		accept();
	}
}
