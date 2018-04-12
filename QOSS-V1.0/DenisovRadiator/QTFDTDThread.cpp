#include "QTFDTDThread.h"
//#include "FDTDRadiator.h"
#include <cmath>

using namespace userInterface;
void QTFDTDThread::run()
{
	FDTDradiator->SetReturnInt(QTFDTDThread::setMainValue, this);	//设置状态查询-整数的回调函数
	FDTDradiator->SetReturnFloat(QTFDTDThread::setSlaverValue, this);	//设置状态进度-浮点数的回调函数

	if (this->m == 0) {
		FDTDradiator->SetUpLowOrderVlasovRadiator(this->m, this->n, this->frequency, this->radius, 20e-3, this->Ns, 14);
	}


	//SourceCenter and Aperture Parameters;
	//设置出射口面 -默认(单位m)

	// 暂时写死 以后有第一个反射镜来的到以下参数
	//由Lc得到phi
	//Lc = 2 * Radius*tan(Pi * 0.5 - phi1);	//2*a*cotphi
	double phi; phi = atan(this->radius*2.0 / this->lcut);
	Position3D AperturePosition;
	AperturePosition.setX(-this->prodis*sin(phi));
	AperturePosition.setY(0);
	AperturePosition.setZ(this->prodis*cos(phi) + this->lcut*0.5);
	Vector3D UDirection;
	UDirection.setX(cos(phi));
	UDirection.setY(0);
	UDirection.setZ(sin(phi));
	UDirection.normalize();
	Vector3D VDirection;
	VDirection.setX(0);
	VDirection.setY(1.0);
	VDirection.setZ(0.0);
	VDirection.normalize();

	Vector3D ApertureDirection;
	ApertureDirection = ApertureDirection.crossProduct(UDirection, VDirection);

	int Nu;	int Nv;	Nu = this->Na; Nv = this->Na;
	double Lu;	Lu = this->aperlen;
	double Lv;	Lv = this->aperlen;

	//设置出射口面的位置	口径中心		 口面指向矢量       U方向矢量  V方向矢量 采样点个数 长度
	FDTDradiator->SetUpAperturePlane(AperturePosition, ApertureDirection, UDirection, VDirection, Nu, Nv, Lu, Lv);
	//设置

	//输出场： 注意：内存在调用处申请，不在DLL里申请！
	vector<vector<complex<double>>> Eu;
	vector<vector<complex<double>>> Ev;
	Eu.resize(Nu);	Ev.resize(Nu);
	for (int u = 0; u < Nu; u++) {
		Eu[u].resize(Nv);
		Ev[u].resize(Nv);
	}
	FDTDradiator->run();
	//Get Surface Field
	FDTDradiator->GetProFieldE(Eu, Ev, Nu, Nv);	//把函数接口改成了double 与主调对应
												//FDTDradiator->GetProPowerRatio(QTFDTDThread::PowerRatio);

												//如果前期已计算完，可直接读取文件	替代->run 和->getProFieldE;	--金铭 20180325
												//FDTDradiator->LoadProFieldE("./PropagatedEField.dat",Eu,Ev,Nu,Nv);

												//VTK画图用
												/*
												GraphTrans graphTrans;
												graphTrans.setGraphTransPar(AperturePosition.X(), AperturePosition.Y(), AperturePosition.Z(),
												0, 1, 0, 15.06);
												double ds = Lu / (Nu - 1);
												field->setNM(Nu, Nu);
												field->setPlane(graphTrans, ds);
												field->setField(Eu, Ev);
												field->setShowPara(1, 1, 0);
												//field->updateData();
												*/
}

void QTFDTDThread::setMainValue(int val, void *user)
{
	((QTFDTDThread*)user)->sendMainValue(val);
}

void QTFDTDThread::setSlaverValue(float val, void *user)
{
	((QTFDTDThread*)user)->sendSlaverValue(val);
}

void QTFDTDThread::killFDTD()
{
	// free FDTD
	deleteLater();
}

void QTFDTDThread::GetProPowerRatio(double& _PowerRatio) {
	_PowerRatio = QTFDTDThread::PowerRatio;
}
