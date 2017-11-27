/*
*	created by liyun 2017/10/17
*   function 设置打开或新建
*   version 1.0
*/

#ifndef PREWINDOW_H
#define PREWINDOW_H

#include <QtWidgets/QDialog>

namespace  userInterface {

	class PreDialog : public QDialog
	{
		Q_OBJECT
	public:
		PreDialog(QWidget *parent = 0);
		~PreDialog();

		private slots:
		void createModel();

	};

}
#endif //PREWINDOW_H