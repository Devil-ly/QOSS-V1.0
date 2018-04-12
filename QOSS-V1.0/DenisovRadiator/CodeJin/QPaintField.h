/*
*	created by liyun 2017/10/17
*   function 重载QLabel 绘制波的2d显示
*   version 1.0
*   调用时先设置setNum() 再设置数据setData()
*/
/*
	应用李赟提供的画图板进行场绘图- 只使用，不修改最好！
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

	// 设置n*m的矩阵
	void setNum(int, int);

	// 设置显示数据 输入一个n*m的二维矩阵
	void setData(vector <vector <double>> &);

	// 输入两个数据 求其平方和 （这个可能不会用，可以忽略）
	void calcData(vector <vector <complex <double>>> &,
		vector <vector <complex <double>>> &);

private:

	int NumOfLength;
	int NumOfWidth;

	vector <vector <double>> data;
};

#endif //QPAINTFIELD_H
