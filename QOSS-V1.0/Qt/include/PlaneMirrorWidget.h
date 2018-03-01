#ifndef PlaneMirrorWidget_H
#define PlaneMirrorWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include "../VTK/include/PlaneMirror.h"


namespace  userInterface {
	class PlaneMirrorWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		PlaneMirrorWidget(QWidget *parent = 0);
		~PlaneMirrorWidget();
		void setMirror(Mirror*);

	private slots:
		void on_radiusChange(QString);
		void on_focusChange(QString);

	private:
		//page1
		QTabWidget * tabWidget;
		QGroupBox * defGroupBox;
		QGroupBox * baseGroupBox;
		QGroupBox * dimGroupBox;

		QLabel * radiuslabel;
		QLabel * depthlabel;

		QLineEdit * radiusLineEidt;
		QLineEdit * depthLineEidt;

		QLabel * label;
		QLineEdit * nameLineEidt;

		PlaneMirror * planeMirror;
	};
}


#endif // PlaneMirrorWidget_H
