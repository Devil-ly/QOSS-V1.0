#include "../include/CalculateFDTDThread.h"
#include "../../FDTDRadiator/FDTDRadiator.h"

using namespace userInterface;
void CalculateFDTDThread::run()
{
	FDTDradiator->SetReturnInt(CalculateFDTDThread::setMainValue, this);	//设置状态查询-整数的回调函数
	FDTDradiator->SetReturnFloat(CalculateFDTDThread::setSlaverValue, this);	//设置状态进度-浮点数的回调函数

	FDTDradiator->SetUpLowOrderVlasovRadiator(0, 1, 44e9, 16e-3, 20e-3, 80, 4);

	//SourceCenter and Aperture Parameters;
	//设置出射口面 -默认(单位m)

	// 暂时写死 以后有第一个反射镜来的到以下参数
	Position3D AperturePosition;	AperturePosition.setX(0.02);			AperturePosition.setY(0);	AperturePosition.setZ(0.2824);
	Vector3D UDirection;			UDirection.setX(cos(15.06 * 3.1415926 / 180));	UDirection.setY(0);	UDirection.setZ(-sin(15.06 * 3.1415926 / 180));	UDirection.normalize();
	Vector3D VDirection;			VDirection.setX(0);						VDirection.setY(1.0);		VDirection.setZ(0.0);					VDirection.normalize();
	Vector3D ApertureDirection;		ApertureDirection = ApertureDirection.crossProduct(UDirection, VDirection);
	int Nu;	int Nv;	Nu = 201; Nv = 201;
	double Lu;	Lu = 0.3;	//100 lambda
	double Lv;	Lv = 0.3;	//100 lambda

											//设置出射口面的位置	口径中心		 口面指向矢量       U方向矢量  V方向矢量 采样点个数 长度
	FDTDradiator->SetUpAperturePlane(AperturePosition, ApertureDirection, UDirection, VDirection, Nu, Nv, Lu, Lv);
	//设置
	//FDTDradiator->SetProFieldFileName("./test1.dat");	//设置输出场分布的名字

	//输出场： 注意：内存在调用处申请，不在DLL里申请！
	vector<vector<complex<double>>> Eu;
	vector<vector<complex<double>>> Ev;
	Eu.resize(Nu);	Ev.resize(Nu);
	for (int u = 0; u < Nu; u++) {
		Eu[u].resize(Nv);
		Ev[u].resize(Nv);
	}
	//FDTDradiator->run();
	//Get Surface Field
	//FDTDradiator->GetProFieldE(Eu, Ev, Nu, Nv);	//把函数接口改成了double 与主调对应

	//如果前期已计算完，可直接读取文件	替代->run 和->getProFieldE;	--金铭 20180325
	FDTDradiator->LoadProFieldE("./PropagatedEField.dat",Eu,Ev,Nu,Nv);

	GraphTrans graphTrans;
	graphTrans.setGraphTransPar(AperturePosition.X(), AperturePosition.Y(), AperturePosition.Z(),
		0, 1, 0, 15.06);
	double ds = Lu / (Nu - 1); 
	field->setNM(Nu, Nu);
	field->setPlane(graphTrans, ds); 
	field->setField(Eu, Ev);
	field->setShowPara(1, 1, 0);
	//field->updateData();

}

void CalculateFDTDThread::setMainValue(int val, void *user)
{
	((CalculateFDTDThread*)user)->sendMainValue(val);
}

void CalculateFDTDThread::setSlaverValue(float val, void *user)
{
	((CalculateFDTDThread*)user)->sendSlaverValue(val);
}

void CalculateFDTDThread::killFDTD()
{
	// free FDTD
	deleteLater();
}
