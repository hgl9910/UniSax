// SNB_SvdReader.h: interface for the SNB_SvdReader class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BasicXmlSaxReader.h"

class TreeModel;

class SvdReader  : public BasicXmlSaxReader
{
public:

	SvdReader();
	virtual ~SvdReader();

	void Parse(LPCSTR xmlFilename, TreeModel * pModel);

protected:

	virtual HRESULT STDMETHODCALLTYPE startDocument();

	virtual HRESULT STDMETHODCALLTYPE endDocument();

	virtual HRESULT STDMETHODCALLTYPE startElement(
									/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
									/* [in] */ int cchNamespaceUri,
									/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
									/* [in] */ int cchLocalName,
									/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
									/* [in] */ int cchRawName,
									/* [in] */ ISAXAttributes __RPC_FAR *pAttributes);

	virtual HRESULT STDMETHODCALLTYPE characters(
									/* [in] */ const wchar_t __RPC_FAR *pwchChars,
									/* [in] */ int cchChars);

	virtual HRESULT STDMETHODCALLTYPE endElement(
									/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
									/* [in] */ int cchNamespaceUri,
									/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
									/* [in] */ int cchLocalName,
									/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
									/* [in] */ int cchRawName);


	CString m_filename;

	TreeModel         * m_pModel;
    CString             m_csCfg;

	CString m_chars;

	bool m_modelInitialized;
};
