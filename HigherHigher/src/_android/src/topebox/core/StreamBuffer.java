//Pine-entertainment
//Android Framework
//duy.nguyen 2014

package topebox.core;
import 	android.util.Log;
public class StreamBuffer{
	protected byte[] _data = null;
	protected int _data_pointer = -1;
	public StreamBuffer(int size)
	{
		_data = new byte[size];
		_data_pointer = 0;
	}
	public StreamBuffer(byte[] data)
	{
		if(data != null){
			//Log.i("DEBUG", "[java] begin stream");
			_data = new byte[data.length];
			for(int i = 0; i< data.length; i++)
			{
				_data[i] = (byte)(data[i] & 0xFF);
				int b = data[i]&0xFF;
				//Log.i("DEBUG", "[java]" + b);
			}
			//Log.i("DEBUG", "[java] end stream\n");
			_data_pointer = 0;
		}else{
			_data = null;
			_data_pointer = 0;
		}
	}
	public byte[] Data()
	{
		return _data;
	}
	public void resetPos()
	{
		_data_pointer = 0;
	}
	
	//read byte at current position
	public byte readByte()
	{
		if(_data_pointer < _data.length)
		{
			return _data[_data_pointer++];
		}
		return 0;
	}
	
	//read 2 bytes at current position at short
	public short readInt16()
	{
		if(_data_pointer <= _data.length - 2){
			short value = 0;
			value = (short)(((_data[_data_pointer]<<8)&0xFF00) | (_data[_data_pointer + 1] & 0x00FF));
			_data_pointer += 2;
			return value;
		}
		return 0;
	}
	public short readInt16Revert()
	{
		if(_data_pointer <= _data.length - 2){
			short value = 0;
			value = (short)(((_data[_data_pointer + 1]<<8)&0xFF00) | (_data[_data_pointer] & 0x00Ff));
			_data_pointer += 2;
			return value;
		}
		return 0;
	}
	//read 4 bytes at current position at int
	public int readInt32()
	{	
		if(_data_pointer <= _data.length - 4){
			int value = 0;
			value = (
			((_data[_data_pointer]<<24) & 0xFF000000) 
			| ((_data[_data_pointer + 1]<<16) & 0x00FF0000) 
			|((_data[_data_pointer + 2]<<8)&0x0000FF00) | 
			(_data[_data_pointer + 3] & 0x000000FF));
			_data_pointer += 4;
			return value;
		}
		return 0;
	}
	public int readInt32Revert()
	{	
		if(_data_pointer <= _data.length - 4){
			int value = 0;
			value = (
			((_data[_data_pointer + 3]<<24) & 0xFF000000) 
			| ((_data[_data_pointer + 2]<<16)&0x00FF0000) 
			| ((_data[_data_pointer + 1]<<8) &0x0000FF00 ) | 
			(_data[_data_pointer]&0x000000FF));
			_data_pointer += 4;
			return value;
		}
		return 0;
	}
	
	//read 8 bytes at current position at long
	public long readInt64()
	{
		if(_data_pointer <= _data.length - 8){
			long value = 0;
			//value = (
			//	((_data[_data_pointer]<<56) & 0xFFFFFFFFFFFFFFFF) | ((_data[_data_pointer + 1]<<48)  & 0xFFFFFFFFFFFFFFFF) |((_data[_data_pointer + 2]<<40) & 0xFFFFFFFFFFFFFFFF) | ((_data[_data_pointer + 3] << 32) & 0xFFFFFFFFFFFFFFFF)
			//	| ((_data[_data_pointer + 4]<<24)  & 0xFFFFFFFFFFFFFFFF) | ((_data[_data_pointer + 5]<<16)   & 0xFFFFFFFFFFFFFFFF) |((_data[_data_pointer + 6]<<8)  & 0xFFFFFFFFFFFFFFFF) | ((_data[_data_pointer + 7])  & 0xFFFFFFFFFFFFFFFF)
			//);
			_data_pointer += 8;
			return value;
		}
		return 0;
	}
	
	public long readInt64Revert()
	{	
		//Log.i("DEBUG","_data_pointer:" + _data_pointer);
		if(_data_pointer <= _data.length - 8){
			long value = 0;
			
			long x1 =   ((_data[_data_pointer + 7] & 0xFF) << 56);
			long x2 =  ((_data[_data_pointer + 6] & 0xFF) << 48); 
			long x3 =  ((_data[_data_pointer + 5] & 0xFF) << 40);
			long x4 =  ((_data[_data_pointer + 4] & 0xFF) << 32);
			long x5 =  ((_data[_data_pointer + 3] & 0xFF) << 24);
			long x6 =  ((_data[_data_pointer + 2] & 0xFF) << 16);
			long x7 =  ((_data[_data_pointer + 1] & 0xFF) << 8 );
			long x8 =  ((_data[_data_pointer + 0] & 0xFF) << 0 );
		
			value = x1 | x2 | x3 | x4 | x5 | x6 | x7 | x8;
			
			/*
			int a = ((_data[_data_pointer + 3] << 24) & 0xFF000000);
			int b = ((_data[_data_pointer + 2] << 16) & 0x00FF0000);
			int c = ((_data[_data_pointer + 1] << 8 ) & 0x0000FF00);
			int d = ((_data[_data_pointer + 0] << 0 ) & 0x000000FF);
			
			Log.i("DEBUG","a:" + a);
			Log.i("DEBUG","b:" + b);
			Log.i("DEBUG","c:" + c);
			Log.i("DEBUG","d:" + d);
			
			*/
			_data_pointer += 8;
			//Log.i("DEBUG","i32:" + value);
			return value;
		}
		return 0;
	}
	
	public float readFloat()
	{
		return 0.0f;
	}
	
	//read a array of byte
	public byte[] readSBin()
	{
		short length = readInt16();
		if(length > 0 && _data_pointer <= _data.length - length){
			byte[] data = new byte[length];
			for(int i = 0; i< length; i++){
				data[i] = readByte();
			}
			return data;
		}
		return null;
	}
	public byte[] readSBinRevert()
	{
		short length = readInt16Revert();
		if(length > 0 && _data_pointer <= _data.length - length){
			byte[] data = new byte[length];
			for(int i = 0; i< length; i++){
				data[i] = readByte();
			}
			return data;
		}
		return null;
	}
	public byte[] readIBin()
	{
		int length = readInt32();
		
		if(length > 0 && _data_pointer <= _data.length - length){
			byte[] data = new byte[length];
			for(int i = 0; i< length; i++){
				data[i] = readByte();
			}
			return data;
		}
		return null;
	}
	public byte[] readIBinRevert()
	{
		int length = readInt32Revert();
		//Log.i("DEBUG", "readIBin len:" + length);
		if(length > 0 && _data_pointer <= _data.length - length){
			byte[] data = new byte[length];
			for(int i = 0; i< length; i++){
				data[i] = readByte();
				//Log.i("DEBUG","b"+i+"="+ data[i]);
			}
			return data;
		}else{
			Log.i("DEBUG", "readIbin: " + _data.length+ " - " + _data_pointer);
		}
		return null;
	}
	
	public String readSStr()
	{
		short length = readInt16();
		if(length > 0 && _data_pointer <= _data.length - length){
			String str = "";
			for(int i = 0; i< length; i++){
				short i16data = (short)(readInt16() & 0xFFFF);
				str += (char) i16data;
			}
			return str;
		}
		return "";
	}
	
	public String readSStrRevert()
	{
		//Log.i("DEBUG","begin read str revert");
		short length = readInt16Revert();
		if(length > 0 && _data_pointer <= _data.length - length){
			String str = "";
			for(int i = 0; i< length; i++){
				short i16data = (short)(readInt16Revert() & 0xFFFF);
				//Log.i("DEBUG",String.valueOf(i16data));
				str += (char) i16data;
			}
			//Log.i("DEBUG","end read str revert\n");
			return str;
		}
		//Log.i("DEBUG","end read str revert\n");
		return "";
	}
	
	public String readIStr()
	{
		int length = readInt32();
		if(length > 0 && _data_pointer <= _data.length - length){
			String str = "";
			for(int i = 0; i< length; i++){
				short i16data = (short)(readInt16() & 0xFFFF);
				str += (char) i16data;
			}
			return str;
		}
		return "";
	}
	
	public String readIStrRevert()
	{
		int length = readInt32Revert();
		if(length > 0 && _data_pointer <= _data.length - length){
			String str = "";
			for(int i = 0; i< length; i++){
				short i16data = (short)(readInt16Revert() & 0xFFFF);
				str += (char) i16data;
			}
			return str;
		}
		return "";
	}
	
	public void writeByte(byte value)
	{
		if(_data_pointer < _data.length){
			_data[_data_pointer] = value;
			_data_pointer++;
		}
	}
	public void writeInt16(short value)
	{
		writeByte((byte)((value>>8) & 0xFF));
		writeByte((byte)(value & 0xFF));
	}
	public void writeInt16Revert(short value)
	{
		writeByte((byte)(value & 0xFF));
		writeByte((byte)((value>>8) & 0xFF));
	}
	public void writeInt32(int value)
	{
		writeByte((byte)((value>>24) & 0xFF));
		writeByte((byte)((value>>16) & 0xFF));
		writeByte((byte)((value>>8) & 0xFF));
		writeByte((byte)(value & 0xFF));
	}
	public void writeInt32Revert(int value)
	{
		writeByte((byte)(value & 0xFF));
		writeByte((byte)((value>>8) & 0xFF));
		writeByte((byte)((value>>16) & 0xFF));
		writeByte((byte)((value>>24) & 0xFF));
	}
	public void writeInt64(long value)
	{
		writeByte((byte)((value>>56) & 0xFF));
		writeByte((byte)((value>>48) & 0xFF));
		writeByte((byte)((value>>40) & 0xFF));
		writeByte((byte)((value>>32) & 0xFF));
		writeByte((byte)((value>>24) & 0xFF));
		writeByte((byte)((value>>16) & 0xFF));
		writeByte((byte)((value>>8) & 0xFF));
		writeByte((byte)(value & 0xFF));
	}
	public void writeInt64Revert(long value)
	{
		writeByte((byte)(value & 0xFF));
		writeByte((byte)((value>>8) & 0xFF));
		writeByte((byte)((value>>16) & 0xFF));
		writeByte((byte)((value>>24) & 0xFF));
		writeByte((byte)((value>>32) & 0xFF));
		writeByte((byte)((value>>40) & 0xFF));
		writeByte((byte)((value>>48) & 0xFF));
		writeByte((byte)((value>>56) & 0xFF));
	}
	public void writeFloat(float value)
	{
	}
	public void writeSBin(byte[] values)
	{
		short len = values!= null? (short)values.length: 0;
		writeInt16(len);
		for(int i = 0; i< len; i++){
			writeByte(values[i]);
		}
	}
	public void writeSBinRevert(byte[] values)
	{
		short len = values!= null? (short)values.length: 0;
		writeInt16Revert(len);
		for(int i = 0; i< len; i++){
			writeByte(values[i]);
		}
	}
	
	public void writeIBin(byte[] values)
	{
		int len = values != null? values.length:0;
		writeInt32(len);
		for(int i = 0; i< len; i++){
			writeByte(values[i]);
		}
	}
	
	public void writeIBinRevert(byte[] values)
	{
		int len = values != null? values.length:0;
		//Log.i("DEBUG STREAM", "write ibin:" + len);
		writeInt32Revert(len);
		for(int i = 0; i< len; i++){
			writeByte(values[i]);
		}
	}
	public void writeSStr(String str)
	{
		short len = str != null? (short)str.length(): 0;
		//Log.i("DEBUG STREAM", "write len:" +len +" str:" + str);
		writeInt16(len);
		for(int i = 0; i< len; i++){
			writeInt16((short)str.charAt(i));
		}
	}
	public void writeSStrRevert(String str)
	{
		short len = str != null? (short)str.length(): 0;
		writeInt16Revert(len);
		for(int i = 0; i< len; i++){
			writeInt16Revert((short)str.charAt(i));
		}
	}
	
	public void writeIStr(String str)
	{
		int len = str != null ? str.length() : 0;
		writeInt32(len);
		for(int i = 0; i< len; i++){
			writeInt16((short)str.charAt(i));
		}
	}
	public void writeIStrRevert(String str)
	{
		int len = str != null ? str.length() : 0;
		writeInt32Revert(len);
		for(int i = 0; i< len; i++){
			writeInt16Revert((short)str.charAt(i));
		}
	}
}