#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>

// 定义当前鼠标所处状态;
enum WindowStretchRectState
{
    NO_SELECT = 0,              // 鼠标未进入下方矩形区域;
    LEFT_TOP_RECT,              // 鼠标在左上角区域;
    TOP_BORDER,                 // 鼠标在上边框区域;
    RIGHT_TOP_RECT,             // 鼠标在右上角区域;
    RIGHT_BORDER,               // 鼠标在右边框区域;
    RIGHT_BOTTOM_RECT,          // 鼠标在右下角区域;
    BOTTOM_BORDER,              // 鼠标在下边框区域;
    LEFT_BOTTOM_RECT,           // 鼠标在左下角区域;
    LEFT_BORDER                 // 鼠标在左边框区域;
};


class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    CustomDialog(QWidget *parent = 0);
    ~CustomDialog();

    WindowStretchRectState getCurrentStretchState(QPoint cursorPos);

        void updateMouseStyle(WindowStretchRectState stretchState);

        void calculateCurrentStrechRect();

        void mouseMoveEvent(QMouseEvent *event);

        void mousePressEvent(QMouseEvent *event);

        void mouseReleaseEvent(QMouseEvent *event);

        void updateWindowSize();
private:
        QRect m_leftTopRect;
            QRect m_leftBottomRect;
            QRect m_rightTopRect;
            QRect m_rightBottomRect;
            QRect m_topBorderRect;
            QRect m_rightBorderRect;
            QRect m_bottomBorderRect;
            QRect m_leftBorderRect;

            WindowStretchRectState m_stretchRectState;
            bool m_isMousePressed;
            bool m_isWindowMax;
            QPoint m_endPoint;
            QPoint m_startPoint;
            QRect m_windowRectBeforeStretch;
            int m_windowMinWidth;
            int m_windowMinHeight;
};

#endif // CUSTOMDIALOG_H
