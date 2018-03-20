#include "../include/CalculatePhsCorThread.h"
#include "../MyData.h"
#include "../Calculation/PVVA.h"
//#include ""

using namespace userInterface;
using namespace calculation;

void CalculatePhsCorThread::setDs_Length(double ds, double length)
{
	this->ds = ds;
	this->length = length;
	isNeedMesh = true;
}
void CalculatePhsCorThread::run()
{
	MyData * myData = MyData::getInstance();
	PVVA pvva;
	// 设置单位
	pvva.setUnit(1);
	// 设置频率
	double fre = 1e10;
	pvva.setFre(fre);
	// 读入源并分配内存
	pvva.setSource(myData->getSourceField());
	//int N = 2;
	for (int i = 1; i <= myData->getNumOfMirrors() - 2; ++i)
	{
		pvva.setMirror(myData->getMirrorByNum(i));
		pvva.CalZ0Theta();
		pvva.Reflect();
		pvva.InterVal();
	}
	vector<vector<complex<double>>> Ex;
	vector<vector<complex<double>>> Ey;
	pvva.getExEyVirtualSurface(Ex, Ey);
	Vector3 reflectposition, reflectnormal;

	Mirror * mirror = myData->getMirrorByNum(myData->getNumOfMirrors());
	pvva.getVirtualSurfaceInterPoint(mirror, reflectposition, reflectnormal);

	mirror->getGraphTrans();
	Vector3 infieldposition, Eu_infield, Ev_infield, En_infield;
	Vector3 mirrorposition, Eu_mirror, Ev_mirror, En_mirror;

}

void CalculatePhsCorThread::killPhsCor()
{
	// free FDTD
	deleteLater();
}
