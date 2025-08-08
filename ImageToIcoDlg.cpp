
// ImageToIcoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ImageToIco.h"
#include "ImageToIcoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageToIcoDlg 对话框



CImageToIcoDlg::CImageToIcoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGETOICO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageToIcoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl); // 绑定滑块控件
	DDX_Control(pDX, PC_SHOW_IMAGE, m_ImagePreview);//绑定图像预览控件
	DDX_Control(pDX, CE_RCORNERS, m_CheckRCorners);
	DDX_Control(pDX, CE_8X8, m_8x8);
	DDX_Control(pDX, CE_16X16, m_16x16);
	DDX_Control(pDX, CE_32X32, m_32x32);
	DDX_Control(pDX, CE_64X64, m_64x64);
	DDX_Control(pDX, CE_128X128, m_128x128);
	DDX_Control(pDX, CE256X256, m_256x256);
	DDX_Control(pDX, CE_512X512, m_512x512);
	DDX_Control(pDX, CE_DELBACKCLORE, m_CeDelBackClore);
}

void CImageToIcoDlg::SetCheck()
{
	// 获取图像的大小
	CRect rect = m_ImagePreview.GetImageSize();
	//设置滑块范围为图像高度的一半
	m_SliderCtrl.SetRange(0, rect.Height() / 2);
	//m_ImagePreview.SetRCornersValue(rect.Height() / 2);

	m_8x8.SetCheck(BST_UNCHECKED);
	m_16x16.SetCheck(BST_UNCHECKED);
	m_32x32.SetCheck(BST_UNCHECKED);
	m_64x64.SetCheck(BST_UNCHECKED);
	m_128x128.SetCheck(BST_UNCHECKED);
	m_256x256.SetCheck(BST_UNCHECKED);
	m_512x512.SetCheck(BST_UNCHECKED);


	if (rect.Height() >= 512) {
		m_8x8.SetCheck(BST_CHECKED);
		m_16x16.SetCheck(BST_CHECKED);
		m_32x32.SetCheck(BST_CHECKED);
		m_64x64.SetCheck(BST_CHECKED);
		m_128x128.SetCheck(BST_CHECKED);
		m_256x256.SetCheck(BST_CHECKED);
		m_512x512.SetCheck(BST_CHECKED);
		return;
	}
	if ((rect.Height() < 512) && (rect.Height() >=256)) {
		m_8x8.SetCheck(BST_CHECKED);
		m_16x16.SetCheck(BST_CHECKED);
		m_32x32.SetCheck(BST_CHECKED);
		m_64x64.SetCheck(BST_CHECKED);
		m_128x128.SetCheck(BST_CHECKED);
		m_256x256.SetCheck(BST_CHECKED);
		m_512x512.SetCheck(BST_UNCHECKED);
		return;
	}
	if ((rect.Height() < 256) && (rect.Height() >=128))
	{
	m_8x8.SetCheck(BST_CHECKED);
	m_16x16.SetCheck(BST_CHECKED);
	m_32x32.SetCheck(BST_CHECKED);
	m_64x64.SetCheck(BST_CHECKED);
	m_128x128.SetCheck(BST_CHECKED);
	m_256x256.SetCheck(BST_UNCHECKED);
	m_512x512.SetCheck(BST_UNCHECKED);
	return;
}
	if ((rect.Height() < 128) && (rect.Height() >= 64))
	{
		m_8x8.SetCheck(BST_CHECKED);
		m_16x16.SetCheck(BST_CHECKED);
		m_32x32.SetCheck(BST_CHECKED);
		m_64x64.SetCheck(BST_CHECKED);
		m_128x128.SetCheck(BST_UNCHECKED);
		m_256x256.SetCheck(BST_UNCHECKED);
		m_512x512.SetCheck(BST_UNCHECKED);
		return;
	}

	if ((rect.Height() < 32) && (rect.Height() >= 16))
	{
		m_8x8.SetCheck(BST_CHECKED);
		m_16x16.SetCheck(BST_CHECKED);
		m_32x32.SetCheck(BST_UNCHECKED);
		m_64x64.SetCheck(BST_UNCHECKED);
		m_128x128.SetCheck(BST_UNCHECKED);
		m_256x256.SetCheck(BST_UNCHECKED);
		m_512x512.SetCheck(BST_UNCHECKED);
		return;
	}
	
	if ((rect.Height() < 16 )&& (rect.Height() >= 8))
	{
		m_8x8.SetCheck(BST_CHECKED);
		m_16x16.SetCheck(BST_UNCHECKED);
		m_32x32.SetCheck(BST_UNCHECKED);
		m_64x64.SetCheck(BST_UNCHECKED);
		m_128x128.SetCheck(BST_UNCHECKED);
		m_256x256.SetCheck(BST_UNCHECKED);
		m_512x512.SetCheck(BST_UNCHECKED);
		return;
	}
	MessageBox(L"位图太小不能制作ICO");
	return; // 如果图像高度小于8，则不需要设置任何复选框
}

BEGIN_MESSAGE_MAP(CImageToIcoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_OPEN_IMAGE, &CImageToIcoDlg::OnBnClickedOpenImage)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(CE_RCORNERS, &CImageToIcoDlg::OnCheckRCorners)
	ON_BN_CLICKED(CE_DELBACKCLORE, &CImageToIcoDlg::OnBnClickedDelbackclore)
	ON_BN_CLICKED(BTN_OUPUTICO, &CImageToIcoDlg::OnBnClickedOuputico)
END_MESSAGE_MAP()


// CImageToIcoDlg 消息处理程序

BOOL CImageToIcoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_SliderCtrl.SetRange(0, 255);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImageToIcoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageToIcoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageToIcoDlg::OnBnClickedOpenImage()
{
	// 打开一个图像文件对话框，允许用户选择一个图像文件
	CFileDialog dlg(
		TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		L"图像文件 (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|所有文件 (*.*)|*.*||",
		this
	);
	
	//点击“打开”按钮
	if (dlg.DoModal() == IDOK) {
		m_ImagePreview.OpenImageFile(dlg.GetPathName());
	}
	if (m_CeDelBackClore.GetCheck() == BST_CHECKED) {
		m_ImagePreview.DelBitMapBackColor();
	}
	
	
	SetCheck(); // 设置复选框状态
	OnBnClickedDelbackclore();
	OnCheckRCorners();
}

void CImageToIcoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int value = m_SliderCtrl.GetPos();
	BOOL bChecked = m_CheckRCorners.GetCheck() == BST_CHECKED;
	if (bChecked)
		m_ImagePreview.SetRCornersValue(value);
	else
		m_ImagePreview.SetRCornersValue(0);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageToIcoDlg::OnCheckRCorners()
{
	BOOL bChecked = m_CheckRCorners.GetCheck() == BST_CHECKED;
	int value = m_SliderCtrl.GetPos();
	if (bChecked)
	{
		m_ImagePreview.SetRCornersValue(value); // 启用圆角
	}
	else
	{
		m_ImagePreview.SetRCornersValue(1); // 关闭圆角
	}
}
void CImageToIcoDlg::OnBnClickedDelbackclore()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_CeDelBackClore.GetCheck() == BST_CHECKED)
	{
		m_ImagePreview.DelBitMapBackColor();
		m_ImagePreview.SetDelBackColor(); // 启用删除背景颜色
	}
	else
	{
		m_ImagePreview.SetDelBackColor(FALSE);  // 禁用删除背景颜色
		m_ImagePreview.SetRCornersValue(1); // 关闭圆角
	}
}

void CImageToIcoDlg::OnBnClickedOuputico()
{
	// 1. 弹出另存为对话框
	CFileDialog dlg(
		FALSE, // FALSE 表示保存
		L"ico",
		NULL,
		OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		L"ICO文件 (*.ico)|*.ico|所有文件 (*.*)|*.*||",
		this
	);
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();

	// 2. 获取各分辨率多选框状态
	bool b8x8 = m_8x8.GetCheck() == BST_CHECKED;
	bool b16x16 = m_16x16.GetCheck() == BST_CHECKED;
	bool b32x32 = m_32x32.GetCheck() == BST_CHECKED;
	bool b64x64 = m_64x64.GetCheck() == BST_CHECKED;
	bool b128x128 = m_128x128.GetCheck() == BST_CHECKED;
	bool b256x256 = m_256x256.GetCheck() == BST_CHECKED;
	bool b512x512 = m_512x512.GetCheck() == BST_CHECKED;

	// 3. 调用预览控件的 SaveToIcoFile
	bool result = m_ImagePreview.SaveToIcoFile(
		path,
		b8x8, b16x16, b32x32, b64x64,
		b128x128, b256x256, b512x512
	);

	// 4. 提示保存结果
	if (result)
		MessageBox(L"ICO文件保存成功！", L"提示", MB_ICONINFORMATION);
	else
		MessageBox(L"ICO文件保存失败！", L"错误", MB_ICONERROR);
}
