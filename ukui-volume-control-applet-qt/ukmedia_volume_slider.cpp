/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */
#include "ukmedia_volume_slider.h"
#include <QPalette>
#include <QPainter>
#include <QPainterPath>
#include <QWidget>
#include <QStyleOptionComplex>
#include <QDebug>

DisplayerMode displayMode = MINI_MODE;
SwitchButtonState buttonState = SWITCH_BUTTON_NORMAL;
UkuiApplicationWidget::UkuiApplicationWidget(QWidget *parent)
{
    Q_UNUSED(parent);
}

void UkuiApplicationWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    p.setBrush(QBrush(QColor(0x00,0x00,0x00,0x00)));
    p.setPen(Qt::NoPen);
    QPainterPath path;
    opt.rect.adjust(0,0,0,0);
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    setProperty("blurRegion",QRegion(path.toFillPolygon().toPolygon()));
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

UkuiApplicationWidget::~UkuiApplicationWidget()
{

}

UkuiMediaButton::UkuiMediaButton(QWidget *parent)
{
    Q_UNUSED(parent);
    this->setFixedSize(36,36);
}

UkuiMediaButton::~UkuiMediaButton()
{

}

/*!
 * \brief
 * \details
 * 绘制窗体的颜色及圆角
 */
//void UkuiMediaButton::paintEvent(QPaintEvent *event)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    if (buttonState == SWITCH_BUTTON_NORMAL) {
//         p.setBrush(QColor(0xff,0xff,0xff,0x00));
////         qDebug() << "正常";
//    }
//    else if (buttonState == SWITCH_BUTTON_HOVER) {
//        p.setBrush(QColor(0xff,0xff,0xff,0x1f));
//        qDebug() << "悬停";
//    }
//    else if (buttonState == SWITCH_BUTTON_PRESS) {
//        p.setBrush(QColor(0xff,0xff,0xff,0x14));
//        qDebug() << "点击";
//    }
//    p.setPen(Qt::NoPen);
//    QPainterPath path;
////    opt.rect.adjust(0,0,0,0);
////    path.addRoundedRect(opt.rect,4,4);
////    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
//    p.drawRoundedRect(opt.rect,4,4);
////    this->setIcon(QIcon("/usr/share/ukui-media/img/complete-module.svg"));
////    setProperty("blurRegion",QRegion(path.toFillPolygon().toPolygon()));
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}

void UkuiMediaButton::enterEvent(QEvent *event)
{
    buttonState = SWITCH_BUTTON_HOVER;
}

void UkuiMediaButton::leaveEvent(QEvent *event)
{
    buttonState = SWITCH_BUTTON_NORMAL;
}

void UkuiMediaButton::mouseMoveEvent(QMouseEvent *e)
{
    buttonState = SWITCH_BUTTON_HOVER;
    QToolButton::mouseMoveEvent(e);
}

void UkuiMediaButton::mousePressEvent(QMouseEvent *e)
{
    buttonState = SWITCH_BUTTON_PRESS;
    if (displayMode == MINI_MODE) {
        Q_EMIT moveMiniSwitchBtnSignale();
        this->setFixedSize(34,34);
        QSize iconSize(14,14);
        this->setIconSize(iconSize);
//        qDebug() << "设置tub ";
        qDebug() << "设置图标3";
        this->setIcon(QIcon("/usr/share/ukui-media/img/complete-module.svg"));
    }
    else {
        this->setFixedSize(34,34);
        QSize iconSize(14,14);
        this->setIconSize(iconSize);
        qDebug() << "设置图标4";
        this->setIcon(QIcon("/usr/share/ukui-media/img/mini-module.svg"));
        Q_EMIT moveAdvanceSwitchBtnSignal();
    }
    QToolButton::mousePressEvent(e);
}

void UkuiMediaButton::mouseReleaseEvent(QMouseEvent *e)
{
    buttonState = SWITCH_BUTTON_NORMAL;
    if (displayMode == MINI_MODE) {
        Q_EMIT miniToAdvanceSignal();
        this->setFixedSize(36,36);
        QSize iconSize(16,16);
        this->setIconSize(iconSize);
        this->setIcon(QIcon("/usr/share/ukui-media/img/complete-module.svg"));
    }
    else {
        Q_EMIT advanceToMiniSignal();
        this->setFixedSize(36,36);
        QSize iconSize(16,16);
        this->setIconSize(iconSize);
        qDebug() << "设置图标2";
        this->setIcon(QIcon("/usr/share/ukui-media/img/mini-module.svg"));
    }
    QToolButton::mouseReleaseEvent(e);
}

UkmediaVolumeSlider::UkmediaVolumeSlider(QWidget *parent)
{
    Q_UNUSED(parent);  
}

void UkmediaVolumeSlider::mousePressEvent(QMouseEvent *ev)
{
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos = ev->pos().x() / (double)width();
    setValue(pos *(maximum() - minimum()) + minimum());
    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
    QCoreApplication::sendEvent(parentWidget(), &evEvent);
}

void UkmediaVolumeSlider::initStyleOption(QStyleOptionSlider *option)
{
    QSlider::initStyleOption(option);
}

UkmediaVolumeSlider::~UkmediaVolumeSlider()
{

}

void UkuiButtonDrawSvg::init(QImage img, QColor color)
{
    themeIcon.image = img;
    themeIcon.color = color;
}

void UkuiButtonDrawSvg::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    p.setBrush(QBrush(QColor(0x13,0x13,0x14,0x00)));
    p.setPen(Qt::NoPen);
    QPainterPath path;
    opt.rect.adjust(0,0,0,0);
    path.addRoundedRect(opt.rect,6,6);
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    p.drawRoundedRect(opt.rect,6,6);
    setProperty("blurRegion",QRegion(path.toFillPolygon().toPolygon()));
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QRect UkuiButtonDrawSvg::IconGeometry()
{
    QRect res = QRect(QPoint(0,0),QSize(24,24));
    res.moveCenter(QRect(0,0,width(),height()).center());
    return res;
}

void UkuiButtonDrawSvg::draw(QPaintEvent* e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    QRect iconRect = IconGeometry();
    if (themeIcon.image.size() != iconRect.size())
    {
        themeIcon.image = themeIcon.image.scaled(iconRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect r = themeIcon.image.rect();
        r.moveCenter(iconRect.center());
        iconRect = r;
    }

    this->setProperty("fillIconSymbolicColor", true);
    filledSymbolicColoredPixmap(themeIcon.image,themeIcon.color);
    painter.drawImage(iconRect, themeIcon.image);
}

bool UkuiButtonDrawSvg::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Paint:
        draw(static_cast<QPaintEvent*>(event));
        break;

    case QEvent::Move:
    case QEvent::Resize:
    {
        QRect rect = IconGeometry();
    }
        break;

    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
        event->accept();
        break;

    default:
        break;
    }

    return QPushButton::event(event);
}


UkuiButtonDrawSvg::UkuiButtonDrawSvg(QWidget *parent)
{
    Q_UNUSED(parent);
}
UkuiButtonDrawSvg::~UkuiButtonDrawSvg()
{

}

QPixmap UkuiButtonDrawSvg::filledSymbolicColoredPixmap(QImage &img, QColor &baseColor)
{

    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                int hue = color.hue();
                if (!qAbs(hue - symbolic_color.hue()) < 10) {
                    color.setRed(baseColor.red());
                    color.setGreen(baseColor.green());
                    color.setBlue(baseColor.blue());
                    img.setPixelColor(x, y, color);
                }
            }
        }
    }

    return QPixmap::fromImage(img);
}

