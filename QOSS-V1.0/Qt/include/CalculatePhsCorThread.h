#ifndef CalculatePhsCorThread_H
#define CalculatePhsCorThread_H

#include "../VTK/include/Field.h"
#include <QThread>

class Mirror;
namespace  userInterface {
	class CalculatePhsCorThread : public QThread
	{
		Q_OBJECT

	public:
		CalculatePhsCorThread() { isNeedMesh = false; };
		//~CalculatePhsCorThread();

		void setDs_Length(int dsIndex, double length);

	signals:
		void sendMainValue(int);
	signals:
		void sendSlaverValue(int);
	signals:
		void sendMirror(Mirror*);
	signals:
		void error(int);

	public slots:

		void killPhsCor();

	protected:
		void run();

	private:
		Field* field;
		bool isNeedMesh;

		double ds;
		double length;
		int dsIndex; // 用户选择的剖分精度 0粗 1标准 2细
	};

}
#endif // CalculatePhsCorThread_H