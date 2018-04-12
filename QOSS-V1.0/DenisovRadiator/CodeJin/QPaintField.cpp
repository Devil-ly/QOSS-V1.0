#include "../CodeJin/QPaintField.h"

QPaintField::QPaintField(QWidget *parent) :
	 QLabel(parent)
{
	NumOfLength = 0;
	NumOfWidth = 0;
}

void QPaintField::paintEvent(QPaintEvent * event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);
	QPen pen;       //设置画笔，颜色、宽度	
	pen.setWidth(1);	
	float max = 0;
	float min = 0;
	for (int i = 0; i < NumOfLength; i++)
		for (int j = 0; j < NumOfWidth; j++)
		{
			float temp = abs(data[i][j]);
			if (temp > max)
				max = temp;
			else if (temp < max)
				min = temp;
		}

	float range = max - min;
	for (int i = 0; i < NumOfLength; i++)
		for (int j = 0; j < NumOfWidth; j++)
		{
			float val = abs(data[i][j]) / range;
			if (val <= 0.14)
			{
				pen.setColor(QColor(139, 0, 255));
			}
			else if (val > 0.14 && val <= 0.28)
			{
				pen.setColor(QColor(0, 0, 255));
			}
			else if (val > 0.28 && val <= 0.42)
			{
				pen.setColor(QColor(0, 127, 255));
			}
			else if (val > 0.42 && val <= 0.56)
			{
				pen.setColor(QColor(0, 255, 0));
			}
			else if (val > 0.56 && val <= 0.70)
			{
				pen.setColor(QColor(255, 255, 0));
			}
			else if (val > 0.70 && val <= 0.84)
			{
				pen.setColor(QColor(255, 165, 0));
			}
			else if(val > 0.84)
			{
				pen.setColor(QColor(255, 0, 0));
			}

			painter.setPen(pen);
			painter.drawPoint(i, j);
		}
}

void QPaintField::setNum(int n, int m)
{
	NumOfLength = n;
	NumOfWidth = m;
}

void QPaintField::setData(vector <vector <double>> & _data)
{
	data = _data;
}

void QPaintField::calcData(vector<vector<complex<double>>>& Ex,
	vector<vector<complex<double>>>& Ey)
{
	data.resize(NumOfLength);
	for (int i = 0; i < NumOfLength; i++)
	{
		data[i].resize(NumOfWidth);
	}//生成矩阵尺寸：N*N
	 //下面开始赋值
	for (int i = 0; i < NumOfLength; i++)
	{
		for (int j = 0; j < NumOfWidth; j++)
		{
			data[i][j] = pow((Ex[i][j] * Ex[i][j] + Ey[i][j] * Ey[i][j]), 0.5).real();
		}
	}
}
