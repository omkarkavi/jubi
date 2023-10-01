#include "Scenium.h"

UpDownLeftRightIum::UpDownLeftRightIum (QObject *parent) : QObject{ parent } {}

bool UpDownLeftRightIum::eventFilter (QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *> (event);
		//		qDebug() << "Key Pressed : " << keyEvent;
		switch (keyEvent->key())
		{
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Right:
		case Qt::Key_Left:
		case Qt::Key_Q:
		case Qt::Key_E:
		case Qt::Key_Space:
			emit keyPressed (keyEvent);
		default:
			break;
		}
	}
	else if (event->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *> (event);
		//		qDebug() << "Key Released : " << keyEvent;
		switch (keyEvent->key())
		{
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Right:
		case Qt::Key_Left:
		case Qt::Key_Q:
		case Qt::Key_E:
		case Qt::Key_Space:
			emit keyReleased (keyEvent);
		default:
			break;
		}
	}

	return QObject::eventFilter (obj, event);
}
