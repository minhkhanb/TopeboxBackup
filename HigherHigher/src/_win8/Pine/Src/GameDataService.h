#pragma once
namespace PhoneDirect3DXamlAppComponent
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class GameDataService sealed{
	public:
		//UploadData: call when c# receives a Save Game Data request, he call this function to get data from c++
		//outarray: buffer to hold data
		//offset: offset of data to be load
		//return number of byte has been copy to outarray.
		int UploadData(Platform::WriteOnlyArray<uint8>^ outarray, int offset);

		//NotifyDownloadData: call when c# receives a Load game data request and he had been loaded data from server. 
		//he call this function to send data to c++
		//inarray: byte array hold data
		void NotifyDownLoadData(const Platform::Array<uint8>^ inarray);
	};
}