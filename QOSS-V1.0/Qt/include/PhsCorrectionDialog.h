#ifndef PhsCorrectionDialog_H
#define PhsCorrectionDialog_H

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

namespace  userInterface {
	class PhsCorrectionDialog : public QDialog
	{
		Q_OBJECT

	public:
		PhsCorrectionDialog(QWidget *parent = 0);
		~PhsCorrectionDialog();

	public slots:

		void on_OK();
		void on_cancel();

	private:

		QLabel * lengthLabel;
		QLineEdit * lengthLineEdit;
		QLabel * dsLabel;
		QComboBox *dsComboBox;
		QPushButton *cancelBtn;
		QPushButton *OKBtn;
	
	};
}


#endif // PhsCorrectionDialog_H
