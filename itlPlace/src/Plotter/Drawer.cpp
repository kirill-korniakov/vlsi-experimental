#include "HPlotter.h"
#include "PlotterData.h"
#include "HDesign.h"

void HPlotter::DrawBar(double x1, double y1, double x2, double y2, Color col, bool doRefresh)
{
    if (IsEnabled())
    {
        CvPoint start, finish;
        start.x = DesignX2ImageX(x1);
        start.y = DesignY2ImageY(y1);
        finish.x = DesignX2ImageX(x2);
        finish.y = DesignY2ImageY(y2);
        cvRectangle(IMG, start, finish, GetCvColor(col), CV_FILLED);

        if (doRefresh)
          _AutoRefresh();
    }
}

void HPlotter::DrawCircle(double x, double y, int radius, Color col, bool doRefresh)
{
    if (IsEnabled())
    {
        CvPoint center;
        center.x = DesignX2ImageX(x);
        center.y = DesignY2ImageY(y);
        cvCircle(IMG, center, radius, GetCvColor(col), 2);

        if (doRefresh)
          _AutoRefresh();
    }
}

void HPlotter::DrawRectangle(double x1, double y1, double x2, double y2, Color col, bool doRefresh)
{
    if (IsEnabled())
    {
        CvPoint start, finish;
        start.x = DesignX2ImageX(x1);
        start.y = DesignY2ImageY(y1);
        finish.x = DesignX2ImageX(x2);
        finish.y = DesignY2ImageY(y2);
        cvRectangle(IMG, start, finish, GetCvColor(col), 1);

        if (doRefresh)
          _AutoRefresh();
    }
}

void HPlotter::DrawFilledRectangle(double x, double y, double width, double height, Color col, bool doRefresh)
{
    if (!IsEnabled())
        return;

    CvPoint start, finish;
    CvScalar cellBorderColor = cvScalar(255.0, 0.0, 0.0);

    //draw border of cells
    start.x = DesignX2ImageX(x);
    start.y = DesignY2ImageY(y);
    finish.x = DesignX2ImageX(x + width);
    finish.y = DesignY2ImageY(y + height);

    cvRectangle(IMG, start, finish, cellBorderColor);

    //  fill the cell
    start.x++; start.y++;
    finish.x--; finish.y--;
    CvScalar fillColor = GetCvColor(col);
    if (start.x <= finish.x && start.y <= finish.y)
    {
        cvRectangle(IMG, start, finish, fillColor, CV_FILLED);
    }

    if (doRefresh)
      _AutoRefresh();
}

void HPlotter::DrawFilledRectangleWithBorder(double x1, double y1, double x2, double y2,
                                             Color borderColor, Color fillColor, bool doRefresh)
{
    if (!IsEnabled())
        return;

    CvPoint start, finish;
    start.x  = DesignX2ImageX(x1);
    start.y  = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);

    //draw border
    cvRectangle(IMG, start, finish, GetCvColor(borderColor), 1);

    //fill rectangle
    start.x++; start.y++;
    finish.x--; finish.y--;

    if (start.x <= finish.x && start.y <= finish.y)
        cvRectangle(IMG, start, finish, GetCvColor(fillColor), CV_FILLED);

    if (doRefresh)
        _AutoRefresh();
}

void HPlotter::DrawLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh)
{
    if (IsEnabled())
    {
        CvPoint start, finish;
        start.x = DesignX2ImageX(x1);
        start.y = DesignY2ImageY(y1);
        finish.x = DesignX2ImageX(x2);
        finish.y = DesignY2ImageY(y2);
        cvLine(IMG, start, finish, GetCvColor(col), 1);

        if (doRefresh)
          _AutoRefresh();
    }
}

void HPlotter::DrawText(string text, double textSize)
{
    if (!IsEnabled())
        return;

    if (textSize == -1)
        textSize = m_hd.cfg.ValueOf("plotter.textSize", 1.0);

    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, textSize, textSize , 0.0, 1, 1);

    int start = 0;
    int t = 1;
    char* str = new char[text.length() * 4];
    string bufText = text + text;
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if ((text[i] == '\n') || (i == text.length() - 1))
        {
            for (unsigned int j = start; j <= i; j++)
                bufText[j - start] = text[j];
            bufText[i - start + 1] = '\0';

            int j2 = 0;
            for (unsigned int j = 0; j < bufText.length(); j++)
            {
                if (bufText[j] == '\t')
                {
                    str[j2] = ' ';
                    j2++;
                    str[j2] = ' ';
                    j2++;
                    str[j2] = ' ';
                    j2++;
                    str[j2] = ' ';
                }
                else
                    str[j2] = bufText[j];
                j2++;
            }

            CvPoint point;
            point.x = DesignX2ImageX(10);
            point.y = int(m_Data->textSpaceHeight - 30 * t * textSize);
            cvPutText(IMG, str, point, &font, GetCvColor(Color_Black));
            start = i+1;
            t++;
        }
    }

    delete [] str;
}

void HPlotter::DrawTextInPoint(string text, double x, double y, double textSize )
{

    if (!IsEnabled())
        return;

    if (textSize == -1)
        textSize = m_hd.cfg.ValueOf("plotter.textSize", 1.0);

    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, textSize, textSize , 0.0, 1, 1);

    int start = 0;
    int t = 1;
    char* str = new char[text.length() * 4];
    string bufText = text + text;
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if ((text[i] == '\n') || (i == text.length() - 1))
        {
            for (unsigned int j = start; j <= i; j++)
                bufText[j - start] = text[j];
            bufText[i - start + 1] = '\0';

            int j2 = 0;
            for (unsigned int j = 0; j < bufText.length(); j++)
            {
                if (bufText[j] == '\t')
                {
                    str[j2] = ' ';
                    j2++;
                    str[j2] = ' ';
                    j2++;
                    str[j2] = ' ';
                    j2++;
                    str[j2] = ' ';
                }
                else
                    str[j2] = bufText[j];
                j2++;
            }

            CvPoint point;
            point.x = DesignX2ImageX(x);
            point.y = DesignY2ImageY(y);
            cvPutText(IMG, str, point, &font, GetCvColor(Color_Black));
            start = i+1;
            t++;
        }
    }

    delete [] str;

}