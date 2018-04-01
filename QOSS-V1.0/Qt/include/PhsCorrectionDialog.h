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

		void getData(int & dsIndex, double & length);
		void getTarget(double&);

		void setIsNeedMesh(bool isNeedMesh);
	public slots:

		void on_OK();
		void on_cancel();

	private:

		QLabel * lengthLabel;
		QLineEdit * lengthLineEdit;
		QLabel * dsLabel;
		QComboBox *dsComboBox;
		QLabel * gaussLabel;
		QLineEdit * gaussLineEdit;
		QPushButton *cancelBtn;
		QPushButton *OKBtn;
	
	};
}


#endif // PhsCorrectionDialog_H
