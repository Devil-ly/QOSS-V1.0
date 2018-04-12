/*
*	created by liyun 2017/10/17
*   function 设置打开或新建
*   version 1.0
*/

#ifndef ModelBtnDialog_H
#define ModelBtnDialog_H

#include <QtWidgets/QDialog>

namespace  userInterface {

	class ModelBtnDialog : public QDialog
	{
		Q_OBJECT
	public:
		ModelBtnDialog(QWidget *parent = 0);
		~ModelBtnDialog();

	private slots:

		void on_TE01();
		void on_TE226();
		void on_power();

	};

}
#endif //PREWINDOW_H