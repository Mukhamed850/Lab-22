#include "widget.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QRandomGenerator>

Widget::Widget(QWidget *parent): QWidget(parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(ShowContextMenu(const QPoint &)));
}

void Widget::paintEvent(QPaintEvent *) {
    drawRectangle();
    drawCircle();
}

void Widget::drawRectangle() {
    QPainter painter(this);
    for(const Shape &shape : rectangles_list) {
            rotate(&painter, shape.object, shape.angle);
            painter.setBrush(shape.color);
            painter.setPen(shape.color);
            painter.drawRect(shape.object);
    }
}

void Widget::drawCircle() {
    QPainter painter(this);
    for(const Shape &shape : circles_list) {
            rotate(&painter, shape.object, shape.angle);
            painter.setBrush(shape.color);
            painter.setPen(shape.color);
            painter.drawEllipse(shape.object);
    }
}


int Widget::inRectangle(QPoint clickPoint) {
    for (int i = 0; i < rectangles_list.size(); ++i) {
        if (clickPoint.x() >= rectangles_list[i].object.x() &&
            clickPoint.x() <= (rectangles_list[i].object.x() + rectangles_list[i].object.width()) &&
            clickPoint.y() >= rectangles_list[i].object.y() &&
            clickPoint.y() <= (rectangles_list[i].object.y() + rectangles_list[i].object.height())) {
            return i;
        }
    }
    return -1;
}

int Widget::inCircle(QPoint clickPoint)
{
    for (int i = 0; i < circles_list.size(); ++i) {
        if (clickPoint.x() >= circles_list[i].object.x() &&
            clickPoint.x() <= (circles_list[i].object.x() + circles_list[i].object.width()) &&
            clickPoint.y() >= circles_list[i].object.y() &&
            clickPoint.y() <= (circles_list[i].object.y() + circles_list[i].object.height())) {
            return i;
        }
    }
    return -1;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPoint = event->pos();
    if(inRectangle(clickPoint) != -1) {
        shape_type = 1;
        current_rectangle_index = inRectangle(clickPoint);
    }
    else if(inCircle(clickPoint) != -1) {
        shape_type = 2;
        current_circle_index = inCircle(clickPoint);
    }
    else
        shape_type = 0;

    if(shape_type)
    {
        if (event->button() == Qt::LeftButton) {
                lastPoint = clickPoint;
        }
        else if(event->button() == Qt::RightButton )
        {
            qDebug() << "Right button is clicked!";
        }
    }
}

void Widget::ShowContextMenu(const QPoint &pos)
{
    if(inRectangle(pos) != -1)
        shape_type = 1;
    else if(inCircle(pos) != -1)
        shape_type = 2;
    else
        shape_type = 0;

    if(shape_type)
    {
        QMenu contextMenu(tr("Контекстное меню"), this);
        QAction action1("Повернуть на 30 градусов", this);
        connect(&action1, SIGNAL(triggered()), this, SLOT(changeAngle()));

        QAction action2("Увеличить длину и ширину на 5 пикселей", this);
        connect(&action2, SIGNAL(triggered()), this, SLOT(scale()));

        contextMenu.addAction(&action1);
        contextMenu.addAction(&action2);
        contextMenu.exec(mapToGlobal(pos));
    }
    else {
        QMenu contextMenu(tr("Контекстное меню"), this);
        QAction action3("Добавить прямоугольник", this);
        connect(&action3, SIGNAL(triggered()), this, SLOT(addNewRectangle()));
        QAction action4("Добавить круг", this);
        connect(&action4, SIGNAL(triggered()), this, SLOT(addNewCircle()));

        contextMenu.addAction(&action3);
        contextMenu.addAction(&action4);
        contextMenu.exec(mapToGlobal(pos));
    }

}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if (shape_type) {
        int delta_x = event->pos().x() - lastPoint.x();
        int delta_y = event->pos().y() - lastPoint.y();
        lastPoint = event->pos();
        switch (shape_type)
        {
        case 1:
            rectangles_list[current_rectangle_index].object.moveTo(rectangles_list[current_rectangle_index].object.x() + delta_x, rectangles_list[current_rectangle_index].object.y() + delta_y);
            break;
        case 2:
            circles_list[current_circle_index].object.moveTo(circles_list[current_circle_index].object.x() + delta_x, circles_list[current_circle_index].object.y() + delta_y);
            break;

        default:
            break;
        }
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && shape_type) {
        shape_type = 0;
    }
}

void Widget::addNewRectangle() {
    Shape newRectangle;
    newRectangle.object = QRect(10, 10, 50, 50);
    newRectangle.color = QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256));
    rectangles_list.append(newRectangle);
    update();
}

void Widget::addNewCircle() {
    Shape newCircle;
    newCircle.object = QRect(70, 70, 70, 70);
    newCircle.color = QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256));
    circles_list.append(newCircle);
    update();
}

void Widget::changeAngle() {
    switch (shape_type) {
    case 1:
        rectangles_list[current_rectangle_index].angle += 30;
        break;
    case 2:
        circles_list[current_circle_index].angle += 30;
        break;
    default:
        break;
    }
}

void Widget::rotate(QPainter* painter, const QRectF& r, qreal angle) {
    painter->translate(r.center());
    painter->rotate(angle);
    painter->translate(-r.center());
    update();
}
void Widget::scale() {
    switch (shape_type)
    {
    case 1:
        rectangles_list[current_rectangle_index].object.setWidth(rectangles_list[current_rectangle_index].object.width()+5);
        rectangles_list[current_rectangle_index].object.setHeight(rectangles_list[current_rectangle_index].object.height()+5);
        break;
    case 2:
        circles_list[current_circle_index].object.setWidth(circles_list[current_circle_index].object.width()+5);
        circles_list[current_circle_index].object.setHeight(circles_list[current_circle_index].object.height()+5);
        break;
    default:
        break;
    }
}
