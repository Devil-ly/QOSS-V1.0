/*
*	created by liyun 2017/10/17
*   function 重载QLabel 绘制波的2d显示
*   version 1.0
*/

#ifndef QPaint2DField1_H
#define QPaint2DField1_H

#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <vector>
#include <complex>

using namespace std;

class QPaint2DField : public QLabel
{

	Q_OBJECT

public:

	explicit QPaint2DField(QWidget *parent = 0);

	void paintEvent(QPaintEvent *event);

	void setData(vector <vector <double>> &);

	void setNum(int n, int m);

	void calcData(vector <vector <complex <double>>> &, vector <vector <complex <double>>> &);

private:

	int NumOfLength;
	int NumOfWidth;

	vector <vector <double>> data;
};

#endif //QPaint2DField_H
