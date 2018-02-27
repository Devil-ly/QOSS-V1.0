/*
*	created by liyun 2017/12/8
*   function 界面 选择镜面类型
*   y^2 = 4Fx  // f为交点
*   R 为y的最大值
*   version 1.0
*/

#ifndef MIRRORTYPEWIDGET_H
#define MIRRORTYPEWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QPushButton>
#include "../VTK/include/Mirror.h"


namespace  userInterface {
	class MirrorTypeWidget : public QDialog
	{
		Q_OBJECT

	public:
		MirrorTypeWidget(QWidget *parent = 0);
		~MirrorTypeWidget();

	signals:
		void sendMirrorType(int);

	private slots:
		void on_planeBtn();
		void on_ellipsoidBtn();
		void on_paraboloidBtn();
		void on_parabolicCylinderBtn();
	private:

		QTabWidget * tabWidget;

		//page1
		QPushButton *planeBtn;
		QPushButton *ellipsoidBtn;
		QPushButton *paraboloidBtn;
		QPushButton *parabolicCylinderBtn;
	};
}



#endif // MIRRORTYPEWIDGET_H
