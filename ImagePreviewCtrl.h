#pragma once
#include <gdiplus.h>
using namespace Gdiplus;
#include <vector>
#include <afxwin.h>
class CImagePreviewCtrl :
    public CStatic
{
public:
    CImagePreviewCtrl();
    ~CImagePreviewCtrl();
    //***********************GDIPUCC*************************//
    Bitmap* m_pBitmap = nullptr;  //主位图
	Bitmap* m_CopyBitmap = nullptr; //原始位图
    ULONG_PTR m_gdiplusToken;
    GdiplusStartupInput m_gdiplusStartupInput;
	bool m_delback = false; //是否删除背景色
    
    //设置显示图片
    void SetBitmap(Bitmap* pBitmap)
    {
        m_pBitmap = pBitmap;
        Invalidate();
        UpdateWindow();
    }
    //打开图片文件并显示
    bool OpenImageFile(const wchar_t* pszFilePath)
    {
        // 释放旧的 Bitmap
        if (m_pBitmap)
        {
            delete m_pBitmap;
            m_pBitmap = nullptr;
        }
        
        Bitmap* pBitmap = new Bitmap(pszFilePath);
       
        if (pBitmap->GetHeight() != pBitmap->GetWidth())
        {
            // 如果图片不是正方形，删除并返回 false
            delete pBitmap;
            MessageBox(L"传入的位图必须是1:1的正方形！");
			return false;
        }

        if (pBitmap->GetLastStatus() == Ok)
        {
            // 释放旧的备份
            if (m_CopyBitmap)
            {
                delete m_CopyBitmap;
                m_CopyBitmap = nullptr;
            }
            // 备份原始位图
            m_CopyBitmap = pBitmap->Clone(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), pBitmap->GetPixelFormat());

            SetBitmap(pBitmap);
            return true;
        }
        else
        {
            delete pBitmap;
            return false;
        }
        return false;
    }
    //设置圆角
    void SetRCornersValue(int Va = 0);
    //获取图片原始大小
    CRect GetImageSize() const
    {
        if (m_pBitmap)
        {
            return CRect(0, 0, m_CopyBitmap->GetWidth(), m_CopyBitmap->GetHeight());
        }
        return CRect(0, 0, 0, 0);
	}
	
    //移除背景色（增加容差20）
    void DelBitMapBackColor() {
        COLORREF clrBk = GetBitmapBackColor();

        if (m_pBitmap && clrBk != RGB(-1, -1, -1)) // 检查是否有有效的背景色
        {
            BYTE bkR = GetRValue(clrBk);
            BYTE bkG = GetGValue(clrBk);
            BYTE bkB = GetBValue(clrBk);

            for (int y = 0; y < (int)m_pBitmap->GetHeight(); ++y)
            {
                for (int  x = 0; x < (int)m_pBitmap->GetWidth(); ++x)
                {
                    Color pixelColor;
                    m_pBitmap->GetPixel(x, y, &pixelColor);
                    BYTE pxR = pixelColor.GetRed();
                    BYTE pxG = pixelColor.GetGreen();
                    BYTE pxB = pixelColor.GetBlue();

                    // 容差范围20
                    if (abs(pxR - bkR) <= 30 &&
                        abs(pxG - bkG) <= 30 &&
                        abs(pxB - bkB) <= 30)
                    {
                        m_pBitmap->SetPixel(x, y, Color(0, 0, 0, 0)); // 设置为透明
                    }
                }
            }
        }
    }
	//获取背景色
    COLORREF GetBitmapBackColor() const
    {
        Color color;
        if (m_CopyBitmap && m_CopyBitmap->GetPixel(10, 10, &color) == Ok)
        {
            return color.ToCOLORREF(); // 需要自己实现或转换
        }
        return RGB(-1, -1, -1);
	}
	//设置是否删除背景色
	void SetDelBackColor(bool delback = true) { m_delback = delback; }
    //写出指定分辨率的ICO文件
    bool SaveToIcoFile(const wchar_t* pszFilePath, bool p8X8, bool p16X16, bool p32X32, bool p64X64,
        bool p128X128, bool p256X256, bool p512X512);
    
    
	


protected:
    //重新绘制函数
    afx_msg void OnPaint()
    {
        CPaintDC dc(this);

        // 获取客户区
        CRect rc;
        GetClientRect(&rc);

        // 创建兼容内存DC和位图
        CDC memDC;
        memDC.CreateCompatibleDC(&dc);
        CBitmap memBitmap;
        memBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
        CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

        // 获取父窗口背景色
        COLORREF clrBk = ::GetSysColor(COLOR_3DFACE);
        CWnd* pParent = GetParent();
        if (pParent)
        {
            HBRUSH hBrush = (HBRUSH)pParent->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)memDC.GetSafeHdc(), (LPARAM)m_hWnd);
            if (hBrush)
            {
                LOGBRUSH logBrush;
                if (GetObject(hBrush, sizeof(logBrush), &logBrush))
                    clrBk = logBrush.lbColor;
            }
        }

        // 用背景色填充整个客户区
        memDC.FillSolidRect(&rc, clrBk);

        if (m_pBitmap)
        {
            Graphics graphics(memDC.GetSafeHdc());
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);
            graphics.DrawImage(m_pBitmap, 0, 0, rc.Width(), rc.Height());
        }
        else
        {
            CStatic::OnPaint();
        }

        // 将内存DC内容拷贝到窗口DC
        dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

        // 释放资源
        memDC.SelectObject(pOldBitmap);
    }
    DECLARE_MESSAGE_MAP()
};

