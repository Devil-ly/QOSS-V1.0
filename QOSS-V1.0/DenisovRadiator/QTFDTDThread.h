#ifndef QTFDTDThread_H
#define QTFDTDThread_H
#include "../../FDTDRadiator/FDTDRadiator.h"
#include <QThread>

namespace  userInterface {
	class QTFDTDThread : public QThread
	{
		Q_OBJECT

	public:
		QTFDTDThread()
		{
			FDTDradiator = new FDTDRadiator;
		}
		//Add
		void SetFDTDThread(double _frequency, double _radius, int _Ns, int _m, int _n, double _lcut, double _prodis, double _aperlen, int _Na) {
			this->frequency = _frequency;
			this->radius = _radius;
			this->Ns = _Ns;
			this->m = _m;
			this->n = _n;
			this->lcut = _lcut;
			this->prodis = _prodis;
			this->aperlen = _aperlen;
			this->Na = _Na;

		}

		~QTFDTDThread() {
			FDTDradiator->~FDTDRadiator();
		}
		void GetProPowerRatio(double& _PowerRatio);
		static void setMainValue(int, void*);
		static void setSlaverValue(float, void*);

	signals:
		void sendMainValue(int);
	signals:
		void sendSlaverValue(float);

		public slots:

		void killFDTD();

	protected:
		void run();

	public:
		FDTDRadiator* FDTDradiator;
	private:

		double frequency;
		double radius;		int Ns;
		//mode
		int m;
		int n;
		//Cut Height
		double lcut;
		double prodis;
		double aperlen;		int Na;
		double PowerRatio;

	};

}
#endif // QTFDTDThread_H