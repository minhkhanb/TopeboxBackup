import java.nio.channels.FileChannel;
import java.nio.ByteBuffer;
import java.io.*;
import java.util.*;
import java.lang.*;
import jxl.*;


public class ExportPineGamePromotion
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
			out.println("//Design exporter");
			out.println("//Programmer: Do Thanh Nghia");
	}
	public static void main (String[] args)
	{
		try
		{
			File f = new File(args[0]);
		
			Workbook workbook = Workbook.getWorkbook(f);
			
			Sheet[] sheets = workbook.getSheets();
		
			exportData(sheets[0]);
			
			
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		
	}
	public static void exportData(Sheet sheet)
	{
		try
		{
			String file_name = "pinegame_promotion_info.pine";
			String apple_list_file_name = "apple_list.pine";
			
			FileOutputStream os_list = new FileOutputStream(apple_list_file_name);
			PrintStream out_list = new PrintStream(os_list);
			
			FileOutputStream os = new FileOutputStream(file_name);
			//write version of document
			Cell cell[] =  sheet.getRow(1);
			String Version = MaskString(cell[1].getContents());
			System.out.println("Version = "+Version);
			WriteInt(os,Integer.parseInt(Version));
			
			/* game instant struct
			num of game: int 4 byte
			Game Name: String
			Apple ID: int 4byte
			Decscription: String
			*/
			
			int  num_of_game =0;
			String game_name= "0";
			int apple_id=0;
			int rate = 0;
			int num_screen = 0;
			String game_decs="0";
			//num of instant game.
			cell = sheet.getRow(3);
			num_of_game = Integer.parseInt(MaskString(cell[1].getContents()));
			System.out.println("num_of instants game = "+num_of_game);
			WriteInt(os,num_of_game);
			for(int i = 0; i < num_of_game; i++)
			{
				cell =  sheet.getRow(6+i);
				game_name = MaskString(cell[1].getContents());
				WriteInt(os,game_name.length());
				WriteString(os,game_name);
				
				apple_id = Integer.parseInt(MaskString(cell[2].getContents()));
				WriteInt(os,apple_id);
				
				game_decs = MaskString(cell[3].getContents());
				WriteInt(os,game_decs.length());
				WriteString(os,game_decs);
				
				rate = Integer.parseInt(MaskString(cell[4].getContents()));
				WriteInt(os,rate);

				num_screen = Integer.parseInt(MaskString(cell[5].getContents()));
				WriteInt(os,num_screen);
				
				System.out.println("instants = "+i + " ;game name = " + game_name + " ;apple id = " + apple_id+" ;Decspription: "+ game_decs + "Rate " + rate + "num_screen = " + num_screen);
				out_list.println(""+apple_id);
			}
			
			//num of basic game.
			cell = sheet.getRow(9);
			num_of_game = Integer.parseInt(MaskString(cell[1].getContents()));
			WriteInt(os,num_of_game);
			System.out.println("\n\nnum_of bassic game = "+num_of_game);
			for(int i = 0; i < num_of_game; i++)
			{
				cell =  sheet.getRow(15+i);
				game_name = MaskString(cell[1].getContents());
				WriteInt(os,game_name.length());
				WriteString(os,game_name);
				
				apple_id = Integer.parseInt(MaskString(cell[2].getContents()));
				WriteInt(os,apple_id);
				
				game_decs = MaskString(cell[3].getContents());
				WriteInt(os,game_decs.length());
				WriteString(os,game_decs);

				rate = Integer.parseInt(MaskString(cell[4].getContents()));
				WriteInt(os,rate);

				num_screen = Integer.parseInt(MaskString(cell[5].getContents()));
				WriteInt(os,num_screen);
				
				System.out.println("instants = "+i + " ;game name = " + game_name + " ;apple id = " + apple_id+" ;Decspription: "+ game_decs + "Rate " + rate + "num_screen = " + num_screen);
				out_list.println(""+apple_id);
			}
			
			os.close();
			out_list.close();
			os_list.close();
		}
		catch(Exception e)
		{
		}
			
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
}
