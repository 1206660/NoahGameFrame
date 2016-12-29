// -------------------------------------------------------------------------
//    @FileName			:    NFCUIManager.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIManager
// -------------------------------------------------------------------------
#ifndef _NFCUIManager_H_
#define _NFCUIManager_H_

#include "NFCUIDialog.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

class NFCUIManager : public NFIModule, public NFSingleton<NFCUIManager>
{
public:
	NFCUIManager() {}
	~NFCUIManager() {}

    NFCUIManager(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

	// ---------  ------------
    virtual Node* RootNode();
    
    virtual NFCUIDialog* ShowDialog(NFCUIDialog *pDialog, bool bPushToHistory = true);
    virtual void CloseDialog(NFCUIDialog *pDialog);
	virtual void ChangeBackDialog();
	virtual bool HasBackDialog();
	virtual void CloseDialog();

public:
	void onBackKeyClicked();
	void onMenuKeyClicked();

private:
    std::vector<NFCUIDialog*> m_vecDialog;
	
    // UI�ĸ����c
	CC_SYNTHESIZE_READONLY(Node*, m_pRootNode, RootNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pPanelNode, PanelNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pDialogNode, DialogNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pBoxNode, BoxNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pEffectNode, EffectNode);

private:
    Vector<NFCUIDialog*> m_pDialogQueue;
	NFCUIDialog *m_pCurrentDialog;
};

#define g_pNFCUIManager (NFCUIManager::Instance())

// ��������߼��޹صĴ�UI�߼���
template<typename T>
class IUniqueDialog: public NFCUIDialog
{	
public:
	// ��ʾ����(����û����ʱ�������ڲ���ʾ���Ѿ�������ֻ��ʾ)
	static void showUI(bool bPushToHistory = true, const void *customData = nullptr) 
	{ 
		IUniqueDialog **p = _getDialog();
		if (!*p) {
			*p = new T;
			(*p)->initLayout();
			(*p)->autorelease();
		}
		
		(*p)->initData((void*)customData);
		g_pNFCUIManager->ShowDialog(*p, bPushToHistory);
	}
	static void closeUI(const void *customData = nullptr) 
	{ 
		IUniqueDialog **p = _getDialog();
		if (*p) {
			g_pNFCUIManager->CloseDialog(*p);
		}
	}
	IUniqueDialog() { // �������ⲿ�ֶ�new
		*_getDialog() = this;
	}
	virtual ~IUniqueDialog() { // �������ⲿ�ֶ�delete
		if(*_getDialog() == this)
			*_getDialog() = nullptr;
	}

private:
	static IUniqueDialog** _getDialog() {
		static T* _instance;
		return (IUniqueDialog**)(&_instance);
	}
};

#endif
