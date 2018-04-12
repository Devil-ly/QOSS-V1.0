#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
#include <vtkAutoInit.h> 

#include "Qt/include/PreDialog.h"
#include "Qt/include/ModelWizard.h"
#include "Qt/include/ModelBtnDialog.h"
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

		QPushButton *createBtn = new QPushButton(tr("Create a new project"));
		createBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		connect(createBtn, SIGNAL(clicked()), this, SLOT(createNew()));

		QPushButton *modelBtn = new QPushButton(tr("Open a model"));
		modelBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		connect(modelBtn, SIGNAL(clicked()), this, SLOT(createModel()));

		QPushButton *openBtn = new QPushButton(tr("Open a project"));
		openBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(logoLabel);
		layout->addWidget(createBtn);
		layout->addWidget(modelBtn);
		layout->addWidget(openBtn);
		QHBoxLayout *layoutH = new QHBoxLayout(this);
		layoutH->addLayout(layout, Qt::AlignLeft);
		layoutH->addSpacing(99);

		isModel = false;

	}

	PreDialog::~PreDialog()
	{

	}

	void PreDialog::createNew()
	{
		ModelWizard modelWizard;
		if (modelWizard.exec() != QDialog::Accepted)
		{
			rejected();
		}
		accept();
	}

	void PreDialog::createModel()
	{
		ModelBtnDialog modelWizard;
		if (modelWizard.exec() != QDialog::Accepted)
		{
			rejected();
		}
		isModel = true;
		accept();
	}
}
