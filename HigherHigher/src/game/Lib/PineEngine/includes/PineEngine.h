#if !defined(_H_PINE_ENGINE)
#define _H_PINE_ENGINE

#include "PineEngine/common.h"

#include "PineEngine/Math/CMatrix4.h"

#include "PineEngine/Base/CResult.h"
#include "PineEngine/Base/EResultCode.h"
#include "PineEngine/Base/CContainer.h"

//String
#include "PineEngine/String/CChar.h"
#include "PineEngine/String/CDataBinding.h"
#include "PineEngine/String/EEncoding.h"
#include "PineEngine/String/CString.h"
#include "PineEngine/String/CBase64.h"

#include "PineEngine/Debug/Interface/ILogger.h"
#include "PineEngine/Debug/Interface/ILogItem.h"
#include "PineEngine/Debug/Interface/ITaskLogLinkerItem.h"

#include "PineEngine/Debug/CNetworkLogItem.h"
#include "PineEngine/Debug/CNetworkLogWorker.h"
#include "PineEngine/Debug/CTaskNetworkLogLinkerItem.h"
#include "PineEngine/Debug/ENetworkLogItemState.h"

#include "PineEngine/Io/Interface/IBuffer.h"
#include "PineEngine/Io/Interface/IStream.h"

#include "PineEngine/Io/ESeekType.h"
#include "PineEngine/Io/EFileLocation.h"
#include "PineEngine/Io/EFileAttribute.h"


#include "PineEngine/Io/CBytes.h"
#include "PineEngine/Io/CBuffer.h"
#include "PineEngine/Io/CResourceStream.h"

#include "PineEngine/Io/CZip.h"
#include "PineEngine/Io/C7z.h"


//Network
#include "PineEngine/Network/Interface/ISocket.h"
#include "PineEngine/Network/CSocket.h"
#include "PineEngine/Network/CWebSocket.h"
#include "PineEngine/Network/CSignalR.h"

//cloud
#include "PineEngine/Cloud/CCloud.h"
#include "PineEngine/Cloud/CCloudKeyValueService.h"
#include "PineEngine/Cloud/ECloudProvider.h"
#include "PineEngine/Cloud/ECloudService.h"
#include "PineEngine/Cloud/ECloudKeyValueChangeReason.h"

//Platform
#include "PineEngine/Platform/Interface/IRectangle.h"
#include "PineEngine/Platform/Interface/IDeviceContext.h"
#include "PineEngine/Platform/Interface/IGraphicDevice.h"
#include "PineEngine/Platform/Interface/ICamera3D.h"

#include "PineEngine/Platform/EDeviceType.h"
#include "PineEngine/Platform/EBlend.h"
#include "PineEngine/Platform/EDepthFunction.h"
#include "PineEngine/Platform/EPrimitiveType.h"
#include "PineEngine/Platform/EDeviceType.h"

#include "PineEngine/Platform/CRectangle.h"
#include "PineEngine/Platform/CDeviceContext.h"
#include "PineEngine/Platform/CGraphicDevice.h"
#include "PineEngine/Platform/CApplication.h"
#include "PineEngine/Platform/CVertexBuffer.h"
#include "PineEngine/Platform/CEffect.h"
#include "PineEngine/Platform/CVertexPositionColor.h"
#include "PineEngine/Platform/CMesh.h"

#include "PineEngine/Lib3D/Interface/ICameraPerspective.h"
#include "PineEngine/Lib3D/CCameraPerspective.h"

#include "PineEngine/Os/OS.h"

#include "PineEngine/Resource/Interface/IResource.h"
#include "PineEngine/Resource/CResourceSegment.h"

#include "PineEngine/Security/Interface/IBasicAuthorize.h"
#include "PineEngine/Security/CBasicAuthorizeV1.h"

//service shell
#include "PineEngine/ServiceShell/CServiceShell.h"
#include "PineEngine/ServiceShell/CRequestStream.h"
#include "PineEngine/ServiceShell/CHttpRequest.h"
#include "PineEngine/ServiceShell/CCryptoRequest.h"
#include "PineEngine/ServiceShell/CBasicAuthorizeV1Request.h"

//social
#include "PineEngine/Social/Facebook/CApplication.h"
#include "PineEngine/Social/Facebook/CScoreItem.h"
#include "PineEngine/Social/Facebook/CSession.h"
#include "PineEngine/Social/Facebook/CFacebookException.h"
#include "PineEngine/Social/Facebook/v2_5/CGameService.h"

#define PE_USING_ENGINE \
using namespace Pine::Platform; \
using namespace Pine::Io; \
using namespace Pine::Math; \
using namespace Pine::String; \
using namespace Pine::Network; \
using namespace Pine::Cloud; \
using namespace Pine::Os; \
using namespace Pine::Resource; \
using namespace Pine::Schedule; \
using namespace Pine::Security; \
using namespace Pine::ServiceShell

//using namespace Pine::Platform; \

#endif