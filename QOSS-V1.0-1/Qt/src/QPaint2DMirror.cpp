#include "..\include\QPaint2DMirror.h"
#include "../util/Definition.h"

QPaint2DMirror::QPaint2DMirror(std::shared_ptr<calculation::MirrorPosition> _mirrorPosition,
	QWidget *parent):mirrorPosition(_mirrorPosition),QLabel(parent)
{
	length = 1.0;
	width = 1.0;
	scale = 1.0;
	mirrorNum = 0;
}

void QPaint2DMirror::paintEvent(QPaintEvent * event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);
	QPen pen;       //ÉèÖÃ»­±Ê£¬ÑÕÉ«¡¢¿í¶È	

	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);

	painter.drawRect(0, 0, MIRROR_POSITION_2DSHOW_X, MIRROR_POSITION_2DSHOW_Y);

	painter.drawText(MIRROR_POSITION_2DSHOW_X / 2, 10, QStringLiteral("L"));
	painter.drawText(MIRROR_POSITION_2DSHOW_X-10, MIRROR_POSITION_2DSHOW_Y/2, QStringLiteral("W"));
	
	pen.setBrush(Qt::blue);
	pen.setWidth(2);
	painter.setPen(pen);

	painter.drawRect((MIRROR_POSITION_2DSHOW_X - length*scale) / 2,
		(MIRROR_POSITION_2DSHOW_Y - width*scale),length*scale, width*scale);

	Vector3 temp = mirrorPosition->getStartingPoint();
	Vector3 next;
	Vector3 mirrorLeft, mirrorRight;

	for (int i = 0; i < mirrorNum; ++i) {
		pen.setBrush(Qt::red);
		pen.setWidth(2);
		painter.setPen(pen);

		next = temp + mirrorPosition->getLightDirection(i) * 
			mirrorPosition->getLightLength(i) * scale;
		painter.drawLine(temp.x + MIRROR_POSITION_2DSHOW_X / 2, 
			MIRROR_POSITION_2DSHOW_Y - temp.z,
			next.x + MIRROR_POSITION_2DSHOW_X / 2, 
			MIRROR_POSITION_2DSHOW_Y - next.z);
		temp = next;
		mirrorLeft = mirrorPosition->getLightDirection(i) +
			mirrorPosition->getLightDirection(i+1);
		mirrorLeft.Normalization();
		mirrorRight = next + mirrorLeft * 20;
		mirrorLeft = next - mirrorLeft * 20;
		
		pen.setBrush(Qt::black);
		pen.setWidth(2);
		painter.setPen(pen);

		painter.drawLine(mirrorLeft.x + MIRROR_POSITION_2DSHOW_X / 2, 
			MIRROR_POSITION_2DSHOW_Y - mirrorLeft.z,
			mirrorRight.x + MIRROR_POSITION_2DSHOW_X / 2, 
			MIRROR_POSITION_2DSHOW_Y - mirrorRight.z);
	}
	pen.setBrush(Qt::red);
	pen.setWidth(2);
	painter.setPen(pen);

	next = temp + mirrorPosition->getLightDirection(mirrorNum) * 
		mirrorPosition->getLightLength(mirrorNum - 1) * scale;
	painter.drawLine(temp.x + MIRROR_POSITION_2DSHOW_X / 2,
		MIRROR_POSITION_2DSHOW_Y - temp.z,
		next.x + MIRROR_POSITION_2DSHOW_X / 2, 
		MIRROR_POSITION_2DSHOW_Y - next.z);

}

void QPaint2DMirror::setMirrorNum(int num)
{
	mirrorNum = num;
}


void QPaint2DMirror::setBoundary(double _length, double _width)
{
	width = _width;
	length = _length;
	scale = double(MIRROR_POSITION_2DSHOW_X) / _length / 1.1;
	if (MIRROR_POSITION_2DSHOW_Y / width / 1.1 < scale)
		scale = MIRROR_POSITION_2DSHOW_Y / width / 1.1;
}

void QPaint2DMirror::setLength(double _length)
{
	length = _length;
	scale = double(MIRROR_POSITION_2DSHOW_X) / _length / 1.1;
	if (MIRROR_POSITION_2DSHOW_Y / width / 1.1 < scale)
		scale = MIRROR_POSITION_2DSHOW_Y / width / 1.1;
}

void QPaint2DMirror::setWidth(double _width)
{
	width = _width;
	scale = double(MIRROR_POSITION_2DSHOW_X) / length / 1.1;
	if (MIRROR_POSITION_2DSHOW_Y / width / 1.1 < scale)
		scale = MIRROR_POSITION_2DSHOW_Y / width / 1.1;
}

