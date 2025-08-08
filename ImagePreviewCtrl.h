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
    Bitmap* m_pBitmap = nullptr;  //��λͼ
	Bitmap* m_CopyBitmap = nullptr; //ԭʼλͼ
    ULONG_PTR m_gdiplusToken;
    GdiplusStartupInput m_gdiplusStartupInput;
	bool m_delback = false; //�Ƿ�ɾ������ɫ
    
    //������ʾͼƬ
    void SetBitmap(Bitmap* pBitmap)
    {
        m_pBitmap = pBitmap;
        Invalidate();
        UpdateWindow();
    }
    //��ͼƬ�ļ�����ʾ
    bool OpenImageFile(const wchar_t* pszFilePath)
    {
        // �ͷžɵ� Bitmap
        if (m_pBitmap)
        {
            delete m_pBitmap;
            m_pBitmap = nullptr;
        }
        
        Bitmap* pBitmap = new Bitmap(pszFilePath);
       
        if (pBitmap->GetHeight() != pBitmap->GetWidth())
        {
            // ���ͼƬ���������Σ�ɾ�������� false
            delete pBitmap;
            MessageBox(L"�����λͼ������1:1�������Σ�");
			return false;
        }

        if (pBitmap->GetLastStatus() == Ok)
        {
            // �ͷžɵı���
            if (m_CopyBitmap)
            {
                delete m_CopyBitmap;
                m_CopyBitmap = nullptr;
            }
            // ����ԭʼλͼ
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
    //����Բ��
    void SetRCornersValue(int Va = 0);
    //��ȡͼƬԭʼ��С
    CRect GetImageSize() const
    {
        if (m_pBitmap)
        {
            return CRect(0, 0, m_CopyBitmap->GetWidth(), m_CopyBitmap->GetHeight());
        }
        return CRect(0, 0, 0, 0);
	}
	
    //�Ƴ�����ɫ�������ݲ�20��
    void DelBitMapBackColor() {
        COLORREF clrBk = GetBitmapBackColor();

        if (m_pBitmap && clrBk != RGB(-1, -1, -1)) // ����Ƿ�����Ч�ı���ɫ
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

                    // �ݲΧ20
                    if (abs(pxR - bkR) <= 30 &&
                        abs(pxG - bkG) <= 30 &&
                        abs(pxB - bkB) <= 30)
                    {
                        m_pBitmap->SetPixel(x, y, Color(0, 0, 0, 0)); // ����Ϊ͸��
                    }
                }
            }
        }
    }
	//��ȡ����ɫ
    COLORREF GetBitmapBackColor() const
    {
        Color color;
        if (m_CopyBitmap && m_CopyBitmap->GetPixel(10, 10, &color) == Ok)
        {
            return color.ToCOLORREF(); // ��Ҫ�Լ�ʵ�ֻ�ת��
        }
        return RGB(-1, -1, -1);
	}
	//�����Ƿ�ɾ������ɫ
	void SetDelBackColor(bool delback = true) { m_delback = delback; }
    //д��ָ���ֱ��ʵ�ICO�ļ�
    bool SaveToIcoFile(const wchar_t* pszFilePath, bool p8X8, bool p16X16, bool p32X32, bool p64X64,
        bool p128X128, bool p256X256, bool p512X512);
    
    
	


protected:
    //���»��ƺ���
    afx_msg void OnPaint()
    {
        CPaintDC dc(this);

        // ��ȡ�ͻ���
        CRect rc;
        GetClientRect(&rc);

        // ���������ڴ�DC��λͼ
        CDC memDC;
        memDC.CreateCompatibleDC(&dc);
        CBitmap memBitmap;
        memBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
        CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

        // ��ȡ�����ڱ���ɫ
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

        // �ñ���ɫ��������ͻ���
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

        // ���ڴ�DC���ݿ���������DC
        dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

        // �ͷ���Դ
        memDC.SelectObject(pOldBitmap);
    }
    DECLARE_MESSAGE_MAP()
};

