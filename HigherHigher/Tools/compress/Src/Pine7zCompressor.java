import java.nio.channels.FileChannel;
import java.nio.ByteBuffer;
import java.io.*;
import java.util.*;
import java.lang.*;
import jxl.*;

class mInt
{
	int value;
};
public class Pine7zCompressor
{
	final static String k_sVersion		= "1.0.0";
	

	public static char GetMaskChar(char ch)
	{
		if(ch == '’')
		{
			ch = '\'';
		}
		if(ch == '“' || ch =='”')
		{
			ch = '"';
		}
		
		if(ch >= '!' && ch <= '~')
		
		{
			return ch;
		}
		return ' ';
	}
	public static void PrintLicence(PrintStream out)
	{
			out.println("//pine compressor");
			out.println("//Programmer: Do Thanh Nghia");
	}
	
	static String GDS_Exporter_h;
	static String GDS_Exporter_cpp;
	final static String TOKEN_ADD_CODE = "//The code generate will auto appends.";
	
	static void Export(Sheet sheet,String name,String type,int[] rows, int[] cols)
	{
		//System.out.println("\nbegin export sheet name" + name);
		int row_num = rows[1] - rows[0] + 1;
		int col_num = cols.length;
		
		String _buff_h = "\n//define atriubte \n";
		
		_buff_h += "#define TOTAL_"+name.trim().toUpperCase() +"\t(" + col_num + ")\n";
		_buff_h += "#define TOTAL_"+name.trim().toUpperCase() +"_ROW\t(" + row_num + ")\n\n";
		for(int i = 0; i < col_num;i++)
		{
			if(rows[0] - 2 >= 0)
			{
				Cell cell[] =  sheet.getRow(rows[0] - 2);
				String value = MaskString(cell[cols[i]].getContents().trim());
				value = value.replace(" ","_");
				value = value.replace("-","_");
				value = value.replace(".","_");
				value = value.replace("(","_");
				value = value.replace(")","_");
				value = value.replace("&","_AND_");
				value = value.replace("|","_OR_");
				value = value.replace("/","_OR_");
				value = value.replace("\\","_OR_");
				value = value.toUpperCase();
				
				_buff_h += "#define "+name.trim().toUpperCase()+ "_" +value +"\t(" + i + ")\n";
			}
		
		}
		_buff_h += "//end define";
			
		String strType = type;
		if(type.equals("spriteID"))
		{
			strType = "int";
		}
		String _buff_cpp = "const " + strType + " Compressor_Exporter::" + name.toLowerCase() +"[" + (row_num*col_num) + "] = \n{\n";
		//System.out.println("_buff_h  = "+_buff_h);
		String _buff_batch = "";
		_buff_batch +=  "@echo off \r\n";
		for(int i = 0; i < row_num; i++)
		{
			Cell cell[] =  sheet.getRow(rows[0]-1+i);
			_buff_batch += "copy\t" +  "\"" + MaskString(cell[cols[1]].getContents()) + "\""+ "\t " + MaskString(cell[cols[2]].getContents())+ "\r\n";
			_buff_batch += "7z.exe a -mx=9 " + MaskString(cell[cols[3]].getContents()) + " " + MaskString(cell[cols[2]].getContents()) + "\r\n";
			_buff_batch += "del\t " + MaskString(cell[cols[2]].getContents())+ "\r\n";
		}
		try
		{
			String filename = "compressor.bat";
			FileOutputStream os = new FileOutputStream(filename);
			PrintStream out = new PrintStream(os);
			out.println(_buff_batch/*.replace("%","")*/);
			os.close();
			out.close();
		}
		catch(Exception e)
		{
		}
			
		for(int i = 0; i < row_num; i++)
		{
			try
			{
				Cell cell[] =  sheet.getRow(rows[0]-1+i);
				for(int j = 0; j < col_num; j++)
				{
					if(type.equals("double")|| type.equals("long"))
					{
						_buff_cpp += ("\t\t" + MaskString(cell[cols[j]].getContents()) + ",");
					}
					else if(type.equals("float"))
					{
						_buff_cpp += ("\t\t" + getFloatVal(cell[cols[j]].getContents()) + ",");
					}
					else if(type.equals("int"))
					{
						_buff_cpp += ("\t\t" + getIntVal(cell[cols[j]].getContents()) + ",");
					}
					else if(type.equals("INT_TIME") )
					{
						_buff_cpp += ("\t\t" + getLongVal(cell[cols[j]].getContents()) + ",");
					}
					else if(type.equals("spriteID") )
					{
						_buff_cpp += ("\t\t" + MaskString(cell[cols[j]].getContents()) + ",");
					}
					else
					{
						_buff_cpp += ("\t\t\"" + MaskString(cell[cols[j]].getContents()) + "\",");
					}
				}
				_buff_cpp += "\n";
			}
			catch(Exception e)
			{
			}
				
		}
		_buff_h += "\nstatic const " + strType + " " + name.toLowerCase() +"[" + (row_num*col_num) + "];"; 
		_buff_cpp += "\n};";
		
		GDS_Exporter_h = InsertStringAtToken(GDS_Exporter_h,_buff_h,TOKEN_ADD_CODE);
		GDS_Exporter_cpp = InsertStringAtToken(GDS_Exporter_cpp,_buff_cpp,TOKEN_ADD_CODE);
		//System.out.println(""+GDS_Exporter_cpp);
	}
	
	public static void main (String[] args)
	{
		try
		{
			System.out.println("\nnum file = " + args.length);
			
			
			
			File config = new File(args[0]);
			File ex_h = new File(args[1]);
			File ex_cpp = new File(args[2]);
			
			File f1 = new File(args[3]);
			File f2 = new File(args[4]);
			/*File f3 = new File(args[5]);
			File f4 = new File(args[6]);
			File f5 = new File(args[7]);
			File f6 = new File(args[8]);
			File f7 = new File(args[9]);*/
			
			GDS_Exporter_h = new String(read(ex_h));
			GDS_Exporter_cpp = new String(read(ex_cpp));
			
			//GDS_Exporter_h = InsertStringAtToken(GDS_Exporter_h,"test",TOKEN_ADD_CODE);
			//GDS_Exporter_h = InsertStringAtToken(GDS_Exporter_h,"test",TOKEN_ADD_CODE);
			//System.out.println("\nGDS_Exporter_h = " + GDS_Exporter_h);
			System.out.println("***************************************");
			System.out.println("Data Manifesh exporter begin\n");
			System.out.println("Processing file: " + args[0]);
			String[] str_file_index;
			String[] str_sheet_index;
			String[] str_struct;
			String[] str_type;
			String[] str_name;
			String[] str_row;
			String[] str_col;
			String[] str_header;
			String[] str_line_col;
			
			
			Workbook workbook = Workbook.getWorkbook(f1);
			
			Workbook workbook_1 = Workbook.getWorkbook(f1);
			Workbook workbook_2 = Workbook.getWorkbook(f2);
			
			if(config != null)
			{
				byte[] _config = read(config);
				mInt index = new mInt();
				index.value = 0;
				
				while(index.value < _config.length-1)
				{
					byte[] _command = read(_config,index,_config.length-1,"+export","-end_export");
					if(_command == null)
					{
						break;
					}
					
					str_file_index = readParamContents(_command," -file:");
					str_sheet_index = readParamContents(_command," -sheet:");
					str_struct = readParamContents(_command," -struct:");
					str_type = readParamContents(_command," -type:");
					str_name = readParamContents(_command," -name:");
					str_row =readParamContents(_command," -row:");
					str_col =readParamContents(_command," -col:");
					str_line_col = readParamContents(_command," -col_line:");
			
					try
					{	
						
						
						int sheet_id = getIntVal(str_sheet_index[0]);
						String struck = str_struct[0];//getFloatVal
						String type =  str_type[0];
						String name = str_name[0];
						int[] row = new int[str_row.length];
						for(int i = 0; i < str_row.length;i++)
						{
							row[i] = getIntVal(str_row[i]);
						}
						int[] col = null;//
						if(str_line_col != null)
						{
							int num_col = getIntVal(str_line_col[1]);
							int begin = 0;
							for(int k = 0; k < str_line_col[0].length(); k++)
							{
								begin =begin*(int)('z'-'a') + (int)(str_line_col[0].charAt(k)-'a');
							}
							col = new int[num_col];
							for(int i = 0; i < num_col; i++)
							{
								col[i] = begin + i;
							}
						}
						else
						{
							col = new int[str_col.length];
							for(int i = 0; i < str_col.length; i++)
							{
								col[i] = 0;
								for(int k = 0; k < str_col[i].length(); k++)
								{
									col[i] =col[i]*(int)('z'-'a') + (int)(str_col[i].charAt(k)-'a');
								}
							}
						}
						if(struck.equals("data"))
						{
							int file_index = getIntVal(str_file_index[0]);						
							if(file_index == 1)
							{								
								Sheet[] sheets = workbook_1.getSheets();
								Export(sheets[sheet_id],name,type,row,col);
							}
							else if(file_index == 2)
							{
								Sheet[] sheets = workbook_2.getSheets();
								Export(sheets[sheet_id],name,type,row,col);
							}
							
						}
					}
					catch(Exception e)
					{
						
					}
				}
			}
			
			String filename = "Compressor_Exporter.cpp";
			FileOutputStream os = new FileOutputStream(filename);
			PrintStream out = new PrintStream(os);
			out.println(GDS_Exporter_cpp/*.replace("%","")*/);
			os.close();
			out.close();
			
			filename = "Compressor_Exporter.h";
			os = new FileOutputStream(filename);
			out = new PrintStream(os);
			out.println(GDS_Exporter_h/*.replace("%","PERCENT")*/);
			os.close();
			out.close();

			
			
			
			System.out.println("\nCompressor Manifesh exporter End");
			System.out.println("***************************************");
			
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		
	}
	
	public static void exportDesginTool(Sheet sheet)
	{
		
	}
	
	static String InsertStringAtToken(String str,String str1, String token)
	{
		return InsertStringAtToken(str,str1,token,"generate code auto");
	}
	static String InsertStringAtToken(String str,String str1, String token,String Comment)
	{
		for(int i = 0; i < str.length(); i++)
		{
			if(str.substring(i,i+token.length()).equals(token))
			{
				String str2 = "\n//@[ginha.do]\n" + "//Note:" + Comment + "\n" + str1;
				String str3 = str.substring(0,i+token.length());
				String str4 = str.substring(i+token.length());
				return (str3 + str2 + str4);
				
			}
		}
		return str;
	}
	static String[] readParamContents(byte[] command,String token)
	{
		String str = "";
		if(token==" -type:")
		{
			str = (new String(command));
		}
		else
		{
			str = (new String(command).toLowerCase());
		}
		
		int index1=0;
		int index2=0;
		for(int i = 0 ; i < str.length()- token.length(); i++)
		{
			if(str.substring(i,i+token.length()).equals(token))
			{
				for(int j = i+token.length(); j < str.length(); j++)
				{
					if(str.charAt(j) == '[')
					{
						index1 = j;
					}
					if(str.charAt(j) == ']')
					{
						index2 = j;
						break;
					}
				}
				break;
			}
		}
		if(index1 != 0 && index2 != 0 && index1 != index2)
		{
			try
			{
				return (str.substring(index1+1,index2).trim()).split(",");
			}
			catch(Exception e)
			{
			}
		}
		return null;
	}
	static public byte[] read(byte[] content,mInt begin,int end,String token_begin, String token_end)
	{
	
		String str = (new String(content)).substring(begin.value);
		int index1 = 0;
		int index2 = 0;
		//System.out.println("str\n" + str + "str.substring(i,token_begin.length()) = " + str.substring(101,101+token_begin.length()) + " " + token_begin.length());
		
		for(int i = 0; i < str.length()-token_begin.length(); i++)
		{
			if(str.substring(i,i+token_begin.length()).equals(token_begin))
			{
				for(int j = i+token_begin.length(); j < str.length()-token_end.length(); j++)
				{
					if(str.substring(j,j+token_end.length()).equals(token_end))
					{
						index2 = i+token_begin.length();
						begin.value += index2;
						//System.out.println("index2 " + index2 + "j = " + j);
						//System.out.println("string: " +  str.substring(i+token_begin.length(),j));
						try
						{
							return str.substring(i+token_begin.length(),j).getBytes("UTF-8");
						}
						catch(Exception e)
						{
						}
							
					}
				}
				break;
			}
		}
		//System.out.println("index1 " +  index1);
		
	
		return null;
	}
	static public byte[] read(File file) throws IOException 
	{
		byte []buffer = new byte[(int) file.length()];
		InputStream ios = null;
		try 
		{
			ios = new FileInputStream(file);
			if ( ios.read(buffer) == -1 )
			{
				throw new IOException("EOF reached while trying to read the whole file");
			} 
			
		} 
		finally 
		{ 
			try 
			{
				if ( ios != null ) 
					ios.close();
			} catch ( IOException e) 
			{
			}
    }

    return buffer;
}


private static void WriteString(FileOutputStream os, String str)
	{
		for(int i = 0; i < str.length(); i++)
		{
			try
			{
				char ch = GetMaskChar(str.charAt(i));
				os.write((ch)&0xFF);
			}
			catch(Exception e)
			{
			System.out.printf("Error e"+e);
			}
				
		}
	}
	private static void WriteInt(FileOutputStream os, int i)
	{
		
		try
		{
			os.write((i>>24)&0xFF);
			os.write((i>>16)&0xFF);
			os.write((i>>8)&0xFF);
			os.write(i&0xFF);
		}
		catch (Exception ex)
		{
			
		}
	}
	private static void WriteFloat(FileOutputStream os, float f)
	{
		int i = Float.floatToIntBits(f);
		
		try
		{
			os.write((i>>24)&0xFF);
			os.write((i>>16)&0xFF);
			os.write((i>>8)&0xFF);
			os.write(i&0xFF);
		}
		catch (Exception ex)
		{
			
		}
	}
	static String MaskString(String strval)
	{
		return MaskString(strval,0);
	}
	static String MaskString(String strval,int value)
	{
		if(strval == null || strval == "" || strval.length() <= 0)
			return ""+value;
		return strval;
	}
	static int getIntVal(String strval)
	{
		if(strval == null || strval == "")
			return 0;
		strval = strval.trim().replace(",", "");
		return Integer.parseInt(strval);
	}
	static double getFloatVal(String strval)
	{
		if(strval == null || strval == "")
			return 0;
		strval = strval.trim().replace(",", "");
		return Double.parseDouble(strval);
	}
	static double getLongVal(String strval)
	{
		if(strval == null || strval == "")
			return 0;
		strval = strval.trim().replace(",", "");
		return Long.parseLong(strval);
	}
	static void Log (String s)
	{
		System.out.println(s);
	}
	
	static byte[] getBytes(String str)
	{
		try
		{
			return str.getBytes("UTF-8");
		}
		catch(Exception e)
		{
		}
		return str.getBytes();
	}
	
	public static String trimCharacter(String str,char ch)
	{
		int length = str.length();
		for(int i = 0; i < length - 1; i++)
		{
			if(str.charAt(i) == ch && str.charAt(i+1) == ch)
			{
				str = str.replace(ch+""+ch,""+ch);
				i--;
				
			}
			length = str.length();
		}
		return str;
	}
}
