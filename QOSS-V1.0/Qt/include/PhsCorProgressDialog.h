#ifndef PhsCorProgressDialog_H
#define PhsCorProgressDialog_H

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>


namespace  userInterface {
	class PhsCorProgressDialog : public QDialog
	{
		Q_OBJECT

	public:
		PhsCorProgressDialog(QWidget *parent = 0);
		~PhsCorProgressDialog();

	public slots:
		void setMainValue(int);
		void setSlaverValue(int);

		void on_close();
		void on_stop();

		void ReceiveError(int);

	signals:
		void sendStop();

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		QProgressBar *mainBar;
		QProgressBar *slaverBar;
		QLabel * txtLabel;

		QPushButton *closeBtn;
		QPushButton *stopBtn;
	
	};
}


#endif // PhsCorProgressDialog_H
