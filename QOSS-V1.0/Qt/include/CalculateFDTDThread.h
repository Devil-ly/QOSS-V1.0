#ifndef CalculateFDTDThread_H
#define CalculateFDTDThread_H
#include "../Calculation/FDTDRadiator.h"
#include "../VTK/include/Field.h"
#include <QThread>

namespace  userInterface {
	class CalculateFDTDThread : public QThread
	{
		Q_OBJECT

	public:
		CalculateFDTDThread(FDTDRadiator* _FDTDradiator, Field* _field)
		{
			this->FDTDradiator = _FDTDradiator;
			this->field = _field;
		}
		//~CalculateFDTDThread();
		static void setMainValue(int, void*);
		static void setSlaverValue(float, void*);

	signals:
		void sendMainValue(int);
	signals:
		void sendSlaverValue(int);

		public slots:

		void killFDTD();

	protected:
		void run();

	private:
		Field* field;
		FDTDRadiator* FDTDradiator;

	};

}
#endif // CalculateFDTDThread_H