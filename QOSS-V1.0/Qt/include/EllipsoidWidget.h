#ifndef EllipsoidWidget_H
#define EllipsoidWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include "../VTK/include/Ellipsoid.h"


namespace  userInterface {
	class EllipsoidWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		EllipsoidWidget(QWidget *parent = 0);
		~EllipsoidWidget();
		void setMirror(Mirror*);

	private slots:
		void on_aChange(QString);
		void on_bChange(QString);
		void on_cChange(QString);

	private:
		//page1
		QTabWidget * tabWidget;
		QGroupBox * defGroupBox;
		QGroupBox * baseGroupBox;
		QGroupBox * dimGroupBox;

		QLabel * alabel;
		QLabel * blabel;
		QLabel * clabel;

		QLineEdit * aLineEidt;
		QLineEdit * bLineEidt;
		QLineEdit * cLineEidt;

		Ellipsoid * ellipsoid;
	};
}


#endif // EllipsoidWidget_H
