#ifndef GAUSSIANWIDGET_H
#define GAUSSIANWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "GraphTransWidget.h"

#include "VTK/include/Gaussain.h"

#include "../VTK/include/PlaneMirror.h"

namespace  userInterface {
	class GaussianWidget : public GraphTransWidget
	{
		Q_OBJECT

	public:
		GaussianWidget(QWidget *parent = 0);
		virtual ~GaussianWidget();

		//void setWidgetData(Field const * _Source);
		//bool getWidgetData(Field * _Source,
		//	Parameterization* parameterization) const;

		// 用于临时显示源的位置
		bool getField(Field*&);

		// 用于临时显示源的位置
		void setMirror(Mirror*);

	private slots:
		void on_widthChange(QString var);
		void on_depthChange(QString var);

	private:
		//void changeText(int index, const string& t);

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
		QLineEdit * RecLineEdit;

		//page3
		QGroupBox * imgGroupBox;

		QGroupBox * sourceGroupBox;
		QLabel * z0label;
		QLabel * w0label;
		QLabel * frelabel;
		QLabel * dslabel;

		QLineEdit * z0LineEdit;
		QLineEdit * w0LineEdit;
		QLineEdit * freLineEdit;
		QLineEdit * DsLineEdit;
		QLineEdit * dsLineEdit;

		PlaneMirror * planeMirror;

	};
}


#endif // GAUSSIANWIDGET_H
