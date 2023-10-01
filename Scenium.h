#ifndef SCENIUM_H
#define SCENIUM_H

#include <QEvent>
#include <QKeyEvent>
#include <QObject>

class UpDownLeftRightIum : public QObject
{
	Q_OBJECT
public:
	explicit UpDownLeftRightIum (QObject *parent = nullptr);

	bool eventFilter (QObject *obj, QEvent *event) override;

signals:
	void keyPressed (QKeyEvent *event);
	void keyReleased (QKeyEvent *event);
};

#endif // SCENIUM_H
