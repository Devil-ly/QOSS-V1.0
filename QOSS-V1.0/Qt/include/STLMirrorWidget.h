#ifndef STLMirrorWidget_H
#define STLMirrorWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include "../VTK/include/STLMirror.h"


namespace  userInterface {
	class STLMirrorWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		STLMirrorWidget(QWidget *parent = 0);
		~STLMirrorWidget();
		void setMirror(Mirror*);

	private slots:
		void on_BrowseBtn();

	private:
		//page1
		QTabWidget * tabWidget;
		QGroupBox * defGroupBox;
		QGroupBox * baseGroupBox;
		QGroupBox * dimGroupBox;

		QPushButton * BrowseBtn;
		QLineEdit * planeLineEdit;

		STLMirror * stlMirror;
	};
}


#endif // STLMirrorWidget_H
