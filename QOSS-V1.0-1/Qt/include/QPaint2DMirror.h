/*
*	created by liyun 2017/11/11
*   function 绘制镜子和光路的2d图像
*   version 1.0
*/

#ifndef QPAINT2DMIRROR_H
#define QPAINT2DMIRROR_H

#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <vector>
#include <complex>
#include <memory>

#include <vector>
#include "../util/Vector3.h"
#include "../MirrorPosition.h"

using namespace std;

class QPaint2DMirror : public QLabel
{

	Q_OBJECT

public:

	explicit QPaint2DMirror(std::shared_ptr<calculation::MirrorPosition> _mirrorPosition,
		QWidget *parent = 0);

	void paintEvent(QPaintEvent *event);

	void setMirrorNum(int num);

	void setBoundary(double _length, double _width);
	void setLength(double _length);
	void setWidth(double _width);
private:

	std::shared_ptr<calculation::MirrorPosition> mirrorPosition;
	double scale;

	double length;
	double width;

	int mirrorNum;
};

#endif //QPAINT2DMIRROR_H
