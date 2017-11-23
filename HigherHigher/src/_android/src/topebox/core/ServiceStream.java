//Pine-entertainment
//Android Framework
//duy.nguyen 2014

package topebox.core;
import 	android.util.Log;
public class ServiceStream{
	static final int CurrentVersion = 0;
	public int Id;
	public int ServiceCode;
	public int Status;
	public int ErrorCode;
	public byte[] RequestData;
	public byte[] ResponseData;
	public ServiceStream(byte[] buff){
		StreamBuffer sbuff = new StreamBuffer(buff);
		int version = sbuff.readByte();
		if(version == 0){
			//Log.i("DEBUG","version:" + version);
			initVersion0(sbuff);
		}else{
			//Log.i("DEBUG","unknown version:" + version);
		}
	}
	public ServiceStream(int svc_code){
		Id = 0;
		ServiceCode = svc_code;
	}
	public byte[] getBinary(){
		//version 0
		if(CurrentVersion == 0){
			int buffsize = 1 + 4 + 4 +4 + 4+ (4 + (RequestData != null? RequestData.length: 0)) + (4 + (ResponseData != null? ResponseData.length: 0));
			StreamBuffer buff = new StreamBuffer(buffsize);
			buff.writeByte((byte)0);
			buff.writeInt32Revert(Id);
			buff.writeInt32Revert(ServiceCode);
			buff.writeInt32Revert(Status);
			buff.writeInt32Revert(ErrorCode);
			buff.writeIBinRevert(RequestData);
			buff.writeIBinRevert(ResponseData);
			return buff.Data();
		}
		return null;
	}
	//implement init by version
	protected void initVersion0(StreamBuffer buff){
		Id = buff.readInt32Revert();
		//Log.i("DEBUG","Id:" + Id);
		ServiceCode = buff.readInt32Revert();
		//Log.i("DEBUG","ServerCode:" + ServiceCode);
		Status = buff.readInt32Revert();
		ErrorCode = buff.readInt32Revert();
		RequestData = buff.readIBinRevert();
		if(RequestData != null){
			//Log.i("Debug","Request data len:" + RequestData.length);
		}
		ResponseData = buff.readIBinRevert();
		//Log.i("DEBUG","ErrorCode:" + ErrorCode);
	}
}