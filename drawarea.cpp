#include "drawarea.h"
#include "mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

DrawArea::DrawArea(QWidget *parent) :
    QWidget(parent),
    xmin(-8.),
    xmax(8.),
    ymin(-6.),
    ymax(6.),
    nodes(),
    mouseButtons(),
    drawTriangleFlag(false),
    inRadius(0.),
    inCenter(),
    gergonnePoint(),
    dragNode(-1)
{
    drawArea = this;
    setMouseTracking(true);
}

void DrawArea::setCoordinates(double xMin, double xMax) {
    if (width() == 0 || height() == 0)
        return;
    if (xMax <= xMin)
        return;
    xmin = xMin;
    xmax = xMax;
    double aspect = double(width())/double(height());
    double ySize = (xmax - xmin)/aspect;
    ymin = (-ySize/2.);
    ymax = ySize/2.;
    update();
}

void DrawArea::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.fillRect(
                QRect(0, 0, width(), height()),
                QBrush(Qt::lightGray)
                );
    drawGrid(&painter);
    drawNodes(&painter);
    if (drawTriangleFlag) {
        drawGergonnePoint(&painter);
    }
}

void DrawArea::drawGrid(QPainter* painter) {
    QPen grayPen(Qt::gray);
    QPen blackPen(Qt::black);

    painter->setPen(grayPen);
    // painter->drawLine(QPointF(0., 0.), QPointF(300., 300.));

    double x = round(xmin);
    while (x <= round(xmax)) {
        QPointF p0 = mapToPixels(R2Point(x, ymin));
        QPointF p1 = mapToPixels(R2Point(x, ymax));
        painter->drawLine(p0, p1);
        x += 1.;
    }

    double y = round(ymin);
    while (y <= round(ymax)) {
        QPointF p0 = mapToPixels(R2Point(xmin, y));
        QPointF p1 = mapToPixels(R2Point(xmax, y));
        painter->drawLine(p0, p1);
        y += 1.;
    }

    painter->setPen(blackPen);
    painter->drawLine(
                mapToPixels(R2Point(xmin, 0.)),
                mapToPixels(R2Point(xmax, 0.))
                );
    painter->drawLine(
                mapToPixels(R2Point(0., ymin)),
                mapToPixels(R2Point(0, ymax))
                );
}

void DrawArea::drawGergonnePoint(QPainter* painter) {
    if (nodes.size() < 3)
        return;
    painter->setRenderHint(QPainter::Antialiasing);
    QPen bluePen(Qt::blue,3);
    painter->setPen(bluePen);

    QPainterPath path;
    path.moveTo(mapToPixels(nodes[0]));
    path.lineTo(mapToPixels(nodes[1]));

    path.lineTo(mapToPixels(nodes[2]));
    path.closeSubpath();
    painter->drawPath(path);
    painter->setBrush(Qt::NoBrush);
    QPointF pixelCenter=mapToPixels(inCenter);
    QPointF shiftedCenter=mapToPixels(inCenter+R2Vector(1.,0)*inRadius);
    double pixelRadius=shiftedCenter.x()-pixelCenter.x();
    QPen redPen(Qt::red,2);
    painter->setPen(redPen);
    painter->drawEllipse(pixelCenter,pixelRadius,pixelRadius);

    QPen greenPen(Qt::darkGreen,2);
    painter->setPen(greenPen);
    for (int i=0;i<3 ;++i ) {
        painter->drawLine(
                    mapToPixels(nodes[i]),
                    mapToPixels(cevians[i]));
    }
    QBrush greenBrush(Qt::darkGreen);
    painter->setBrush(greenBrush);
    painter->drawEllipse(
                mapToPixels(gergonnePoint),5,5);

}

void DrawArea::resizeEvent(QResizeEvent* /* event */) {
    if (width() == 0 || height() == 0)
        return;
    setCoordinates(xmin, xmax);
}

void DrawArea::drawNodes(QPainter* painter) {
    R2Vector dx(0.2,0.);
    R2Vector dy(0.,0.2);
    QPen redPen(Qt::red,2);
    QPen bluePen(Qt::blue,2);
    QPen greenPen(Qt::darkGreen,2);
    for (size_t i=0;i<nodes.size() ;++i ) {
        if (mouseButtons[i]==Qt::RightButton){
            painter->setPen(greenPen);
        } else if (mouseButtons[i]==Qt::MiddleButton) {
            painter->setPen(bluePen);
        } else{
            painter->setPen(redPen);
        }
        R2Point p=nodes[i];
        painter->drawLine(
                    mapToPixels(p-dx),
                    mapToPixels(p+dx)
                    );
        painter->drawLine(
                    mapToPixels(p-dy),
                    mapToPixels(p+dy)
                    );

    }
}





void DrawArea::mousePressEvent(QMouseEvent* event) {
    int vIdx = findVertex(event->pos());
    if (vIdx >= 0) {
        dragNode = vIdx;
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent* event) {
    QPointF mousePos = event->pos();
    if (dragNode >= 0) {
        nodes[dragNode] = mapFromPixels(mousePos);
        if (drawTriangleFlag) {
            computeGergonePoint();
        }
        update();
        event->accept();
        return;
    }
    int vertexIdx = findVertex(mousePos);
    if (vertexIdx < 0) {
        // cursor().setShape(Qt::ArrowCursor);
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    } else {
        // cursor().setShape(Qt::OpenHandCursor);
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent* event) {
    QPointF q=event->pos();
    R2Point p=mapFromPixels(q);
    if (dragNode>=0){
        dragNode=-1;
        setCursor(Qt::ArrowCursor);
    } else {
        nodes.push_back(p);
        mouseButtons.push_back(event->button());
        update();
    }
}

void DrawArea::clear() {
    nodes.clear();
    mouseButtons.clear();
    drawTriangleFlag = false;
    dragNode = (-1);
    update();
}


void DrawArea::computeGergonePoint() {
    drawTriangleFlag=true;
    R2Vector v01 =nodes[1]-nodes[0];
    v01.normalize();
    R2Vector v02=nodes[2]-nodes[0];
    v02.normalize();
    R2Vector bis0=v01+v02;

    R2Vector v10=nodes[0]-nodes[1];
    v10.normalize();
    R2Vector v12=nodes[2]-nodes[1];
    v12.normalize();
    R2Vector bis1=v10+v12;
    intersectStraightLines(
                nodes[0],bis0,
            nodes[1],bis1,
            inCenter
            );
    inRadius=inCenter.distanceToLine(
                nodes[0],v01);
    intersectStraightLines(
                nodes[1],nodes[2]-nodes[1],inCenter,(nodes[2]-nodes[1]).normal(),cevians[0]);
    intersectStraightLines(
                nodes[2],nodes[0]-nodes[2],inCenter,(nodes[0]-nodes[2]).normal(),cevians[1]);
    intersectStraightLines(
                nodes[1],nodes[1]-nodes[0],inCenter,(nodes[1]-nodes[0]).normal(),cevians[2]);

    // gergonne point
    intersectStraightLines(
                nodes[0],cevians[0]-nodes[0],
            nodes[1],cevians[1]-nodes[1],
            gergonnePoint);
}

void DrawArea::onDraw() {
    if (nodes.size() >= 3) {
        computeGergonePoint();
        drawTriangleFlag=true;
    }
    update();
}

static const double SEARCH_DIST = 0.1;

int DrawArea::findVertex(QPointF mousePos) const {
    R2Point t = mapFromPixels(mousePos);
    for (int i = 0; i < int(nodes.size()); ++i) {
        double dist = t.distance(nodes[i]);
        if (dist <= SEARCH_DIST) {
            return i;
        }
    }
    return (-1);
}
