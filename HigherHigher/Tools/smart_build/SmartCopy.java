import java.io.File;
import java.util.Date;
import java.io.*;

public class SmartCopy {
	public static int _num = 0;
    public static void main(String args[]) {
	
		Process theProcess = null;
		BufferedReader inStream = null;
		String cmd = "";
		String input_file = "";
		String output_file="";
		for(int i = 0; i < args.length; i++)
		{
			cmd = cmd + " " + args[i];
			if (args[i].compareTo("-i") == 0)
			{
				input_file = args[i+1];
			}
			
		}
		//System.out.println("cmd = " +cmd);
		try 
		{
			File file = new File(input_file);
			if(file != null)
			{
				output_file = file.getName();
				File file_out = new File(output_file);
				boolean need_copy = true;
			
				//String text_info = cmd + " " + lastModified +" " + date;
				//write to log
				if(file_out != null)
				{
					if(file_out.lastModified() == file.lastModified())
					{
						need_copy = false;
					}
				}
			
				if(need_copy)
				{
					// System.out.println("copy file: " + input_file);
					cmd = "CMD /C COPY /Y " + file.getAbsolutePath();// + "\\" + input_file;
					//System.out.println("cmd = " + cmd);
					theProcess = Runtime.getRuntime().exec(cmd);
					BufferedReader is = new BufferedReader(new InputStreamReader(theProcess.getInputStream()));  
					String line;  
					while ((line = is.readLine()) != null) 
					{  
						System.out.println(line);  
					}  
				}
				else
				{
					// System.out.println("smart file pass: " + input_file);
				}
			}
			
		
 
        } 
		catch (Exception ex)
		{
			System.out.println("Exception  = " + ex);
			//Logger.getLogger(JavaFX_NIOFile.class.getName()).log(Level.SEVERE, null, ex);
        }
		
	

    }    
}