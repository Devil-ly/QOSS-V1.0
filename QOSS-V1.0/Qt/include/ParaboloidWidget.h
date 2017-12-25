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

namespace  userInterface {
	class ParaboloidWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		ParaboloidWidget(QWidget *parent = 0, int wayButton = 0);
		~ParaboloidWidget();

	private slots:
		//void on_DefChange(QString);

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
	};
}


#endif // ParaboloidWidget_H
