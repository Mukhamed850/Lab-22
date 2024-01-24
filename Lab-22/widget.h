#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

struct Shape {
    QRect  object;
    QColor color;
    qreal angle = 0;
};


class Widget: public QWidget
{
    Q_OBJECT
    public:
        Widget(QWidget* p = nullptr);
        void paintEvent(QPaintEvent *p = nullptr);

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        void drawRectangle();
        void drawCircle();
        void rotate(QPainter* p, const QRectF& r, qreal angle);

        int inRectangle(QPoint p);
        int inCircle(QPoint p);

        QPoint lastPoint;
        int shape_type;


        QList<Shape> rectangles_list = {};
        QList<Shape> circles_list    = {};

        int current_rectangle_index = 0;
        int current_circle_index    = 0;

    public slots:
        void ShowContextMenu(const QPoint &pos);
        void changeAngle();
        void scale();
        void addNewRectangle();
        void addNewCircle();
};
