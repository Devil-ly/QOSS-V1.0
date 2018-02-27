#ifndef ParabolicCylinderWidget_H
#define ParabolicCylinderWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include "../VTK/include/ParabolicCylinder.h"


namespace  userInterface {
	class ParabolicCylinderWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		ParabolicCylinderWidget(QWidget *parent = 0);
		~ParabolicCylinderWidget();
		void setMirror(Mirror*);

	private slots:
		void on_radiusChange(QString);
		void on_focusChange(QString);
		void on_zmaxChange(QString);
		void on_zminChange(QString);

	private:
		//page1
		QTabWidget * tabWidget;
		QGroupBox * defGroupBox;
		QGroupBox * baseGroupBox;
		QGroupBox * dimGroupBox;

		QLabel * radiuslabel;
		QLabel * depthlabel;
		QLabel * zmaxlabel;
		QLabel * zminlabel;

		QLineEdit * radiusLineEidt;
		QLineEdit * depthLineEidt;
		QLineEdit * zmaxLineEidt;
		QLineEdit * zminLineEidt;

		QLabel * label;
		QLineEdit * nameLineEidt;

		ParabolicCylinder * parabolicCylinder;
	};
}


#endif // ParabolicCylinderWidget_H
