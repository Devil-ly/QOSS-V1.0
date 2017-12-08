#ifndef ParaboloidWidget_H
#define ParaboloidWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>


#include "VTK/include/Surface.h"
#include "Qt/include/GraphTransWidget.h"

class ParaboloidWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	ParaboloidWidget(QWidget *parent = 0, int wayButton = 0);
	~ParaboloidWidget();

	void setWidgetData(Surface const* _modleData);
	bool getWidgetData(Surface * _modleData, 
		Parameterization* parameterization) const;
	QString getName() const;

private slots:
    void on_DefChange(QString);

private:
	void changeText(int, const string&);
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

#endif // ParaboloidWidget_H
