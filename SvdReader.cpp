// SNB_SvdReader.cpp: implementation of the SNB_SvdReader class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SvdReader.h"
#include "TreeModel.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SvdReader::SvdReader()
{
	// Konstruktor der Obeklasse aufrufen
	::BasicXmlSaxReader();
    m_pModel = NULL;
	m_modelInitialized = false;
}

SvdReader::~SvdReader()
{
	m_pModel = NULL;
}

void SvdReader::Parse(LPCSTR xmlFilename, TreeModel * pModel)
{
	m_filename = xmlFilename;

	m_pModel = pModel;

	// reader initialisieren
	Initialize();

	// parser starten
    ParseDocument(CA2W(xmlFilename));
  //  ParseDocument(xmlFilename);

}

HRESULT STDMETHODCALLTYPE SvdReader::startDocument()
{
	HRESULT hr = S_OK;
	Log::GetInstance()->LogMsg(_T("startDocument\n"));


	return hr;
}


HRESULT STDMETHODCALLTYPE SvdReader::endDocument()
{
	HRESULT hr = S_OK;
//	TRACE(_T("endDocument line =%d, col=%d\n"),LocateLineNumber(),LocateColNumber());

	Log::GetInstance()->LogMsg(_T("EndDocument\n"));

	return hr;
}

HRESULT STDMETHODCALLTYPE SvdReader::startElement(
									/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
									/* [in] */ int cchNamespaceUri,
									/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
									/* [in] */ int cchLocalName,
									/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
									/* [in] */ int cchRawName,
									/* [in] */ ISAXAttributes __RPC_FAR *pAttributes)
{
	HRESULT hr = S_OK;
//	ATLTRACE(_T("startElement line =%d, col=%d\n"),LocateLineNumber(),LocateColNumber());
     
	CString element = TranslateString(pwchLocalName,cchLocalName);
    m_pModel->AddElement(element);

    // XML Attribute lesen und als Property in dem Element speichern
    int length;
    pAttributes->getLength(&length);
    for ( int i=0; i<length; i++ ) 
    {
	    const wchar_t * wPropName, * wValueName; int wPropNamLen, wValueNamLen;
	    pAttributes->getLocalName(i,&wPropName,&wPropNamLen); 

	    CString csProperty = TranslateString(wPropName,wPropNamLen) ;

	    pAttributes->getValue(i,&wValueName,&wValueNamLen);
 
	    CString value = TranslateString(wValueName,wValueNamLen) ;

	    m_pModel->AddProperty(csProperty,value);
    }


	m_chars = "";
	return hr;
}

HRESULT STDMETHODCALLTYPE SvdReader::characters(
									/* [in] */ const wchar_t __RPC_FAR *pwchChars,
									/* [in] */ int cchChars)
{
	HRESULT hr = S_OK;
//	ATLTRACE(_T("characters line =%d, col=%d\n"),LocateLineNumber(),LocateColNumber());

	m_chars += TranslateString(pwchChars,cchChars);
	return hr;
}

HRESULT STDMETHODCALLTYPE SvdReader::endElement(
									/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
									/* [in] */ int cchNamespaceUri,
									/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
									/* [in] */ int cchLocalName,
									/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
									/* [in] */ int cchRawName)
{
	HRESULT hr = S_OK;
//	ATLTRACE(_T("endElement line =%d, col=%d\n"),LocateLineNumber(),LocateColNumber());

	m_pModel->AddData(m_chars);

	m_chars = "";
	m_pModel->CloseElement();

    
	return hr;
}
