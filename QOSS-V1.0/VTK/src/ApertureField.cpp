#include "..\include\ApertureField.h"
#include "../util/Constant_Var.h"
#include <sstream> 

ApertureField::ApertureField(const GraphTrans & _graphTrans, 
	const std::vector<double> parameter)
{
	isSource = true;

	graphTrans = _graphTrans;
	setData(parameter);
	if (data[6] < 0.5)
		isAmPhs = true;
	else
		isAmPhs = false;
	//updateData();
}

ApertureField::ApertureField(const ApertureField & _ApertureField)
{
}

ApertureField::~ApertureField()
{
}

void ApertureField::setFileAddress(const string & file)
{
	fileAddress = file;
}

void ApertureField::setUnit(double _unit)
{
}

void ApertureField::updateData()
{
	if (is3D)
		calActor3D();
	else
		calActor();
}

void ApertureField::readData()
{
	allocateMemory();
	ifstream file(fileAddress);
	int beginY = int(data[4]) - 1;
	int beginX = int(data[5]) - 1;
	string temp;
	while (beginY--)
		getline(file, temp);
	for (int i = 0; i < N_width; i++)
	{
		for (int j = 0; j < M_depth; j++)
		{
			getline(file, temp);
			istringstream tempLine(temp);
			int tempBegin = beginX;
			double temp1;
			while (tempBegin--)
				tempLine >> temp1;
			double a1, p1, a2, p2;
			tempLine >> a1 >> p1 >> a2 >> p2;
			if (isAmPhs)
			{
				p1 += data[3];
				a1 *= data[2];
				p2 += data[3];
				a2 *= data[2];
				Ex[i][j] = complex<double>(a1*cos(p1 / 180 * Pi),
					a1*sin(p1 / 180 * Pi));
				Ey[i][j] = complex<double>(a2*cos(p2 / 180 * Pi),
					a2*sin(p2 / 180 * Pi));
				//Ex[i][j] = complex<double>(0, 0);
			}
			else
			{
				Ex[i][j] = data[2] * complex<double>(
					a1 * cos(data[3] / 180 * Pi), p1 * sin(data[3] / 180 * Pi));
				Ey[i][j] = data[2] * complex<double>(
					a2 * cos(data[3] / 180 * Pi), p2 * sin(data[3] / 180 * Pi));
			}

		}
	}
	file.close();

}

QTreeWidgetItem * ApertureField::getTree()
{
	return nullptr;
}
