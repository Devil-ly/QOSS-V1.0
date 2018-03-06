#ifndef APERTUREFIELDWIDGET_H
#define APERTUREFIELDWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "Qt/include/GraphTransWidget.h"

#include "VTK/include/ApertureField.h"
#include "../VTK/include/PlaneMirror.h"

namespace  userInterface {

	class ApertureFieldWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		ApertureFieldWidget(QWidget *parent = 0);
		~ApertureFieldWidget();

		bool getField(Field *& ptr);

		// 用于临时显示源的位置
		void setMirror(Mirror*);

		private slots:
		void EfileEeven();

		void on_widthChange(QString var);
		void on_depthChange(QString var);

	private:

		//page1
		QTabWidget * tabWidget;
		QGroupBox * defGroupBox;
		QGroupBox * baseGroupBox;
		QGroupBox * dimGroupBox;

		QLabel * widthlabel;
		QLabel * depthlabel;

		QLineEdit * widthLineEdit;
		QLineEdit * depthLineEdit;

		QLabel * label;
		QLineEdit * RecLineEidt;

		//page3
		QGroupBox * imgGroupBox;

		QLabel * scalelabel;
		QLabel * phaselabel;
		QLabel * datadeflabel;

		QLineEdit * scaleLineEidt;
		QLineEdit * phaseLineEidt;
		QComboBox * datadefComboBox;

		QGroupBox * sourceGroupBox;
		QLabel * Efieldlabel;
		QLabel * StartNumlabel;
		QLabel * StartNumColumnlabel;

		QPushButton * Efieldbtn;

		QLineEdit * EfieldLineEidt;
		QLineEdit * StartNumLineEidt;
		QLineEdit * StartNumColumnEidt;

		QGroupBox * destinationGroupBox;
		QLabel * UNumlabel;
		QLabel * VNumlabel;

		QLineEdit * UNumLineEidt;
		QLineEdit * VNumLineEidt;

		PlaneMirror * planeMirror;
	};
}
#endif // RECTANGLEWIDGET_H
