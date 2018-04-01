#ifndef CalculatePVVAThread_H
#define CalculatePVVAThread_H
#include "../MyData.h"
#include "../VTK/include/Field.h"
#include <QThread>

namespace  userInterface {
	class CalculatePVVAThread : public QThread
	{
		Q_OBJECT

	public:
		CalculatePVVAThread(double fre, double dis, int numMirror)
		{
			this->fre = fre;
			this->dis = dis;
			this->numMirror = numMirror;
		}
		//~CalculatePVVAThread();
		static void setSlaverValue(float, void*);

	signals:
		void sendMainValue(int);
	signals:
		void sendSlaverValue(int);
	signals:
		void sendField(Field*);

		public slots:

		void killFDTD();

	protected:
		void run();

	private:
		Field* field;
		double fre;
		double dis;
		int numMirror;

	};

}
#endif // CalculatePVVAThread_H