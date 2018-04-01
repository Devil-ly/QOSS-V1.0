#ifndef PVVAProgressDialog_H
#define PVVAProgressDialog_H

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>


namespace  userInterface {
	class PVVAProgressDialog : public QDialog
	{
		Q_OBJECT

	public:
		PVVAProgressDialog(QWidget *parent = 0);
		~PVVAProgressDialog();

		void setMirrorNum(int num);

	public slots:
		void setMainValue(int);
		void setSlaverValue(int);

		void on_close();
		void on_stop();

	signals:
		void sendStop();

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		int MirrorNum;
		QProgressBar *mainBar;
		QProgressBar *slaverBar;
		QLabel * txtLabel;

		QPushButton *closeBtn;
		QPushButton *stopBtn;
	
	};
}


#endif // PVVAProgressDialog_H
