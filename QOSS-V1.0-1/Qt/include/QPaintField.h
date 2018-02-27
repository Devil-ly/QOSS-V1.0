/*
*	created by liyun 2017/10/17
*   function 重载QLabel 绘制波的2d显示
*   version 1.0
*/

#ifndef QPAINTFIELD_H
#define QPAINTFIELD_H

#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <vector>
#include <complex>

using namespace std;

class QPaintField : public QLabel
{

	Q_OBJECT

public:

	explicit QPaintField(QWidget *parent = 0);

	void paintEvent(QPaintEvent *event);

	void setData(vector <vector <double>> &);

	void calcData(vector <vector <complex <double>>> &, vector <vector <complex <double>>> &);

private:

	int NumOfLength;
	int NumOfWidth;

	vector <vector <double>> data;
};

#endif //QPAINTFIELD_H
