#ifndef FDTDProgressDialog_H
#define FDTDProgressDialog_H

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>


namespace  userInterface {
	class FDTDProgressDialog : public QDialog
	{
		Q_OBJECT

	public:
		FDTDProgressDialog(QWidget *parent = 0);
		~FDTDProgressDialog();

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
		QProgressBar *mainBar;
		QProgressBar *slaverBar;
		QLabel * txtLabel;

		QPushButton *closeBtn;
		QPushButton *stopBtn;
	
	};
}


#endif // FDTDProgressDialog_H
