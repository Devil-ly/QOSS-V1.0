#include "../include/CalculatePhsCorThread.h"
#include "../MyData.h"
#include "../Calculation/PVVA.h"
#include "../Calculation/PhaseCorrection.h"
#include "../VTK/include/PhsCorMirror.h"

//#define Test

using namespace userInterface;
using namespace calculation;

void CalculatePhsCorThread::setDs_Length(int dsIndex, double length)
{
	this->dsIndex = dsIndex;
	this->length = length;
	isNeedMesh = true;
	switch (dsIndex)
	{
	case 0:
		ds = length / 100;
		break;
	case 1:
		ds = length / 200;
		break;
	case 2:
		ds = length / 300;
		break;
	default:
		break;
	}
}

void CalculatePhsCorThread::run()
{
	MyData * myData = MyData::getInstance();

	Vector3 reflectposition, reflectnormal;

	Mirror * mirror = myData->getMirrorByNum(myData->getNumOfMirrors() - 1);
	GraphTrans graphTrans = myData->getPhsCorField()->getGraphTrans();
	Vector3 Org_Source(graphTrans.getTrans_x(), graphTrans.getTrans_y(), graphTrans.getTrans_z());
	Vector3 n_Source(0,0,1);

	Vector3D RotateAsixSou(graphTrans.getRotate_x(),
		graphTrans.getRotate_y(),
		graphTrans.getRotate_z());
	Matrix4D rotatMatrixSou = Matrix4D::getRotateMatrix(
		graphTrans.getRotate_theta(), RotateAsixSou);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(
		graphTrans.getTrans_x(),
		graphTrans.getTrans_y(),
		graphTrans.getTrans_z());
	n_Source = rotatMatrixSou * n_Source;

	RayTracing rayTracing(mirror);
	bool isInter;
	double dir_t;
	rayTracing.calcNormalOfLine_Mirror(Org_Source,
		n_Source, reflectnormal, reflectposition, isInter, dir_t);
	reflectnormal.Normalization();

	if (!isInter)
	{
		// 如果入射场没有和反射面不相交 报错
		error(0);
		return;
	}
	sendMainValue(1);
	PhsCorMirror  *phsCorMirror;

	//phsCorMirror = new PhsCorMirror;
	//phsCorMirror->getLattice();
	//vector<vector<Vector3>> test(2, vector<Vector3>(2, Vector3()));
	//phsCorMirror->setLattice(test);
	//cout << phsCorMirror << endl;
	if (isNeedMesh)
	{
		phsCorMirror = new PhsCorMirror;

		if (!phsCorMirror->sampling(ds, length, reflectposition, n_Source, graphTrans, mirror))
			//全局坐标采样
		{
			// 截取的面过大
			error(1);
			return;
		}
		//phsCorMirror->updateData();
	}
	else
		phsCorMirror = dynamic_cast<PhsCorMirror*>(mirror);
	sendMainValue(2);
#ifndef Test
	// 场的位置信息
	Vector3 infieldposition(0, 0, 0), Eu_infield(1, 0, 0), Ev_infield(0, 1, 0), En_infield(0, 0, 1);
	infieldposition = translateMatrix * rotatMatrixSou * infieldposition;
	Eu_infield = rotatMatrixSou * Eu_infield;
	Ev_infield = rotatMatrixSou * Ev_infield;
	En_infield = rotatMatrixSou * En_infield;

	// 镜面的位置信息
	GraphTrans graphTrans1 = mirror->getGraphTrans();
	Vector3D RotateAsixSou1(graphTrans1.getRotate_x(),
		graphTrans1.getRotate_y(),
		graphTrans1.getRotate_z());
	Matrix4D rotatMatrixSou1 = Matrix4D::getRotateMatrix(
		graphTrans1.getRotate_theta(), RotateAsixSou1);
	Matrix4D translateMatrix1 = Matrix4D::getTranslateMatrix(
		graphTrans1.getTrans_x(),
		graphTrans1.getTrans_y(),
		graphTrans1.getTrans_z());
	Vector3 mirrorposition(0, 0, 0), Eu_mirror(1, 0, 0), Ev_mirror(0, 1, 0), En_mirror(0, 0, 1);
	mirrorposition = translateMatrix1 * rotatMatrixSou1 * mirrorposition;
	Eu_mirror = rotatMatrixSou1 * Eu_mirror;
	Ev_mirror = rotatMatrixSou1 * Ev_mirror;
	En_mirror = rotatMatrixSou1 * En_mirror;

	int N_mirror0 = phsCorMirror->getLattice().size();
	//int N_mirror0 = 201;
	vector<vector<Vector3>> mirror1(N_mirror0, vector<Vector3>(N_mirror0, 0));

	PhaseCorrection M;
	//double Target_W = 0.016;
	//double inFieldDs = myData->getPhsCorField()->getDs();
	M.Set(myData->getFrequency(), target_W, N_mirror0,
		myData->getPhsCorField()->getEx().size(),
		phsCorMirror->getLattice(),
		myData->getPhsCorField()->getEx(),
		myData->getPhsCorField()->getEy(), 
		myData->getPhsCorField()->getDs());

	M.SetCoordinate(infieldposition, Eu_infield, Ev_infield, En_infield,
		mirrorposition, Eu_mirror, Ev_mirror, En_mirror,
		reflectposition, reflectnormal);

	mirror1 = M.Single_Correction();
	
	phsCorMirror->setLattice(mirror1); // 局部坐标
	
#endif // Test

	sendMainValue(3);
	phsCorMirror->updateData();
	
	emit sendMirror(phsCorMirror);

}

void CalculatePhsCorThread::killPhsCor()
{
	// free FDTD
	deleteLater();
}

void CalculatePhsCorThread::setTarget_W(double temp)
{
	target_W = temp;
}
