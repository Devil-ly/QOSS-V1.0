#ifndef CalculatePhsCorThread_H
#define CalculatePhsCorThread_H

#include "../VTK/include/Field.h"
#include <QThread>

namespace  userInterface {
	class CalculatePhsCorThread : public QThread
	{
		Q_OBJECT

	public:
		CalculatePhsCorThread() { isNeedMesh = false; };
		//~CalculatePhsCorThread();

		void setDs_Length(double ds, double length);

	signals:
		void sendMainValue(int);
	signals:
		void sendSlaverValue(int);

		public slots:

		void killPhsCor();

	protected:
		void run();

	private:
		Field* field;
		bool isNeedMesh;

		double ds;
		double length;
	};

}
#endif // CalculatePhsCorThread_H