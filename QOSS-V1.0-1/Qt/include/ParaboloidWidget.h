#ifndef ParaboloidWidget_H
#define ParaboloidWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include "../VTK/include/Paraboloid.h"


namespace  userInterface {
	class ParaboloidWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		ParaboloidWidget(QWidget *parent = 0);
		~ParaboloidWidget();
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

		Paraboloid * paraboloid;
	};
}


#endif // ParaboloidWidget_H
