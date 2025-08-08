#include "pch.h"
#include "ImagePreviewCtrl.h"


BEGIN_MESSAGE_MAP(CImagePreviewCtrl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

CImagePreviewCtrl::CImagePreviewCtrl()
{
   
    GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}

CImagePreviewCtrl::~CImagePreviewCtrl()
{
    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = nullptr;
    }
    if (m_CopyBitmap) {
        delete m_CopyBitmap;
        m_CopyBitmap = nullptr;
    }
    GdiplusShutdown(m_gdiplusToken);
}

void CImagePreviewCtrl::SetRCornersValue(int Va)
{
    if (!m_pBitmap || Va <= 0)
        return;

    // ��ȡԭͼ�ߴ�
    int width = m_CopyBitmap->GetWidth();
    int height = m_CopyBitmap->GetHeight();

    // �½� ARGB λͼ
    Bitmap* pRoundedBitmap = new Bitmap(width, height, PixelFormat32bppARGB);
    Graphics graphics(pRoundedBitmap);

    // ���ø�����
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // ����Բ��·��
    GraphicsPath path;
    path.AddArc(0, 0, Va * 2, Va * 2, 180, 90);
    path.AddArc(width - Va * 2, 0, Va * 2, Va * 2, 270, 90);
    path.AddArc(width - Va * 2, height - Va * 2, Va * 2, Va * 2, 0, 90);
    path.AddArc(0, height - Va * 2, Va * 2, Va * 2, 90, 90);
    path.CloseFigure();

    // ��·���ü�
    graphics.SetClip(&path);

    // ����ԭͼ����λͼ
    graphics.DrawImage(m_CopyBitmap, 0, 0, width, height);





    // �滻ԭλͼ
    delete m_pBitmap;
    m_pBitmap = pRoundedBitmap;

    //�����Ҫɾ������ɫ
    if (m_delback) {
        DelBitMapBackColor();
    }


    // ˢ����ʾ
    Invalidate();
    UpdateWindow();
}

bool CImagePreviewCtrl::SaveToIcoFile(const wchar_t* pszFilePath,
    bool p8X8, bool p16X16, bool p32X32, bool p64X64,
    bool p128X128, bool p256X256, bool p512X512)
{
    if (!m_pBitmap) return false;

    // 1. �����ֱ����б�
    struct IconSize { int w, h; bool enabled; };
    IconSize sizes[] = {
        {8, 8, p8X8}, {16, 16, p16X16}, {32, 32, p32X32}, {64, 64, p64X64},
        {128, 128, p128X128}, {256, 256, p256X256}, {512, 512, p512X512}
    };

    // 2. ��ȡ PNG ������ CLSID
    CLSID pngClsid = {};
    {
        UINT num = 0;
        UINT size = 0;
        GetImageEncodersSize(&num, &size);
        if (size == 0) return false;

        std::vector<BYTE> buffer(size); // ���ֽڷ���
        ImageCodecInfo* codecs = reinterpret_cast<ImageCodecInfo*>(buffer.data());
        if (GetImageEncoders(num, size, codecs) != Ok) return false;

        for (UINT i = 0; i < num; ++i) {
            if (codecs[i].MimeType && wcsstr(codecs[i].MimeType, L"image/png") != nullptr) {
                pngClsid = codecs[i].Clsid;
                break;
            }
        }
    }
    if (pngClsid == CLSID{}) return false;

    // 3. ���� PNG ����
    struct IconEntry {
        int w, h;
        std::vector<BYTE> pngData;
    };
    std::vector<IconEntry> entries;
    entries.reserve(7);

    for (auto& sz : sizes) {
        if (!sz.enabled) continue;

        Bitmap bmp(sz.w, sz.h, PixelFormat32bppARGB);
        Graphics g(&bmp);
        g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        g.SetSmoothingMode(SmoothingModeHighQuality);
        g.SetPixelOffsetMode(PixelOffsetModeHighQuality);
        g.DrawImage(m_pBitmap, 0, 0, sz.w, sz.h);

        // ����Ϊ PNG ���ڴ���
        IStream* pStream = nullptr;
        if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &pStream))) continue;

        if (bmp.Save(pStream, &pngClsid, NULL) != Ok) {
            pStream->Release();
            continue;
        }

        // ��ȡ PNG ����
        HGLOBAL hMem = NULL;
        if (FAILED(GetHGlobalFromStream(pStream, &hMem))) {
            pStream->Release();
            continue;
        }

        SIZE_T pngSize = GlobalSize(hMem);
        BYTE* pData = (BYTE*)GlobalLock(hMem);
        if (!pData) {
            pStream->Release();
            continue;
        }

        std::vector<BYTE> pngData(pData, pData + pngSize);
        GlobalUnlock(hMem);
        pStream->Release();

        entries.push_back({ sz.w, sz.h, std::move(pngData) });
    }

    if (entries.empty()) return false;

    // 4. ���� ICO �ļ�ͷ��Ŀ¼
    struct ICONDIR {
        WORD idReserved; // ����������Ϊ 0
        WORD idType;     // 1 ��ʾͼ��
        WORD idCount;    // ͼ������
    };
    struct ICONDIRENTRY {
        BYTE bWidth;
        BYTE bHeight;
        BYTE bColorCount;
        BYTE bReserved;
        WORD wPlanes;
        WORD wBitCount;
        DWORD dwBytesInRes;
        DWORD dwImageOffset;
    };

    std::vector<BYTE> icoData;
    ICONDIR dir = { 0, 1, (WORD)entries.size() };
    icoData.insert(icoData.end(), (BYTE*)&dir, (BYTE*)&dir + sizeof(dir));

    DWORD offset = (DWORD)(sizeof(ICONDIR) + sizeof(ICONDIRENTRY) * entries.size());
    for (auto& e : entries) {
        ICONDIRENTRY entry = {};
        entry.bWidth = (e.w == 256) ? 0 : (BYTE)e.w;
        entry.bHeight = (e.h == 256) ? 0 : (BYTE)e.h;
        entry.bColorCount = 0;
        entry.bReserved = 0;
        entry.wPlanes = 1;
        entry.wBitCount = 32;
        entry.dwBytesInRes = (DWORD)e.pngData.size();
        entry.dwImageOffset = offset;
        icoData.insert(icoData.end(), (BYTE*)&entry, (BYTE*)&entry + sizeof(entry));
        offset += entry.dwBytesInRes;
    }

    // 5. ��� PNG ����
    for (auto& e : entries) {
        icoData.insert(icoData.end(), e.pngData.begin(), e.pngData.end());
    }

    // 6. д���ļ�
    FILE* fp = nullptr;
    if (_wfopen_s(&fp, pszFilePath, L"wb") != 0 || !fp) return false;
    size_t written = fwrite(icoData.data(), 1, icoData.size(), fp);
    fclose(fp);

    return (written == icoData.size());
}
