#include "cef_view.hpp"

#include <include/cef_parser.h>
#include <include/cef_task.h>
#include <renderer/utilities/cef/cef_texture.hpp>

renderer::utilities::cef::c_cef_view::c_cef_view(std::string url, bool bIsLocal, bool bTransparent)
{
	m_bIsLocal = bIsLocal;
	m_bIsTransparent = bTransparent;
	m_bBeingDestroyed = false;
	m_fVolume = 1.0f;

	m_sURL = url;
	//memset(m_mouseButtonStates, 0, sizeof(m_mouseButtonStates));

	// Initialise properties
	//    m_Properties["mobile"] = "0";
}

renderer::utilities::cef::c_cef_view::~c_cef_view()
{
	//log << __FUNCTION__ << std::endl;
	m_pWebView = nullptr;

	m_RenderData.cv.notify_all();
}

void renderer::utilities::cef::c_cef_view::Initialise()
{
	static auto renderer = c_renderer::instance();

	CreateTexture();

	CefBrowserSettings browserSettings;

	browserSettings.javascript_access_clipboard = cef_state_t::STATE_DISABLED;
	browserSettings.webgl = cef_state_t::STATE_ENABLED;
	browserSettings.javascript = cef_state_t::STATE_ENABLED;

	CefWindowInfo windowInfo;
	windowInfo.SetAsWindowless(renderer->window);

	CefBrowserHost::CreateBrowser(windowInfo, this, m_sURL, browserSettings, nullptr, CefRequestContext::GetGlobalContext());
}

void renderer::utilities::cef::c_cef_view::CreateTexture()
{
	static auto renderer = c_renderer::instance();

	RECT viewRect;
	GetClientRect(renderer->window, &viewRect);

	m_RenderData.width = viewRect.right;
	m_RenderData.height = viewRect.bottom;

	// Setup the description of the texture
	D3D11_TEXTURE2D_DESC m_TextureDesc;
	ZeroMemory(&m_TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	m_TextureDesc.Width = m_RenderData.width;
	m_TextureDesc.Height = m_RenderData.height;
	m_TextureDesc.MipLevels = 1;
	m_TextureDesc.ArraySize = 1;
	m_TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	m_TextureDesc.SampleDesc.Count = 1;
	m_TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE /*| D3D11_BIND_RENDER_TARGET*/;
	m_TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create the empty texture
	HRESULT res = renderer->d3d11_device->CreateTexture2D(&m_TextureDesc, nullptr, &m_pTexture);

	// Setup the shader resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC m_SrvDesc;
	m_SrvDesc.Format = m_TextureDesc.Format;
	m_SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_SrvDesc.Texture2D.MostDetailedMip = 0;
	m_SrvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	renderer->d3d11_device->CreateShaderResourceView(m_pTexture, &m_SrvDesc, &m_pTextureView);
}

void renderer::utilities::cef::c_cef_view::CheckResize(int width, int height)
{
	if (width == 0) return;
	if (m_RenderData.width != width || m_RenderData.height != height)
	{
		m_RenderData.changed = false;

		if (m_pTexture)
			m_pTexture->Release();

		if (m_pTextureView)
			m_pTextureView->Release();

		m_pTexture = NULL;
		m_pTextureView = NULL;

		CreateTexture();
		m_pWebView->GetHost()->WasResized();

		m_RenderData.cv.notify_all();
	}
}

void renderer::utilities::cef::c_cef_view::CloseBrowser()
{
	if (m_pWebView)
		m_pWebView->GetHost()->CloseBrowser(true);

	m_RenderData.cv.notify_all();
}

bool renderer::utilities::cef::c_cef_view::CanGoBack()
{
	if (!m_pWebView)
		return false;

	return m_pWebView->CanGoBack();
}

bool renderer::utilities::cef::c_cef_view::CanGoForward()
{
	if (!m_pWebView)
		return false;

	return m_pWebView->CanGoForward();
}

bool renderer::utilities::cef::c_cef_view::GoBack()
{
	if (!m_pWebView)
		return false;

	if (!m_pWebView->CanGoBack())
		return false;

	m_pWebView->GoBack();
	return true;
}

bool renderer::utilities::cef::c_cef_view::GoForward()
{
	if (!m_pWebView)
		return false;

	if (!m_pWebView->CanGoForward())
		return false;

	m_pWebView->GoForward();
	return true;
}

void renderer::utilities::cef::c_cef_view::Refresh(bool bIgnoreCache)
{
	if (!m_pWebView)
		return;

	if (bIgnoreCache)
	{
		m_pWebView->ReloadIgnoreCache();
	}
	else
	{
		m_pWebView->Reload();
	}
}

void renderer::utilities::cef::c_cef_view::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	if (m_bBeingDestroyed)
		return;

	rect.x = 0;
	rect.y = 0;
	rect.width = m_RenderData.width;
	rect.height = m_RenderData.height;
}

void renderer::utilities::cef::c_cef_view::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) {}
void renderer::utilities::cef::c_cef_view::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) {}

void renderer::utilities::cef::c_cef_view::OnPaint(CefRefPtr<CefBrowser> browser, CefRenderHandler::PaintElementType paintType, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height)
{
	if (paintType != PaintElementType::PET_VIEW)
		return;

	static auto texture = renderer::utilities::cef::c_cef_texture::instance();

	std::lock_guard<std::mutex> lock(texture->paintMutex);
	texture->m_drawData.push_back(std::make_unique<renderer::utilities::cef::draw_data_t>(width, height, (const unsigned*)buffer, dirtyRects));

	if (texture->m_drawData.size() > 60)
		texture->m_drawData.pop_front();
}

bool renderer::utilities::cef::c_cef_view::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, cef_cursor_type_t type, const CefCursorInfo& cursorInfo)
{
	return true;
}

void renderer::utilities::cef::c_cef_view::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transitionType) {}
void renderer::utilities::cef::c_cef_view::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {}
void renderer::utilities::cef::c_cef_view::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedURL) {}

bool renderer::utilities::cef::c_cef_view::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect)
{
	return false;
}

void renderer::utilities::cef::c_cef_view::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	m_pWebView = nullptr;
}

void renderer::utilities::cef::c_cef_view::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	m_pWebView = browser;
}

bool renderer::utilities::cef::c_cef_view::OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url, CefJSDialogHandler::JSDialogType dialog_type, const CefString& message_text, const CefString& default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
{
	suppress_message = true;
	return false;
}

bool renderer::utilities::cef::c_cef_view::OnFileDialog(CefRefPtr<CefBrowser> browser,
	FileDialogMode mode,
	const CefString& title,
	const CefString& default_file_path,
	const std::vector<CefString>& accept_filters,
	CefRefPtr<CefFileDialogCallback> callback)
{
	return true;
}

void renderer::utilities::cef::c_cef_view::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) {}

bool renderer::utilities::cef::c_cef_view::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& title)
{
	return true;
}

bool renderer::utilities::cef::c_cef_view::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line)
{
	return true;
}


void renderer::utilities::cef::c_cef_view::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	model->Clear();
}

