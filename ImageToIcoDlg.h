
// ImageToIcoDlg.h: 头文件
//

#pragma once
#include "ImagePreviewCtrl.h"

// CImageToIcoDlg 对话框
class CImageToIcoDlg : public CDialogEx
{
// 构造
public:
	CImageToIcoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGETOICO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CImagePreviewCtrl m_ImagePreview;
	// 添加圆角滑块变量
	CSliderCtrl m_SliderCtrl;
	void SetCheck();
	//下列是多选框控件变量
	CButton m_CheckRCorners;
	CButton m_8x8;
	CButton m_16x16;
	CButton m_32x32;
	CButton m_64x64;
	CButton m_128x128;
	CButton m_256x256;
	CButton m_512x512;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenImage();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckRCorners();

	CButton m_CeDelBackClore;
	afx_msg void OnBnClickedDelbackclore();
	afx_msg void OnBnClickedOuputico();
};
