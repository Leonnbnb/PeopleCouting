// 描点式
#include "stdafx.h"
#include "mouseROI.h"

void MouseDraw(int event,int x,int y,int flags,void*param)
{
	MouseArgs* m_arg = (MouseArgs*) param;
	if( !m_arg->img )
		return;

	if( event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) )
	{
		m_arg->p_end = m_arg->p_start;
	}
	else if( event == CV_EVENT_LBUTTONDOWN )
	{
		m_arg->p_start = cvPoint(x,y);
		cvCircle(m_arg->img,m_arg->p_start,2,cvScalar(0,0,255));
		cvSeqPush( m_arg->seq, &m_arg->p_start);  // 描点记录
		m_arg->points += 1;
		if(m_arg->p_start.x>0 && m_arg->p_end.x>0){
			cvLine( m_arg->img, m_arg->p_start, m_arg->p_end, cvScalar(0,0,255) );
			cvLine( m_arg->img, m_arg->p_start, m_arg->p_start, cvScalar(128,0,255) );
		}
	}

}
/*
// 拖动式
void MouseDraw(int event,int x,int y,int flags,void*param)
{
    MouseArgs* m_arg = (MouseArgs*) param;
    if( !m_arg->img )
        return;
  
    if( event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) )
    {
        m_arg->p_start = cvPoint(x,y);
    }
    else if( event == CV_EVENT_LBUTTONDOWN )
    {
        m_arg->p_start = cvPoint(x,y);
        cvSeqPush( m_arg->seq, &m_arg->p_start);
        m_arg->points += 1;
        if(m_arg->p_start.x>0 && m_arg->p_end.x>0){
            cvLine( m_arg->img, m_arg->p_start, m_arg->p_start, cvScalar(128,0,255) );
        }
    }
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
    {
        CvPoint pt = cvPoint(x,y);
        if( m_arg->p_start.x > 0 ){
            cvLine( m_arg->img, m_arg->p_start, pt, cvScalar(128,0,255) );
            m_arg->p_start = pt;
            cvSeqPush( m_arg->seq, &m_arg->p_start);
            m_arg->points += 1;
        }
  
    }
  
}
*/


