#include "../include/CalculatePVVAThread.h"
#include <../Calculation/FDTDRadiator.h>
#include "../Calculation/PVVA.h"

using namespace userInterface;
using namespace calculation;
void CalculatePVVAThread::run()
{
	emit sendMainValue(0);
	MyData *myData = MyData::getInstance();
	PVVA pvva;
	pvva.SetReturnFloat(CalculatePVVAThread::setSlaverValue, this);
	// 设置单位
	pvva.setUnit(1);
	// 设置频率
	pvva.setFre(fre);
	// 读入源并分配内存
	pvva.setSource(myData->getSourceField());
	//int N = 2;
	for (int i = 1; i <= numMirror; ++i)
	{
		emit sendMainValue(i);
		pvva.setMirror(myData->getMirrorByNum(i));
		pvva.CalZ0Theta();
		pvva.Reflect();
		pvva.InterVal();		
	}
	emit sendMainValue(numMirror + 1);
	pvva.Result(dis);
	Field *tempField = new Field;
	pvva.getField(tempField);
	emit sendField(tempField);
	emit sendMainValue(numMirror + 2);
}


void CalculatePVVAThread::setSlaverValue(float val, void *user)
{
	((CalculatePVVAThread*)user)->sendSlaverValue(val);
}

void CalculatePVVAThread::killFDTD()
{
	deleteLater();
}
