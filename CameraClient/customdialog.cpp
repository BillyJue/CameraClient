#include "customdialog.h"
#include <QMouseEvent>
#define STRETCH_RECT_HEIGHT 4       // 拉伸小矩形的高度;
#define STRETCH_RECT_WIDTH 4        // 拉伸小矩形的宽度;
CustomDialog::CustomDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
        setMouseTracking(true);
        m_stretchRectState = NO_SELECT;
        m_windowMinHeight = size().height();
        m_windowMinWidth = size().width();
        m_isWindowMax = false;
        m_isMousePressed = false;
}

CustomDialog::~CustomDialog()
{

}


// 计算拉伸区域Rect位置;
// 以下8个Rect对应上图中8个区域;

void CustomDialog::calculateCurrentStrechRect()
{
    // 四个角Rect;
    m_leftTopRect = QRect(0, 0, STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT);
    m_leftBottomRect = QRect(0, this->height() - STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, STRETCH_RECT_WIDTH);
    m_rightTopRect = QRect(this->width() - STRETCH_RECT_WIDTH, 0, STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT);
    m_rightBottomRect = QRect(this->width() - STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT);

    // 四条边Rect;
    m_topBorderRect = QRect(STRETCH_RECT_WIDTH, 0, this->width() - STRETCH_RECT_WIDTH * 2, STRETCH_RECT_HEIGHT);
    m_rightBorderRect = QRect(this->width() - STRETCH_RECT_WIDTH, STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT * 2);
    m_bottomBorderRect = QRect(STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT, this->width() - STRETCH_RECT_WIDTH * 2, STRETCH_RECT_HEIGHT);
    m_leftBorderRect = QRect(0, STRETCH_RECT_HEIGHT, STRETCH_RECT_WIDTH, this->height() - STRETCH_RECT_HEIGHT * 2);
}

// 根据当前鼠标位置，定位鼠标在具体哪一块拉伸区域;
WindowStretchRectState CustomDialog::getCurrentStretchState(QPoint cursorPos)
{
    WindowStretchRectState stretchState;
    if (m_leftTopRect.contains(cursorPos))
    {
        stretchState = LEFT_TOP_RECT;
    }
    else if (m_rightTopRect.contains(cursorPos))
    {
        stretchState = RIGHT_TOP_RECT;
    }
    else if (m_rightBottomRect.contains(cursorPos))
    {
        stretchState = RIGHT_BOTTOM_RECT;
    }
    else if (m_leftBottomRect.contains(cursorPos))
    {
        stretchState = LEFT_BOTTOM_RECT;
    }
    else if (m_topBorderRect.contains(cursorPos))
    {
        stretchState = TOP_BORDER;
    }
    else if (m_rightBorderRect.contains(cursorPos))
    {
        stretchState = RIGHT_BORDER;
    }
    else if (m_bottomBorderRect.contains(cursorPos))
    {
        stretchState = BOTTOM_BORDER;
    }
    else if (m_leftBorderRect.contains(cursorPos))
    {
        stretchState = LEFT_BORDER;
    }
    else
    {
        stretchState = NO_SELECT;
    }

    return stretchState;
}

// 根据getCurrentStretchState返回状态进行更新鼠标样式;
void CustomDialog::updateMouseStyle(WindowStretchRectState stretchState)
{
    switch (stretchState)
    {
    case NO_SELECT:
        setCursor(Qt::ArrowCursor);
        break;
    case LEFT_TOP_RECT:
    case RIGHT_BOTTOM_RECT:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TOP_BORDER:
    case BOTTOM_BORDER:
        setCursor(Qt::SizeVerCursor);
        break;
    case RIGHT_TOP_RECT:
    case LEFT_BOTTOM_RECT:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case LEFT_BORDER:
    case RIGHT_BORDER:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }

}

void CustomDialog::mouseMoveEvent(QMouseEvent *event)
{
    // 如果窗口最大化是不能拉伸的;
    // 也不用更新鼠标样式;
    if (m_isWindowMax)
    {
        return __super::mouseMoveEvent(event);
    }

    // 如果当前鼠标未按下，则根据当前鼠标的位置更新鼠标的状态及样式;
    if (!m_isMousePressed)
    {
        QPoint cursorPos = event->pos();
        // 根据当前鼠标的位置显示不同的样式;
        m_stretchRectState = getCurrentStretchState(cursorPos);
        updateMouseStyle(m_stretchRectState);
    }
    // 如果当前鼠标左键已经按下，则记录下第二个点的位置，并更新窗口的大小;
    else
    {
        m_endPoint = this->mapToGlobal(event->pos());
        updateWindowSize();
    }

    return __super::mouseMoveEvent(event);
}


void CustomDialog::mousePressEvent(QMouseEvent *event)
{
    // 当前鼠标进入了以上指定的8个区域，并且是左键按下时才开始进行窗口拉伸;
    if (m_stretchRectState != NO_SELECT && event->button() == Qt::LeftButton)
    {
        m_isMousePressed = true;
        // 记录下当前鼠标位置，为后面计算拉伸位置;
        m_startPoint = this->mapToGlobal(event->pos());
        // 保存下拉伸前的窗口位置及大小;
        m_windowRectBeforeStretch = this->geometry();
    }

    return __super::mousePressEvent(event);
}

void CustomDialog::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标松开后意味之窗口拉伸结束，置标志位，并且重新计算用于拉伸的8个区域Rect;
    m_isMousePressed = false;
    calculateCurrentStrechRect();

    return __super::mouseReleaseEvent(event);
}

// 拉伸窗口过程中，根据记录的坐标更新窗口大小;
void CustomDialog::updateWindowSize()
{
    // 拉伸时要注意设置窗口最小值;
    QRect windowRect = m_windowRectBeforeStretch;
    int delValue_X = m_startPoint.x() - m_endPoint.x();
    int delValue_Y = m_startPoint.y() - m_endPoint.y();

    if (m_stretchRectState == LEFT_BORDER)
    {
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setX(topLeftPoint.x() - delValue_X);
        windowRect.setTopLeft(topLeftPoint);
    }
    else if (m_stretchRectState == RIGHT_BORDER)
    {
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setX(bottomRightPoint.x() - delValue_X);
        windowRect.setBottomRight(bottomRightPoint);
    }
    else if (m_stretchRectState == TOP_BORDER)
    {
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setY(topLeftPoint.y() - delValue_Y);
        windowRect.setTopLeft(topLeftPoint);
    }
    else if (m_stretchRectState == BOTTOM_BORDER)
    {
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setY(bottomRightPoint.y() - delValue_Y);
        windowRect.setBottomRight(bottomRightPoint);
    }
    else if (m_stretchRectState == LEFT_TOP_RECT)
    {
        QPoint topLeftPoint = windowRect.topLeft();
        topLeftPoint.setX(topLeftPoint.x() - delValue_X);
        topLeftPoint.setY(topLeftPoint.y() - delValue_Y);
        windowRect.setTopLeft(topLeftPoint);
    }
    else if (m_stretchRectState == RIGHT_TOP_RECT)
    {
        QPoint topRightPoint = windowRect.topRight();
        topRightPoint.setX(topRightPoint.x() - delValue_X);
        topRightPoint.setY(topRightPoint.y() - delValue_Y);
        windowRect.setTopRight(topRightPoint);
    }
    else if (m_stretchRectState == RIGHT_BOTTOM_RECT)
    {
        QPoint bottomRightPoint = windowRect.bottomRight();
        bottomRightPoint.setX(bottomRightPoint.x() - delValue_X);
        bottomRightPoint.setY(bottomRightPoint.y() - delValue_Y);
        windowRect.setBottomRight(bottomRightPoint);
    }
    else if (m_stretchRectState == LEFT_BOTTOM_RECT)
    {
        QPoint bottomLeftPoint = windowRect.bottomLeft();
        bottomLeftPoint.setX(bottomLeftPoint.x() - delValue_X);
        bottomLeftPoint.setY(bottomLeftPoint.y() - delValue_Y);
        windowRect.setBottomLeft(bottomLeftPoint);
    }

    // 避免宽或高为零窗口显示有误，这里给窗口设置最小拉伸高度、宽度;
    if (windowRect.width() < m_windowMinWidth)
    {
        windowRect.setLeft(this->geometry().left());
        windowRect.setWidth(m_windowMinWidth);
    }
    if (windowRect.height() < m_windowMinHeight)
    {
        windowRect.setTop(this->geometry().top());
        windowRect.setHeight(m_windowMinHeight);
    }

    this->setGeometry(windowRect);
}
