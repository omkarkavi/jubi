#include "widget.h"
#include <QtMath>
#include "ui_widget.h"

int xl = -400, xr = 400, yu = -400, yd = 400;

Widget::Widget (QWidget *parent) : QWidget (parent), ui (new Ui::Widget)
{
	ui->setupUi(this);
	aThingyToMove = new QGraphicsItemGroup;

	upArrow = new QGraphicsSimpleTextItem ("⤊");
	upArrow->setBrush (QBrush (Qt::gray));
	downArrow = new QGraphicsSimpleTextItem ("↓");
	downArrow->setBrush (QBrush (Qt::yellow));
	leftArrow = new QGraphicsSimpleTextItem ("←");
	leftArrow->setBrush (QBrush (Qt::yellow));
	rightArrow = new QGraphicsSimpleTextItem ("→");
	rightArrow->setBrush (QBrush (Qt::yellow));

	aThingyToMove->addToGroup (upArrow);
	aThingyToMove->addToGroup (downArrow);
	aThingyToMove->addToGroup (rightArrow);
	aThingyToMove->addToGroup (leftArrow);
	pointingLine = new QGraphicsLineItem (QLineF (0, 0, 0, -2000));
	pointingLine->setPen (QPen (Qt::darkGray));
	pointingLine->setTransform (QTransform().translate (0, -15));

	upArrow->setTransform (QTransform().translate (0 - upArrow->boundingRect().width() / 2, -10 - upArrow->boundingRect().height() / 2));
	downArrow->setTransform (QTransform().translate (0 - downArrow->boundingRect().width() / 2, 10 - downArrow->boundingRect().height() / 2));
	rightArrow->setTransform (QTransform().translate (10 - rightArrow->boundingRect().width() / 2, 0 - rightArrow->boundingRect().height() / 2));
	leftArrow->setTransform (QTransform().translate (-10 - leftArrow->boundingRect().width() / 2, 0 - leftArrow->boundingRect().height() / 2));
	qDebug() << aThingyToMove->transformOriginPoint();
	qDebug() << aThingyToMove->boundingRect().center();
	aThingyToMove->setTransformOriginPoint (aThingyToMove->boundingRect().center());
	aThingyToMove->addToGroup (pointingLine);
	aThingyToMove->setData (0, QStringLiteral ("Main Sprite"));

	scene = new QGraphicsScene;
	filter = new UpDownLeftRightIum;
	ui->graphicsView->setScene (scene);
	ui->graphicsView->installEventFilter (filter);
	scene->installEventFilter (filter);
	addEnemies();

	scene->addItem (aThingyToMove);
	//	boundingBoxRect = new QGraphicsRectItem;
	//	boundingBoxRect->setRect (pointingLine->sceneBoundingRect());
	//	boundingBoxRect->setPen (QPen (Qt::darkBlue));
	//	scene->addItem (boundingBoxRect);
	aThingyToMove->setPos (0, 0);
	QGraphicsLineItem *lI1 = scene->addLine (QLineF (0, -100, 0, 100), QPen (Qt::cyan));
	lI1->setData (0, QStringLiteral ("Y Axis"));
	QGraphicsLineItem *lI2 = scene->addLine (QLineF (-100, 0, 100, 0), QPen (Qt::cyan));
	lI2->setData (0, QStringLiteral ("X Axis"));
	QGraphicsRectItem *sceneRect = scene->addRect (QRectF (-400, -400, 800, 800), QPen (Qt::darkRed));
	sceneRect->setData (0, "Scene Bounds");
	ui->graphicsView->setSceneRect (sceneRect->rect());
	ui->graphicsView->centerOn (sceneRect->rect().center());
	connect (&spriteMovementTimer, &QTimer::timeout, this, &Widget::spriteMovement_M2);
	spriteMovementTimer.start (20);
	connect (filter, &UpDownLeftRightIum::keyPressed, this,
		[this] (QKeyEvent *event)
		{
		switch (event->key())
		{
		case Qt::Key_Up:
			currentMovement.up = true;
			break;
		case Qt::Key_Down:
			currentMovement.down = true;
			break;
		case Qt::Key_Right:
			currentMovement.right = true;
			break;
		case Qt::Key_Left:
			currentMovement.left = true;
			break;
		case Qt::Key_E:
			currentMovement.clock = true;
			break;
		case Qt::Key_Q:
			currentMovement.anticlock = true;
			break;
		case Qt::Key_Space:
			currentMovement.poofingOn = true;
			pointingLine->setPen (QPen (Qt::red));
			break;
		default:
			break;
		}
		//		qDebug() << "current movement set: " << currentMovement.up << currentMovement.down << currentMovement.right << currentMovement.left;
	});
	connect (filter, &UpDownLeftRightIum::keyReleased, this,
		[this] (QKeyEvent *event)
		{
		switch (event->key())
		{
		case Qt::Key_Up:
			currentMovement.up = false;
			break;
		case Qt::Key_Down:
			currentMovement.down = false;
			break;
		case Qt::Key_Right:
			currentMovement.right = false;
			break;
		case Qt::Key_Left:
			currentMovement.left = false;
			break;
		case Qt::Key_E:
			currentMovement.clock = false;
			break;
		case Qt::Key_Q:
			currentMovement.anticlock = false;
			break;
		case Qt::Key_Space:
			currentMovement.poofingOn = false;
			pointingLine->setPen (QPen (Qt::darkGray));
			break;
		default:
			break;
		}
	});
}

Widget::~Widget()
{
	delete ui;
}

void Widget::spriteMovement()
{
	bool didSth = false;
	QTransform initPos = aThingyToMove->sceneTransform();

	//	qDebug() << "current movement get : " << currentMovement.up << currentMovement.down << currentMovement.right << currentMovement.left;
	if (currentMovement.up)
	{
		//		qDebug() << "UP";
		aThingyToMove->setTransform (aThingyToMove->sceneTransform().translate (0, -1.5));
		upArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		upArrow->setBrush (Qt::gray);
	}
	if (currentMovement.down)
	{
		//		qDebug() << "DOWN";
		aThingyToMove->setTransform (aThingyToMove->sceneTransform().translate (0, 0.7));
		downArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		downArrow->setBrush (Qt::gray);
	}
	if (currentMovement.right)
	{
		//		qDebug() << "RIGHT";
		aThingyToMove->setTransform (aThingyToMove->sceneTransform().translate (0.8, 0));
		rightArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		rightArrow->setBrush (Qt::gray);
	}
	if (currentMovement.left)
	{
		//		qDebug() << "LEFT";
		aThingyToMove->setTransform (aThingyToMove->sceneTransform().translate (-0.8, 0));
		leftArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		leftArrow->setBrush (Qt::gray);
	}
	if (currentMovement.clock)
	{
		qDebug() << "CLOCK";
		aThingyToMove->setTransform (aThingyToMove->sceneTransform().rotate (1));
		//		aThingyToMove->setRotation (aThingyToMove->rotation() + 1);
		aThingyToMove->sceneTransform();
		//				aThingyToMove->setTransform (aThingyToMove->transform().rotate (1));

		//		qDebug() << aThingyToMove->transformOriginPoint() << aThingyToMove->sceneTransform();

		//		leftArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		//		leftArrow->setBrush (Qt::gray);
	}
	if (currentMovement.anticlock)
	{
		qDebug() << "ANTICLOCK";
		aThingyToMove->setTransform (aThingyToMove->sceneTransform().rotate (-1));
		//		aThingyToMove->setRotation (aThingyToMove->rotation() - 1);
		//		aThingyToMove->setTransform (aThingyToMove->transform().rotate (-1));

		//		leftArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		//		leftArrow->setBrush (Qt::gray);
	}
	if (didSth)
	{
		qDebug() << "SpriteMovement... Initial Position: " << initPos;
		qDebug() << "_________________________________________SpriteMovement... Final Position: " << aThingyToMove->sceneTransform() << "||"
				 << aThingyToMove->rotation();
	}
}

void Widget::spriteMovement_M2()
{
	bool didSth = false;
	QTransform initPos = aThingyToMove->sceneTransform();

	//	qDebug() << "current movement get : " << currentMovement.up << currentMovement.down << currentMovement.right << currentMovement.left;
	if (currentMovement.up)
	{
		//		qDebug() << "UP";
		QPointF speedPoint (0, -1.5);
		QPointF finalPosPoint (speedPoint.rx() * qCos (qDegreesToRadians (-aThingyToMove->rotation()))
				+ speedPoint.ry() * qSin (qDegreesToRadians (-aThingyToMove->rotation())),
			speedPoint.rx() * qSin (qDegreesToRadians (-aThingyToMove->rotation()))
				+ speedPoint.ry() * qCos (qDegreesToRadians (-aThingyToMove->rotation())));
		aThingyToMove->setPos (aThingyToMove->pos() + finalPosPoint);
		upArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		upArrow->setBrush (Qt::gray);
	}
	if (currentMovement.down)
	{
		//		qDebug() << "DOWN";
		QPointF speedPoint (0, 0.7);
		QPointF finalPosPoint (speedPoint.rx() * qCos (qDegreesToRadians (-aThingyToMove->rotation()))
				+ speedPoint.ry() * qSin (qDegreesToRadians (-aThingyToMove->rotation())),
			speedPoint.rx() * qSin (qDegreesToRadians (-aThingyToMove->rotation()))
				+ speedPoint.ry() * qCos (qDegreesToRadians (-aThingyToMove->rotation())));
		aThingyToMove->setPos (aThingyToMove->pos() + finalPosPoint);
		downArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		downArrow->setBrush (Qt::gray);
	}
	if (currentMovement.right)
	{
		//		qDebug() << "RIGHT";
		QPointF speedPoint (0.8, 0);
		QPointF finalPosPoint (speedPoint.rx() * qCos (qDegreesToRadians (aThingyToMove->rotation()))
				+ speedPoint.ry() * qSin (qDegreesToRadians (aThingyToMove->rotation())),
			speedPoint.rx() * qSin (qDegreesToRadians (aThingyToMove->rotation()))
				+ speedPoint.ry() * qCos (qDegreesToRadians (aThingyToMove->rotation())));
		aThingyToMove->setPos (aThingyToMove->pos() + finalPosPoint);
		rightArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		rightArrow->setBrush (Qt::gray);
	}
	if (currentMovement.left)
	{
		//		qDebug() << "LEFT";
		QPointF speedPoint (-0.8, 0);
		QPointF finalPosPoint (speedPoint.rx() * qCos (qDegreesToRadians (aThingyToMove->rotation()))
				+ speedPoint.ry() * qSin (qDegreesToRadians (aThingyToMove->rotation())),
			speedPoint.rx() * qSin (qDegreesToRadians (aThingyToMove->rotation()))
				+ speedPoint.ry() * qCos (qDegreesToRadians (aThingyToMove->rotation())));
		aThingyToMove->setPos (aThingyToMove->pos() + finalPosPoint);
		leftArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		leftArrow->setBrush (Qt::gray);
	}
	if (currentMovement.clock)
	{
		qDebug() << "CLOCK";
		//		aThingyToMove->setTransform (aThingyToMove->sceneTransform().rotate (1));
		aThingyToMove->setRotation (aThingyToMove->rotation() + 1);
		//	aThingyToMove->sceneTransform();
		//				aThingyToMove->setTransform (aThingyToMove->transform().rotate (1));

		//		qDebug() << aThingyToMove->transformOriginPoint() << aThingyToMove->sceneTransform();

		//		leftArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		//		leftArrow->setBrush (Qt::gray);
	}
	if (currentMovement.anticlock)
	{
		qDebug() << "ANTICLOCK";
		//		aThingyToMove->setTransform (aThingyToMove->sceneTransform().rotate (-1));
		aThingyToMove->setRotation (aThingyToMove->rotation() - 1);
		//		aThingyToMove->setTransform (aThingyToMove->transform().rotate (-1));

		//		leftArrow->setBrush (Qt::green);
		didSth = true;
	}
	else
	{
		//		leftArrow->setBrush (Qt::gray);
	}
	if (didSth)
	{
		qDebug() << "SpriteMovement... Initial Position: " << initPos;
		qDebug() << "_________________________________________SpriteMovement... Final Position: " << aThingyToMove->sceneTransform() << "||"
				 << aThingyToMove->rotation();
		//		qDebug() << aThingyToMove->collidingItems (Qt::IntersectsItemShape);
		QList<QGraphicsItem *> collidingThingies = pointingLine->collidingItems (Qt::IntersectsItemShape);
		QString output = "_____________________________\n";
		if (!collidingThingies.isEmpty())
		{
			for (auto g : collidingThingies)
			{
				//				qDebug() << g->data (0) << g->data (1) << g->data (2) << g->data (3) << g->data (4) << g->data (5);
				output.append (g->data (0).toString() + "\n");
				poofTheEnemy (g);
			}
		}
		ui->textBrowser->setText (output);
	}
}

bool Widget::poofTheEnemy (QGraphicsItem *potentialEnemy)
{
	if (potentialEnemy->data (0) == "Enemy" && potentialEnemy->data (1) == "Poofable" && currentMovement.poofingOn)
	{
		potentialEnemy->setVisible (false);
		ui->sB_D3DZC0unter->setValue (ui->sB_D3DZC0unter->value() + 1);
		return true;
	}
	return false;
}

void Widget::addEnemies()
{
	for (int i = 0; i < 10; i++)
	{
		int x = QRandomGenerator::global()->bounded (xl, xr);
		int y = QRandomGenerator::global()->bounded (yu, yd);

		QGraphicsEllipseItem *a = new QGraphicsEllipseItem (x, y, 10, 10);
		a->setPen (QPen (Qt::white));
		a->setBrush (QBrush (Qt::red));
		a->setData (0, QStringLiteral ("Enemy"));
		a->setData (1, QStringLiteral ("Poofable"));
		scene->addItem (a);
		enemyList << a;
	}
	enemyRespawnTimer.start (1000);
	connect (&enemyRespawnTimer, &QTimer::timeout, this,
		[this] ()
		{
		int n = QRandomGenerator::global()->bounded (10);
		enemyList[n]->setVisible (true);
	});
}

void Widget::on_pB_SwitchMovementMethod_clicked()
{
	connect (&spriteMovementTimer, &QTimer::timeout, this, &Widget::spriteMovement);
	disconnect (&spriteMovementTimer, &QTimer::timeout, this, &Widget::spriteMovement_M2);
}

void Widget::on_pB_Reset_clicked() { aThingyToMove->setTransform (QTransform{}); }

void Widget::on_pB_Shear_clicked() { aThingyToMove->setTransform (aThingyToMove->sceneTransform().shear (1, 0)); }

void Widget::on_pB_BoundBoxRectToggle_clicked() {}
